#ifndef ABSOLEM_FIRMWARE_INO
#define ABSOLEM_FIRMWARE_INO

#undef min
#undef max

#include "src/controllers/Nrf52Bluefruit.h"
#include "src/decoder/wiring/MatrixWiring.h"
#include "src/decoder/debounce/DebouncePerKey.h"
#include "src/decoder/Decoder.h"

#include "src/interpreter/Interpreter.h"
#include "src/interpreter/triggers/PressTrigger.h"
#include "src/interpreter/actions/KeyCodeAction.h"

using namespace absolem;


Nrf52Bluefruit controller(true);
MatrixWiring wiring(&controller, {}, {});
DebouncePerKey debounce(5);
Decoder decoder(&wiring, &debounce);

void keyboardSetup() {
  controller.manufacturer = "MrZealot";
  controller.model = "Absolem";
}


Interpreter interpreter(&controller);
void keymapSetup() {
  controller.name = "Absolem No.2";

  interpreter.addRule(1, {
    Rule(new PressTrigger(true), new KeyCodeAction(0x04)),
    Rule(new PressTrigger(false), new KeyCodeAction(0x00))
  });
}

void setup() {
  controller.setup();
  keyboardSetup();
  keymapSetup();
}

void loop() {
  controller.tick();
  interpreter.enqueue(decoder.getEvents());
  interpreter.tick();
}

#endif
