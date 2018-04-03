/* -*- mode: c++ -*-
 * Kaleidoscope-Adhoc-Macros -- Adhoc Macros
 * Copyright (C) 2018 Christopher Auer
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

#pragma once

#include <Kaleidoscope.h>
#define ADHOC_MACROS_FLASH_INTERVAL 500
#define ADHOC_MACROS_MAX_REPETITIONS 1000
namespace kaleidoscope {

class AdhocMacros : public KaleidoscopePlugin {
 public:
  typedef struct {
    Key mappedKey;
    byte row;
    byte col;
    uint8_t keyState;
  } AdhocKeyEvent;

  AdhocMacros(void);

  void begin(void) final;


  // macro
  static AdhocKeyEvent* adhocMacro;
  static uint16_t maxAdhocMacroLength;

  // flashing effect
  static byte flashRow;
  static byte flashCol;
  static cRGB flashColor;

  // recording
  static void startRecording();
  static void stopRecording();
  static void toggleRecording();

  // replaying
  static void replay();
  static void abortReplay();
  static void toggleReplay();
  static void repeatAddDigit(int digit);
  static void resetRepeat();


 private:
  static Key record(Key mappedKey, byte row, byte col, uint8_t keyState);
  static void logger(Key mappedKey, byte row, byte col, uint8_t keyState);
  static void loop(bool isPostClear);
  static bool isRecording;
  static bool isReplaying;
  static uint16_t recordingIndex;
  static uint16_t repeat;
};

}

extern kaleidoscope::AdhocMacros AdhocMacros;
