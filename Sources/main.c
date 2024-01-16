/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "pico/stdlib.h"

/**
  @file main.c
 */
/*
 * ============================================================================
 * History
 * =======
 * Nov 2019		Initial version
 *
 *
 *
 *
 *
 *
 *
 * (C) Copyright,  Bridgetek Pte. Ltd.
 * ============================================================================
 *
 * This source code ("the Software") is provided by Bridgetek Pte Ltd
 * ("Bridgetek") subject to the licence terms set out
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

/* INCLUDES ************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "bsp/board.h"
#include "tusb.h"

#include "EVE.h"
#include "eve_ui.h"
#include "eve_keyboard.h"
#include "keyboard.h"


/** @name Current key map
 * @details Use to store current mapping of reports
 */
//@{
uint8_t kbd_modifier = 0;
uint8_t kbd_keycode = 0;
uint8_t kbd_led_status = 0;
uint16_t kbd_ctrl_map = 0;
//@}

/* CONSTANTS ***********************************************************************/

#define DEBUG

const uint LED_PIN = PICO_DEFAULT_LED_PIN;

/** @name Tag map
 * @details Used to map tags to scancodes and report types.
 */
const struct tagmap_s tagmap[] = {
    {TAG_A, REPORT_ID_KEYBOARD, HID_KEY_A},
    {TAG_B, REPORT_ID_KEYBOARD, HID_KEY_B},
    {TAG_C, REPORT_ID_KEYBOARD, HID_KEY_C},
    {TAG_D, REPORT_ID_KEYBOARD, HID_KEY_D},
    {TAG_E, REPORT_ID_KEYBOARD, HID_KEY_E},
    {TAG_F, REPORT_ID_KEYBOARD, HID_KEY_F},
    {TAG_G, REPORT_ID_KEYBOARD, HID_KEY_G},
    {TAG_H, REPORT_ID_KEYBOARD, HID_KEY_H},
    {TAG_I, REPORT_ID_KEYBOARD, HID_KEY_I},
    {TAG_J, REPORT_ID_KEYBOARD, HID_KEY_J},
    {TAG_K, REPORT_ID_KEYBOARD, HID_KEY_K},
    {TAG_L, REPORT_ID_KEYBOARD, HID_KEY_L},
    {TAG_M, REPORT_ID_KEYBOARD, HID_KEY_M},
    {TAG_N, REPORT_ID_KEYBOARD, HID_KEY_N},
    {TAG_O, REPORT_ID_KEYBOARD, HID_KEY_O},
    {TAG_P, REPORT_ID_KEYBOARD, HID_KEY_P},
    {TAG_Q, REPORT_ID_KEYBOARD, HID_KEY_Q},
    {TAG_R, REPORT_ID_KEYBOARD, HID_KEY_R},
    {TAG_S, REPORT_ID_KEYBOARD, HID_KEY_S},
    {TAG_T, REPORT_ID_KEYBOARD, HID_KEY_T},
    {TAG_U, REPORT_ID_KEYBOARD, HID_KEY_U},
    {TAG_V, REPORT_ID_KEYBOARD, HID_KEY_V},
    {TAG_W, REPORT_ID_KEYBOARD, HID_KEY_W},
    {TAG_X, REPORT_ID_KEYBOARD, HID_KEY_X},
    {TAG_Y, REPORT_ID_KEYBOARD, HID_KEY_Y},
    {TAG_Z, REPORT_ID_KEYBOARD, HID_KEY_Z},
    {TAG_1, REPORT_ID_KEYBOARD, HID_KEY_1},
    {TAG_2, REPORT_ID_KEYBOARD, HID_KEY_2},
    {TAG_3, REPORT_ID_KEYBOARD, HID_KEY_3},
    {TAG_4, REPORT_ID_KEYBOARD, HID_KEY_4},
    {TAG_5, REPORT_ID_KEYBOARD, HID_KEY_5},
    {TAG_6, REPORT_ID_KEYBOARD, HID_KEY_6},
    {TAG_7, REPORT_ID_KEYBOARD, HID_KEY_7},
    {TAG_8, REPORT_ID_KEYBOARD, HID_KEY_8},
    {TAG_9, REPORT_ID_KEYBOARD, HID_KEY_9},
    {TAG_0, REPORT_ID_KEYBOARD, HID_KEY_0},
    {TAG_ENTER, REPORT_ID_KEYBOARD, HID_KEY_ENTER},
    {TAG_ESCAPE, REPORT_ID_KEYBOARD, HID_KEY_ESCAPE},
    {TAG_BACKSPACE, REPORT_ID_KEYBOARD, HID_KEY_BACKSPACE},
    {TAG_TAB, REPORT_ID_KEYBOARD, HID_KEY_TAB},
    {TAG_SPACE, REPORT_ID_KEYBOARD, HID_KEY_SPACE},
    {TAG_MINUS, REPORT_ID_KEYBOARD, HID_KEY_MINUS},
    {TAG_EQUALS, REPORT_ID_KEYBOARD, HID_KEY_EQUAL},
    {TAG_SQB_OPEN, REPORT_ID_KEYBOARD, HID_KEY_BRACKET_LEFT},
    {TAG_SQB_CLS, REPORT_ID_KEYBOARD, HID_KEY_BRACKET_RIGHT},
    {TAG_BACKSLASH, REPORT_ID_KEYBOARD, HID_KEY_BACKSLASH},
    {TAG_HASH, REPORT_ID_KEYBOARD, HID_KEY_EUROPE_1},
    {TAG_SEMICOLON, REPORT_ID_KEYBOARD, HID_KEY_SEMICOLON},
    {TAG_SQUOTE, REPORT_ID_KEYBOARD, HID_KEY_APOSTROPHE},
    {TAG_TILDE, REPORT_ID_KEYBOARD, HID_KEY_GRAVE},
    {TAG_COMMA, REPORT_ID_KEYBOARD, HID_KEY_COMMA},
    {TAG_DOT, REPORT_ID_KEYBOARD, HID_KEY_PERIOD},
    {TAG_SLASH, REPORT_ID_KEYBOARD, HID_KEY_SLASH},
    {TAG_CAPS_LOCK, REPORT_ID_KEYBOARD, HID_KEY_CAPS_LOCK},
    {TAG_F1, REPORT_ID_KEYBOARD, HID_KEY_F1},
    {TAG_F2, REPORT_ID_KEYBOARD, HID_KEY_F2},
    {TAG_F3, REPORT_ID_KEYBOARD, HID_KEY_F3},
    {TAG_F4, REPORT_ID_KEYBOARD, HID_KEY_F4},
    {TAG_F5, REPORT_ID_KEYBOARD, HID_KEY_F5},
    {TAG_F6, REPORT_ID_KEYBOARD, HID_KEY_F6},
    {TAG_F7, REPORT_ID_KEYBOARD, HID_KEY_F7},
    {TAG_F8, REPORT_ID_KEYBOARD, HID_KEY_F8},
    {TAG_F9, REPORT_ID_KEYBOARD, HID_KEY_F9},
    {TAG_F10, REPORT_ID_KEYBOARD, HID_KEY_F10},
    {TAG_F11, REPORT_ID_KEYBOARD, HID_KEY_F11},
    {TAG_F12, REPORT_ID_KEYBOARD, HID_KEY_F12},
    {TAG_PRINT_SCREEN, REPORT_ID_KEYBOARD, HID_KEY_PRINT_SCREEN},
    {TAG_SCROLL_LOCK, REPORT_ID_KEYBOARD, HID_KEY_SCROLL_LOCK},
    {TAG_PAUSE, REPORT_ID_KEYBOARD, HID_KEY_PAUSE},
    {TAG_INSERT, REPORT_ID_KEYBOARD, HID_KEY_INSERT},
    {TAG_HOME, REPORT_ID_KEYBOARD, HID_KEY_HOME},
    {TAG_PAGE_UP, REPORT_ID_KEYBOARD, HID_KEY_PAGE_UP},
    {TAG_DEL, REPORT_ID_KEYBOARD, HID_KEY_DELETE},
    {TAG_END, REPORT_ID_KEYBOARD, HID_KEY_END},
    {TAG_PAGE_DOWN, REPORT_ID_KEYBOARD, HID_KEY_PAGE_DOWN},
    {TAG_RIGHT_ARROW, REPORT_ID_KEYBOARD, HID_KEY_ARROW_RIGHT},
    {TAG_LEFT_ARROW, REPORT_ID_KEYBOARD, HID_KEY_ARROW_LEFT},
    {TAG_DOWN_ARROW, REPORT_ID_KEYBOARD, HID_KEY_ARROW_DOWN},
    {TAG_UP_ARROW, REPORT_ID_KEYBOARD, HID_KEY_ARROW_UP},
    {TAG_NUMBER_LOCK, REPORT_ID_KEYBOARD, HID_KEY_NUM_LOCK},
    {TAG_PAD_DIV, REPORT_ID_KEYBOARD, HID_KEY_KEYPAD_DIVIDE},
    {TAG_PAD_MUL, REPORT_ID_KEYBOARD, HID_KEY_KEYPAD_MULTIPLY},
    {TAG_PAD_MINUS, REPORT_ID_KEYBOARD, HID_KEY_KEYPAD_SUBTRACT},
    {TAG_PAD_PLUS, REPORT_ID_KEYBOARD, HID_KEY_KEYPAD_ADD},
    {TAG_PAD_ENTER, REPORT_ID_KEYBOARD, HID_KEY_KEYPAD_ENTER},
    {TAG_PAD_1, REPORT_ID_KEYBOARD, HID_KEY_KEYPAD_1},
    {TAG_PAD_2, REPORT_ID_KEYBOARD, HID_KEY_KEYPAD_2},
    {TAG_PAD_3, REPORT_ID_KEYBOARD, HID_KEY_KEYPAD_3},
    {TAG_PAD_4, REPORT_ID_KEYBOARD, HID_KEY_KEYPAD_4},
    {TAG_PAD_5, REPORT_ID_KEYBOARD, HID_KEY_KEYPAD_5},
    {TAG_PAD_6, REPORT_ID_KEYBOARD, HID_KEY_KEYPAD_6},
    {TAG_PAD_7, REPORT_ID_KEYBOARD, HID_KEY_KEYPAD_7},
    {TAG_PAD_8, REPORT_ID_KEYBOARD, HID_KEY_KEYPAD_8},
    {TAG_PAD_9, REPORT_ID_KEYBOARD, HID_KEY_KEYPAD_9},
    {TAG_PAD_0, REPORT_ID_KEYBOARD, HID_KEY_KEYPAD_0},
    {TAG_PAD_DOT, REPORT_ID_KEYBOARD, HID_KEY_KEYPAD_DECIMAL},
    {TAG_APP, REPORT_ID_KEYBOARD, HID_KEY_APPLICATION},
    {TAG_CTRLL, REPORT_ID_KEYBOARD, HID_KEY_CONTROL_LEFT},
    {TAG_SHIFTL, REPORT_ID_KEYBOARD, HID_KEY_SHIFT_LEFT},
    {TAG_ALT, REPORT_ID_KEYBOARD, HID_KEY_ALT_LEFT},
    {TAG_WINL, REPORT_ID_KEYBOARD, HID_KEY_GUI_LEFT},
    {TAG_CTRLR, REPORT_ID_KEYBOARD, HID_KEY_CONTROL_RIGHT},
    {TAG_SHIFTR, REPORT_ID_KEYBOARD, HID_KEY_SHIFT_RIGHT},
    {TAG_ALTGR, REPORT_ID_KEYBOARD, HID_KEY_ALT_RIGHT},
    {TAG_WINR, REPORT_ID_KEYBOARD, HID_KEY_GUI_RIGHT},

    {TAG_SC_POWER, REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_POWER},
    {TAG_SC_SLEEP, REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_SLEEP},
    {KEY_SC_WAKEUP, REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_RESET},

    {TAG_CC_PLAY, REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_PLAY_PAUSE},
    {TAG_CC_NEXT, REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_SCAN_NEXT},
    {TAG_CC_PREV, REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_SCAN_PREVIOUS},
    {TAG_CC_STOP, REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_STOP},
    {TAG_CC_MUTE, REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_MUTE},
    {TAG_CC_VOL_UP, REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_INCREMENT},
    {TAG_CC_VOL_DOWN, REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_DECREMENT},

    {TAG_CC_CUT, REPORT_ID_KEYBOARD, HID_KEY_CUT},
    {TAG_CC_COPY, REPORT_ID_KEYBOARD, HID_KEY_COPY},
    {TAG_CC_PASTE, REPORT_ID_KEYBOARD, HID_KEY_PASTE},
    {TAG_CC_UNDO, REPORT_ID_KEYBOARD, HID_KEY_UNDO},
    {TAG_CC_REDO, REPORT_ID_KEYBOARD, HID_KEY_AGAIN},
    {TAG_CC_FIND, REPORT_ID_KEYBOARD, HID_KEY_FIND},
    //{TAG_CC_MENU, REPORT_ID_KEYBOARD, HID_KEY_MENU},
    //{TAG_CC_SELECT, REPORT_ID_KEYBOARD, HID_KEY_SELECT},
    //{TAG_CC_EXECUTE, REPORT_ID_KEYBOARD, HID_KEY_EXECUTE},
};

