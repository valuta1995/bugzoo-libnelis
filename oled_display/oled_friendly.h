#ifndef _OLED_FRIENDLY_H_
#define _OLED_FRIENDLY_H_

#include "oled_ssd1322.h"

#define FONT_SMALL u8g2_font_5x7_tf
#define FONT_MEDIUM u8g2_font_9x15_tf
#define FONT_MEDIUM_BOLD u8g2_font_9x15B_tf
#define FONT_MEDIUM_MONO u8g2_font_profont15_tf
#define FONT_LARGE u8g2_font_10x20_tf
#define FONT_LARGE_MONO u8g2_font_profont22_tf


/**
 * Display a single line message to the Box' oled display. Only works on a limited set of fonts.
 * These fonts have been analysed for the proper positioning; other fonts will not work.
 * @param message A c string containing the message (null terminated).
 * @param font The font to be used. Should be one of the fonts listed in this header file. Defaults to Medium.
 */
void oled_friendly_display_one_line(char * message, const uint8_t * font=FONT_MEDIUM);

#endif