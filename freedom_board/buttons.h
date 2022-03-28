#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "board_freedom.h"

extern volatile bool buttons_changed;

/**
 * Initialize the front panel buttons (not the keypad).
 */
void button_init();

/**
 * Interrupt routine to be called by the system when a button is pressed.
 */
void button_isr();

/**
 * Get the current state of one of the buttons.
 * @param nr number of the button to be checked.
 * @return boolean state of the button.
 */
bool button_get_state(uint8_t nr);

/**
 * Get the state of all the buttons.
 * @return a byte where each bit (starting least significant) is the state of each of the buttons (starting at 0).
 */
uint8_t button_get_all();

#endif