/* -*- mode: c++ -*-
 * Kaleidoscope-AdhocMacros 
 * Copyright (C) 2018  Christopher Auer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Kaleidoscope.h>
#include <Kaleidoscope-Macros.h>
#include <Kaleidoscope-AdhocMacros.h>

enum {
    ADHOC_MACROS_REPLAY, ADHOC_MACROS_TOGGLE_RECORDING,
    ADHOC_MACROS_RESET_REPEAT,
    ADHOC_MACROS_REPEAT_1,
    ADHOC_MACROS_REPEAT_2,
    ADHOC_MACROS_REPEAT_3,
    ADHOC_MACROS_REPEAT_4,
    ADHOC_MACROS_REPEAT_5,
    ADHOC_MACROS_REPEAT_6,
    ADHOC_MACROS_REPEAT_7,
    ADHOC_MACROS_REPEAT_8,
    ADHOC_MACROS_REPEAT_9,
    ADHOC_MACROS_REPEAT_0 };


enum { QWERTY, FUNCTION }; // layers

// NOTE: To enter the digits, I use the lowest row "parallel" to the number keys in the FN
// to not interfere with the function keys.
// Alternatively, you can add an additional layer that you use for recording and where
// you assign the number keys to enter the repetition count.
const Key keymaps[][ROWS][COLS] PROGMEM = {

  [QWERTY] = KEYMAP_STACKED
  (___,           Key_1, Key_2, Key_3, Key_4, Key_5, ___,
   Key_Backtick,  Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_Backslash,    Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_Tab, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,
   // Thumb Bar
   Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
   // Fn
   ShiftToLayer(FUNCTION),

   M(MACRO_ANY),                     Key_6, Key_7, Key_8,     Key_9,      Key_0,         ___,
   Key_Enter,                        Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
                                     Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
   M(ADHOC_MACROS_REPLAY), Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,
   // Thumb Bar
   Key_RightShift,Key_LeftAlt,  Key_Spacebar, Key_RightControl,
   // Fn
   ShiftToLayer(FUNCTION)),


  [FUNCTION] =  KEYMAP_STACKED
  (___,      Key_F1,           Key_F2,      Key_F3,     Key_F4,        Key_F5,  XXX,
   Key_Tab,  ___,              ___,         ___,        ___,           ___,     ___,
   ___,      ___,              Key_Home,    Key_PageUp, Key_PageDown,  Key_End,
   M(ADHOC_MACROS_RESET_REPEAT),      M(ADHOC_MACROS_REPEAT_1), M(ADHOC_MACROS_REPEAT_2), M(ADHOC_MACROS_REPEAT_3), M(ADHOC_MACROS_REPEAT_4), M(ADHOC_MACROS_REPEAT_5), ___,
   // Thumb Bar

   Key_LeftBracket, Key_Delete, Key_LeftCurlyBracket, Key_LeftParen,
   // Fn
   ___,

   ___,                             Key_F6,                  Key_F7,                     Key_F8,                 Key_F9,                      Key_F10,                  Key_F11,
   ___,                             Consumer_PlaySlashPause, Consumer_ScanPreviousTrack, Consumer_ScanNextTrack, Consumer_VolumeDecrement,    Consumer_VolumeIncrement, Key_F12,
                                    Key_LeftArrow,           Key_DownArrow,              Key_UpArrow,            Key_RightArrow,              Consumer_Mute,            ___,


   M(ADHOC_MACROS_TOGGLE_RECORDING), M(ADHOC_MACROS_REPEAT_6), M(ADHOC_MACROS_REPEAT_7), M(ADHOC_MACROS_REPEAT_8), M(ADHOC_MACROS_REPEAT_9), M(ADHOC_MACROS_REPEAT_0), Key_Pipe,
   // Thumb Bar
   Key_RightParen, Key_RightCurlyBracket, Key_Enter, Key_RightBracket,
   // Fn
   ___)

};

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  switch (macroIndex) {
    case MACRO_ANY:
      anyKeyMacro(keyState);
      break;
    case ADHOC_MACROS_REPLAY:
      if (keyToggledOff(keyState))
        AdhocMacros.toggleReplay();
      break;
    case ADHOC_MACROS_TOGGLE_RECORDING:
      if (keyToggledOff(keyState))
        AdhocMacros.toggleRecording();
      break;
    case ADHOC_MACROS_RESET_REPEAT: if (keyToggledOff(keyState)) AdhocMacros.resetRepeat(); break;
    case ADHOC_MACROS_REPEAT_1: if (keyToggledOff(keyState)) AdhocMacros.repeatAddDigit(1); break;
    case ADHOC_MACROS_REPEAT_2: if (keyToggledOff(keyState)) AdhocMacros.repeatAddDigit(2); break;
    case ADHOC_MACROS_REPEAT_3: if (keyToggledOff(keyState)) AdhocMacros.repeatAddDigit(3); break;
    case ADHOC_MACROS_REPEAT_4: if (keyToggledOff(keyState)) AdhocMacros.repeatAddDigit(4); break;
    case ADHOC_MACROS_REPEAT_5: if (keyToggledOff(keyState)) AdhocMacros.repeatAddDigit(5); break;
    case ADHOC_MACROS_REPEAT_6: if (keyToggledOff(keyState)) AdhocMacros.repeatAddDigit(6); break;
    case ADHOC_MACROS_REPEAT_7: if (keyToggledOff(keyState)) AdhocMacros.repeatAddDigit(7); break;
    case ADHOC_MACROS_REPEAT_8: if (keyToggledOff(keyState)) AdhocMacros.repeatAddDigit(8); break;
    case ADHOC_MACROS_REPEAT_9: if (keyToggledOff(keyState)) AdhocMacros.repeatAddDigit(9); break;
    case ADHOC_MACROS_REPEAT_0: if (keyToggledOff(keyState)) AdhocMacros.repeatAddDigit(0); break;

    }
  return MACRO_NONE;
}


void setup() {
  Kaleidoscope.setup();
  Kaleidoscope.use(
          &Macros,
          &AdhocMacros);

  // setup macro memory
  static kaleidoscope::AdhocMacros::AdhocKeyEvent adhocMacro[100];
  kaleidoscope::AdhocMacros::adhocMacro = adhocMacro;
  kaleidoscope::AdhocMacros::maxAdhocMacroLength = 100;
}

void loop() {
  Kaleidoscope.loop();
}

