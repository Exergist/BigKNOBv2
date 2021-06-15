/*
//    _       _           _  __  _   _    ____    ____
//   | |     (_)         | |/ / | \ | |  / __ \  |  _ \
//   | |__    _    __ _  | ' /  |  \| | | |  | | | |_) |
//   | '_ \  | |  / _` | |  <   | . ` | | |  | | |  _ <
//   | |_) | | | | (_| | | . \  | |\  | | |__| | | |_) |
//   |_.__/  |_|  \__, | |_|\_\ |_| \_|  \____/  |____/
//                 __/ |                       Version 2
//                |___/    
*/

//{ NOTES

// Custom keymap for bigKNOBv2 by Exergist (2021)
// Modified from original source code with:
//   • Layer cycling via encoder rotation
//   • Next layer via encoder single-tap
//   • Previous layer via encoder press-hold
//   • RGB LEDs toggle on/off via encoder double-tap
//   • Raw HID bicommunication with C# application on host computer
//   • RGB LED 'ErrorFlash' to indicate issues
//   • RGB LED 'LightFlash' for general indication and debugging
//   • Harmonized function key mapping
//   • Restructured layout and code organization
//   • Expanded new line and tab formatting
//   • Comments

//}

//{ ACKNOWLEDGEMENTS

// bigKNOBv2 hardware and original source code by Craig Gardner (https://github.com/LeafCutterLabs)
// ASCII art by patorjk (https://patorjk.com/software/taag/)

//}

//{ INCLUDE & DEFINE

#include QMK_KEYBOARD_H

#include "raw_hid.h"

#define RBG_VAL 120

//}

//{ GLOBAL VARIABLES

uint8_t selected_layer = 0; // Currently selected layer
int encoder_buffer = 0; // Counter for encoder input events
int encoder_threshold = 6; // Number of encoder input events (steps) before layer may cycle

//}

//{ METHOD DECLARATION

void NextLayer(void);
void PreviousLayer(void);
void MoveToLayer(int layer);
void ChangeLedColor(void);
void ToggleRgbLED(void);
void ErrorFlash(void);
void LightFlash(void);
void SendHID(void);

//}

//{ CUSTOM KEYCODE DECLARATION

enum custom_keycodes
{
	KC_SendHID = SAFE_RANGE
	/* CTRLF13 = SAFE_RANGE,
	CTRLF14,
	CTRLF15,
	CTRLF16 */
};

//}

//{ LAYERS

// Layer declarations
enum layers
{ 
	_PRIMARY,
	_SECONDARY,
	_TERTIARY
};

// Method executed every time active layer is changed
layer_state_t layer_state_set_user(layer_state_t state)
{
	encoder_buffer = 0;
	selected_layer = biton32(state);
	ChangeLedColor(); // Call method to change RGB LEDs color based on current layer
	return state; // Return current layer (state)
}

//}

//{ TAP DANCE

//{ Tap Dance Declarations

// Tap Dance identifier declaration
enum
{
	ENCODER_DANCE
};

// Tap Dance action types declaration
typedef enum
{
	TD_NONE,
	TD_UNKNOWN,
	TD_SINGLE_TAP,
	TD_SINGLE_HOLD,
	TD_DOUBLE_TAP
} td_state_t;

// Tap Dance state information Struct declaration
typedef struct
{
	bool is_press_action;
	td_state_t state;
} td_tap_t;

//}

//{ Tap Dance Definitions

// Method for retrieving requested Tap Dance state based on user input
td_state_t cur_dance(qk_tap_dance_state_t *state)
{
    if (state->count == 1) // Single-tap
	{
        if (state->interrupted || !state->pressed)
			return TD_SINGLE_TAP;
        else // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
			return TD_SINGLE_HOLD;
    }
	else if (state->count == 2) // Double-tap
		return TD_DOUBLE_TAP;
	else
		return TD_UNKNOWN;
}

// Create an instance of 'td_tap_t' for the 'encoder' tap dance
static td_tap_t encoderTap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

// Method run when Tap Dance action finishes
void encoder_finished(qk_tap_dance_state_t *state, void *user_data)
{
    encoderTap_state.state = cur_dance(state);
    switch (encoderTap_state.state)
	{
        	case TD_SINGLE_TAP:
			NextLayer(); // Call method to move to next layer
			break;
        	case TD_SINGLE_HOLD:
			PreviousLayer(); // Call method to move to previous layer
			break;
        	case TD_DOUBLE_TAP:
			ToggleRgbLED(); // Call method to toggle the RGB LEDs on/off
			break;
		case TD_NONE:
			// do nothing
			break;
		case TD_UNKNOWN:
			// do nothing
			break;
    }
	encoderTap_state.state = TD_NONE; // Reset encoderTap_state
}

