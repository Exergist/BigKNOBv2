![image](https://user-images.githubusercontent.com/31357974/120911848-11029f80-c650-11eb-8896-d142378cb411.png)

# qmk.BigKNOBv2

## What is this?
QMK files for BigKNOBv2 macropad controller with RGB LED key switches.

## Key Map (v7.0)

 * Encoder
   * Single-Tap = Next Layer
   * Double-Tap = Toggle RGB LEDs On/Off
   * Press-Hold = Previous Layer
   * Clockwise Rotation = Next Layer
   * Counter-Clockwise Rotation = Previous Layer
 * Button 1:
   * Layer 0 = F13
   * Layer 1 = F17
   * Layer 2 = F21
 * Button 2:
   * Layer 0 = F14
   * Layer 1 = F18
   * Layer 2 = F22
 * Button 3:
   * Layer 0 = F15
   * Layer 1 = F19
   * Layer 2 = F23
 * Button 4:
   * Layer 0 = F16
   * Layer 1 = F20
   * Layer 2 = F24
 * RGB LED Colors:
   * Layer 0 = Red
   * Layer 1 = Green
   * Layer 2 = Blue

## How to install the firmware?
 1. Go through [The QMK Tutorial](https://docs.qmk.fm/#/newbs).
 2. Insert the desired 'config.h,' 'rules.mk,' and 'keymap.c' files where appropriate prior to building and uploading the firmware. 
   
## Notes
 * My BigKNOBv2 was custom-made and ordered through [Etsy](https://www.etsy.com/shop/LeafCutterLabs?ref=simple-shop-header-name&listing_id=955302431). 
 * The "modified" keymap was optimized for use with the BigKNOBv2 dented encoder. 

## Acknowledgements

* BigKNOBv2 hardware design and QMK source files are by Craig Gardner from [LeafCutterLabs](https://github.com/LeafCutterLabs).
* [QMK](https://qmk.fm/) is developed and maintained by Jack Humbert of OLKB with contributions from the community.
* Keymap header ASCII art by [patorjk](https://patorjk.com/software/taag/).
* filterpaper, drashna, ridingquerty, **MORE** and probably others from the [qmk Discord](https://discord.gg/Uq7gcHh) for their qmk guideance.
