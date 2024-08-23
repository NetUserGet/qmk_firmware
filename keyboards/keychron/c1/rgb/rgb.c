/* Copyright 2024 NetUserGet
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

#include "quantum.h"

bool mode_leds_show = true;

enum MAC_LAYERS {
    MAC_BASE = 0,
    MAC_FN,
};

enum WIN_LAYERS {
    WIN_BASE = 2,
    WIN_FN,
}; /* Taken from the default keymap for readability */

#ifdef DIP_SWITCH_ENABLE
    static void mode_leds_update(void){
        if (mode_leds_show && layer_state_is(WIN_BASE)) {
            gpio_write_pin_high(LED_WIN_PIN);
        } else if (mode_leds_show && layer_state_is(MAC_BASE)) {
            gpio_write_pin_high(LED_MAC_PIN);
        }
    }

    bool dip_switch_update_kb(uint8_t index, bool active) {
        if (!dip_switch_update_user(index, active)) {
            return false;
        }
        if (index == 0) {
            if (active) {
                /* ternary operators are hard for some to consive in their mind  */
                set_single_persistent_default_layer(WIN_BASE); /* default layer switch*/
            }
            set_single_persistent_default_layer(MAC_BASE);
        }

        mode_leds_update();
        return true;
    }

    void keyboard_pre_init_kb(void) {
        // Setup Win & Mac LED Pins as output
        gpio_set_pin_output(LED_WIN_PIN);
        gpio_set_pin_output(LED_MAC_PIN);
    }
#endif // DIP_SWITCH_ENABLE


#ifdef RGB_MATRIX_SLEEP
    void suspend_power_down_kb(void) {
        // Turn leds off
        mode_leds_show = false;
        mode_leds_update();

        rgb_matrix_set_suspend_state(true);
    }

    void suspend_wakeup_init_kb(void) {
        mode_leds_show = true;
        mode_leds_update();

        rgb_matrix_set_suspend_state(false);
    }
#endif // RGB_MATRIX_SLEEP
