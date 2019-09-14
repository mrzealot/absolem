#include "Nrf52Bluefruit.h"

#if defined(DEBUG) && (defined(DEBUG_GLOBAL) || defined(DEBUG_NRF52BLUEFRUIT))
#define DD(x) x
#else
#define DD(x)
#endif

namespace absolem {

    #if defined(DEBUG) || defined(PROFILING)
    void Nrf52Bluefruit::debug(char* message, ...) {
        char buffer[512];
        va_list args;
        va_start(args, message);
        vsprintf(buffer, message, args);
        va_end(args);
        Serial.println(buffer);
    }
    #endif

    Time Nrf52Bluefruit::time() {
        return micros();
    }

    void Nrf52Bluefruit::delay(Time time) {
        PF(1);
        if (time > 10000) {
            ::delay((Time)(time / 1000));
        } else {
            ::delayMicroseconds(time);
        }
    }

    void Nrf52Bluefruit::setup() {
        #if defined(DEBUG) || defined(PROFILING)
        Serial.begin(9600);
        debug("Serial interface started...");
        #endif

        // explicitly set the analog reference for battery measurements
        analogReference(AR_INTERNAL_3_0);
        analogReadResolution(12);
        delay(1);

        Bluefruit.begin();
        Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
        Bluefruit.setName(name);

        // Configure and Start Device Information Service
        bledis.setManufacturer(manufacturer);
        bledis.setModel(model);
        bledis.begin();

        blehid.begin();
        blebas.begin();
        blebas.write((Byte)charge());
        blebasTimer = 0;

        // Advertising packet
        Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
        Bluefruit.Advertising.addTxPower();
        Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);
        
        // Include BLE HID service
        Bluefruit.Advertising.addService(blehid);

        // There is enough room for the dev name in the advertising packet
        Bluefruit.Advertising.addName();
        
        /* Start Advertising
        * - Enable auto advertising if disconnected
        * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
        * - Timeout for fast mode is 30 seconds
        * - Start(timeout) with timeout = 0 will advertise forever (until connected)
        * 
        * For recommended advertising interval
        * https://developer.apple.com/library/content/qa/qa1931/_index.html   
        */
        Bluefruit.Advertising.restartOnDisconnect(true);
        Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
        Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
        Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
    }


    float Nrf52Bluefruit::charge() {
        // read raw analog value --> straight from an Adafruit tutorial
        // https://learn.adafruit.com/bluefruit-nrf52-feather-learning-guide/nrf52-adc
        float voltage = analogRead(A7);
        DD(debug("Nrf52Bluefruit::charge: raw battery pin reading is %f", voltage);)
        
        voltage *= 1.403; // voltage divider compensation
        voltage *= 3.0;   // multiply by our reference voltage
        voltage /= 4096;  // convert to voltage by dividing with the samling resolution
        DD(debug("Nrf52Bluefruit::charge: battery voltage is %f", voltage);)
        
        // convert to percentage --> based on a 3-line approximation of
        // a common discharge curve...
        // https://learn.adafruit.com/li-ion-and-lipoly-batteries/voltages
        float percent = 0;
        if (voltage >= 3.8) {
            // 3.79 instead of 3.8 to actually have some time with 100%
            percent = 80 + ((voltage - 3.79) / 0.4) * 20;
            // then (because of the above) we actually need cap it
            percent = percent > 100 ? 100 : percent;
        } else if (voltage >= 3.5) {
            percent = 20 + ((voltage - 3.5) / 0.3) * 60;
        // let's say 3.2 is the minimum, don't know where the
        // protection circuitry kicks in exactly...
        } else if (voltage >= 3.2) {
            percent = ((voltage - 3.2) / 0.3) * 20;
        }
        DD(debug("Nrf52Bluefruit::charge: final percentage is %f", percent);)
        return percent;
    }

    void Nrf52Bluefruit::sleep() {
        // prepare wakeup, and then go to sleep, TODO
    }

    void Nrf52Bluefruit::hibernate() {
        // an even deeper sleep, TODO
    }

    void Nrf52Bluefruit::tick() {
        // refresh battery service, but only about every 5 minutes
        // which (with about 100 ticks per sec) is 30k
        blebasTimer++;
        if (blebasTimer >= 30000) {
            DD(debug("Nrf52Bluefruit::refreshing blebas");)
            blebas.write((Byte)charge());
            blebasTimer = 0;
        }
    }

    void Nrf52Bluefruit::input(Pin pin) {
        pinMode(pin, INPUT_PULLUP);
    }

    void Nrf52Bluefruit::output(Pin pin) {
        pinMode(pin, OUTPUT);
    }

    void Nrf52Bluefruit::disable(Pin pin) {
        pinMode(pin, INPUT); // virtually disables the pin
    }

    void Nrf52Bluefruit::on(Pin pin) {
        digitalWrite(pin, LOW); // low is the logical "on"
    }

    void Nrf52Bluefruit::off(Pin pin) {
        digitalWrite(pin, HIGH);
    }

    bool Nrf52Bluefruit::read(Pin pin) {
        return digitalRead(pin) == LOW;
    }

    bool Nrf52Bluefruit::report(Modifiers mods, KeyCode keys[6]) {
        DD(debug("Nrf52Bluefruit::report: reporting... %d + %d", mods, keys[0]);)
        return blehid.keyboardReport(mods, keys);
    }

    bool Nrf52Bluefruit::report(UsageCode usage) {
        return blehid.consumerReport(usage);
    }

    void Nrf52Bluefruit::reset() {
        NVIC_SystemReset();
    }

    
} // namespace absolem
