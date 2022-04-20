#ifndef _OLED_FRIENDLY_H_
#define _OLED_FRIENDLY_H_

#include "oled_ssd1322.h"
#include <cstdint>

#define FONT_TINY u8g2_font_tom_thumb_4x6_tf
#define FONT_TINY_HEIGHT 6
#define FONT_TINY_OFFSET 5

#define FONT_SMALL u8g2_font_nokiafc22_tf
#define FONT_SMALL_HEIGHT 8
#define FONT_SMALL_OFFSET 7

#define FONT_MEDIUM u8g2_font_helvR08_tf
#define FONT_MEDIUM_HEIGHT 13
#define FONT_MEDIUM_OFFSET 8

#define FONT_MEDIUM_BOLD u8g2_font_helvB08_tf
#define FONT_MEDIUM_BOLD_HEIGHT 13
#define FONT_MEDIUM_BOLD_OFFSET 8

#define FONT_MEDIUM_MONO u8g2_font_profont12_tf
#define FONT_MEDIUM_MONO_HEIGHT 12
#define FONT_MEDIUM_MONO_OFFSET 8

#define FONT_LARGE u8g2_font_helvR14_tf
#define FONT_LARGE_HEIGHT 22
#define FONT_LARGE_OFFSET 14

#define FONT_LARGE_BOLD u8g2_font_helvB14_tf
#define FONT_LARGE_BOLD_HEIGHT 23
#define FONT_LARGE_BOLD_OFFSET 14

#define FONT_LARGE_MONO u8g2_font_profont22_tf
#define FONT_LARGE_MONO_HEIGHT 22
#define FONT_LARGE_MONO_OFFSET 14



/**
 * Display a single line message to the Box' oled display. Only works on a limited set of fonts.
 * These fonts have been analysed for the proper positioning; other fonts will not work.
 * @param message A c string containing the message (null terminated).
 * @param font The font to be used. Should be one of the fonts listed in this header file. Defaults to Medium.
 */
void oled_friendly_display_one_line(const char * message, const uint8_t * font=FONT_MEDIUM);


/**
 * Display a multi-line message. See one line version for more info.
 * @param x
 * @param y
 * @param w
 * @param h
 * @param message A c string containing the message (null terminated).
 * @param font The font to be used. Should be one of the fonts listed in this header file. Defaults to Medium.
 */
void oled_friendly_display_message(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char * message, const uint8_t * font=FONT_MEDIUM);

/**
 * Displays a single line message to the middle of the oled display.
 * The message looks like a pop up.
 * @param message A c string containing the message (null terminated).
 * @param font The font to be used. Should be one of the fonts listed in this header file. Defaults to Large.
 */
void oled_friendly_splash(const char * message, const uint8_t * font=FONT_LARGE);

/**
 * Get the maximum height of any glyph in the font.
 */
int8_t get_font_height(const uint8_t * font);

/**
 * Get the maximum height of any glyph of the font above the baseline.
 */
int8_t get_font_offset(const uint8_t * font);


#endif