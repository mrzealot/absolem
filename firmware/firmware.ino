#ifndef ABSOLEM_FIRMWARE_INO
#define ABSOLEM_FIRMWARE_INO

#define PROFILING_MAIN
#include <nrf.h>
#include "nrf_timer.h"

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
#include "src/interpreter/actions/SequenceAction.h"
#include "src/interpreter/actions/FlushAction.h"
#include "src/interpreter/actions/ConsumerAction.h"
#include "src/interpreter/actions/BatteryAction.h"

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



// -------
// this is NOT elegant yet, I'm just trying to replicate the
// functionality of my proto build with this mental model


#define TERM 200000


Trigger* pressTrigger = new PressTrigger(true);
Trigger* releaseTrigger = new PressTrigger(false);
Trigger* tapTrigger = new MultiTrigger(TERM, 1, false);
Trigger* holdTrigger = new MultiTrigger(TERM, 0, true);
Trigger* tapHoldTrigger = new MultiTrigger(TERM, 1, true);
//Trigger* tripleTapTrigger = new MultiTrigger(TERM, 3, false);

Action* noop = new Action();
Action* flushAction = new FlushAction();


Rule helper(Trigger* trigger, Modifiers mods, KeyCode key) {
  return Rule(
      trigger,
      new KeyCodeAction(
        true,
        mods,
        false,
        key,
        Rule(
          releaseTrigger,
          new KeyCodeAction(false, mods, false, key)
        )
      )
    );
}

List<Rule> kc(Modifiers mods, KeyCode key) {
  return {helper(pressTrigger, mods, key)};
}

List<Rule> kc(KeyCode key) {
  return {helper(pressTrigger, 0, key)};
}


List<Rule> us(UsageCode usage) {
  return {
    Rule(
      pressTrigger,
      new ConsumerAction(
        true,
        usage,
        Rule(
          releaseTrigger,
          new ConsumerAction(
            false,
            usage
          )
        )
      )
    )
  };
}

List<Rule> fullLayer(Byte layer) {
  return {
    Rule(
      holdTrigger,
      new LayerAction(
        true,
        layer,
        false,
        Rule(
          releaseTrigger,
          new LayerAction(false, layer, false)
        )
      )
    ),
    Rule(
      tapTrigger,
      new LayerAction(
        true,
        layer,
        true
      )
    )
  };
}

List<Rule> fullMod(KeyCode mod, Modifiers mods) {
  return {
    Rule(
      holdTrigger,
      new KeyCodeAction(
        true,
        0,
        false,
        mod,
        Rule(
          releaseTrigger,
          new KeyCodeAction(false, 0, false, mod)
        )
      )
    ),
    Rule(
      tapTrigger,
      new KeyCodeAction(
        true,
        mods,
        true,
        0,
        Rule(
          releaseTrigger,
          new KeyCodeAction(false, mods, true, 0)
        )
      )
    )
  };
}


List<Rule> modTap(KeyCode mod, KeyCode key, bool realMod = true) {
  Action* keyAction = new KeyCodeAction(
    true,
    0,
    false,
    key,
    Rule(
      releaseTrigger,
      new KeyCodeAction(false, 0, false, key)
    )
  );
  return {
    //Rule(
    //  tapHoldTrigger,
    //  keyAction
    //),
    Rule(
      holdTrigger,
      new KeyCodeAction(
        true,
        realMod ? 0 : mod,
        false,
        realMod ? mod : 0,
        Rule(
          releaseTrigger,
          new KeyCodeAction(
            false,
            realMod ? 0 : mod,
            false,
            realMod ? mod : 0
          )
        )
      )
    ),
    Rule(
      tapTrigger,
      keyAction
    )
  };
}



List<Rule> layerTap(Byte layer, KeyCode key) {
  Action* keyAction = new KeyCodeAction(
    true,
    0,
    false,
    key,
    Rule(
      releaseTrigger,
      new KeyCodeAction(false, 0, false, key)
    )
  );
  return {
    //Rule(
    //  tapHoldTrigger,
    //  keyAction
    //),
    Rule(
      holdTrigger,
      new LayerAction(
        true,
        layer,
        false,
        Rule(
          releaseTrigger,
          new LayerAction(false, layer, false)
        )
      )
    ),
    Rule(
      tapTrigger,
      keyAction
    )
  };
}