static struct key_scan key_scan;
static struct key_report key_report;

/* LOCAL FUNCTIONS / INLINES *******************************************************/

void led_state(uint8_t state);
void keyboard_update(uint8_t leds);
bool is_report_busy(void);
void send_hid_report_keyboard(uint8_t modifier, uint8_t keycode);
void send_hid_report_control(uint16_t map);
void hid_task(void);
void setup(void);

/* TINYUSB FUNCTIONS ***************************************************************/

// Marker to show a report is in progress.
static bool report_busy = 0;

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
    eve_keyboard_start();
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
    eve_ui_wait();
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
    (void) remote_wakeup_en;
    eve_ui_wait();
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
    eve_keyboard_start();
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

bool is_report_busy(void)
{
    return report_busy;
}

void send_hid_report_keyboard(uint8_t modifier, uint8_t keycode)
{
    static uint8_t keycode_map[6] = {0,0,0,0,0,0};

    // skip if hid is not ready yet
    if ( !tud_hid_ready() ) return;

    keycode_map[0] = keycode;

    // use to avoid send multiple consecutive zero report for keyboard
    static bool has_keyboard_key = false;

    report_busy = true;

    if (keycode)
    {
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, modifier, keycode_map);
        has_keyboard_key = true;
        led_state(1);
    }
    else
    {
        // send empty key report if previously has key pressed
        if (has_keyboard_key) tud_hid_keyboard_report(REPORT_ID_KEYBOARD, modifier, NULL);
        has_keyboard_key = false;
        led_state(0);
    }
}

