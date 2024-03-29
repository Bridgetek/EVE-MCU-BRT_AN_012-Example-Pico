/**
  @file eve_ui_main.c
 */
/*
 * ============================================================================
 * History
 * =======
 * 2017-03-15 : Created
 *
 * (C) Copyright Bridgetek Pte Ltd
 * ============================================================================
 *
 * This source code ("the Software") is provided by Bridgetek Pte Ltd
 * ("Bridgetek") subject to the licence terms set out
 * http://www.ftdichip.com/FTSourceCodeLicenceTerms.htm ("the Licence Terms").
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

#include "eve_ui.h"

/* CONSTANTS ***********************************************************************/

/**
 @brief Allow click on the BridgeTek Logo to make a screenshot.
 */
#define ENABLE_SCREENSHOT

/* GLOBAL VARIABLES ****************************************************************/

/**
 @brief Dimensions of custom images.
 */
//@{
uint16_t img_bridgetek_logo_width;
uint16_t img_bridgetek_logo_height;
uint16_t img_settings_width;
uint16_t img_settings_height;
uint16_t img_cancel_width;
uint16_t img_cancel_height;
uint16_t img_tick_width;
uint16_t img_tick_height;
uint16_t img_refresh_width;
uint16_t img_refresh_height;
uint16_t img_keypad_width;
uint16_t img_keypad_height;
uint16_t img_keyboard_width;
uint16_t img_keyboard_height;
uint16_t img_media_width;
uint16_t img_media_height;
//@}

/**
 @brief Free RAM_DL after custom images.
 */
uint32_t img_end_address;

/* LOCAL VARIABLES *****************************************************************/

/* MACROS **************************************************************************/

/* LOCAL FUNCTIONS / INLINES *******************************************************/

/* FUNCTIONS ***********************************************************************/

void eve_ui_setup()
{
	uint32_t img_address;

	EVE_Init();

	eve_ui_calibrate();

	img_address = eve_ui_load_fonts();
	// Decode JPEG images from flash into RAM_DL on FT8xx.
	// Start at RAM_G after fonts (as font addresses must be fixed).
	img_end_address = eve_ui_load_images(img_address);
}

void eve_ui_wait(void)
{
	uint8_t key_code = 0;
	uint8_t key_detect = 0;

	eve_ui_splash("Waiting for host...", 0);

	key_detect = eve_ui_read_tag(&key_code);
	if (key_detect)
	{
	}
	sleep_ms(100);
}

void eve_ui_calibrate()
{
	struct touchscreen_calibration calib;

	eve_ui_arch_flash_calib_init();

	if (eve_ui_arch_flash_calib_read(&calib) != 0)
	{
		EVE_LIB_BeginCoProList();
		EVE_CMD_DLSTART();
		EVE_CLEAR_COLOR_RGB(0, 0, 0);
		EVE_CLEAR(1,1,1);
		EVE_COLOR_RGB(255, 255, 255);
		EVE_CMD_TEXT(EVE_DISP_WIDTH/2, EVE_DISP_HEIGHT/2,
				FONT_HEADER, EVE_OPT_CENTERX | EVE_OPT_CENTERY,
				"Please tap on the dots");
		EVE_CMD_CALIBRATE(0);
		EVE_LIB_EndCoProList();
		EVE_LIB_AwaitCoProEmpty();

		calib.transform[0] = HAL_MemRead32(EVE_REG_TOUCH_TRANSFORM_A);
		calib.transform[1] = HAL_MemRead32(EVE_REG_TOUCH_TRANSFORM_B);
		calib.transform[2] = HAL_MemRead32(EVE_REG_TOUCH_TRANSFORM_C);
		calib.transform[3] = HAL_MemRead32(EVE_REG_TOUCH_TRANSFORM_D);
		calib.transform[4] = HAL_MemRead32(EVE_REG_TOUCH_TRANSFORM_E);
		calib.transform[5] = HAL_MemRead32(EVE_REG_TOUCH_TRANSFORM_F);
		eve_ui_arch_flash_calib_write(&calib);
	}
	HAL_MemWrite32(EVE_REG_TOUCH_TRANSFORM_A, calib.transform[0]);
	HAL_MemWrite32(EVE_REG_TOUCH_TRANSFORM_B, calib.transform[1]);
	HAL_MemWrite32(EVE_REG_TOUCH_TRANSFORM_C, calib.transform[2]);
	HAL_MemWrite32(EVE_REG_TOUCH_TRANSFORM_D, calib.transform[3]);
	HAL_MemWrite32(EVE_REG_TOUCH_TRANSFORM_E, calib.transform[4]);
	HAL_MemWrite32(EVE_REG_TOUCH_TRANSFORM_F, calib.transform[5]);
}

