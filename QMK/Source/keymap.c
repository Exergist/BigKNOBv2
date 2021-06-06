//{ NOTES

// Keymap for bigKNOBv2 by LeafCutterLabs

// Modified from original source code with:
//   • Layer cycling via encoder rotation
//   • Layer advancing via encoder tap
//   • RGB LEDs toggle on/off via encoder press-hold-release
//   • Restructured layout and code organization
//   • Expanded new line and tab formatting
//   • Comments

//}

//{ INCLUDE & DEFINE

#include QMK_KEYBOARD_H

#define RBG_VAL 120

//}

//{ GLOBAL VARIABLES

uint8_t selected_layer = 0; // Currently selected layer
int encoder_buffer = 0; // Counter for encoder input events
int encoder_threshold = 6; // Number of encoder input events (steps) before layer may cycle
uint16_t key_timer; // Variable for tracking elapsed time

//}

//{ METHOD DECLARATION

void NextLayer(void);
void PreviousLayer(void);
void ChangeLedColor(void);
void ToggleRgbLED(void);

//}

//{ CUSTOM ENUMERATION

// Custom keycode declarations
enum custom_keycodes
{
	CTRLF13 = SAFE_RANGE,
	CTRLF14,
	CTRLF15,
	CTRLF16,
	NextLayer_ToggleLED
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
		NextLayer_ToggleLED, KC_F14, KC_F15, KC_F16, KC_F17 // [Tap = Activate Next Layer, Press-Hold-Release = Toggle RGB LEDs], F14, F15, F16, F17
	),
	[_SECONDARY] = LAYOUT // Layer 1
	(
		NextLayer_ToggleLED, KC_F19, KC_F20, KC_F21, KC_F22 // [Tap = Activate Next Layer, Press-Hold-Release = Toggle RGB LEDs], F19, F20, F21, F22
	),
	[_TERTIARY] = LAYOUT // Layer 2
	(
		NextLayer_ToggleLED, CTRLF13, CTRLF14, CTRLF15, CTRLF16 // [Tap = Activate Next Layer, Press-Hold-Release = Toggle RGB LEDs], CTRLF13, CTRLF14, CTRLF15, CTRLF16
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
	ChangeLedColor(); // Call method to change RGB LEDs color based on current layer
	return state; // Return current layer (state)
}

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
				NextLayer(); // Call method to move to next layer
			else
				PreviousLayer(); // Call method to move to previous layer
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
		case NextLayer_ToggleLED:
			if (record->event.pressed) // Check if the triggering action was a physical key "press" (down)
				key_timer = timer_read(); // Start timer
			else
			{
				if (timer_elapsed(key_timer) < TAPPING_TERM) // Check if elapsed (timer) time is less than TAPPING_TERM
					NextLayer(); // Call method to move to next layer
				else
					ToggleRgbLED(); // Call method to toggle the RGB LEDs on/off
			}
			break;
	}
	return true; // Effectively process ALL keycodes "normally"
};

//}

//{ METHODS

// Method for moving to next layer in the stack
void NextLayer(void)
{
	int moveLayer = selected_layer;
	if (moveLayer < highest_layer_number)
		moveLayer++;
	else
		moveLayer = 0;
	layer_move(moveLayer); // Shift to moveLayer
}

// Method for moving to the previous layer in the stack
void PreviousLayer(void)
{
	int moveLayer = selected_layer;
	if (moveLayer > 0)
		moveLayer--;
	else
		moveLayer = highest_layer_number;
	layer_move(moveLayer); // Shift to moveLayer
}

// Method for toggling RGB LEDs on/off
void ToggleRgbLED(void)
{
	rgblight_toggle(); // Toggle the RGB LEDs on/off
	if (rgblight_is_enabled() == true) // Check if RGB LEDs are ON
		ChangeLedColor(); // Call method to change RGB LEDs color based on current layer
}

// Method for changing RGB LEDs color based on current layer
void ChangeLedColor(void)
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
	
//}