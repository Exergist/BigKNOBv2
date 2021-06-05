//{ NOTES

// Keymap for bigKNOBv2 by LeafCutterLabs

// Modified from original source code with:
//   • Layer cycling via encoder rotation
//   • Layer advancing via encoder single-tap
//   • RGB LED toggle on/off via encoder double-tap
//   • Restructured layout and code organization
//   • Expanded new line and tab formatting
//   • Comments
// Rename this file as 'keymap.c' prior to compiling

//}

//{ INCLUDE & DEFINE

#include QMK_KEYBOARD_H

#define RBG_VAL 120

//}

//{ GLOBAL VARIABLES

uint8_t selected_layer = 0; // Currently selected layer
int encoder_buffer = 0; // Counter for encoder input events
int encoder_threshold = 6; // Number of encoder input events before layer may cycle

//}

//{ CUSTOM ENUMERATION

// Custom keycode declarations
enum custom_keycodes
{
    CTRLF13 = SAFE_RANGE,
	CTRLF14,
	CTRLF15,
	CTRLF16,
	CTRLF17
};

// Tap Dance declarations
enum tap_dance
{
	TD_NextLayer_ToggleLED
};

// Layer declarations
enum layers
{ 
    _PRIMARY,
    _SECONDARY,
    _TERTIARY
};

//}

//{ KEYMAP

// Defines the keyboard layers and their associated keycodes
// Button closest to usb is first
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
{
	[_PRIMARY] = LAYOUT // Layer 0
	(
		TD(TD_NextLayer_ToggleLED), KC_F14, KC_F15, KC_F16, KC_F17 // [Single-tap = Activate Next Layer, Double-tap = toggle RGB LEDs], F14, F15, F16, F17
	),
	[_SECONDARY] = LAYOUT // Layer 1
	(
		TD(TD_NextLayer_ToggleLED), KC_F19, KC_F20, KC_F21, KC_F22 // [Single-tap = Activate Next Layer, Double-tap = toggle RGB LEDs], F19, F20, F21, F22
	),
	[_TERTIARY] = LAYOUT // Layer 2
	(
		TD(TD_NextLayer_ToggleLED), CTRLF13, CTRLF14, CTRLF15, CTRLF16 // [Single-tap = Activate Next Layer, Double-tap = toggle RGB LEDs], CTRLF13, CTRLF14, CTRLF15, CTRLF16
	),
};

//}

//{ LAYERS

// Calculate highest layer number
int highest_layer_number = sizeof(keymaps)/sizeof(keymaps[0]) - 1;

// Executes every time layer is changed
layer_state_t layer_state_set_user(layer_state_t state)
{
	encoder_buffer = 0;
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

//}

//{ TAP DANCE

// Method run when Tap Dance action finishes
void dance_cln_finished(qk_tap_dance_state_t *state, void *user_data)
{
    if (state->count == 1) // Check if single-tap was received
	{
		int moveLayer = selected_layer;
		if (moveLayer < highest_layer_number)
			moveLayer++;
		else
			moveLayer = 0;
		layer_move(moveLayer); // Shift to moveLayer
    }
	else
	{
		rgblight_toggle(); // Toggle the RGB LEDs
		if (rgblight_is_enabled() == true) // Check if RGB LEDs are ON
		{
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
		}
    }
}

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] =
{
	[TD_NextLayer_ToggleLED] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_cln_finished, NULL)
};

//}

//{ ENCODER

// Define behavior for rotation of the encoder
void encoder_update_user(uint8_t index, bool clockwise)
{
	if (index == 0)
	{
		// Increment/decrement the encoder_buffer
		if (clockwise)
		{
			if (encoder_buffer < 0)
				encoder_buffer = 0;
			encoder_buffer++;
		}
		else
		{
			if (encoder_buffer > 0)
				encoder_buffer = 0;
			encoder_buffer--;
		}
		
		// Cycle layer if appropriate
		if (abs(encoder_buffer) >= encoder_threshold) // Check if encoder_buffer grew beyond encoder_threshold
		{
			if (encoder_buffer > 0)
			{
				if (selected_layer < highest_layer_number)
					selected_layer++;
				else
					selected_layer = 0;
				layer_move(selected_layer); // Shift to selected_layer
			}
			else
			{
				if (selected_layer > 0)
					selected_layer--;
				else
					selected_layer = highest_layer_number;
				layer_move(selected_layer); // Shift to selected_layer
			}
		}
	}
}

//}

//{ CUSTOM KEYCODES

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

//}