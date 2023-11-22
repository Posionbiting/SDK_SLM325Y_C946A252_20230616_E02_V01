/**
  ******************************************************************************
  * @file    mg_example_spi.c
  * @author  MEIG Development Team
  * @version V1.0.1
  * @date    06-Feb-2023
  * @brief   This file provides a simple SPI application example.
  ******************************************************************************
  * @attention
  *
  * THE SOFTWARE IS PROTECTED BY COPYRIGHT AND THE INFORMATION CONTAINED HEREIN
  * IS CONFIDENTIAL. THE SOFTWARE MAY NOT BE COPIED AND THE INFORMATION CONTAINED
  * HEREIN MAY NOT BE USED OR DISCLOSED EXCEPT WITH THE WRITTEN PERMISSION OF
  * MEIG SMART TECHNOLOGY CO., LTD.
  *
  * <h2><center>&copy; COPYRIGHT 2023 MEIG SMART</center></h2>
  ******************************************************************************
  */

#include "mg_trace.h"
#include <stdio.h>
#include "mg_spi.h"
#include "mg_common.h"
#include "mg_os.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'S', 'P', 'I')
#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

#define W25X_BUSY                   (0)
#define W25X_NotBUSY                (1)

#define FLASH_SECTOR_SIZE           (4096)
#define FLASH_SECTORS_PER_BLOCK     (8)
#define FLASH_SECTOR_NUM            (4096)
#define FLASH_BLOCK_NUM             (512)
#define FLASH_WAIT_MAX_TIME         (8000000)

#define FLASH_PAGE_SIZE             (256)//Bytes
#define FLASH_PAGE_SHIFT            (8)
#define FLASH_PAGE_NUM_PER_SECTOR   (FLASH_SECTOR_SIZE >> FLASH_PAGE_SHIFT)

#define W25X_WriteEnable            0x06    //Write Enable
#define W25X_WriteEnableVSR         0x50    //Write Enable for Volatile Status Register
#define W25X_WriteDisable           0x04    //Write Disable
#define W25X_ReadStatusReg1         0x05    //Read Status Register1:S7~S0
#define W25X_ReadStatusReg2         0x35    //Read Status Register2:S15~S8
#define W25X_WriteStatusReg         0x01    //Write Status Register2:BYTE1:S7~S0  BYTE2：S15~S8
#define W25X_PageProgram            0x02    //PageProgram:BYTE1:A23~A16  BYTE2:A15~A8  BYTE3:A7~A0  BYTE4:D7~D0
#define W25X_SectorErase            0x20    //Sector Erase:4K  BYTE1:A23~A16  BYTE2:A15~A8  BYTE3:A7~A0
#define W25X_BlockErase32K          0x52    //Block Erase:32K  BYTE1:A23~A16  BYTE2:A15~A8  BYTE3:A7~A0
#define W25X_BlockErase64K          0xD8    //Block Erase:64K  BYTE1:A23~A16  BYTE2:A15~A8  BYTE3:A7~A0
#define W25X_ChipErase              0xC7
#define W25X_EraseSuspend           0x75
#define W25X_EraseResume            0x7A
#define W25X_PowerDown              0xB9
#define W25X_ContinuousReadMode     0xFF
#define W25X_ReadData               0x03    //Read Data:BYTE1:A23~A16  BYTE2:A15~A8  BYTE3:A7~A0  BYTE4:D7~D0
#define W25X_FastReadData           0x0B    //Fast Read Data:BYTE1:A23~A16  BYTE2:A15~A8  BYTE3:A7~A0  BYTE4:dummy  BYTE5:D7~D0
#define W25X_FastReadDual           0x3B    //Fast Read Dual:BYTE1:A23~A16  BYTE2:A15~A8  BYTE3:A7~A0  BYTE4:dummy  BYTE5:D7~D0
#define W25X_ReleasePowerDown       0xAB
#define W25X_DeviceID               0xAB
#define W25X_ManufactDeviceID       0x90
#define W25X_JedecDeviceID          0x9F

static void prvSpiFlashWriteEnable(void)
{
    u8 cmd = W25X_WriteEnable;

    MG_SPI_PinControl(SPI_CH1, FALSE);
    MG_SPI_Send(SPI_CH1, &cmd, 1);
    MG_SPI_PinControl(SPI_CH1, TRUE);
}

static void prvSpiFlashWriteDisable(void)
{
    u8 cmd = W25X_WriteDisable;

    MG_SPI_PinControl(SPI_CH1, FALSE);
    MG_SPI_Send(SPI_CH1, &cmd, 1);
    MG_SPI_PinControl(SPI_CH1, TRUE);
}

static void prvSpiFlashWaitBusy(void)
{
    u8 cmd = W25X_ReadStatusReg1;
    u8 sDataBuf;

    MG_SPI_PinControl(SPI_CH1, FALSE);
    do
    {
        MG_SPI_Send(SPI_CH1, &cmd, 1);
        MG_SPI_Recv(SPI_CH1, &sDataBuf, 1);
    } while ((sDataBuf & 0x01) == 0x01);

    MG_SPI_PinControl(SPI_CH1, TRUE);
}

static u32 prvSpiFlashReadBusy(void)
{
    u8 cmd = W25X_ReadStatusReg1;
    u8 sDataBuf;

    MG_SPI_PinControl(SPI_CH1, FALSE);
    MG_SPI_Send(SPI_CH1, &cmd, 1);
    MG_SPI_Recv(SPI_CH1, &sDataBuf, 1);
    MG_SPI_PinControl(SPI_CH1, TRUE);

    if (sDataBuf & 0x01)
        return W25X_BUSY;
    else
        return W25X_NotBUSY;
}

