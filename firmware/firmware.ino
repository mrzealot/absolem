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
#include "src/interpreter/actions/ResetAction.h"

#include "src/interpreter/modules/ReporterModule.h"

using namespace absolem;


Nrf52Bluefruit controller;
MatrixWiring wiring(&controller, {2, 3, 4, 5, 16, 15, 7, 11}, {29, 8, 14, 13, 12});
DebouncePerKey debounce(&controller, 5);
Decoder decoder(&wiring, &debounce);

void keyboardSetup() {
  controller.manufacturer = "MrZealot";
  controller.model = "Absolem";
}


Interpreter interpreter(&controller);

ReporterModule reporter;




void keymapSetup() {
  controller.name = "Absolem #2";
  controller.setup();

  interpreter.addModule("reporter", &reporter);

  interpreter.addRule(2, {
    Rule(new PressTrigger(true), new KeyCodeAction(true, 0x04)),
    Rule(new PressTrigger(false), new KeyCodeAction(false, 0x04))
  });

  interpreter.addRule(4, {
    Rule(new PressTrigger(true), new ResetAction())
  });
}

void setup() {
  keyboardSetup();
  keymapSetup();
}

void loop() {
  controller.tick();
  interpreter.enqueue(decoder.getEvents());
  interpreter.tick();
}

#endif