List<Rule> seq(List<Pair<Modifiers, KeyCode>> list, bool shiftAtEnd = false) {
  List<Action*> actions;
  KeyCode last = 0;
  for (auto& pair : list) {
    if (last == pair.second) {
      actions.push_back(flushAction); 
    }
    actions.push_back(new KeyCodeAction(true, pair.first, false, pair.second));
    actions.push_back(flushAction);
    actions.push_back(new KeyCodeAction(false, pair.first, false, pair.second));
    last = pair.second;
  }
  if (shiftAtEnd) {
    actions.push_back(flushAction);
    actions.push_back(new KeyCodeAction(true, MOD_MASK_SHIFT, true, 0));
  }
  return {
    Rule(pressTrigger, new SequenceAction(actions))
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
  interpreter.addModule(&layer);


  interpreter.addRule(3, modTap(KC_LGUI, KC_ESC));
  interpreter.addRule(4, layerTap(4, KC_BSPACE)); // Num
  interpreter.addRule(5, fullMod(KC_LSHIFT, MOD_SHIFT));

  interpreter.addRule(6, kc(HU_EE));
  interpreter.addRule(7, fullLayer(2)); // Misc
  interpreter.addRule(8, fullLayer(1)); // Sym
  interpreter.addRule(9, kc(HU_H));
  interpreter.addRule(10, kc(HU_K));

  interpreter.addRule(11, kc(HU_O));
  interpreter.addRule(12, kc(HU_I));
  interpreter.addRule(13, kc(HU_E));
  interpreter.addRule(14, kc(HU_N));
  interpreter.addRule(15, kc(HU_M));

  interpreter.addRule(16, kc(HU_AA));
  interpreter.addRule(17, kc(HU_Y));
  interpreter.addRule(18, kc(HU_U));
  interpreter.addRule(19, kc(HU_L));
  interpreter.addRule(20, kc(HU_J));

  interpreter.addRule(23, modTap(KC_LCTRL, KC_TAB));
  interpreter.addRule(24, layerTap(3, KC_SPACE)); // Nav
  interpreter.addRule(25, modTap(MOD_MASK_CSA, KC_ENTER, false));

  interpreter.addRule(26, kc(HU_Z));
  interpreter.addRule(27, kc(HU_X));
  interpreter.addRule(28, kc(HU_C));
  interpreter.addRule(29, kc(HU_D));
  interpreter.addRule(30, kc(HU_V));

  interpreter.addRule(31, kc(HU_A));
  interpreter.addRule(32, kc(HU_R));
  interpreter.addRule(33, kc(HU_S));
  interpreter.addRule(34, kc(HU_T));
  interpreter.addRule(35, kc(HU_G));

  interpreter.addRule(36, kc(HU_Q));
  interpreter.addRule(37, kc(HU_W));
  interpreter.addRule(38, kc(HU_F));
  interpreter.addRule(39, kc(HU_P));
  interpreter.addRule(40, kc(HU_B));







  interpreter.addRule(103, kc(HU_MINS));
  interpreter.addRule(104, kc(HU_COMM));
  interpreter.addRule(105, kc(HU_DOT));

  interpreter.addRule(106, kc(HU_GRV_MODS, HU_GRV));
  interpreter.addRule(107, kc(HU_PLUS_MODS, HU_PLUS));
  interpreter.addRule(108, kc(KC_NO));
  interpreter.addRule(109, kc(HU_EQL_MODS, HU_EQL));
  interpreter.addRule(110, kc(HU_AMPR_MODS, HU_AMPR));

  interpreter.addRule(111, kc(HU_SCLN_MODS, HU_SCLN));
  interpreter.addRule(112, kc(HU_COLN_MODS, HU_COLN));
  interpreter.addRule(113, {Rule(pressTrigger, new BatteryAction(string2code))});
  interpreter.addRule(114, kc(HU_ASTR_MODS, HU_ASTR));
  interpreter.addRule(115, kc(HU_PERC_MODS, HU_PERC));

  interpreter.addRule(116, kc(HU_AT_MODS, HU_AT));
  interpreter.addRule(117, kc(KC_NO));
  interpreter.addRule(118, kc(KC_NO));
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


  

  interpreter.addRule(203, seq({
    Pair<Modifiers, KeyCode>(0, KC_SPACE),
    Pair<Modifiers, KeyCode>(0, HU_MINS),
    Pair<Modifiers, KeyCode>(0, HU_MINS),
    Pair<Modifiers, KeyCode>(0, KC_SPACE),
  }));
  interpreter.addRule(204, seq({
    Pair<Modifiers, KeyCode>(0, HU_COMM),
    Pair<Modifiers, KeyCode>(0, KC_SPACE),
  }));
  interpreter.addRule(205, seq({
    Pair<Modifiers, KeyCode>(0, HU_DOT),
    Pair<Modifiers, KeyCode>(0, KC_SPACE)
  }, true));

  interpreter.addRule(228, seq({
    Pair<Modifiers, KeyCode>(HU_QST_MODS, HU_QST),
    Pair<Modifiers, KeyCode>(0, KC_SPACE)
  }, true));
  interpreter.addRule(229, seq({
    Pair<Modifiers, KeyCode>(HU_EXLM_MODS, HU_EXLM),
    Pair<Modifiers, KeyCode>(0, KC_SPACE)
  }, true));

  interpreter.addRule(231, kc(HU_II));
  interpreter.addRule(232, kc(HU_OEE));
  interpreter.addRule(233, kc(HU_OE));
  interpreter.addRule(234, kc(HU_OO));

  interpreter.addRule(237, kc(HU_UEE));
  interpreter.addRule(238, kc(HU_UE));
  interpreter.addRule(239, kc(HU_UU));







  interpreter.addRule(303, kc(KC_ESC));
  interpreter.addRule(304, kc(KC_BSPC));
  interpreter.addRule(305, kc(KC_DEL));

  interpreter.addRule(306, kc(KC_ENTER));
  interpreter.addRule(307, kc(KC_END));
  interpreter.addRule(308, kc(KC_DOWN));
  interpreter.addRule(309, kc(KC_HOME));
  //interpreter.addRule(310, kc(KC_MENU)); --> need generic system control reports... (0x01)

  interpreter.addRule(311, kc(KC_TAB));
  interpreter.addRule(312, kc(KC_RIGHT));
  interpreter.addRule(313, kc(KC_UP));
  interpreter.addRule(314, kc(KC_LEFT));
  interpreter.addRule(315, kc(MOD_CTRL, HU_MINS));

  interpreter.addRule(316, kc(KC_INS));
  interpreter.addRule(317, kc(KC_PGDOWN));
  interpreter.addRule(318, kc(KC_SPACE));
  interpreter.addRule(319, kc(KC_PGUP));
  interpreter.addRule(320, kc(MOD_MASK_CS, HU_PLUS)); // CS, because + needs shift

  //interpreter.addRule(323, );
  //interpreter.addRule(324, );
  //interpreter.addRule(325, );

  interpreter.addRule(326, kc(MOD_CTRL, HU_Z));
  interpreter.addRule(327, kc(MOD_CTRL, HU_X));
  interpreter.addRule(328, kc(MOD_CTRL, HU_C));
  interpreter.addRule(329, kc(MOD_CTRL, HU_D));
  interpreter.addRule(330, kc(MOD_CTRL, HU_V));

  interpreter.addRule(331, fullMod(KC_LGUI, MOD_GUI));
  interpreter.addRule(332, fullMod(KC_LALT, MOD_ALT));
  interpreter.addRule(333, fullMod(KC_LSHIFT, MOD_SHIFT));
  interpreter.addRule(334, fullMod(KC_LCTRL, MOD_CTRL));
  interpreter.addRule(335, us(CON_VOLUME_DECREMENT));

  interpreter.addRule(336, us(CON_MUTE));
  interpreter.addRule(337, us(CON_SCAN_PREVIOUS_TRACK));
  interpreter.addRule(338, us(CON_PLAY_PAUSE));
  interpreter.addRule(339, us(CON_SCAN_NEXT_TRACK));
  interpreter.addRule(340, us(CON_VOLUME_INCREMENT));






  //interpreter.addRule(403, );
  //interpreter.addRule(404, );
  //interpreter.addRule(405, );

  interpreter.addRule(406, kc(HU_COMM));
  interpreter.addRule(407, kc(HU_3));
  interpreter.addRule(408, kc(HU_2));
  interpreter.addRule(409, kc(HU_1));
  interpreter.addRule(410, kc(HU_PLUS_MODS, HU_PLUS));

  interpreter.addRule(411, kc(HU_DOT));
  interpreter.addRule(412, kc(HU_6));
  interpreter.addRule(413, kc(HU_5));
  interpreter.addRule(414, kc(HU_4));
  interpreter.addRule(415, kc(HU_0));

  interpreter.addRule(416, kc(KC_BSPC));
  interpreter.addRule(417, kc(HU_9));
  interpreter.addRule(418, kc(HU_8));
  interpreter.addRule(419, kc(HU_7));
  interpreter.addRule(420, kc(HU_MINS));

  interpreter.addRule(423, kc(KC_TAB));
  interpreter.addRule(424, kc(KC_SPACE));
  interpreter.addRule(425, kc(KC_ENTER));

  interpreter.addRule(426, kc(KC_F11));
  interpreter.addRule(427, kc(KC_F12));
  interpreter.addRule(428, kc(KC_F13));
  interpreter.addRule(429, kc(KC_PSCREEN));
  interpreter.addRule(430, kc(KC_PAUSE));

  interpreter.addRule(431, kc(KC_F6));
  interpreter.addRule(432, kc(KC_F7));
  interpreter.addRule(433, kc(KC_F8));
  interpreter.addRule(434, kc(KC_F9));
  interpreter.addRule(435, kc(KC_F10));

  interpreter.addRule(436, kc(KC_F1));
  interpreter.addRule(437, kc(KC_F2));
  interpreter.addRule(438, kc(KC_F3));
  interpreter.addRule(439, kc(KC_F4));
  interpreter.addRule(440, kc(KC_F5));

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

#define TICK_FREQ 10000

void loop() {
  Time begin = controller.time();
  interpreter.enqueue(decoder.getEvents());
  interpreter.tick();
  Time end = controller.time();
  Time diff = end - begin;
  if (diff >= TICK_FREQ) {
    //prof_overload += diff - PROF_TARGET;
  } else if (diff > 0) { // don't let a potential overflow block
    //prof_underload += PROF_TARGET - diff;
    controller.delay(TICK_FREQ - diff);
  }
  //dump_profiling_data();
}

#endif
