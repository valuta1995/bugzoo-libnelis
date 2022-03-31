#include "oled_friendly.h"
#include <cstdint>
 

int8_t get_font_height(const uint8_t * font) {
    if (font == FONT_SMALL) {
        return 7;
    } else if (font == FONT_MEDIUM || font == FONT_MEDIUM_BOLD || font == FONT_MEDIUM_MONO) {
        return 15;
    } else if (font == FONT_LARGE) {
        return 20;
    } else if (font == FONT_LARGE_MONO) {
        return 22;
    } else {
        return -1;
    }
}

int8_t get_font_offset(const uint8_t * font) {
    if (font == FONT_SMALL) {
        return 6;
    } else if (font == FONT_MEDIUM_MONO) {
        return 9;
    } else if (font == FONT_MEDIUM || font == FONT_MEDIUM_BOLD) {
        return 10;
    } else if (font == FONT_LARGE) {
        return 13;
    } else if (font == FONT_LARGE_MONO) {
        return 14;
    } else {
        return -1;
    }
}

uint8_t previous_tile_width = 0;
uint8_t previous_tile_height = 0;

void oled_update_rot2_area(uint8_t tx, uint8_t ty, uint8_t tw, uint8_t th) {
    u8g2_UpdateDisplayArea(&oled, 32 - (tx + tw), 8 - (ty + th), tw, th);
}

void oled_friendly_display_one_line(char * message, const uint8_t * font) {

    // Clear the screen
    u8g2_ClearBuffer(&oled);

    // Get the font specs.
    int8_t height = get_font_height(font);
    int8_t offset = get_font_offset(font);
    if (height == -1 || offset == -1) {
        return;
    }

    // Set the font up
    u8g2_SetFont(&oled, font);

    // Calculate the drawn area.
    uint8_t new_tile_height = (height + 7) / 8;
    uint8_t tile_height = previous_tile_height > new_tile_height ? previous_tile_height : new_tile_height;
    previous_tile_height = new_tile_height;

    uint8_t new_tile_width = (u8g2_GetStrWidth(&oled, message) + 7) / 8;
    uint8_t tile_width = previous_tile_width > new_tile_width ? previous_tile_width : new_tile_width;
    previous_tile_width = new_tile_width;

    // 'Draw' the message.
    u8g2_DrawUTF8(&oled, 0, offset, message);
    // u8g2_DrawBox(&oled, 0, 0, u8g2_GetDisplayWidth(&oled), u8g2_GetDisplayHeight(&oled));
    // u8g2_DrawFrame(&oled, 0, 0, u8g2_GetDisplayWidth(&oled), height);
    oled_update_rot2_area(0, 0, tile_width, tile_height);
}