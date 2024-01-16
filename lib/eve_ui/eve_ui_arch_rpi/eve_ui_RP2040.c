/**
  @file eve_ui_RP2040.c
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

// Guard against being used for incorrect CPU type.
#if defined(PLATFORM_RP2040)

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "pico/time.h"

#include "EVE_config.h"
#include "EVE.h"
#include "HAL.h"

#include "eve_ui.h"

/* Flash read/write capability for storing Non-volatile data */
#include <hardware/flash.h>
#include <hardware/sync.h>

/* CONSTANTS ***********************************************************************/

/**
 * @brief Location in flash to store touchscreen configuration.
 */
#define FLASH_OFFSET_CONFIG (256 * 1024)

/**
 @brief Key for identifying if touchscreen calibration values are programmed into
  the Flash.
 */
#define VALID_KEY_TOUCHSCREEN 0xd72f91a3

/* GLOBAL VARIABLES ****************************************************************/

/* LOCAL VARIABLES *****************************************************************/

/* MACROS **************************************************************************/

/* LOCAL FUNCTIONS / INLINES *******************************************************/

/**
 * @brief Functions used to store calibration data in file.
 */
//@{
int8_t eve_ui_arch_flash_calib_init(void)
{
    return 0;
}

int8_t eve_ui_arch_flash_calib_write(struct touchscreen_calibration *calib)
{
#if (PICO_FLASH_SIZE_BYTES < FLASH_OFFSET_CONFIG + FLASH_SECTOR_SIZE)
#error Configuration written above top of flash
#endif
    // Data to write to flash must be aligned to a flash page
    uint8_t config[FLASH_PAGE_SIZE] __aligned(FLASH_PAGE_SIZE);
    uint32_t ints = save_and_disable_interrupts();

	calib->key = VALID_KEY_TOUCHSCREEN;
    memset(config, 0xff, FLASH_PAGE_SIZE);
    memcpy(config, calib, sizeof(struct touchscreen_calibration));

    flash_range_erase(FLASH_OFFSET_CONFIG, FLASH_SECTOR_SIZE);
    flash_range_program(FLASH_OFFSET_CONFIG, (const uint8_t *)config, FLASH_PAGE_SIZE);
    restore_interrupts (ints);

    return 0;
}

int8_t eve_ui_arch_flash_calib_read(struct touchscreen_calibration *calib)
{
    struct touchscreen_calibration *p = (struct touchscreen_calibration *)(XIP_BASE + FLASH_OFFSET_CONFIG);
	if (p->key == VALID_KEY_TOUCHSCREEN)
    {
        memcpy(calib, p, sizeof(struct touchscreen_calibration));
        return 0;
    }

    return -2;
}
//@}

void eve_ui_arch_write_cmd_from_pm(const uint8_t *ImgData, uint32_t length)
{
	uint32_t offset = 0;
	uint8_t ramData[512];
	uint32_t left;

	while (offset < length)
	{
		memcpy(ramData, (const void *)ImgData, 512);

		if (length - offset < 512)
		{
			left = length - offset;
		}
		else
		{
			left = 512;
		}
		EVE_LIB_WriteDataToCMD(ramData, left);
		offset += left;
		ImgData += left;
	};
}

void eve_ui_arch_write_ram_from_pm(const uint8_t *ImgData, uint32_t length, uint32_t dest)
{
	uint32_t offset = 0;
	uint8_t ramData[512];
	uint32_t left;

	while (offset < length)
	{
		memcpy(ramData, (const void *)ImgData, 512);

		if (length - offset < 512)
		{
			left = length - offset;
		}
		else
		{
			left = 512;
		}
		EVE_LIB_WriteDataToRAMG(ramData, left, dest);
		offset += left;
		ImgData += left;
		dest += left;
	};
}

void eve_ui_memcpy_pm(void *dst, const void* src, size_t s)
{
	memcpy(dst, src, s);
}

void eve_ui_arch_sleepms(uint32_t delay)
{
	sleep_ms(delay);
}

/* FUNCTIONS ***********************************************************************/

#endif /* defined(PLATFORM_RP2040) */
