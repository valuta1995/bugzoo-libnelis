#include "keypad.h"
#include <cstdint>

#define ROW_READ_MASK 0b11000011
#define COL_WRITE_MASK 0b00111100


Thread keypad_listener_thread;
Queue<keyboard_event_t, 32> keypadEventQueue;

volatile int8_t rise_count = 0, fall_count = 0;
volatile bool risen = false, fallen = false;

volatile uint8_t idfk_flag = 0;


void onRise() {
    core_util_atomic_incr_s8(&rise_count, 1);
    risen = true;
}

void onFall() {
    core_util_atomic_incr_s8(&fall_count, 1);
    fallen = true;
}

void keypad_listener_process_keypress() {
    uint8_t row_value = (uint8_t) mcp.read_mask(ROW_READ_MASK);
    uint8_t col_value = 0;

    if (row_value == 0) return;

    // Pull only the lower column bits down and see the result
    mcp.write_mask(0b00110000, COL_WRITE_MASK);
    if (mcp.read_mask(ROW_READ_MASK)) {
        // Upper bits, pull the lower of the lower bits down.
        mcp.write_mask(0b00111000, COL_WRITE_MASK);
        if (mcp.read_mask(ROW_READ_MASK)) {
            col_value = 0b00000100;
        } else {
            col_value = 0b00001000;
        }
    } else {
        // Upper bits, pull the lower of the upper bits down.
        mcp.write_mask(0b00101100, COL_WRITE_MASK);
        if (mcp.read_mask(ROW_READ_MASK)) {
            col_value = 0b00010000;
        } else {
            col_value = 0b00100000;
        }
    }

    mcp.write_mask(0b00000000, COL_WRITE_MASK);

    struct keyboard_event_t * e_kbd = new keyboard_event_t();
    e_kbd->raw = row_value | col_value;
    if (!keypadEventQueue.try_put(e_kbd)) {
        delete e_kbd;
    }
}

void keypad_listener_main() {
    // Set all columns low.
    mcp.write_mask(0, COL_WRITE_MASK);
    mcp_int_a.fall(&onFall);
    mcp_int_a.rise(&onRise);

    while (true) {
        int8_t n_rises = core_util_atomic_exchange_s8(&rise_count, 0);
        for (int i = 0; i < n_rises; i++) {
            uint16_t keypad_value = mcp.read_mask(ROW_READ_MASK);
        }

        int8_t n_falls = core_util_atomic_exchange_s8(&fall_count, 0);
        if (n_falls > 0) {
            // Disable interrupts from the MCP
            uint16_t gpinten_old = mcp.readRegister(GPINTEN);
            mcp.writeRegister(GPINTEN, (uint16_t)(gpinten_old & 0b00000000));
            mcp_int_a.fall(NULL);
            mcp_int_a.rise(NULL);

            for (int i = 0; i < n_falls; i++) {
                keypad_listener_process_keypress();
            }
            
            // Restore previous state
            mcp.writeRegister(GPINTEN, gpinten_old);
            mcp_int_a.fall(&onFall);
            mcp_int_a.rise(&onRise);
        }
    }
}

void keypad_init() {
    keypad_listener_thread.start(keypad_listener_main);
}

bool keypad_has_key() {
    return !keypadEventQueue.empty();
}

uint8_t keypad_get_key() {
    struct keyboard_event_t * e_kbd;
    if (keypadEventQueue.try_get(&e_kbd)) {
        uint8_t raw = e_kbd->raw;
        delete e_kbd;
        return raw;
    }
    return 0;
}

char keypad_get_char() {
    uint8_t raw = keypad_get_key();
    switch (raw) {
        case NUM_1_KEY:
            return '1';
        case NUM_2_KEY:
            return '2';
        case NUM_3_KEY:
            return '3';
        case NUM_4_KEY:
            return '4';
        case NUM_5_KEY:
            return '5';
        case NUM_6_KEY:
            return '6';
        case NUM_7_KEY:
            return '7';
        case NUM_8_KEY:
            return '8';
        case NUM_9_KEY:
            return '9';
        case NUM_0_KEY:
            return '0';

        case DOT_KEY:
            return '.';
        case UP_KEY:
            return 'U';
        case DOWN_KEY:
            return 'D';

        case ESC_KEY:
            return '\e';
        case ENTER_KEY:
            return '\n';
        case DEL_KEY:
            return 0x7f; // DEL

        default:
            return '\0';
    }
}


int keypad_get_int() {
    char c = getchar();
    if ('0' <= c && c <= '9') {
        return c - '0';
    } else {
        return -1;
    }
}