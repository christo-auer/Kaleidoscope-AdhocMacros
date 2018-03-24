/* -*- mode: c++ -*-
 * Kaleidoscope-KeyLogger -- A key logger
 * Copyright (C) 2016, 2017  Gergely Nagy
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

#include <Kaleidoscope-KeyLogger.h>

namespace kaleidoscope {

KeyLogger::KeyLogger(void) {
}

void KeyLogger::begin(void) {
  Serial.begin(9600);
  Kaleidoscope.useEventHandlerHook(this->logger);
}

Key KeyLogger::logger(Key mapped_key, byte row, byte col, uint8_t key_state) {
  if (!keyToggledOn(key_state) && !keyToggledOff(key_state))
    return mapped_key;
  if (key_state & INJECTED)
    return mapped_key;

  Serial.print(F("KL: row="));
  Serial.print(row, DEC);
  Serial.print(F(", col="));
  Serial.print(col, DEC);
  Serial.print(F(", pressed="));
  Serial.print(keyToggledOn(key_state), DEC);
  Serial.print(F(", defaultLayer="));
  Serial.print(Layer.defaultLayer(), DEC);
  Serial.print(F(", layerState="));
  Serial.print(Layer.getLayerState(), BIN);
  Serial.print(F(", mapped_key.flags="));
  Serial.print(mapped_key.flags, BIN);
  Serial.print(F(", mapped_key.keyCode="));
  Serial.println(mapped_key.keyCode, HEX);

  return mapped_key;
}

}

kaleidoscope::KeyLogger KeyLogger;
