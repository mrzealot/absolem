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

        Bluefruit.begin();
        Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
        Bluefruit.setName(name);

        // Configure and Start Device Information Service
        bledis.setManufacturer(manufacturer);
        bledis.setModel(model);
        bledis.begin();

        blehid.begin();

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

    void Nrf52Bluefruit::sleep() {
        // prepare wakeup, and then go to sleep, TODO
    }

    void Nrf52Bluefruit::hibernate() {
        // an even deeper sleep, TODO
    }

    void Nrf52Bluefruit::tick() {
        // refresh battery service, TODO
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

    void Nrf52Bluefruit::report(Modifiers mods, KeyCode keys[6]) {
        DD(debug("Nrf52Bluefruit::report: reporting... %d + %d", mods, keys[0]);)
        blehid.keyboardReport(mods, keys);
    }

    void Nrf52Bluefruit::report(UsageCode usage) {
        blehid.consumerReport(usage);
    }

    void Nrf52Bluefruit::reset() {
        NVIC_SystemReset();
    }

    
} // namespace absolem
