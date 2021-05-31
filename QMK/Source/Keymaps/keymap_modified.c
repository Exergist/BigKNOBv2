// Modified from original source code with:
//   • Layer cycling via encoder
//   • Expanded new line and tab formatting
//   • Comments
// Rename this file as 'keymap.c' prior to compiling

#include QMK_KEYBOARD_H

#define _MAIN 0
#define RBG_VAL 120

// Global variable initialization
uint8_t selected_layer = 0;
int layer_buffer = 0; // Counter for encoder input events
int layer_cycle_threshold = 6; // Number of encoder input events before layer cycle is viable

// Create custom keycodes via enumeration
enum custom_keycodes
{
    CTRLF13 = SAFE_RANGE,
	CTRLF14,
	CTRLF15,
	CTRLF16,
	CTRLF17
};

// Defines the keyboard layers and their associated keycodes
// Button closest to usb is first
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
{
	[_MAIN] = LAYOUT // Layer 0
	(
		TO(1), KC_F14, KC_F15, KC_F16, KC_F17 // Activate Layer 1, F14, F15, F16, F17
	),
	[1] = LAYOUT // Layer 1
	(
		TO(2), KC_F19, KC_F20, KC_F21, KC_F22 // Activate Layer 2, F19, F20, F21, F22
	),
	[2] = LAYOUT // Layer 2
	(
		TO(0), CTRLF13, CTRLF14, CTRLF15, CTRLF16 // Activate Layer 0, CTRLF13, CTRLF14, CTRLF15, CTRLF16
	),
};

// Executes every time layer is changed
layer_state_t layer_state_set_user(layer_state_t state)
{
	layer_buffer = 0;
	selected_layer = biton32(state);
	switch(selected_layer) // Handle various layer levels
	{
		case 0:
			rgblight_sethsv(HSV_RED); // LAYER 0: Set all RGB backlights to red
			break;
		case 1:
			rgblight_sethsv(HSV_GREEN); // LAYER 1: Set all RGB backlights to green
			break;
		case 2: 
			rgblight_sethsv(HSV_BLUE); // LAYER 2: Set all RGB backlights to blue
			break;
		default:
			rgblight_sethsv(HSV_YELLOW); // LAYER UNKNOWN: Set all RGB backlights to yellow
			break;
	}
	return state; // Return current layer (state)
}

int highest_layer_number = sizeof(keymaps)/sizeof(keymaps[0]) - 1; // Calculate highest layer number

// Define behavior for custom keycodes and process other keycodes normally
bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    switch (keycode) // Switch statement for handling triggering keycodes
	{
		case CTRLF13: // Triggering keycode is the (custom) CTRLF13
			if (record->event.pressed)  // Check if the triggering action was a physical key "press" (down)
				tap_code16(LCTL(KC_F13)); // Send command to "tap" (down and up events) the Left Control and F13 keys
			else // Triggering action was a physical key "release" (up)
			{
				// when keycode QMKBEST is released
			}
			break; // Break out of switch statement
		case CTRLF14:
			if (record->event.pressed)
				tap_code16(LCTL(KC_F14)); // Send command to "tap" (down and up events) the Left Control and F14 keys
			else
			{
				// when keycode QMKBEST is released
			}
			break;
		case CTRLF15:
			if (record->event.pressed)
				tap_code16(LCTL(KC_F15)); // Send command to "tap" (down and up events) the Left Control and F15 keys
			else
			{
				// when keycode QMKBEST is released
			}
			break;
		case CTRLF16:
			if (record->event.pressed)
				tap_code16(LCTL(KC_F16)); // Send command to "tap" (down and up events) the Left Control and F16 keys
			else
			{
				// when keycode QMKBEST is released
			}
			break;
    }
    return true; // Effectively process ALL keycodes "normally"
};

/* // Define behavior for rotation of the encoder
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
} */

// Define behavior for rotation of the encoder
void encoder_update_user(uint8_t index, bool clockwise)
{
	if (index == 0)
	{
		// Increment/decrement the layer_buffer
		if (clockwise)
		{
			if (layer_buffer < 0)
				layer_buffer = 0;
			layer_buffer++;
		}
		else
		{
			if (layer_buffer > 0)
				layer_buffer = 0;
			layer_buffer--;
		}
		
		// Cycle layer if appropriate
		if (abs(layer_buffer) >= layer_cycle_threshold) // Check if layer_buffer grew beyond layer_cycle_threshold
		{
			if (layer_buffer > 0)
			{
				///tap_code(KC_X); // debug
				layer_buffer = 0;
				if (selected_layer < highest_layer_number)
					selected_layer++;
				else
					selected_layer = 0;
				layer_move(selected_layer);
			}
			else
			{
				///tap_code(KC_Y); // debug
				layer_buffer = 0;
				if (selected_layer > 0)
					selected_layer--;
				else
					selected_layer = highest_layer_number;
				layer_move(selected_layer);
			}
		}
	}
}