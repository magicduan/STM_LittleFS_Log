#include "stm_littlefs.h"

// variables used by the filesystem
lfs_t lfs;
lfs_file_t file;

// configuration of the filesystem is provided by this struct
const struct lfs_config cfg = {
    // block device operations
    .read  = stm32_interl_flash_block_read,
    .prog  = stm32_interl_flash_block_prog,
    .erase = stm32_interl_flash_block_erase,
    .sync  = stm32_interl_flash_block_sync,

    // block device configuration
    .read_size = STM32_G431RB_READ_SIZE,
    .prog_size = STM32_G431RB_PROG_SIZE,
    .block_size = STM32_G431RB_BLOCK_SIZE,  //2K
    .block_count = STM32_G431RB_BLOCK_COUNT, 
    .cache_size = 128,
    .lookahead_size = 8,
    .block_cycles = 500,
};

int stm32_interl_flash_block_read(const struct lfs_config *c, lfs_block_t block,
            lfs_off_t off, void *buffer, lfs_size_t size)
{
    uint32_t src_addr = FS_BASE_ADDR + c->block_size*block +off;
    uint32_t read_size = 0;
    uint16_t* pDest = (uint16_t*)buffer;
    uint16_t* pSrc = (uint16_t*)src_addr;
    while(read_size < size){
        *pDest = *(pSrc);
        pDest++;
        pSrc++;
        read_size += 2;
    }

    return 0;
}            

int stm32_interl_flash_block_prog(const struct lfs_config *c, lfs_block_t block,
            lfs_off_t off, const void *buffer, lfs_size_t size)
{
    HAL_FLASH_Unlock();
    uint32_t  dest_addr = FS_BASE_ADDR + c->block_size*block +off;
    uint64_t *pSrc = (uint64_t*)buffer;
    uint32_t  write_size = 0;
    while(write_size < size){
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,dest_addr,*(pSrc)) != HAL_OK){
            HAL_FLASH_Lock();
            return HAL_FLASH_GetError();
        }
        pSrc++;
        dest_addr += 8;
        write_size += 8;
    }
    HAL_FLASH_Lock();
    return 0;
}

int stm32_interl_flash_block_erase(const struct lfs_config *c, lfs_block_t block)
{
    static FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PageError;
    HAL_FLASH_Unlock();

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Banks = FLASH_BANK_1;
    EraseInitStruct.Page = FS_BASE_PAGE_START + block;
    EraseInitStruct.NbPages = 1;
    
    if (HAL_FLASHEx_Erase(&EraseInitStruct,&PageError)!= HAL_OK){
        return HAL_FLASH_GetError();
    }

    HAL_FLASH_Lock();
    return 0;
}

int stm32_interl_flash_block_sync(const struct lfs_config *c)
{
    return 0;
}
// // entry point
// int main(void) {
//     // mount the filesystem
//     int err = lfs_mount(&lfs, &cfg);

//     // reformat if we can't mount the filesystem
//     // this should only happen on the first boot
//     if (err) {
//         lfs_format(&lfs, &cfg);
//         lfs_mount(&lfs, &cfg);
//     }

//     // read current count
//     uint32_t boot_count = 0;
//     lfs_file_open(&lfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
//     lfs_file_read(&lfs, &file, &boot_count, sizeof(boot_count));

//     // update boot count
//     boot_count += 1;
//     lfs_file_rewind(&lfs, &file);
//     lfs_file_write(&lfs, &file, &boot_count, sizeof(boot_count));

//     // remember the storage is not updated until the file is closed successfully
//     lfs_file_close(&lfs, &file);

//     // release any resources we were using
//     lfs_unmount(&lfs);

//     // print the boot count
//     printf("boot_count: %d\n", boot_count);
// }
