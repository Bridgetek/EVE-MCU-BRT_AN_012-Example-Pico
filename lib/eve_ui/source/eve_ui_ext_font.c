/**
 @file arial_L4_15.c
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

uint32_t eve_ui_load_font(const uint8_t *font_data, uint32_t font_size, uint8_t handle, uint32_t start_addr)
{
	const EVE_GPU_FONT_HEADER *font_hdr = (EVE_GPU_FONT_HEADER *)font_data;

	eve_ui_arch_write_ram_from_pm(font_data, font_size, start_addr);

	EVE_LIB_BeginCoProList();
	EVE_CMD_DLSTART();
	EVE_CLEAR(1,1,1);
	EVE_COLOR_RGB(255, 255, 255);
	EVE_BEGIN(EVE_BEGIN_BITMAPS);
	EVE_BITMAP_HANDLE(handle);
	EVE_BITMAP_HANDLE(handle);
	EVE_BITMAP_SOURCE(font_hdr->PointerToFontGraphicsData);
	EVE_BITMAP_LAYOUT(font_hdr->FontBitmapFormat,
			font_hdr->FontLineStride, font_hdr->FontHeightInPixels);
	EVE_BITMAP_SIZE(EVE_FILTER_NEAREST, EVE_WRAP_BORDER, EVE_WRAP_BORDER,
			font_hdr->FontWidthInPixels,
			font_hdr->FontHeightInPixels);
	EVE_CMD_SETFONT(handle, start_addr);
	EVE_END();

	EVE_DISPLAY();
	EVE_CMD_SWAP();
	EVE_LIB_EndCoProList();
	EVE_LIB_AwaitCoProEmpty();

	return ((font_size + start_addr) + 16) & (~15);
}
