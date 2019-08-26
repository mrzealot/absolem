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
#include "src/interpreter/triggers/MultiTrigger.h"
#include "src/interpreter/actions/KeyCodeAction.h"
#include "src/interpreter/actions/ResetAction.h"
#include "src/interpreter/actions/LayerAction.h"

#include "src/interpreter/modules/ReporterModule.h"
#include "src/interpreter/modules/CacheModule.h"
#include "src/interpreter/modules/LayerModule.h"

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
LayerModule layer(100);



Rule helper(Trigger* trigger, Modifiers mods, Key key) {
  return Rule(
      trigger,
      new KeyCodeAction(
        true,
        mods,
        key,
        Rule(
          new PressTrigger(false),
          new KeyCodeAction(false, mods, key)
        )
      )
    );
}

List<Rule> kc(Modifiers mods, Key key) {
  return {helper(new PressTrigger(true), mods, key)};
}

List<Rule> ly(Byte layer) {
  return {
    Rule(
      new PressTrigger(true),
      new LayerAction(
        true,
        layer,
        Rule(
          new PressTrigger(false),
          new LayerAction(false, layer)
        )
      )
    )
  };
}

#include "src/profiling/Timer.h"

#define nrf_timer_num   (1)
#define cc_channel_num  (0)
TimerClass timer(nrf_timer_num, cc_channel_num);


#define TERM 200000

void keymapSetup() {
  controller.name = "Absolem #2";
  controller.setup();

  interpreter.addModule(&reporter);
  interpreter.addModule(&cache);
  interpreter.addModule(&layer);


  //interpreter.addRule(3, { // instead of escape, as an escape... ha, get it? :)
  //  Rule(new PressTrigger(true), new ResetAction())
  //});
  interpreter.addRule(3, {
    helper(new MultiTrigger(TERM, 1, true), 0, HU_C),
    helper(new MultiTrigger(TERM, 0, true), 0, HU_B),
    helper(new MultiTrigger(TERM, 1, false), 0, HU_A),
  });
  interpreter.addRule(4, kc(0, KC_BSPACE));
  interpreter.addRule(5, kc(0, KC_LSHIFT));

  interpreter.addRule(6, kc(0, HU_EE));
  interpreter.addRule(7, kc(0, KC_NO));
  interpreter.addRule(8, ly(1));
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







  interpreter.addRule(103, kc(0, HU_MINS));
  interpreter.addRule(104, kc(0, HU_COMM));
  interpreter.addRule(105, kc(0, HU_DOT));

  interpreter.addRule(106, kc(HU_GRV_MODS, HU_GRV));
  interpreter.addRule(107, kc(HU_PLUS_MODS, HU_PLUS));
  interpreter.addRule(108, kc(0, KC_NO));
  interpreter.addRule(109, kc(HU_EQL_MODS, HU_EQL));
  interpreter.addRule(110, kc(HU_AMPR_MODS, HU_AMPR));

  interpreter.addRule(111, kc(HU_SCLN_MODS, HU_SCLN));
  interpreter.addRule(112, kc(HU_COLN_MODS, HU_COLN));
  interpreter.addRule(113, kc(0, KC_NO));
  interpreter.addRule(114, kc(HU_ASTR_MODS, HU_ASTR));
  interpreter.addRule(115, kc(HU_PERC_MODS, HU_PERC));

  interpreter.addRule(116, kc(HU_AT_MODS, HU_AT));
  interpreter.addRule(117, kc(0, KC_NO));
  interpreter.addRule(118, kc(0, KC_NO));
  interpreter.addRule(119, kc(HU_CIRC_MODS, HU_CIRC));
  interpreter.addRule(120, kc(HU_TILD_MODS, HU_TILD));

  interpreter.addRule(123, kc(HU_UNDS_MODS, HU_UNDS));
  interpreter.addRule(124, kc(HU_QUOT_MODS, HU_QUOT));
  interpreter.addRule(125, kc(HU_DQOT_MODS, HU_DQOT));

  interpreter.addRule(126, kc(HU_HASH_MODS, HU_HASH));
  interpreter.addRule(127, kc(HU_DLR_MODS, HU_DLR));
  interpreter.addRule(128, kc(HU_QST_MODS, HU_QST));
  interpreter.addRule(129, kc(HU_EXLM_MODS, HU_EXLM));
  interpreter.addRule(130, kc(HU_PIPE_MODS, HU_PIPE));

  interpreter.addRule(131, kc(HU_LESS_MODS, HU_LESS));
  interpreter.addRule(132, kc(HU_LCBR_MODS, HU_LCBR));
  interpreter.addRule(133, kc(HU_LBRC_MODS, HU_LBRC));
  interpreter.addRule(134, kc(HU_LPRN_MODS, HU_LPRN));
  interpreter.addRule(135, kc(HU_SLSH_MODS, HU_SLSH));

  interpreter.addRule(136, kc(HU_MORE_MODS, HU_MORE));
  interpreter.addRule(137, kc(HU_RCBR_MODS, HU_RCBR));
  interpreter.addRule(138, kc(HU_RBRC_MODS, HU_RBRC));
  interpreter.addRule(139, kc(HU_RPRN_MODS, HU_RPRN));
  interpreter.addRule(140, kc(HU_BSLS_MODS, HU_BSLS));
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
