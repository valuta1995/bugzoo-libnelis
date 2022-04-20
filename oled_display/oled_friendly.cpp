#include "oled_friendly.h"
#include <cstdint>
#include <string>

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

uint8_t previous_tile_width = 0;
uint8_t previous_tile_height = 0;

void oled_friendly_update_rot2_area(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    uint16_t x_end = x + w;
    uint16_t y_end = y + h;

    uint16_t x_prime = SCREEN_WIDTH - x_end;
    uint16_t y_prime = SCREEN_HEIGHT - y_end;

    uint16_t x_prime_end = SCREEN_WIDTH - x;
    uint16_t y_prime_end = SCREEN_HEIGHT - y;

    uint8_t x_tile = x_prime / 8;
    uint8_t y_tile = y_prime / 8;

    uint8_t x_tile_end = (x_prime_end + 7) / 8;
    uint8_t y_tile_end = (y_prime_end + 7) / 8;

    uint8_t w_tile = x_tile_end - x_tile;
    uint8_t h_tile = y_tile_end - y_tile;
    u8g2_UpdateDisplayArea(&oled, x_tile, y_tile, w_tile, h_tile);
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

void oled_friendly_display_message(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char * message, const uint8_t * font) {
    uint16_t num_tabs = 0;
    for (uint16_t i = 0; i < strlen(message); i++) {
        if (message[i] == '\t') {
            num_tabs++;
        }
    }
    uint16_t _i = 0;
    char _message[strlen(message) + 3 * num_tabs];
    uint16_t i_line = 0;
    for (uint16_t i = 0; i < strlen(message); i++, _i++, i_line++) {
        if (message[i] == '\n') {
            i_line = 0;
        }
        if (message[i] == '\t') {
            uint16_t delta = (i_line / 4 + 1) * 4 - i_line;
            for (uint8_t _j = 0; _j < delta; _j++) {
                _message[_i + _j] = ' ';
            }
            _i += delta - 1;
        } else {
            _message[_i] = message[i];
        }
    }

    uint8_t font_height = get_font_height(font);
    uint8_t max_lines = (h % font_height == 0 ? 0 : 1) + h/font_height;
    const char * strings[max_lines];

    uint16_t strindex = 0;
    uint16_t count = 0;

    strings[0] = _message;
    count++;

    while (_message[strindex] != '\0') {
        if (_message[strindex] == '\n') {
            _message[strindex] = '\0';
            strings[count] = &_message[strindex + 1];
            count++;
        }
        strindex++;
    }

    uint16_t y_offset = get_font_offset(font);

    for (uint16_t i = 0; i < count; i++) {
        printf("%s\n", strings[i]);
        
        u8g2_DrawUTF8(&oled, x, y + y_offset + font_height * i, strings[i]);
    }

    
    oled_friendly_update_rot2_area(x, y, w, h);

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
    u8g2_DrawBox(&oled, start_x, start_y, text_width, text_height);
    
    // Draw the white border.
    u8g2_SetDrawColor(&oled, 1);
    u8g2_DrawFrame(&oled, start_x - 1, start_y - 1, text_width + 2, text_height);

    // Draw the message
    u8g2_DrawUTF8(&oled, start_x, start_y + text_offset + 2, message);

    // Update only the relevant parts.
    oled_friendly_update_rot2_area(start_x, start_y, text_width, text_height);
}