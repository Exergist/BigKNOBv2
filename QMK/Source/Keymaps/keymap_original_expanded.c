// Original source code from vendor (LeafCutterLabs) with:
//   • Expanded new line and tab formatting
//   • Comments
// Rename this file as 'keymap.c' prior to compiling

#include QMK_KEYBOARD_H

#define _MAIN 0

uint8_t selected_layer = 0; // Initialization for selected_layer

// Create custom keycodes via enumeration
enum custom_keycodes
{
    CTRLF13 = SAFE_RANGE,
	CTRLF14,
	CTRLF15,
	CTRLF16,
	CTRLF17
};

// Define behavior for custom keycodes and process other keycodes normally
bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    switch (keycode) // Switch statement for handling triggering keycodes
	{
		case CTRLF13: // Triggering keycode is the (custom) CTRLF13
			if (record->event.pressed)  // Check if the triggering action was a physical key "press" (down)
			{
				register_code(KC_LCTL); // Send the Left Control keycode "down" event
				SEND_STRING(SS_TAP(X_F13)); // Send event to "tap" (down and up events) the F13 keycode
				unregister_code(KC_LCTL); // Send the Left Control keycode "up" event
			} 
			else // Triggering action was a physical key "release" (up)
			{
				// when keycode QMKBEST is released
			}
			break; // Break out of switch statement
		case CTRLF14:
			if (record->event.pressed)
			{
				register_code(KC_LCTL);
				SEND_STRING(SS_TAP(X_F14));
				unregister_code(KC_LCTL);
			}
			else
			{
				// when keycode QMKBEST is released
			}
			break;
		case CTRLF15:
			if (record->event.pressed)
			{
				register_code(KC_LCTL);
				SEND_STRING(SS_TAP(X_F15));
				unregister_code(KC_LCTL);
			}
			else
			{
				// when keycode QMKBEST is released
			}
			break;
		case CTRLF16:
			if (record->event.pressed)
			{
				register_code(KC_LCTL);
				SEND_STRING(SS_TAP(X_F16));
				unregister_code(KC_LCTL);
			}
			else
			{
				// when keycode QMKBEST is released
			}
			break;
    }
    return true; // Effectively process ALL keycodes "normally"
};

// Define behavior for rotation of the encoder
void encoder_update_user(uint8_t index, bool clockwise)
{
	if (index == 0) // Check if triggering control was "first" encoder
	{ 
		if (clockwise) // Check if encoder was rotated clockwise
		{
			tap_code(KC_VOLU); // Increase system volume
		}
		else
		{
			tap_code(KC_VOLD); // Decrease system volume
		}
	} 
}

#define RBG_VAL 120
// Executes every time layer is changed
layer_state_t layer_state_set_user(layer_state_t state)
{
	switch(biton32(state)) // Handle various layer levels
	{
		case 1:
			rgblight_sethsv(HSV_GREEN); // Set all RGB backlights to green
			break;
		case 2: 
			rgblight_sethsv(HSV_BLUE); // Set all RGB backlights to blue
			break;
		default:
			rgblight_sethsv(HSV_RED); // Set all RGB backlights to red
			break;
	}
	return state; // Return current layer (state)
}

// Defines the keyboard layers and their associated keycodes
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = { //buttion closest to usb is first
	[_MAIN] = LAYOUT(
		TO(1), KC_F14, KC_F15, KC_F16, KC_F17 // Activate Layer 1, F14, F15, F16, F17
	),
	[1] = LAYOUT(
		TO(2), KC_F19, KC_F20, KC_F21, KC_F22 // Activate Layer 2, F19, F20, F21, F22
	),
	[2] = LAYOUT(
		//TO(0), KC_A, KC_B, KC_C, KC_D
		TO(0), CTRLF13, CTRLF14, CTRLF15, CTRLF16 // Activate Layer 0, CTRLF13, CTRLF14, CTRLF15, CTRLF16
	),
};