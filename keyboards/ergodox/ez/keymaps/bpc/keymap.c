#include "ergodox.h"
#include "debug.h"
#include "action_layer.h"
#include "sendchar.h"
#include "virtser.h"

#define BASE 0 // default layer
#define SYMB 1 // symbols
#define MDIA 2 // media keys

enum CUSTOM_KEYCODES {
  VRSN,
  RGB_SLD
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* Keymap 0: Basic layer.
     Refer to the layer configuration in the README for a visual layout. 
  */
  // If it accepts an argument (i.e, is a function), it doesn't need KC_.
  // Otherwise, it needs KC_*

  [BASE] = KEYMAP(  // layer 0 : default
                    // left hand
                  KC_ESC,             KC_1,         KC_2,          KC_3,    KC_4,    KC_5,   KC_LEFT,
                  KC_TAB,             KC_QUOT,      KC_COMM,       KC_DOT,  KC_P,    KC_Y,   KC_LEAD,
                  KC_BSPC,            KC_A,         KC_O,          KC_E,    KC_U,    KC_I,
                  KC_LSFT,            KC_SCLN,      KC_Q,          KC_J,    KC_K,    KC_X,   LT(MDIA, KC_EQL),
                  LT(SYMB, KC_GRAVE), KC_QUOT,      LALT(KC_LSFT), KC_LEFT, KC_RGHT,
                  // left thumb cluster
                  ALT_T(KC_APP),  KC_LGUI,
                  KC_HOME,
                  KC_SPC, KC_ENT, CTL_T(KC_END),
                  // right hand
                  KC_RGHT,     KC_6,   KC_7,   KC_8,   KC_9,   KC_0,             KC_BSLS,
                  RCTL(KC_G),  KC_F,   KC_G,   KC_C,   KC_R,   KC_L,             KC_SLSH,
                  KC_D,        KC_H,   KC_T,   KC_N,   KC_S, KC_MINS,
                  RCTL(KC_X),  KC_B,   KC_M,   KC_W,   KC_V, KC_Z,   KC_RSFT,
                  KC_UP,  KC_DOWN,KC_LBRC,KC_RBRC,          MO(SYMB),
                  // right thumb cluster
                  KC_RALT,        CTL_T(KC_ESC),
                  KC_PGUP,
                  KC_PGDN,KC_TAB, KC_SPC
                    ),
  /* Keymap 1: Symbol Layer */
  // SYMBOLS
  [SYMB] = KEYMAP(
                  // left hand
                  VRSN,   KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,
                  KC_TRNS,KC_EXLM,KC_AT,  KC_LCBR,KC_RCBR,KC_PIPE,KC_TRNS,
                  RGB_VAI,KC_HASH,KC_DLR, KC_LPRN,KC_RPRN,KC_GRV,
                  RGB_VAD,KC_PERC,KC_CIRC,KC_LBRC,KC_RBRC,KC_TILD,KC_TRNS,
                  KC_TRNS,RGB_SLD,RGB_MOD,RGB_HUD,RGB_HUI,
                  KC_TRNS,KC_TRNS,
                  KC_TRNS,
                  RGB_TOG,KC_TRNS,KC_TRNS,
                  // right hand
                  KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11, KC_F12, KC_TRNS,
                  KC_TRNS, KC_UP,   KC_7,   KC_8,    KC_9,    KC_ASTR, KC_TRNS,
                  KC_DOWN, KC_4,   KC_5,    KC_6,    KC_MINS, KC_TRNS,
                  KC_TRNS, LSFT(KC_SCLN), KC_1,   KC_2,    KC_3,    KC_PLUS, MO(MDIA),
                  KC_0,    KC_DOT, KC_SLSH, KC_EQL,  KC_TRNS,
                  KC_TRNS, KC_TRNS,
                  KC_TRNS,
                  KC_TRNS, KC_TRNS, KC_TRNS
                  ),
  /* Keymap 2: Media and mouse keys */
  // MEDIA AND MOUSE
  [MDIA] = KEYMAP(
                  RESET,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                  KC_TRNS, KC_TRNS, KC_TRNS, KC_MS_U, KC_TRNS, KC_TRNS, KC_TRNS,
                  KC_TRNS, KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R, KC_TRNS,
                  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                  KC_TRNS, KC_TRNS, KC_TRNS, KC_BTN1, KC_BTN2,
                  KC_TRNS, KC_TRNS,
                  KC_TRNS,
                  KC_TRNS, KC_TRNS, KC_TRNS,
                  // right hand
                  KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                  KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MPLY,
                  KC_TRNS,  KC_TRNS, KC_TRNS, KC_MPRV, KC_MNXT, KC_TRNS, KC_TRNS,
                  KC_VOLU, KC_VOLD, KC_MUTE, KC_TRNS, KC_TRNS,
                  KC_TRNS, KC_TRNS,
                  KC_TRNS,
                  KC_TRNS, KC_TRNS, KC_WBAK
                  ),
};

const uint16_t PROGMEM fn_actions[] = {
  [1] = ACTION_LAYER_TAP_TOGGLE(SYMB)                // FN1 - Momentary Layer 1 (Symbols)
};

uint8_t chord[4] = {0,0,0,0};
uint8_t pressed_count = 0;

void send_chord(void)
{
  for(uint8_t i = 0; i < 4; i++)
  {
    if(chord[i])
      virtser_send(chord[i]);
  }
  virtser_send(0);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
  switch (keycode) {
  case VRSN:
    if (record->event.pressed) {
      SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP);
    }
    return false;
    break;
  case RGB_SLD:
    if (record->event.pressed) {
#ifdef RGBLIGHT_ENABLE
      rgblight_mode(1);
#endif
    }
    return false;
    break;
  default:
    // We need to track keypresses in all modes, in case the user
    // changes mode whilst pressing other keys.
    if (record->event.pressed)
      pressed_count++;
    else
      pressed_count--;
    return true;
  }
  return true;
}

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
  return MACRO_NONE;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
};

LEADER_EXTERNS();

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
    uint8_t layer = biton32(layer_state);

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    switch (layer) {
      // TODO: Make this relevant to the ErgoDox EZ.
        case 1:
            ergodox_right_led_1_on();
            break;
        case 2:
            ergodox_right_led_2_on();
            break;
        default:
            // none
            break;
    }

    LEADER_DICTIONARY() {
      leading = false;
      leader_end();

      SEQ_ONE_KEY(KC_D) {
        ergodox_led_all_off();
        wait_ms(100);
        ergodox_right_led_3_on();
        wait_ms(100);
        ergodox_right_led_2_on();
        wait_ms(100);
        ergodox_right_led_1_on();
        wait_ms(100);
        ergodox_right_led_3_off();
        wait_ms(100);
        ergodox_right_led_2_off();
        wait_ms(100);
        ergodox_right_led_1_off();
        
      }
    }
    };