void eve_ui_screenshot()
{
#ifdef ENABLE_SCREENSHOT
	uint8_t buffer[256];
	int i, j;

	// Write screenshot into RAM_G
	EVE_LIB_BeginCoProList();
	EVE_CMD_DLSTART();
	EVE_CMD_SNAPSHOT(img_end_address);
	EVE_LIB_EndCoProList();
	EVE_LIB_AwaitCoProEmpty();

	printf("ARGB start\n"); // Use this marker to identify the start of the image.
	for (i = 0; i < (EVE_DISP_WIDTH * 2) * EVE_DISP_HEIGHT; i += sizeof(buffer))
	{
		EVE_LIB_ReadDataFromRAMG(buffer, sizeof(buffer), img_end_address+ i);
		for (j = 0; j < sizeof(buffer); j++)
		{
			printf("%c", buffer[j]);
		}
	}
	printf("ARGB end\n"); // Marker to identify the end of the image.

	eve_ui_splash("Screenshot completed...", 0);
	eve_ui_arch_sleepms(2000);

#endif // ENABLE_SCREENSHOT
}

void eve_ui_header_bar(uint32_t options)
{
	uint32_t x = EVE_SPACER;

	EVE_TAG(TAG_NO_ACTION);
	EVE_COLOR_RGB(128, 128, 128);
	EVE_BEGIN(EVE_BEGIN_RECTS);
	EVE_VERTEX2F(0 * 16, 0 * 16);
	EVE_VERTEX2F(EVE_DISP_WIDTH * 16, (EVE_DISP_HEIGHT / 8) * 16);

	if (options & EVE_HEADER_LOGO)
	{
		EVE_TAG(TAG_LOGO);
		EVE_BEGIN(EVE_BEGIN_BITMAPS);
		EVE_VERTEX_TRANSLATE_X(((EVE_DISP_WIDTH/2)-((uint32_t)img_bridgetek_logo_width/2)) * 16);
		EVE_VERTEX2II(0, 0, BITMAP_BRIDGETEK_LOGO, 0);
	}
	EVE_VERTEX_TRANSLATE_Y(EVE_SPACER * 16);
	if (options & EVE_HEADER_SETTINGS_BUTTON)
	{
		EVE_TAG(TAG_SETTINGS);
		EVE_BEGIN(EVE_BEGIN_BITMAPS);
		EVE_VERTEX_TRANSLATE_X(x * 16);
		EVE_VERTEX2II(0, 0, BITMAP_SETTINGS, 0);
		x += (img_settings_width + EVE_SPACER);
	}
	if (options & EVE_HEADER_REFRESH_BUTTON)
	{
		EVE_TAG(TAG_REFRESH);
		EVE_BEGIN(EVE_BEGIN_BITMAPS);
		EVE_VERTEX_TRANSLATE_X(x * 16);
		EVE_VERTEX2II(0, 0, BITMAP_REFRESH, 0);
		x += (img_refresh_width + EVE_SPACER);
	}
	if (options & EVE_HEADER_CANCEL_BUTTON)
	{
		EVE_TAG(TAG_CANCEL);
		EVE_BEGIN(EVE_BEGIN_BITMAPS);
		EVE_VERTEX_TRANSLATE_X(x * 16);
		EVE_VERTEX2II(0, 0, BITMAP_CANCEL, 0);
		x += (img_cancel_width + EVE_SPACER);
	}
	if (options & EVE_HEADER_SAVE_BUTTON)
	{
		EVE_TAG(TAG_SAVE);
		EVE_BEGIN(EVE_BEGIN_BITMAPS);
		EVE_VERTEX_TRANSLATE_X(x * 16);
		EVE_VERTEX2II(0, 0, BITMAP_SAVE, 0);
		x += (img_tick_width + EVE_SPACER);
	}

	x = EVE_DISP_WIDTH - EVE_SPACER;
	if (options & EVE_HEADER_KEYPAD_BUTTON)
	{
		EVE_TAG(TAG_KEYPAD);
		EVE_BEGIN(EVE_BEGIN_BITMAPS);
		x -= (img_keypad_width + EVE_SPACER);
		EVE_VERTEX_TRANSLATE_X(x * 16);
		EVE_VERTEX2II(0, 0, BITMAP_KEYPAD, 0);
	}
	if (options & EVE_HEADER_KEYBOARD_BUTTON)
	{
		EVE_TAG(TAG_KEYBOARD);
		EVE_BEGIN(EVE_BEGIN_BITMAPS);
		x -= (img_keyboard_width + EVE_SPACER);
		EVE_VERTEX_TRANSLATE_X(x * 16);
		EVE_VERTEX2II(0, 0, BITMAP_KEYBOARD, 0);
	}
	if (options & EVE_HEADER_EXTRA_BUTTON)
	{
		EVE_TAG(TAG_MEDIA);
		EVE_BEGIN(EVE_BEGIN_BITMAPS);
		x -= (img_media_width + EVE_SPACER);
		EVE_VERTEX_TRANSLATE_X(x * 16);
		EVE_VERTEX2II(0, 0, BITMAP_MEDIA, 0);
	}
	if (options & EVE_HEADER_SPECIAL_BUTTON)
	{
		EVE_TAG(TAG_SPECIAL);
		EVE_BEGIN(EVE_BEGIN_BITMAPS);
		x -= (img_tick_width + EVE_SPACER);
		EVE_VERTEX_TRANSLATE_X(x * 16);
		EVE_VERTEX2II(0, 0, BITMAP_SAVE, 0);
	}
	EVE_VERTEX_TRANSLATE_X(0);
	EVE_VERTEX_TRANSLATE_Y(0);
}

