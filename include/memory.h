#ifdef	CONFIG_SDRAM
#include "sdramc.h"
#endif
#ifdef	CONFIG_SDDRC
#include "sddrc.h"
#endif
#ifdef	CONFIG_DDR2
#include "ddr2.h"
#include "ddramc.h"
#endif
#ifdef CONFIG_PSRAM
#include "psram.h"
#endif
#ifdef CONFIG_NANDFLASH
#include "nandflash.h"
#endif
#ifdef CONFIG_DATAFLASH
#include "dataflash.h"
#endif
#ifdef CONFIG_FLASH
#include "flash.h"
#endif

