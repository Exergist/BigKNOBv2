# qmk.BigKNOBv2

## What is this?
QMK files for BigKNOBv2 macropad controller. 



## Key Maps

### Modified

 * Encoder
   * Press = Next Layer
   * Clockwise Rotation = Next Layer
   * Counter-Clockwise Rotation = Previous Layer
 * Button 1:
   * Layer 1 = F14
   * Layer 2 = F19
   * Layer 3 = CTRL + F13
 * Button 2:
   * Layer 1 = F15
   * Layer 2 = F20
   * Layer 3 = CTRL + F14
 * Button 3:
   * Layer 1 = F16
   * Layer 2 = F21
   * Layer 3 = CTRL + F15
 * Button 4:
   * Layer 1 = F17
   * Layer 2 = F22
   * Layer 3 = CTRL + F16

### Original
 * Encoder
   * Press = Next Layer
   * Clockwise Rotation = Volume Up
   * Counter-Clockwise Rotation = Volume Down
 * Button 1:
   * Layer 1 = F14
   * Layer 2 = F19
   * Layer 3 = CTRL + F13
 * Button 2:
   * Layer 1 = F15
   * Layer 2 = F20
   * Layer 3 = CTRL + F14
 * Button 3:
   * Layer 1 = F16
   * Layer 2 = F21
   * Layer 3 = CTRL + F15
 * Button 4:
   * Layer 1 = F17
   * Layer 2 = F22
   * Layer 3 = CTRL + F16

## How to install the firmware?
 1. Rename your desired 'keymap_DESCRIPTION.c' file as 'keymap.c' (no quotes).
 2. Go through [The QMK Tutorial](https://docs.qmk.fm/#/newbs).
 3. Insert the desired 'config.h,' 'rules.mk,' and 'keymap.c' files where appropriate prior to building and uploading the firmware. 
   
## Notes
 * My BigKNOBv2 was custom-made by LeafCutterLabs and ordered through [Etsy](https://www.etsy.com/listing/955302431/bigknob-a-custom-programmable-macropad?ref=shop_home_recs_3&frs=1&crt=1). 
 * My BigKNOBv2 uses the dented encoder, and the "modified" keymap was optimized around it (YMMV). 
 * BigKNOBv2 has key switches with RGB backlighting (as opposed to RGB underglow for the whole device).

## Acknowledgements

* BigKNOBv2 hardware design and QMK source files are by [LeafCutterLabs](https://github.com/LeafCutterLabs).
* [QMK](https://qmk.fm/) is developed and maintained by Jack Humbert of OLKB with contributions from the community.
