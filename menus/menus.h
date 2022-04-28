#ifndef _MENUS_H_
#define _MENUS_H_

#include <cstdint>

#define MENU_TITLE_FONT FONT_MEDIUM_BOLD
#define MENU_ITEM_FONT FONT_MEDIUM

/**
 * Make a type "menu_funct_ptr_t" which describes a void function taking an entry id and a caller menu item.
 * This function pointer is the template for functions called by the menu.
 */
typedef void (*menu_func_ptr_t)(uint8_t entry_id, struct menu_t * caller);

/**
 * A struct definining an instance of a menu.
 */
struct menu_t {
    /**
     * The name of the menu.
     * This string is displayed on top of the active menu.
     * It is also the text displayed in menu items of other menus.
     */
    char name[32];
    
    /**
     * Function to be called if this menu item is activated.
     * NULL for no function.
     */
    menu_func_ptr_t function;
    
    /**
     * Sub-menus
     */
    struct menu_t * sub_menus[32];
    
    /**
     * If this menu was invoked by a different menu the invong menu is here.
     */
    struct menu_t * caller;
};

/**
 * A pointer to the currently active menu.
 * Make sure this is set before using menus.
 */
extern menu_t * currently_active_menu;

/**
 * A simple menu entry that simply prints the item number and the menu it is in over serial.
 */
extern struct menu_t mi_dummy_serial;

/**
 * A simple menu entry that does nothing. The action is an empty function.
 */
extern struct menu_t mi_do_nothing;

/**
 * Draw a menu to the screen.
 * This function only draws it to the screen and has no logic whatsoever.
 * @param current_menu The menu to be drawn.
 */
void menu_draw(struct menu_t * current_menu);

/**
 * Remove the currently active menu from the stack and return it.
 * @return The menu that is active.
 */
struct menu_t * menu_pop();

/**
 * Place a new menu on the stack.
 * @param new_menu The menu that should be active next.
 */
void menu_push(struct menu_t * new_menu);

/**
 * Activate an item of the current menu.
 * If the item is another menu; it is made active and displayed.
 * If the item is an action; the action is executed.
 * @param id The number of the element selected in the current menu.
 * @return false if the item does not exist. true if the display/action was succesful.
 */
bool menu_activate_item(uint8_t id);

/**
 * Pushes and draws a menu.
 * Equivalent to calling menu_push and then menu_draw.
 * @param new_menu The menu to be activated.
 */
void menu_activate_menu(struct menu_t * new_menu);

/**
 * Pop a menu and draws the prior menu.
 * Equivalent to calling menu_pop and then menu_draw(currently_active_menu).
 * @return false on failure, true on success.
 */
bool menu_deactivate_menu();

/**
 * Let the menu operating system run.
 */
void menu_yield();

/**
 * Stop the menu os, returning instead to where menu_yield() was called.
 */
void menu_unyield();

#endif // _MENUS_H_