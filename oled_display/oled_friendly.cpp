#include "oled_friendly.h"
#include <cstdint>

#define SCREEN_WIDTH 256
#define SCREEN_WIDTH_TILES SCREEN_WIDTH / 8

#define SCREEN_HEIGHT 64
#define SCREEN_HEIGHT_TILES SCREEN_HEIGHT / 8

int8_t get_font_height(const uint8_t * font) {
	if (font == FONT_TINY) {
		return FONT_TINY_HEIGHT;
	} else if (font == FONT_SMALL) {
		return FONT_SMALL_HEIGHT;
	} else if (font == FONT_MEDIUM) {
		return FONT_MEDIUM_HEIGHT;
	} else if (font == FONT_MEDIUM_BOLD) {
		return FONT_MEDIUM_BOLD_HEIGHT;
	} else if (font == FONT_MEDIUM_MONO) {
		return FONT_MEDIUM_MONO_HEIGHT;
	} else if (font == FONT_LARGE) {
		return FONT_LARGE_HEIGHT;
	} else if (font == FONT_LARGE_BOLD) {
		return FONT_LARGE_BOLD_HEIGHT;
	} else if (font == FONT_LARGE_MONO) {
		return FONT_LARGE_MONO_HEIGHT;
	} else {
		return -1;
	}
}

int8_t get_font_offset(const uint8_t * font) {
	if (font == FONT_TINY) {
		return FONT_TINY_OFFSET;
	} else if (font == FONT_SMALL) {
		return FONT_SMALL_OFFSET;
	} else if (font == FONT_MEDIUM) {
		return FONT_MEDIUM_OFFSET;
	} else if (font == FONT_MEDIUM_BOLD) {
		return FONT_MEDIUM_BOLD_OFFSET;
	} else if (font == FONT_MEDIUM_MONO) {
		return FONT_MEDIUM_MONO_OFFSET;
	} else if (font == FONT_LARGE) {
		return FONT_LARGE_OFFSET;
	} else if (font == FONT_LARGE_BOLD) {
		return FONT_LARGE_BOLD_OFFSET;
	} else if (font == FONT_LARGE_MONO) {
		return FONT_LARGE_MONO_OFFSET;
	} else {
		return -1;
	}
}

uint8_t previous_x_tile = 0;
uint8_t previous_y_tile = 0;
uint8_t previous_tile_width = 0;
uint8_t previous_tile_height = 0;

uint8_t min(uint8_t n, uint8_t m) {
    return n < m ? n : m;
}

uint8_t max(uint8_t n, uint8_t m) {
    return n > m ? n : m;
}

void oled_friendly_update_rot2_area(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {

    uint16_t x_end = x + w;
    uint16_t y_end = y + h;

    uint16_t x_prime = (SCREEN_WIDTH - 1) - x_end;
    uint16_t y_prime = (SCREEN_HEIGHT - 1) - y_end;

    uint16_t x_prime_end = (SCREEN_WIDTH - 1) - x;
    uint16_t y_prime_end = (SCREEN_HEIGHT - 1) - y;

    uint8_t x_tile = x_prime / 8;
    uint8_t y_tile = y_prime / 8;

    uint8_t x_tile_end = (x_prime_end + 7) / 8;
    uint8_t y_tile_end = (y_prime_end + 7) / 8;

    uint8_t w_tile = x_tile_end - x_tile;
    uint8_t h_tile = y_tile_end - y_tile;

    uint8_t final_x, final_y, final_x_end, final_y_end;
    if (previous_tile_height != 0 && previous_tile_width != 0) {
        uint8_t previous_x_tile_end = previous_x_tile + previous_tile_width;
        uint8_t previous_y_tile_end = previous_y_tile + previous_tile_height;

        final_x = min(previous_x_tile, x_tile);
        final_y = min(previous_y_tile, y_tile);
        final_x_end = max(previous_x_tile_end, x_tile_end);
        final_y_end = max(previous_y_tile_end, y_tile_end);
    } else {
        final_x = x_tile;
        final_y = y_tile;
        final_x_end = x_tile_end;
        final_y_end = y_tile_end;
    }

    previous_x_tile = x_tile;
    previous_y_tile = y_tile;
    previous_tile_height = h_tile;
    previous_tile_width = w_tile;
    
    uint8_t final_w = final_x_end - final_x;
    uint8_t final_h = final_y_end - final_y;

    u8g2_UpdateDisplayArea(&oled, final_x, final_y, final_w, final_h);
}

void oled_friendly_display_one_line(const char * message, const uint8_t * font) {

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

    uint16_t text_width = u8g2_GetStrWidth(&oled, message);

    // Calculate the drawn area.
    uint8_t new_tile_height = (height + 7) / 8;
    uint8_t tile_height = previous_tile_height > new_tile_height ? previous_tile_height : new_tile_height;
    previous_tile_height = new_tile_height;

    uint8_t new_tile_width = (text_width + 7) / 8;
    uint8_t tile_width = previous_tile_width > new_tile_width ? previous_tile_width : new_tile_width;
    previous_tile_width = new_tile_width;

    // 'Draw' the message.
    u8g2_DrawUTF8(&oled, 0, offset, message);
    // u8g2_DrawBox(&oled, 0, 0, u8g2_GetDisplayWidth(&oled), u8g2_GetDisplayHeight(&oled));
    // u8g2_DrawFrame(&oled, 0, 0, u8g2_GetDisplayWidth(&oled), height);
    oled_friendly_update_rot2_area(0, 0, text_width, height);
}

void oled_friendly_splash(const char * message, const uint8_t * font) {

    // Clear the screen
    // u8g2_ClearBuffer(&oled);

    // Get the font specs.
    int8_t text_height = get_font_height(font);
    int8_t text_offset = get_font_offset(font);
    if (text_height == -1 || text_offset == -1) {
        return;
    }

    // Set the font up
    u8g2_SetFont(&oled, font);

    // Get the text width
    uint16_t text_width = u8g2_GetStrWidth(&oled, message);

    uint16_t start_x = SCREEN_WIDTH / 2 - text_width / 2;
    uint16_t start_y = SCREEN_HEIGHT / 2 - text_offset /2;

    // Draw the black background.
    u8g2_SetDrawColor(&oled, 0);
    u8g2_DrawBox(&oled, start_x - 2, start_y - 1, text_width + 2, text_height + 1);
    
    // Draw the white border.
    u8g2_SetDrawColor(&oled, 1);
    u8g2_DrawFrame(&oled, start_x - 2, start_y - 1, text_width + 4, text_height + 1);

    // Draw the message
    u8g2_DrawUTF8(&oled, start_x, start_y + text_offset + 2, message);

    // Update only the relevant parts.
    oled_friendly_update_rot2_area(start_x - 2, start_y - 1, text_width + 4, text_height + 1);
}