/* // Method run when Tap Dance action resets
void encoder_reset(qk_tap_dance_state_t *state, void *user_data)
{
	switch (encoderTap_state.state)
	{
        	case TD_SINGLE_TAP:
			break;
        	case TD_SINGLE_HOLD:
			break;
        	case TD_DOUBLE_TAP:
			break;
		case TD_NONE:
			// do nothing
			break;
		case TD_UNKNOWN:
			// do nothing
			break;
	}
    encoderTap_state.state = TD_NONE;
} */

// Tap Dance action definition
qk_tap_dance_action_t tap_dance_actions[] = {
    [ENCODER_DANCE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, encoder_finished, NULL)
	///[ENCODER_DANCE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, encoder_finished, encoder_reset)
};

//}

//}

//{ KEYMAP

//{ Layout
//               .-------------------------------------------------------------------------------.
//               |      _.-""""-._                                                               |
//               |    .'          `.   .------------..------------..------------..------------.  |
//               |   /              \  |            ||            ||            ||            |  |
//               |  |                | |            ||            ||            ||            |  |
//  USB-C <====> |  |     Encoder    | |  Button 1  ||  Button 2  ||  Button 3  ||  Button 4  |  |
//               |  |                | |            ||            ||            ||            |  |
//               |   \              /  |            ||            ||            ||            |  |
//               |    `._        _.'   '------------''------------''------------''------------'  |
//               |       `-....-'                                                                |
//               '-------------------------------------------------------------------------------'
//}

// Defines the macropad layers and their associated keycodes for encoder and key presses
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
{
	// Format = Encoder, Button 1, Button 2, Button 3, Button 4
	
	// TD(ENCODER_DANCE) = [Single-Tap = Activate Next Layer, Double-Tap = Toggle RGB LEDs On/Off, Press-Hold = Activate Previous Layer]
	
	[_PRIMARY] = LAYOUT // Layer 0
	(
		// ENCODER_DANCE, F13, F14, F15, F16
		///TD(ENCODER_DANCE), KC_F13, KC_F14, KC_F15, KC_F16
		TD(ENCODER_DANCE), KC_F13, KC_F14, KC_F15, KC_SendHID
	),
	[_SECONDARY] = LAYOUT // Layer 1
	(
		// ENCODER_DANCE, F17, F18, F19, F20
		TD(ENCODER_DANCE), KC_F17, KC_F18, KC_F19, KC_F20
	),
	[_TERTIARY] = LAYOUT // Layer 2
	(
		// ENCODER_DANCE, F21, F22, F23, F24
		TD(ENCODER_DANCE), KC_F21, KC_F22, KC_F23, KC_F24
	)
};

//}

//{ ENCODER ROTATION

