#include "menus.h"
#include "oled_friendly.h"
#include "keypad.h"
#include <cstdint>
#include <cstdio>

struct menu_t * currently_active_menu = NULL;

void mf_serial_echo(uint8_t entry_id, struct menu_t * caller) {
    printf("Dummy selected at entry number %d! From menu entry %s\n", entry_id, caller->name);
}
 
struct menu_t mi_dummy_serial = {
    "Dummy",
    &mf_serial_echo,
    {},
    NULL
};
 
void mf_do_nothig(uint8_t entry_id, struct menu_t * caller) {}
 
struct menu_t mi_do_nothing = {
    " - ",
    &mf_do_nothig,
    {},
    NULL
};

void _menu_draw_header(const uint8_t * title_font, int8_t title_offset, int8_t title_height, char * name) {
    u8g2_ClearBuffer(&oled);
    u8g2_SetFont(&oled, title_font);
    u8g2_DrawStr(&oled, 0, title_offset, name);
    u8g2_DrawLine(&oled, 0, title_height, 255, title_height);
}

char _menu_compute_index_character(uint8_t item_id) {
    int8_t key_face_number = item_id + 1;
    if (key_face_number < 10) {
        return item_id + '1';
    } else if (key_face_number == 10) {
        return '0';
    } else {
        return ' ';
    }
}

void menu_draw(struct menu_t * current_menu) {
    // Select fonts
    const uint8_t * title_font = MENU_TITLE_FONT;
    const uint8_t * item_font = MENU_ITEM_FONT;
    
    // Calculate offsets
    int8_t title_offset = get_font_offset(title_font);
    int8_t title_height = get_font_height(title_font);
    int8_t item_offset = get_font_offset(item_font);
    int8_t item_height = get_font_height(item_font);

    _menu_draw_header(title_font, title_offset, title_height, current_menu->name);
 
    // Prepare drawing items
    u8g2_SetFont(&oled, item_font);
 
    int8_t current_item_id = 0;
    int16_t height_shift = 0;
    int16_t width_shift = 0;
    struct menu_t * current_item = current_menu->sub_menus[current_item_id];
    uint16_t max_string_width = 0;
    while (current_item != NULL) {

        // Calculate the max width of the index number
        char number[] = {'\0', '\0'};
        number[0] = 'M';
        uint8_t index_number_width = u8g2_GetStrWidth(&oled, number) * 3 / 2;
        number[0] = _menu_compute_index_character(current_item_id);
 
        // Calculate the offset from the top-left of the screen.
        int16_t current_base_height = title_height + 2 + item_offset + (current_item_id * item_height);
        int16_t current_height = current_base_height - height_shift;
        if (current_height > 64) {
            height_shift = current_item_id * item_height;
            current_height = current_base_height - height_shift;
            width_shift += max_string_width + index_number_width;

            // Reset the widest element; we're starting a new column.
            max_string_width = 0;
        }
 
        // Draw the item
        u8g2_DrawStr(&oled, width_shift, current_height, number);
        uint8_t item_width = u8g2_GetStrWidth(&oled, current_item->name);
        u8g2_DrawStr(&oled, width_shift + index_number_width, current_height, current_item->name);
 
        // Track the widest element of this column.
        if (item_width + index_number_width > max_string_width) {
            max_string_width = item_width + index_number_width;
        }

        // Go to the next item
        current_item_id++;
        current_item = current_menu->sub_menus[current_item_id];
    }
    
    // Display the menu.
    u8g2_SendBuffer(&oled);
}

struct menu_t * menu_pop() {
    struct menu_t * result = currently_active_menu;
    if (result->caller != NULL) {
        currently_active_menu = result->caller;
    } else {
        printf("Tried to exit menu %s, but its caller is null.\n", result->name);
    }
    return result;
}
 
void menu_push(struct menu_t * new_menu) {
    new_menu->caller = currently_active_menu;
    currently_active_menu = new_menu;
}

void menu_activate_menu(struct menu_t *new_menu) {
    menu_push(new_menu);
    menu_draw(currently_active_menu);
}

bool menu_deactivate_menu() {
    if (menu_pop() != currently_active_menu) {
        menu_draw(currently_active_menu);
        return true;
    } else {
        return false;
    }
}

bool menu_activate_item(uint8_t id) {
    struct menu_t * new_item = currently_active_menu->sub_menus[id];
    if (new_item == NULL) {
        // Item does not exist
        return false;

    } else if (new_item->function == NULL) {
        // Item is a menu
        menu_activate_menu(new_item);
    } else {
        // Item is an action
        new_item->function(id, currently_active_menu);
        keypad_clear_keys();
    }
    return true;
}

void menu_yield() {
    while (true) {
        bool error_state = false;
        while (!keypad_has_key()) ThisThread::sleep_for(25ms);

        char c = keypad_get_char();
        if ('0' <= c && c <= '9') {
            int8_t key_number = c - '0';
            int8_t item_id = key_number == 0 ? 10 - 1 : key_number - 1;
 
            if (!menu_activate_item(item_id)) {
                error_state = true;
            }
        } else if (c == '\e' || c == '0') {
            if (!menu_deactivate_menu()) {
                error_state = true;
            }
        } else {
            error_state = true;
        }

        if (error_state) {
            buzz = 1;
            ThisThread::sleep_for(5ms);
            buzz = 0;
            oled_friendly_splash("Unknown button.");
            ThisThread::sleep_for(1s);
            menu_draw(currently_active_menu);
            keypad_clear_keys();
        }
    }
}