static void prvSpiFlashEraseChip(void)
{
    u8 cmd = W25X_ChipErase;

    prvSpiFlashWriteEnable();
    prvSpiFlashWaitBusy();
    MG_SPI_PinControl(SPI_CH1, FALSE);
    MG_SPI_Send(SPI_CH1, &cmd, 1);
    MG_SPI_PinControl(SPI_CH1, TRUE);
    prvSpiFlashWaitBusy();
    prvSpiFlashWriteDisable();
}

static void prvSpiFlashEraseBlock(u32 erase_addr, u8 mode)
{
    u8 cmd = W25X_BlockErase32K;
    u8 addr[4] = {0x00};

    if (mode == 1)
    {
        MG_SPI_Send(SPI_CH1, &cmd, 1);
    }
    else
    {
        cmd = W25X_BlockErase64K;
        MG_SPI_Send(SPI_CH1, &cmd, 1);
    }
    addr[0] = (erase_addr & 0xFF000000) >> 24;
    addr[1] = (erase_addr & 0xFF0000) >> 16;
    addr[2] = (erase_addr & 0xFF00) >> 8;
    addr[3] = (erase_addr & 0xFF);

    prvSpiFlashWriteEnable();
    MG_SPI_PinControl(SPI_CH1, FALSE);
    MG_SPI_Send(SPI_CH1, &cmd, 1);
    MG_SPI_Send(SPI_CH1, addr, 4);
    MG_SPI_PinControl(SPI_CH1, TRUE);

    prvSpiFlashWaitBusy();
    prvSpiFlashWriteDisable();
}

static void prvSpiFlashEraseSector(u32 erase_addr)
{
    u8 cmd = W25X_SectorErase;
    u8 addr[4] = {0x00};

    addr[0] = (erase_addr & 0xFF000000) >> 24;
    addr[1] = (erase_addr & 0xFF0000) >> 16;
    addr[2] = (erase_addr & 0xFF00) >> 8;
    addr[3] = (erase_addr & 0xFF);

    prvSpiFlashWriteEnable();
    MG_SPI_PinControl(SPI_CH1, FALSE);
    MG_SPI_Send(SPI_CH1, &cmd, 1);
    MG_SPI_Send(SPI_CH1, addr, 4);
    MG_SPI_PinControl(SPI_CH1, TRUE);

    prvSpiFlashWaitBusy();
    prvSpiFlashWriteDisable();
}

static s32 prvSpiFlashRead(u32 addr, void *pbuf, u32 size)
{
    u8 offset[4] = {0};
    u8 cmd = W25X_ReadData;
    u32 ret = 0;

    offset[0] = W25X_ReadData;
    offset[1] = (addr & 0xff0000) >> 16;
    offset[2] = (addr & 0xff00) >> 8;
    offset[3] = addr & 0xff;

    MG_SPI_PinControl(SPI_CH1, FALSE);
    MG_SPI_Send(SPI_CH1, &cmd, 1);
    MG_SPI_Send(SPI_CH1, offset, 4);
    ret = MG_SPI_Recv(SPI_CH1, pbuf, size);
    MG_SPI_PinControl(SPI_CH1, TRUE);

    return ret;
}

static BOOL prvSpiFlashWritePage(u32 addr, u8 *data, u16 size)
{
    BOOL ret = FALSE;
    u8 cmd = W25X_PageProgram;

    prvSpiFlashWriteEnable();

    MG_SPI_PinControl(SPI_CH1, FALSE);
    MG_SPI_Send(SPI_CH1, &cmd, 1);
    MG_SPI_Send(SPI_CH1, (u8 *)&addr, 4);
    MG_SPI_Send(SPI_CH1, data, size);
    MG_SPI_PinControl(SPI_CH1, TRUE);
    prvSpiFlashWaitBusy();
    prvSpiFlashWriteDisable();

    return ret;
}

static u32 prvSpiFlashReadID(void)
{
    u8 cmd = W25X_ManufactDeviceID;
    u8 __attribute__((aligned(16))) flashIDBuf[3] = {0};

    MG_SPI_PinControl(SPI_CH1, FALSE);
    MG_SPI_Send(SPI_CH1, &cmd, 1);
    MG_SPI_Recv(SPI_CH1, flashIDBuf, 3);
    MG_SPI_PinControl(SPI_CH1, TRUE);

    return (flashIDBuf[0] << 16 | flashIDBuf[1] << 8 | flashIDBuf[2]);
}
static void prvThreadEntry(void *param)
{
    u16 mdid = 0x0000;

    ST_SpiConfig cfg =
    {
        .cpol = TRUE,
        .cpha = TRUE,
        .frsize = 8,
        .order = 0,
        .master = 0,
        .freq = 1000000,
    };

    MG_OS_ThreadSleep(1000 * 5);

    MG_SPI_Init(SPI_CH1, &cfg);
    mdid = prvSpiFlashReadID();
    APP_DEBUG("flash ID = 0x%x", mdid);

    prvSpiFlashEraseSector(0x00);

    while (1)
    {
        mdid = prvSpiFlashReadID();
        APP_DEBUG("flash ID = 0x%x", mdid);
        MG_OS_ThreadSleep(1000);
    }
}

void proc_main_task(void)
{
    MG_OS_ThreadCreate("mg_spi", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
}
/******************* (C) COPYRIGHT 2023 MEIG SMART*****END OF FILE*************/