# qmk.BigKNOBv2

## What is this?
QMK files for BigKNOBv2 macropad controller. 



## Key Maps

### Latest Release (v4.0)

 * Encoder
   * Single-Press = Next Layer
   * Press-Hold-Release = Toggle RGB LEDs On/Off
   * Clockwise Rotation = Next Layer
   * Counter-Clockwise Rotation = Previous Layer
 * Button 1:
   * Layer 0 = F14
   * Layer 1 = F19
   * Layer 2 = CTRL + F13
 * Button 2:
   * Layer 0 = F15
   * Layer 1 = F20
   * Layer 2 = CTRL + F14
 * Button 3:
   * Layer 0 = F16
   * Layer 1 = F21
   * Layer 2 = CTRL + F15
 * Button 4:
   * Layer 0 = F17
   * Layer 1 = F22
   * Layer 2 = CTRL + F16
 * RGB LED Colors:
   * Layer 0 = Red
   * Layer 1 = Green
   * Layer 2 = Blue

### Original
 * Encoder
   * Press = Next Layer
   * Clockwise Rotation = Volume Up
   * Counter-Clockwise Rotation = Volume Down
 * Button 1:
   * Layer 0 = F14
   * Layer 1 = F19
   * Layer 2 = CTRL + F13
 * Button 2:
   * Layer 0 = F15
   * Layer 1 = F20
   * Layer 2 = CTRL + F14
 * Button 3:
   * Layer 0 = F16
   * Layer 1 = F21
   * Layer 2 = CTRL + F15
 * Button 4:
   * Layer 0 = F17
   * Layer 1 = F22
   * Layer 2 = CTRL + F16
 * RGB LED Colors:
   * Layer 0 = Red
   * Layer 1 = Green
   * Layer 2 = Blue

## How to install the firmware?
 1. Go through [The QMK Tutorial](https://docs.qmk.fm/#/newbs).
 2. Insert the desired 'config.h,' 'rules.mk,' and 'keymap.c' files where appropriate prior to building and uploading the firmware. 
   
## Notes
 * My BigKNOBv2 was custom-made and ordered through [Etsy](https://www.etsy.com/shop/LeafCutterLabs?ref=simple-shop-header-name&listing_id=955302431). 
 * My BigKNOBv2 uses the dented encoder, and the "modified" keymap was optimized around it (YMMV). 
 * BigKNOBv2 has key switches with RGB backlighting (as opposed to RGB underglow for the whole device).

## Acknowledgements

* BigKNOBv2 hardware design and QMK source files are by Craig Gardner from [LeafCutterLabs](https://github.com/LeafCutterLabs).
* [QMK](https://qmk.fm/) is developed and maintained by Jack Humbert of OLKB with contributions from the community.
