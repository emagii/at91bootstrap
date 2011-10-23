/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2006, Atmel Corporation

 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 * File Name           : main.c
 * Object              :
 * Creation            : ODi Apr 19th 2006
 *-----------------------------------------------------------------------------
 */
#include "common.h"
#include "dataflash.h"
#include "gpio.h"
#include "nandflash.h"
#include "flash.h"

#ifdef CONFIG_USER_HW_INIT
void user_hw_init(void);
#endif

/* Function import from startup.s file */
extern void Jump(unsigned int addr);

extern unsigned int load_SDCard(void *dst);

void LoadLinux();

void LoadWince();

unsigned int	img_address	= IMG_ADDRESS;
unsigned int	img_size	= IMG_SIZE;
unsigned int	jump_addr	= JUMP_ADDR;
unsigned int	altboot		= 0;

/*------------------------------------------------------------------------------*/
/* Function Name       : Wait							*/
/* Object              : software loop waiting function				*/
/*------------------------------------------------------------------------------*/
#ifdef WINCE
void Wait(unsigned int count)
{
    volatile unsigned int i;
    volatile unsigned int j = 0;

    for (i = 0; i < count; i++)
        j++;
}
#else
void Wait(unsigned int count)
{
    unsigned int i;

    for (i = 0; i < count; i++)
        asm volatile ("    nop");
}
#endif

/*------------------------------------------------------------------------------*/
/* Function Name       : main							*/
/* Object              : Main function						*/
/* Input Parameters    : none							*/
/* Output Parameters   : True							*/
/*------------------------------------------------------------------------------*/
int main(void)
{
#ifdef CONFIG_HW_INIT
    hw_init();
#endif

#ifdef CONFIG_USER_HW_INIT
    user_hw_init();
#endif

#if defined(CONFIG_AT91SAM9X5EK)
    extern void load_1wire_info();
    load_1wire_info();
#endif

/* Check if we want to boot the alternate image */
#if defined(CONFIG_DUAL_BOOT)
    if(alternate_boot_button()) {
	dbg_log(1, "Downloading Alternate image...\r\n");
	img_address	= ALT_IMG_ADDRESS;
	img_size	= ALT_IMG_SIZE;
	jump_addr	= ALT_JUMP_ADDR;
	altboot=1;
    } else {
	dbg_log(1, "Downloading image...\n\r");
    }
#else
    dbg_log(1, "Downloading image...\n\r");
#endif

#if defined(CONFIG_LOAD_LINUX)
    LoadLinux();
#elif defined(CONFIG_LOAD_NK) || defined(CONFIG_LOAD_EBOOT)
    LoadWince();
/* Booting stand-alone application, e.g. U-Boot */
#elif defined (CONFIG_DATAFLASH)
    load_df(AT91C_SPI_PCS_DATAFLASH, img_address, img_size, jump_addr);
#elif defined(CONFIG_NANDFLASH)
    read_nandflash((unsigned char *)jump_addr, (unsigned long)img_address, (int)img_size);
#elif defined(CONFIG_SDCARD)
    load_SDCard((void *)jump_addr);
#elif CONFIG_FLASH
    load_norflash(img_address, img_size, jump_addr);
#else
#error "No booting media specified!"
#endif

    dbg_log(1, "Done!\n\r");

#ifdef	CONFIG_LOAD_NK
    jump_addr += 0x1000;
#endif

#ifdef WINCE
    Jump(jump_addr);
#else                           /* !WINCE */
    return jump_addr;
#endif
}