void send_hid_report_control(uint16_t map)
{
    // use to avoid send multiple consecutive zero report
    static bool has_consumer_key = false;

    report_busy = true;

    if (map)
    {
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &map, 2);
        has_consumer_key = true;
        led_state(1);
    }
    else
    {
        // send empty key report (release key) if previously has key pressed
        if (has_consumer_key) tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &map, 2);
        has_consumer_key = false;
        led_state(0);
    }
}

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
void hid_task()
{
    // Poll every 10ms
    const uint32_t interval_ms = 10;
    static uint32_t start_ms = 0;

    if ( board_millis() - start_ms < interval_ms) return; // not enough time
    start_ms += interval_ms;

    uint32_t const btn = board_button_read();

    // Remote wakeup
    if ( tud_suspended() && btn )
    {
        // Wake up host if we are in suspend mode
        // and REMOTE_WAKEUP feature is enabled by host
        tud_remote_wakeup();
    }
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint8_t len)
{
    (void) instance;
    (void) report;
    (void) len;
    
    report_busy = false;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
    // TODO not Implemented
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    (void) instance;

    if (report_type == HID_REPORT_TYPE_OUTPUT)
    {
        // Set keyboard LED e.g Capslock, Numlock etc...
        if (report_id == REPORT_ID_KEYBOARD)
        {
            // bufsize should be (at least) 1
            if ( bufsize < 1 ) return;

            uint8_t const kbd_leds = buffer[0];
            keyboard_update(kbd_leds);
        }
    }
}

