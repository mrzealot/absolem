#ifndef ABSOLEM_FIRMWARE_INO
#define ABSOLEM_FIRMWARE_INO

#define PROFILING_MAIN
#include <nrf.h>
#include "nrf_timer.h"

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
#include "src/interpreter/modules/CacheModule.h"

#include "src/hidspec/keycodes_hungarian.h"


#include "src/profiling/profiling.h"

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
CacheModule cache;



List<Rule> kc(Modifiers mods, Key key) {
  return {
    Rule(
      new PressTrigger(true),
      new KeyCodeAction(
        true,
        mods,
        key,
        Rule(
          new PressTrigger(false),
          new KeyCodeAction(false, mods, key)
        )
      )
    )
  };
}

#include "src/profiling/Timer.h"

#define nrf_timer_num   (1)
#define cc_channel_num  (0)
TimerClass timer(nrf_timer_num, cc_channel_num);


void keymapSetup() {
  controller.name = "Absolem #2";
  controller.setup();

  interpreter.addModule(&reporter);
  interpreter.addModule(&cache);


  interpreter.addRule(3, { // instead of escape, as an escape... ha, get it? :)
    Rule(new PressTrigger(true), new ResetAction())
  });
  interpreter.addRule(4, kc(0, KC_BSPACE));
  interpreter.addRule(5, kc(0, KC_LSHIFT));

  interpreter.addRule(6, kc(0, HU_EE));
  interpreter.addRule(7, kc(0, KC_NO));
  interpreter.addRule(8, kc(0, KC_NO));
  interpreter.addRule(9, kc(0, HU_H));
  interpreter.addRule(10, kc(0, HU_K));

  interpreter.addRule(11, kc(0, HU_O));
  interpreter.addRule(12, kc(0, HU_I));
  interpreter.addRule(13, kc(0, HU_E));
  interpreter.addRule(14, kc(0, HU_N));
  interpreter.addRule(15, kc(0, HU_M));

  interpreter.addRule(16, kc(0, HU_AA));
  interpreter.addRule(17, kc(0, HU_Y));
  interpreter.addRule(18, kc(0, HU_U));
  interpreter.addRule(19, kc(0, HU_L));
  interpreter.addRule(20, kc(0, HU_J));

  interpreter.addRule(23, kc(0, KC_TAB));
  interpreter.addRule(24, kc(0, KC_SPACE));
  interpreter.addRule(25, kc(0, KC_ENTER));

  interpreter.addRule(26, kc(0, HU_Z));
  interpreter.addRule(27, kc(0, HU_X));
  interpreter.addRule(28, kc(0, HU_C));
  interpreter.addRule(29, kc(0, HU_D));
  interpreter.addRule(30, kc(0, HU_V));

  interpreter.addRule(31, kc(0, HU_A));
  interpreter.addRule(32, kc(0, HU_R));
  interpreter.addRule(33, kc(0, HU_S));
  interpreter.addRule(34, kc(0, HU_T));
  interpreter.addRule(35, kc(0, HU_G));

  interpreter.addRule(36, kc(0, HU_Q));
  interpreter.addRule(37, kc(0, HU_W));
  interpreter.addRule(38, kc(0, HU_F));
  interpreter.addRule(39, kc(0, HU_P));
  interpreter.addRule(40, kc(0, HU_B));
}

//void profiling_wrapper() {
//  profiling_loop();
//  timer.attachInterrupt(&profiling_wrapper, 1000); // microseconds
//}

void setup() {
  keyboardSetup();
  keymapSetup();
  //clear_profiling_data();
  //timer.attachInterrupt(&profiling_wrapper, 1000); // microseconds
}

#define TICK_FREQ 25000

void loop() {
  Time begin = controller.time();
  interpreter.enqueue(decoder.getEvents());
  interpreter.tick();
  Time end = controller.time();
  Time diff = end - begin;
  if (diff >= TICK_FREQ) {
    //prof_overload += diff - PROF_TARGET;
  } else {
    //prof_underload += PROF_TARGET - diff;
    controller.delay(TICK_FREQ - diff);
  }
  //dump_profiling_data();
}

#endif
