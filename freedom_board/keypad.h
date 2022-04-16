#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#include "board_freedom.h"
#include <cstdint>

#define KEYPAD_SCAN_RATE_MS     50ms
#define KEYPRESS_BEEP_ENABLED   true
#define KEYPRESS_BEEP_LENGTH_MS 2

/**
 * Numerical keys. Represented by numbers 0 through 1. Or by characters '0' through '1';
 */
#define NUM_0_KEY 0b01001000
#define NUM_1_KEY 0b00000101
#define NUM_2_KEY 0b00001001
#define NUM_3_KEY 0b00010001
#define NUM_4_KEY 0b00000110
#define NUM_5_KEY 0b00001010
#define NUM_6_KEY 0b00010010
#define NUM_7_KEY 0b10000100
#define NUM_8_KEY 0b10001000
#define NUM_9_KEY 0b10010000

/**
 * Special keys represented by '.', 'U', and 'D'.
 */
#define DOT_KEY 0b01010000
#define UP_KEY 0b00100001
#define DOWN_KEY 0b00100010

/**
 * Even more special keys. Represented by '\e' (escape), '\n' (enter or newline), and 0x7f (delete)
 */
#define ESC_KEY 0b01000100
#define ENTER_KEY 0b01100000
#define DEL_KEY 0b10100000


struct keyboard_event_t {
    uint8_t raw;
    // uint32_t time;
};

extern Queue<keyboard_event_t, 32> keypadEventQueue;

/**
 * Initialize the keypad on the front panel of the control box.
 */
void keypad_init();

/**
 * Check if a key has been pressed.
 * @return True if a keypress is available. False if no keypresses are available.
 */
bool keypad_has_key();

/**
 * Get the raw data from the most recent keypress.
 * @return Raw data byte from the last key press or 0 if there was no keypress.
 */
uint8_t keypad_get_key();

/**
 * Get the character representation of the last key press.
 * @return Character byte representing the last key press or '\0' if the keypress could not be decoded.
 */
char keypad_get_char();

/**
 * Get the numerical representation of the last key press.
 * @return Int representing the number of the last keypress or -1 if the last keypress could not be decoded (or was not a number).
 */
int keypad_get_int();

/**
 * Delete all keys in the queue.
 */
void keypad_clear_keys();

#endif