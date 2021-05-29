// Original code from vendor
// Rename this file as 'keymap.c' prior to compiling

#include QMK_KEYBOARD_H

#define _MAIN 0

uint8_t selected_layer = 0;

enum custom_keycodes {
    CTRLF13 = SAFE_RANGE,
	CTRLF14,
	CTRLF15,
	CTRLF16,
	CTRLF17,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case CTRLF13:
        if (record->event.pressed) {
            register_code(KC_LCTL);
			SEND_STRING(SS_TAP(X_F13));
			unregister_code(KC_LCTL);
        } else {
            // when keycode QMKBEST is released
        }
        break;
	case CTRLF14:
        if (record->event.pressed) {
            register_code(KC_LCTL);
			SEND_STRING(SS_TAP(X_F14));
			unregister_code(KC_LCTL);
        } else {
            // when keycode QMKBEST is released
        }
        break;
	case CTRLF15:
        if (record->event.pressed) {
            register_code(KC_LCTL);
			SEND_STRING(SS_TAP(X_F15));
			unregister_code(KC_LCTL);
        } else {
            // when keycode QMKBEST is released
        }
        break;
	case CTRLF16:
        if (record->event.pressed) {
            register_code(KC_LCTL);
			SEND_STRING(SS_TAP(X_F16));
			unregister_code(KC_LCTL);
        } else {
            // when keycode QMKBEST is released
        }
        break;
    }
    return true;
};

void encoder_update_user(uint8_t index, bool clockwise) {
  if (index == 0) { 
    if (clockwise) {
      tap_code(KC_VOLU);
    } else {
      tap_code(KC_VOLD);
    }
  } 
}

#define RBG_VAL 120
layer_state_t layer_state_set_user(layer_state_t state) {
  switch(biton32(state)) {
  case 1:
    // violet
    rgblight_sethsv(HSV_GREEN);
    break;
  case 2:
    // green
    rgblight_sethsv(HSV_BLUE);
    break;
  default:
    // dark blue
    rgblight_sethsv(HSV_RED);
    break;
  }
  return state;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = { //buttion closest to usb is first
  [_MAIN] = LAYOUT(
     TO(1), KC_F14, KC_F15, KC_F16, KC_F17 
  ),
  [1] = LAYOUT(
     TO(2), KC_F19, KC_F20, KC_F21, KC_F22
  ),
  [2] = LAYOUT(
     //TO(0), KC_A, KC_B, KC_C, KC_D
	 TO(0), CTRLF13, CTRLF14, CTRLF15, CTRLF16
  ),
};
