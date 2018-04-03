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
#include <Kaleidoscope-AdhocMacros.h>
#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>
#include <LEDUtils.h>

namespace kaleidoscope {

#define CHECK_ADHOC_MACRO_ARRAY if (adhocMacro == NULL || maxAdhocMacroLength == 0) return;

// public setitngs
AdhocMacros::AdhocKeyEvent* AdhocMacros::adhocMacro = NULL;
uint16_t AdhocMacros::maxAdhocMacroLength = 0;
cRGB AdhocMacros::flashColor = CRGB(255, 0, 0);
byte AdhocMacros::flashRow = 0;
byte AdhocMacros::flashCol = 0;

// private state
bool AdhocMacros::isRecording = false;
bool AdhocMacros::isReplaying = false;
uint16_t AdhocMacros::recordingIndex = 0;
uint16_t AdhocMacros::repeat = 0;

AdhocMacros::AdhocMacros(void) {
}

void AdhocMacros::begin(void) {
    // register callbacks
    Kaleidoscope.useEventHandlerHook(this->record);
    Kaleidoscope.useLoopHook(this->loop);
  Serial.begin(9600);
}

void AdhocMacros::loop(bool isPostClear) {
    CHECK_ADHOC_MACRO_ARRAY;

    if (!isPostClear)
        return;

    // if we are recording, flash the LED
    if (isRecording){
        cRGB currentColor = 
            ((millis()/ADHOC_MACROS_FLASH_INTERVAL)%2 == 0) ? flashColor : CRGB(0,0,0);
        ::LEDControl.setCrgbAt(flashRow,flashCol,currentColor);
    }

    // if we are not replaying, we have nothing to do
    if (!isReplaying)
        return;


    // some repetitions to replay?
    if (repeat > 0){
        // play one full cycle
        uint16_t replayingIndex = 0;
        kaleidoscope::hid::releaseAllKeys();
        while (replayingIndex < recordingIndex){

            // get the key event
            AdhocKeyEvent* key = &adhocMacro[replayingIndex++];

            // key toggled on?
            if (keyToggledOn(key->keyState)){
                // turn on LED
                ::LEDControl.setCrgbAt(key->row, key->col, flashColor);
            } else if (keyToggledOff(key->keyState)){
                // turn off LED
                ::LEDControl.refreshAt(key->row, key->col);
            }

            // sync LEDs
            ::LEDControl.syncLeds();

            handleKeyswitchEvent(key->mappedKey, key->row, key->col, key->keyState | INJECTED);

            // flush
            kaleidoscope::hid::sendKeyboardReport();
        }

        kaleidoscope::hid::releaseAllKeys();

        // one repetition less
        repeat--;
    }else{
        // no repetitions left
        abortReplay();
    }
}

// callback for a key event, records keys
Key AdhocMacros::record(Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (adhocMacro == NULL || maxAdhocMacroLength == 0) 
        return mappedKey;

    // nothing happened?
    if (!keyToggledOn(keyState) && !keyToggledOff(keyState))
        return mappedKey;

    // if we are not recording, there is nothing left to do here
    if (!isRecording)
        return mappedKey;

    // we wait with recording until a key is toggled on
    if (recordingIndex == 0 && keyToggledOff(keyState))
        return mappedKey;

    // record the key event
    adhocMacro[recordingIndex++] = { mappedKey, row, col, keyState };

    // TODO fix this: only release key after clear in loop
    // insert an artificial key release to simulate Keyboardio's behaviour of
    // cleaning the keys each loop
    if (keyState & INJECTED && keyToggledOn(keyState)){
        adhocMacro[recordingIndex++] = { mappedKey, row, col, WAS_PRESSED | INJECTED };
    }

    // abort recording, if no space is left
    // the "-1" is there because per call of record, there can be
    // two array entries
    if (recordingIndex >= maxAdhocMacroLength-1){
        stopRecording();
    }

    return mappedKey;
}

// resets repeat counter
void AdhocMacros::resetRepeat(){
    repeat = 0;
}

// adds a single digit to the repeat counter
void AdhocMacros::repeatAddDigit(int digit){
    repeat *= 10;
    repeat += digit;
    repeat = repeat <= ADHOC_MACROS_MAX_REPETITIONS ? repeat : ADHOC_MACROS_MAX_REPETITIONS;
}


// turns recording on
void AdhocMacros::startRecording() {
    CHECK_ADHOC_MACRO_ARRAY;

    // if already recording to replaying, get out
    if (isRecording || isReplaying)
        return;

    // turn recording on
    isRecording = true;

    // reset recorder
    recordingIndex = 0;
}

// stops recording
void AdhocMacros::stopRecording() {
    CHECK_ADHOC_MACRO_ARRAY;

    // if not recording to replaying, get out
    if (!isRecording || isReplaying)
        return;

    // stop recording
    isRecording = false;

    // remove from the back of the recording sequence all key events in which
    // a key was turned on
    while (recordingIndex > 0 && keyToggledOn(adhocMacro[recordingIndex-1].keyState)){
        recordingIndex--;
    }

    // clear flashing LED
    ::LEDControl.refreshAt(flashRow,flashCol);
}


// toggles between recording and not-recording
// useful to use a single key to start and stop recording
void AdhocMacros::toggleRecording() {
    CHECK_ADHOC_MACRO_ARRAY;

    // do nothing, if we are replaying
    if (isReplaying)
        return;

    // toggle
    if (isRecording)
        stopRecording();
    else
        startRecording();
}

// starts replaying
void AdhocMacros::replay() {
    CHECK_ADHOC_MACRO_ARRAY;

    // if we are recording or already replaying, do nothing
    if (isRecording || isReplaying)
        return;

    // start replaying
    isReplaying = true;

    // if the repeat counter is still 0, set it to 1 for at least
    // one repetition
    if (repeat == 0) 
        repeat = 1;
}

// aborts replaying; note the current repetition is still fully executed
void AdhocMacros::abortReplay() {
    CHECK_ADHOC_MACRO_ARRAY;

    if (isRecording || !isReplaying)
        return;
    isReplaying = false;
    repeat = 0;
}

// toggles between replaying and not replaying; useful for using a
// single key 
void AdhocMacros::toggleReplay(){
    CHECK_ADHOC_MACRO_ARRAY;

    // if we are recording, do nothing
    if (isRecording)
        return;

    // toggle
    if (isReplaying)
        abortReplay();
    else
        replay();
}

}


kaleidoscope::AdhocMacros AdhocMacros;
