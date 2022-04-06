/**
    @file eve_ui_keyboard.h
 **/
/*
 * ============================================================================
 * History
 * =======
 *
 * Copyright (C) Bridgetek Pte Ltd
 * ============================================================================
 *
 * This source code ("the Software") is provided by Bridgetek Pte Ltd
 *  ("Bridgetek") subject to the licence terms set out
 * http://brtchip.com/BRTSourceCodeLicenseAgreement/ ("the Licence Terms").
 * You must read the Licence Terms before downloading or using the Software.
 * By installing or using the Software you agree to the Licence Terms. If you
 * do not agree to the Licence Terms then do not download or use the Software.
 *
 * Without prejudice to the Licence Terms, here is a summary of some of the key
 * terms of the Licence Terms (and in the event of any conflict between this
 * summary and the Licence Terms then the text of the Licence Terms will
 * prevail).
 *
 * The Software is provided "as is".
 * There are no warranties (or similar) in relation to the quality of the
 * Software. You use it at your own risk.
 * The Software should not be used in, or for, any medical device, system or
 * appliance. There are exclusions of Bridgetek liability for certain types of loss
 * such as: special loss or damage; incidental loss or damage; indirect or
 * consequential loss or damage; loss of income; loss of business; loss of
 * profits; loss of revenue; loss of contracts; business interruption; loss of
 * the use of money or anticipated savings; loss of information; loss of
 * opportunity; loss of goodwill or reputation; and/or loss of, damage to or
 * corruption of data.
 * There is a monetary cap on Bridgetek's liability.
 * The Software may have subsequently been amended by another user and then
 * distributed by that other user ("Adapted Software").  If so that user may
 * have additional licence terms that apply to those amendments. However, Bridgetek
 * has no liability in relation to those amendments.
 * ============================================================================
 */

#ifndef _EVE_UI_KEYBOARD_H
#define _EVE_UI_KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "EVE_config.h"
#include "EVE.h"
#include "HAL.h"

/**
 @brief Custom font and bitmap definitions.
 @details These utilise handles 0 to 14.
 */
//@{
#define FONT_HEADER 28
#define FONT_BODY 27
#define FONT_CUSTOM 27
#define BITMAP_BRIDGETEK_LOGO 8
#define BITMAP_MEDIA 7
#define BITMAP_SETTINGS 6
#define BITMAP_CANCEL 5
#define BITMAP_SAVE 4
#define BITMAP_REFRESH 3
#define BITMAP_KEYPAD 2
#define BITMAP_KEYBOARD 1
#define FONT_CUSTOM_EXTENDED 0
//@}

/**
 * @brief Theme colours.
 * @details The highlight colour is used for an active keypress on a button or
 * a selected option (e.g. caps lock). The alpha
 */
//@{
#define EVE_COLOUR_HIGHLIGHT 0x808080UL
#define EVE_COLOUR_BG_1 0x404040UL
#define EVE_COLOUR_BG_2 0x202020UL
#define EVE_COLOUR_FG_1 0xffffffUL
//@}

#define EVE_SPACER (EVE_DISP_WIDTH / 120)

/** @name Special Key TAG Definitions
 * @details Special key definitions switching between screen layouts.
 */
//@{
#define TAG_RESERVED_START 	240
#define TAG_DE_ALPHA 		241			// Switch to Alphanumeric Screen Germany/Austria
#define TAG_US_ALPHA 		242			// Switch to Alphanumeric Screen US
#define TAG_UK_ALPHA 		243			// Switch to Alphanumeric Screen UK
#define TAG_SPECIAL			246
#define TAG_MEDIA			247
#define TAG_REFRESH			248
#define TAG_SAVE			249
#define TAG_KEYPAD			250
#define TAG_KEYBOARD		251
#define TAG_SETTINGS		252			// Click on Settings
#define TAG_LOGO			253			// Click on Bridgetek logo
#define TAG_CANCEL			254			// Cancel
#define TAG_NO_ACTION		255
//@}

/** @name Key TAG Definitions
 * @details Key definitions for TAGs for touchscreen.
 * These are unique tags for each virtual button or key used on
 * the touchscreen. Keys which send data to the host are mapped to
 * scancodes.
 */
