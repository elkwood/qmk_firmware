/* Copyright 2018 REPLACE_WITH_YOUR_NAME
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
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
#include QMK_KEYBOARD_H
#include "analog.c"
#include <print.h>

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
  QMKBEST = SAFE_RANGE,
  QMKURL
};

// Joystick
// Set Pins
uint8_t xPin  = 2;   // VRx / /B4
uint8_t yPin  = 3;   // VRy // B5

// Set Parameters
uint16_t minAxisValue = 0;
// uint16_t maxAxisValue = 1023;
uint16_t maxAxisValue = 1023;
uint16_t floatTollerance = 10;

int8_t xPolarity = 1;
int8_t yPolarity = 1;

uint16_t xOrigin, yOrigin;
 

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6,
        KC_TAB,  KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y,
        KC_CAPS,   KC_A, KC_S, KC_D, KC_F, KC_G,
        KC_U, KC_LSHIFT, KC_Z, KC_X, KC_C, KC_V, KC_B,
        KC_LCTRL,  KC_J, KC_K, KC_LALT
    )
};

uint16_t processPin(uint8_t pin) {
  uint16_t pos = analogRead(pin);
  return pos;
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;dx
  xOrigin = processPin(xPin);
  yOrigin = processPin(yPin);
}

void matrix_init_keymap(void) {
    // Account for drift
    
}

uint16_t axisCoordinate(uint8_t pin, uint16_t origin) {

    int16_t position = processPin(pin);
    uint16_t range;
    range = minAxisValue;
    
    uint16_t rangedVal = -127*fminf(1.f, (position - (float)origin) / (range - (float)origin));
    // dprintf("ranged --- %d , %d \n", (position - (float)origin), position );
    if (rangedVal > 0){
      //the value is in the higher range
      range = maxAxisValue;
      rangedVal = 127*fminf(1.f, (position - (float)origin) /(range - (float)origin));
    }
    return rangedVal;
}
 
void matrix_scan_user(void) {
  // processNewRanges();
  uint16_t x = axisCoordinate(xPin, xOrigin);
  uint16_t y = axisCoordinate(yPin, yOrigin);
  // dprintf("%" PRIu16 ",", x);
  // dprintf("%" PRIu16 "\n", y);
  int result = atan2(y, x);
  // result = result * 180.0/M_PI;
  dprintf("x:%4d,y:%4d -- %4d", x,y, result );
  dprint("\n");
  // int16_t pos = analogRead(xPin);
  //   dprintf("%" PRIu16 "\n", pos);
}