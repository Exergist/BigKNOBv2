![image](https://user-images.githubusercontent.com/31357974/120911848-11029f80-c650-11eb-8896-d142378cb411.png)

# BigKNOBv2

## What Is This?
QMK files for BigKNOBv2 macropad controller with RGB LED key switches.

Also includes C# application (using [HidLibrary](https://github.com/mikeobrien/HidLibrary)) for bidirectional communication between BigKNOBv2 and host computer via Raw HID.

## Key Map

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

## Raw HID Functionality
 * Move to specified layer

## How to Install the Firmware?
 1. Go through [The QMK Tutorial](https://docs.qmk.fm/#/newbs).
 2. Insert the desired _config.h_, _rules.mk_, and _keymap.c_ files where appropriate prior to building and uploading the firmware. 
   
## Notes
 * My BigKNOBv2 was custom-made and ordered through [Etsy](https://www.etsy.com/shop/LeafCutterLabs?ref=simple-shop-header-name&listing_id=955302431). 
 * The keymap was optimized for use with the BigKNOBv2 dented encoder. 
 * Additional QMK resources are compiled in the [QMK Syllabus](https://docs.qmk.fm/#/syllabus).

## Acknowledgements

* BigKNOBv2 hardware design and QMK source files are by Craig Gardner from [LeafCutterLabs](https://github.com/LeafCutterLabs).
* [QMK](https://qmk.fm/) is developed and maintained by Jack Humbert of OLKB with contributions from the community.
* [HidLibrary](https://github.com/mikeobrien/HidLibrary) is developed and maintained by Mike O'Brien and Austin Mullins (and other contributors)
* Keymap header ASCII art by [patorjk](https://patorjk.com/software/taag/).
* Many thanks to the following people (and probably others) from the [qmk Discord](https://discord.gg/Uq7gcHh) for their guideance:
  * filterpaper
  * drashna
  * Dasky
  * ridingquerty
  * Erovia
  * sigprof
  * fauxpark
  * bomtarnes
  * CovfefeMcWhiskey
  * trankillity
  * tzarc
