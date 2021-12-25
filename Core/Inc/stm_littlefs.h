#ifndef _STM_LITTLEFS_H_
#define _STM_LITTLEFS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"
#include "lfs.h"

#define FLASH_BASE_ADDR 0x08000000
#define FS_BASE_ADDR (FLASH_BASE_ADDR + 0x00019000) //Create LittleFS on 100KB address
#define FS_BASE_PAGE_START 0x32 //One Page = 2KB, 88KB Page Num = 50(0x32)
#define STM32_G431RB_BLOCK_COUNT 14  //28KB
// #define FS_BASE_ADDR (FLASH_BASE_ADDR + 0x00016000) //Create LittleFS on 100KB address
// #define FS_BASE_PAGE_START 0x2C //One Page = 2KB, 88KB Page Num = 44(0x2C)
// #define STM32_G431RB_BLOCK_COUNT 20  //40KB
#define STM32_G431RB_READ_SIZE 16
#define STM32_G431RB_PROG_SIZE 64
#define STM32_G431RB_BLOCK_SIZE 2048 //2KB


int stm32_interl_flash_block_read(const struct lfs_config *c, lfs_block_t block,
            lfs_off_t off, void *buffer, lfs_size_t size);
int stm32_interl_flash_block_prog(const struct lfs_config *c, lfs_block_t block,
            lfs_off_t off, const void *buffer, lfs_size_t size);
int stm32_interl_flash_block_erase(const struct lfs_config *c, lfs_block_t block);  
int stm32_interl_flash_block_sync(const struct lfs_config *c);

extern lfs_t lfs;
extern lfs_file_t file;
extern const struct lfs_config cfg;

#ifdef __cplusplus
}
#endif

#endif
