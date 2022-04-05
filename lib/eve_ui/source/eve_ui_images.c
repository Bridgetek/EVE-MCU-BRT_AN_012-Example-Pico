/**
 @file bridgetek.png.c
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

#include "eve_ui.h"

uint32_t eve_ui_load_image(const uint8_t *img_data, uint32_t start_addr, uint8_t handle,
		uint16_t *width, uint16_t *height)
{
	uint8_t buf[128];
	uint32_t img_width = 0;
	uint32_t img_height = 0;
	uint32_t img_addr;
	uint8_t *flash_addr = (uint8_t *)(img_data);
	int8_t flag;
	int i;

	flag = 0;
	EVE_LIB_BeginCoProList();
	EVE_CMD_LOADIMAGE(start_addr, 0);
	// Send raw JPEG encoded image data to coprocessor. It will be decoded
	// as the data is received.
	while (flag != 2)
	{
		memcpy(buf, flash_addr, sizeof(buf));
		for (i = 0; i < sizeof(buf); i++)
		{
			if (buf[i] == 0xff)
			{
				flag = 1;
			}
			else
			{
				if (flag == 1)
				{
					if (buf[i] == 0xd9)
					{
						flag = 2;
						i++;
						break;
					}
				}
				flag = 0;
			}
		}
		EVE_LIB_WriteDataToCMD(buf, (i + 3)&(~3));
		flash_addr += i;
	};

	EVE_LIB_EndCoProList();
	EVE_LIB_AwaitCoProEmpty();

	// We know the start address in RAM so do not seek this information.
	// Obtain the width and height of the expanded JPEG image.
	EVE_LIB_GetProps(&img_addr, &img_width, &img_height);

	*width = (uint16_t)img_width;
	*height = (uint16_t)img_height;

	EVE_LIB_BeginCoProList();
	EVE_CMD_DLSTART();

	EVE_BEGIN(EVE_BEGIN_BITMAPS);
	EVE_BITMAP_HANDLE(handle);
	EVE_BITMAP_SOURCE(start_addr);
	EVE_BITMAP_LAYOUT(EVE_FORMAT_RGB565, img_width * 2, img_height);
	EVE_BITMAP_LAYOUT_H((img_width * 2) >> 10, img_height >> 9);
	EVE_BITMAP_SIZE(EVE_FILTER_NEAREST, EVE_WRAP_BORDER, EVE_WRAP_BORDER,
			img_width, img_height);
	EVE_BITMAP_SIZE_H(img_width >> 9, img_height >> 9);
	EVE_END();

	EVE_DISPLAY();
	EVE_CMD_SWAP();
	EVE_LIB_EndCoProList();
	EVE_LIB_AwaitCoProEmpty();

	start_addr += ((img_width * 2) * img_height);
	start_addr = (start_addr + 3) & (~3);

	return start_addr;
}

extern const uint8_t img_bridgetek_logo_jpg[] asm("img_bridgetek_logo_jpg");
extern const uint8_t img_settings_jpg[] asm("img_settings_jpg");
extern const uint8_t img_cancel_jpg[] asm("img_cancel_jpg");
extern const uint8_t img_tick_jpg[] asm("img_z_jpg");
extern const uint8_t img_refresh_jpg[] asm("img_refresh_jpg");
extern const uint8_t img_keypad_jpg[] asm("img_keypad_jpg");
extern const uint8_t img_keyboard_jpg[] asm("img_keyboard_jpg");
extern const uint8_t img_media_jpg[] asm("img_media_jpg");

uint32_t eve_ui_load_images(uint32_t start_addr)
{
	uint32_t dummy = start_addr;

	// Load images statically and sequentially.
	dummy = eve_ui_load_image(img_bridgetek_logo_jpg, dummy,
			BITMAP_BRIDGETEK_LOGO, &img_bridgetek_logo_width, &img_bridgetek_logo_height);

	dummy = eve_ui_load_image(img_settings_jpg, dummy,
			BITMAP_SETTINGS, &img_settings_width, &img_settings_height);

	dummy = eve_ui_load_image(img_cancel_jpg, dummy,
			BITMAP_CANCEL, &img_cancel_width, &img_cancel_height);

	dummy = eve_ui_load_image(img_tick_jpg, dummy,
			BITMAP_SAVE, &img_tick_width, &img_tick_height);

	dummy = eve_ui_load_image(img_refresh_jpg, dummy,
			BITMAP_REFRESH, &img_refresh_width, &img_refresh_height);

	dummy = eve_ui_load_image(img_keypad_jpg, dummy,
			BITMAP_KEYPAD, &img_keypad_width, &img_keypad_height);

	dummy = eve_ui_load_image(img_keyboard_jpg, dummy,
			BITMAP_KEYBOARD, &img_keyboard_width, &img_keyboard_height);

	dummy = eve_ui_load_image(img_media_jpg, dummy,
			BITMAP_MEDIA, &img_media_width, &img_media_height);

	return dummy;
}
