/* Copyright 2023 nanamachi, original code by yushakobo
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

#include <stdio.h>

// Defines names for use in layer keycodes and the keymap
enum layer_names {
  _QWERTY = 0,
  _LOWER,
  _RAISE,
  _ADJUST
};

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

void render_mode_status(void) {
  // Host Keyboard Layer Status
  oled_write_P(PSTR("MODE:\n"), false);

  switch (get_highest_layer(layer_state)) {
      case _QWERTY:
          oled_write_P(PSTR("qwu-,"), false);
          break;
      case _LOWER:
          oled_write_P(PSTR("qwert"), true);
          break;
      case _RAISE:
          oled_write_P(PSTR("LWR"), false);
          oled_write_P(PSTR("\x03\x04"), false);
          break;
      case _ADJUST:
          oled_write_P(PSTR("\x05\x06"), false);
          oled_write_P(PSTR("UPR"), false);
          break;
      default:
          // Or use the write_ln shortcut over adding '\n' to the end of your string
          oled_write_P(PSTR("undef"), false);
  }
}

void render_lock_status(void) {
  // Host Keyboard LED Status
  led_t led_state = host_keyboard_led_state();
  oled_write_P(led_state.num_lock ? PSTR("NumLk") : PSTR("     "), false);
  oled_write_P(led_state.caps_lock ? PSTR("CpsLk") : PSTR("     "), false);
  oled_write_P(led_state.scroll_lock ? PSTR("ScrLk") : PSTR("     "), false);
}


static void render_logo(void) {
    static const char PROGMEM nanamachi_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0x00
    };

    oled_write_P(nanamachi_logo, false);
}

static void render_rgbled_status(bool full) {
#ifdef RGBLIGHT_ENABLE
  char buf[24];
  if (RGBLIGHT_MODES > 1 && rgblight_is_enabled()) {
      if (full) {
          snprintf(buf, sizeof(buf), "M| %2dH| %2dS| %2dV| %2d",
                   rgblight_get_mode(),
                   rgblight_get_hue()/RGBLIGHT_HUE_STEP,
                   rgblight_get_sat()/RGBLIGHT_SAT_STEP,
                   rgblight_get_val()/RGBLIGHT_VAL_STEP);
      } else {
          snprintf(buf, sizeof(buf), "[%2d] ", rgblight_get_mode());
      }
  } else {
      snprintf(buf, sizeof(buf), "      LED  OFF      ");
  }
  oled_write(buf, false);
#endif
}

bool oled_task_user(void) {
  if(is_keyboard_master()){
    render_logo();
      oled_write_P(PSTR("\n-----\n"), false);
    render_mode_status();
      oled_write_P(PSTR("\n-----\n"), false);

    if (get_highest_layer(layer_state) == _RAISE) {
      render_rgbled_status(true);
    } else {
      render_lock_status();
      oled_write_P(PSTR("     "), false);
    }
  }else{
    render_logo();
    render_rgbled_status(true);
  }
    return false;
}
#endif