//@{
enum {
	TAG_NULL,
	TAG_A, TAG_B, TAG_C, TAG_D, TAG_E, TAG_F, TAG_G, TAG_H, TAG_I,
	TAG_J, TAG_K, TAG_L, TAG_M, TAG_N, TAG_O, TAG_P, TAG_Q, TAG_R,
	TAG_S, TAG_T, TAG_U, TAG_V, TAG_W, TAG_X, TAG_Y, TAG_Z,
	TAG_1, TAG_2, TAG_3, TAG_4, TAG_5, TAG_6, TAG_7, TAG_8, TAG_9,	TAG_0,
	TAG_ENTER, TAG_ESCAPE, TAG_BACKSPACE, TAG_TAB, TAG_SPACE,
	TAG_MINUS, TAG_EQUALS, TAG_SQB_OPEN, TAG_BRACKET_CLS,
	TAG_BACKSLASH, TAG_HASH, TAG_SEMICOLON, TAG_SQUOTE,
	TAG_TILDE, TAG_COMMA, TAG_DOT, TAG_SLASH,
	TAG_CAPS_LOCK,
	TAG_F1, TAG_F2, TAG_F3, TAG_F4, TAG_F5, TAG_F6, TAG_F7, TAG_F8,
	TAG_F9, TAG_F10, TAG_F11, TAG_F12,
	TAG_PRINT_SCREEN, TAG_SCROLL_LOCK,
	TAG_PAUSE, TAG_INSERT,	TAG_HOME, TAG_PAGE_UP, TAG_DEL,	TAG_END,
	TAG_PAGE_DOWN, TAG_RIGHT_ARROW, TAG_LEFT_ARROW, TAG_DOWN_ARROW,
	TAG_UP_ARROW,
	TAG_NUMBER_LOCK,
	TAG_PAD_DIV, TAG_PAD_MUL, TAG_PAD_MINUS, TAG_PAD_PLUS, TAG_PAD_ENTER,
	TAG_PAD_1, TAG_PAD_2, TAG_PAD_3, TAG_PAD_4, TAG_PAD_5, TAG_PAD_6,
	TAG_PAD_7, TAG_PAD_8, TAG_PAD_9, TAG_PAD_0, TAG_PAD_DOT,
	TAG_APP,
	TAG_CTRLL, TAG_SHIFTL, TAG_ALT,	TAG_WINL, TAG_CTRLR, TAG_SHIFTR,
	TAG_ALTGR, TAG_WINR,
	TAG_SC_POWER, TAG_SC_SLEEP, TAG_SC_RESET,
	TAG_CC_PLAY, TAG_CC_NEXT, TAG_CC_PREV, TAG_CC_STOP,
	TAG_CC_MUTE, TAG_CC_VOL_UP, TAG_CC_VOL_DOWN,
	TAG_CUT, TAG_COPY, TAG_PASTE, TAG_UNDO, TAG_REDO, TAG_FIND,
	TAG_MENU, TAG_SELECT, TAG_EXECUTE,
};

struct key_state
{
	uint8_t ShiftL :1;
	uint8_t ShiftR :1;
	uint8_t CtrlL :1;
	uint8_t CtrlR :1;
	uint8_t Alt :1;
	uint8_t AltGr :1;
	uint8_t WinL :1;
	uint8_t WinR :1;
	uint8_t Caps :1;
	uint8_t Numeric : 1;
	uint8_t Scroll : 1;
};

/**
 * @brief Keyboard screen components to show on display.
 */
//@{
#define EVE_HEADER_LOGO				(1 << 0) // Show logo
#define EVE_HEADER_SETTINGS_BUTTON	(1 << 1) // Show settings button
#define EVE_HEADER_CANCEL_BUTTON	(1 << 2) // Show cancel button
#define EVE_HEADER_REFRESH_BUTTON	(1 << 3) // Show refresh button
#define EVE_HEADER_SAVE_BUTTON		(1 << 4) // Show save button
#define EVE_HEADER_KEYPAD_BUTTON	(1 << 5) // Show keypad button (switch to keypad)
#define EVE_HEADER_KEYBOARD_BUTTON	(1 << 6) // Show keyboard button (switch to keyboard)
#define EVE_HEADER_EXTRA_BUTTON		(1 << 7) // Show media button (switch to media controls)
#define EVE_HEADER_SPECIAL_BUTTON	(1 << 8) // Show media button (switch to media controls)
//@}

#define EVE_OPTIONS_READ_ONLY		(1 << 8) // Flag an item as read only

/**
 @brief Pointers to custom images in external C file.
 */
//@{
extern uint16_t img_bridgetek_logo_width;
extern uint16_t img_bridgetek_logo_height;

extern uint16_t img_settings_width;
extern uint16_t img_settings_height;

extern uint16_t img_cancel_width;
extern uint16_t img_cancel_height;

extern uint16_t img_tick_width;
extern uint16_t img_tick_height;

extern uint16_t img_refresh_width;
extern uint16_t img_refresh_height;

extern uint16_t img_keypad_width;
extern uint16_t img_keypad_height;

extern uint16_t img_keyboard_width;
extern uint16_t img_keyboard_height;

extern uint16_t img_media_width;
extern uint16_t img_media_height;

extern uint32_t img_end_address;
//@}

uint32_t eve_ui_keyboard_load_fonts(uint32_t start_addr);
uint32_t eve_ui_keyboard_load_images(uint32_t start_addr);

void eve_ui_keyboard_start(void);
int8_t eve_ui_keyboard_loop(struct key_state *state, uint8_t *tag);
void eve_ui_keyboard_splash(char *msg, uint32_t options);

/* Not implemented */
#if 0
void eve_ui_multiline_init(char *toast, uint32_t options, uint32_t format);
void eve_ui_multiline_add(char *message, uint32_t format);
void eve_ui_multiline_display(void);
uint16_t eve_ui_present_list(char *toast, uint32_t options, char **list, uint16_t count);
uint16_t eve_ui_present_options_list(char *toast, uint32_t options, char **list, uint32_t *list_options, uint16_t count);
uint16_t eve_ui_keyboard_line_input(char *toast, uint32_t options, char *buffer, uint16_t len);
uint16_t eve_ui_keyboard_number_input(char *toast, uint32_t options, char *buffer, uint16_t len);
uint16_t eve_ui_keyboard_ipaddr_input(char *toast, uint32_t options, char *buffer, uint16_t len);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* _EVE_UI_KEYBOARD_H */