/* KEYBOARD MANAGEMENT *************************************************************/

void keyboard_start()
{
	eve_keyboard_start();
}

void keyboard_update(uint8_t leds)
{
    memset(&key_report, 0, sizeof(key_report));
    if (leds & KEYBOARD_LED_NUMLOCK) key_report.Numeric = 1;
    if (leds & KEYBOARD_LED_CAPSLOCK) key_report.Caps = 1;
    if (leds & KEYBOARD_LED_SCROLLLOCK) key_report.Scroll = 1;
    
	eve_keyboard_loop(&key_report, &key_scan);

	kbd_led_status = leds;
}

int keyboard_loop(uint8_t *modifier, uint8_t *keycode, uint16_t *ctrl_map, uint8_t leds)
{
	static uint8_t active_report = -1;
	static uint8_t active_keycode = 0;

	int i;
	uint8_t report_send = -1;
	uint16_t scancode = 0;

    memset(&key_report, 0, sizeof(key_report));
    if (leds & KEYBOARD_LED_NUMLOCK) key_report.Numeric = 1;
    if (leds & KEYBOARD_LED_CAPSLOCK) key_report.Caps = 1;
    if (leds & KEYBOARD_LED_SCROLLLOCK) key_report.Scroll = 1;
    
	if (eve_keyboard_loop(&key_report, &key_scan) == 0)
	{
		for (i = 0; i < sizeof(tagmap) / sizeof(tagmap[0]); i++)
		{
			if (tagmap[i].tag == key_scan.KeyTag)
			{
				active_report = tagmap[i].report;
				scancode = tagmap[i].scancode;
				break;
			}
		}

		if (scancode)
		{
			if (active_report == REPORT_ID_KEYBOARD)
			{
                *modifier = 0;
				if (key_scan.ShiftL) *modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
				if (key_scan.ShiftR) *modifier |= KEYBOARD_MODIFIER_RIGHTSHIFT;
				if (key_scan.CtrlL) *modifier |= KEYBOARD_MODIFIER_LEFTCTRL;
				if (key_scan.CtrlR) *modifier |= KEYBOARD_MODIFIER_RIGHTCTRL;
				if (key_scan.Alt) *modifier |= KEYBOARD_MODIFIER_LEFTALT;
				if (key_scan.AltGr) *modifier |= KEYBOARD_MODIFIER_RIGHTALT;
				if (key_scan.WinL) *modifier |= KEYBOARD_MODIFIER_LEFTGUI;
				if (key_scan.WinR) *modifier |= KEYBOARD_MODIFIER_RIGHTGUI;

				*keycode = scancode & 0xff;
			}
			else if (active_report == REPORT_ID_CONSUMER_CONTROL)
			{
                *ctrl_map = scancode;
			}
			else if (active_report == REPORT_ID_CONSUMER_CONTROL)
			{
				*ctrl_map = scancode;
			}
			else
			{
				active_report = -1;
			}
			report_send = active_report;
		}
		else
		{
			if (active_report == REPORT_ID_KEYBOARD)
			{
				*keycode = 0;
                *modifier = 0;

				// Pressing an alphanumeric or symbol key will clear
				// the state of the special function keys.
				key_scan.ShiftL = 0;
				key_scan.ShiftR = 0;
				key_scan.CtrlL = 0;
				key_scan.CtrlR = 0;
				key_scan.Alt = 0;
				key_scan.AltGr = 0;
				key_scan.WinL = 0;
				key_scan.WinR = 0;
			}
			else if (active_report == REPORT_ID_CONSUMER_CONTROL)
			{
				*ctrl_map = 0;
			}
			else if (active_report == REPORT_ID_CONSUMER_CONTROL)
			{
				*ctrl_map = 0;
			}

			report_send = active_report;
			active_report = -1;
		}
	}
	return report_send;
}