void eve_ui_play_sound(uint8_t sound, uint8_t volume)
{
	HAL_MemWrite8(EVE_REG_VOL_SOUND, volume);
	HAL_MemWrite8(EVE_REG_SOUND, sound);
	HAL_MemWrite8(EVE_REG_PLAY, 1);
}

uint8_t eve_ui_read_tag(uint8_t *key)
{
	uint8_t Read_tag;
	uint8_t key_detect = 0;

	Read_tag = HAL_MemRead8(EVE_REG_TOUCH_TAG);

	if (!(HAL_MemRead16(EVE_REG_TOUCH_RAW_XY) & 0x8000))
	{
		key_detect = 1;
		*key = Read_tag;
	}

	return key_detect;
}

void eve_ui_splash(char *toast, uint32_t options)
{
	EVE_LIB_BeginCoProList();
	EVE_CMD_DLSTART();
	EVE_CLEAR_COLOR_RGB(0, 0, 0);
	EVE_CLEAR(1,1,1);
	//EVE_CLEAR_TAG(TAG_NO_ACTION);
	EVE_COLOR_RGB(255, 255, 255);
	eve_ui_header_bar(EVE_HEADER_LOGO);
	EVE_CMD_TEXT(EVE_DISP_WIDTH/2, EVE_DISP_HEIGHT/2,
			FONT_HEADER, EVE_OPT_CENTERX | EVE_OPT_CENTERY, toast);
	//eve_ui_header_bar(options);
	EVE_DISPLAY();
	EVE_CMD_SWAP();
	EVE_LIB_EndCoProList();
	EVE_LIB_AwaitCoProEmpty();
}