// Define behavior for rotation of the encoder
void encoder_update_user(uint8_t index, bool clockwise)
{
	if (index == 0)
	{
		// Increment/decrement the encoder_buffer
		if (clockwise == true)
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

//{ CUSTOM KEYCODE PROCESSING

// Define behavior for custom keycodes
bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
	switch (keycode) // Switch statement for handling triggering keycodes
	{
		/* case CTRLF13: // Triggering keycode is the (custom) CTRLF13
			if (record->event.pressed)  // Check if the triggering action was a physical key "press" (down)
				tap_code16(LCTL(KC_F13)); // Send command to "tap" (down and up events) the Left Control and F13 keys
			else // Triggering action was a physical key "release" (up)
			{
				// when keycode QMKBEST is released
			}
			return false; // Return from this method without "normal" handling of keycode
		case CTRLF14:
			if (record->event.pressed)
				tap_code16(LCTL(KC_F14)); // Send command to "tap" (down and up events) the Left Control and F14 keys
			else
			{
				// when keycode QMKBEST is released
			}
			return false;
		case CTRLF15:
			if (record->event.pressed)
				tap_code16(LCTL(KC_F15)); // Send command to "tap" (down and up events) the Left Control and F15 keys
			else
			{
				// when keycode QMKBEST is released
			}
			return false;
		case CTRLF16:
			if (record->event.pressed)
				tap_code16(LCTL(KC_F16)); // Send command to "tap" (down and up events) the Left Control and F16 keys
			else
			{
				// when keycode QMKBEST is released
			}
			return false; */
		case KC_SendHID:
			if (record->event.pressed)
				SendHID(); // Call command to send Raw HID data to host computer
			else
			{
				// when keycode QMKBEST is released
			}
			return false;
	}
	return true; // Effectively process ALL other keycodes "normally"
};

//}

//{ RAW HID

//{ bigKNOBv2 HID Info

// This is specific to your bigKNOBv2
// Vendor ID	= 	0xCEEB
// Product ID 	= 	0x0007
// Usage Page 	= 	0xFF60
// Usage (ID) 	= 	0x61

//}

// Method for receiving data from host computer via a HID interface
void raw_hid_receive(uint8_t *data, uint8_t length)
{
	///raw_hid_send(data, length); // Echo received data back to host computer via a HID interface (debug)
	
	int action = data[0]; // First data packet is the desired 'action'
	int context = data[1]; // Second data packet is the 'context' for the desired 'action'
	
	// Process requested contextual action
	switch(action)
	{
		case 1: // Move to specified layer
			MoveToLayer(context);
			break;
		default:
			ErrorFlash(); // Flash RGB LEDs to indicate issue
			break;
	}
	
	SendHID(); // Call method to send data to host computer
}

// Method for sending data to host computer via a HID interface
void SendHID(void)
{
	uint8_t data[RAW_EPSIZE]; // Declare array for storing data for sending to host computer
    memset(data, 0, RAW_EPSIZE);
	data[0] = 'B';
	
	raw_hid_send(data, RAW_EPSIZE); // Call method to send data to host computer
	LightFlash(); // debug
}

//}

//{ CUSTOM METHODS

// Calculate highest layer number
int highest_layer_number = sizeof(keymaps)/sizeof(keymaps[0]) - 1;

// Method for moving to next layer in the stack
void NextLayer(void)
{
	int moveLayer = selected_layer;
	if (moveLayer < highest_layer_number)
		moveLayer++;
	else
		moveLayer = 0;
	MoveToLayer(moveLayer);
}

// Method for moving to the previous layer in the stack
void PreviousLayer(void)
{
	int moveLayer = selected_layer;
	if (moveLayer > 0)
		moveLayer--;
	else
		moveLayer = highest_layer_number;
	MoveToLayer(moveLayer);
}

// Method for moving to a specific layer in the stack
void MoveToLayer(int layer)
{
	if (layer >= 0 && layer <= highest_layer_number)
		layer_move(layer);
	else
		ErrorFlash(); // Flash RGB LEDs to indicate issue
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
			rgblight_sethsv(HSV_RED); // LAYER 0: Set all RGB LEDs to red
			break;
		case 1:
			rgblight_sethsv(HSV_GREEN); // LAYER 1: Set all RGB LEDs to green
			break;
		case 2: 
			rgblight_sethsv(HSV_BLUE); // LAYER 2: Set all RGB LEDs to blue
			break;
		default:
			ErrorFlash(); // Flash RGB LEDs to indicate issue
			rgblight_sethsv(HSV_YELLOW); // LAYER UNKNOWN: Set all RGB LEDs to yellow
			break;
	}
}

// Method for flashing RGB LEDs to alert of an issue or unexpected situation
void ErrorFlash(void)
{
	bool isLightingEnabled = rgblight_is_enabled(); // Store current state of RGB LEDs
	
	float timeStep = 125; // Time step for cycling flashing colors (ms)
	int flashTime = 1000; // Amount of time to flash LEDs (ms)
	float timer = 0;
	
	// Turn on RGB LEDs if applicable
	if (isLightingEnabled == false)
		ToggleRgbLED();
	
	// Loop over flashTime while cycling displayed colors every timeStep
	while (timer < flashTime)
	{
		rgblight_sethsv(HSV_RED);
		wait_ms(timeStep);
		rgblight_sethsv(HSV_YELLOW);
		wait_ms(timeStep);
		timer += (timeStep * 2);
	}
	
	// Turn off RGB LEDs or switch back to active layer's RGB LED color
	if (isLightingEnabled == false)
		ToggleRgbLED();
	else
		ChangeLedColor();
}

// Method for flashing RGB LEDs for confirmation and debugging of actions
void LightFlash(void)
{
	float timeStep = 125; // Time step for cycling flashing colors (ms)
	int flashTime = 1000; // Amount of time to flash LEDs (ms)
	float timer = 0;
	
	// Loop over flashTime while toggling RGB LEDs every timeStep
	while (timer < flashTime)
	{
		ToggleRgbLED();
		wait_ms(timeStep);
		ToggleRgbLED();
		wait_ms(timeStep);
		timer += (timeStep * 2);
	}
}
//}