/* FUNCTIONS ***********************************************************************/

int main(void)
{
    uint8_t report_id;

    /* Setup UART */
    setup();
	eve_ui_setup();
	// Draw a waiting for host message.
	eve_ui_wait();

    board_init();
    tusb_init();

    /* Start example code */
    while (1)
    {			
        tud_task(); // tinyusb device task

        if (!is_report_busy())
        {
            report_id = keyboard_loop(&kbd_modifier, &kbd_keycode, &kbd_ctrl_map, kbd_led_status);
            
            if (report_id == REPORT_ID_KEYBOARD)
            {
                send_hid_report_keyboard(kbd_modifier, kbd_keycode);
            }
            if (report_id == REPORT_ID_CONSUMER_CONTROL)
            {
                send_hid_report_control(kbd_ctrl_map);
            }
        }

        hid_task();
    }

    // function never returns
    for (;;) ;
}

void led_state(uint8_t state)
{
    gpio_put(LED_PIN, state);
}

void setup(void)
{
    int ch = 0;

#ifdef DEBUG
    // Initialise stdio ports as configured in CMakeLists.txt
    stdio_init_all();
#endif

    // Turn on the pico LED to show activity
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);
    
#ifdef DEBUG
    /* Print out a welcome message... */
    printf ("(C) Copyright, Bridgetek Pte. Ltd. \r\n \r\n");
    printf ("---------------------------------------------------------------- \r\n");
    printf ("Welcome to BRT_AN_012 Example for Raspberry Pi Pico RP2040\r\n");
    printf ("\n");
	printf ("Pin configuration for example:\n");
	printf ("Use SPI 1 hardware bus to match IDM2040-7A from Bridgetek\n");
	printf ("Pin 15 - MOSI (GPIO11)\n");
	printf ("Pin 16 - MISO (GPIO12)\n");
	printf ("Pin 14 - SCLK (GPIO10)\n");
	printf ("Pin 17 - CS (GPIO13) - Note this is not the SPI0_CS0 pin\n");
	printf ("Pin 10 - PD# (GPIO7) - Powerdown pin\n");
	printf ("Pin 40 - 5v supply for FT8xx\n");
	printf ("Pin 23 - signal GND for SPI\n");
#endif
}