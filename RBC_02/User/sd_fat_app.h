
#ifndef __SD_FAT_APP_H
#define	__SD_FAT_APP_H

#include "./sdio/bsp_sdio_sdcard.h"
#include "ff.h"
#include "globalval.h"


void sd_fs_init(void);
void SD_SAVE(void);

#endif /* __SD_FS_PP_H */

