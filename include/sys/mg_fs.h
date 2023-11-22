/**
  ******************************************************************************
  * @file    mg_fs.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all the fs function APIs.
  ******************************************************************************
  *  @attention
  *
  * THE SOFTWARE IS PROTECTED BY COPYRIGHT AND THE INFORMATION CONTAINED HEREIN
  * IS CONFIDENTIAL. THE SOFTWARE MAY NOT BE COPIED AND THE INFORMATION CONTAINED
  * HEREIN MAY NOT BE USED OR DISCLOSED EXCEPT WITH THE WRITTEN PERMISSION OF
  * MEIG SMART TECHNOLOGY CO., LTD.
  *
  * <h2><center>&copy; COPYRIGHT 2022 MEIG SMART</center></h2>
  ******************************************************************************
  */
#ifndef __MG_FS_H__
#define __MG_FS_H__
#include "mg_type.h"
#include "mg_opt_fs.h"

#ifdef __cplusplus
extern "C" {
#endif
/****************************************************************************
 * Def config
 ***************************************************************************/
#define MG_FS_PATH_MAX_LEN          128


/******************************************************************************
* Function:     MG_FS_Open
*
* Description:
*               Open a file
*
* Parameters:
*               filePathName:
*                   [in] file path
*
*               opt:
*                   [in]A u32 that defines the file's opening and access mode.
*                       The possible values are shown as follow:
*                       O_RDWR, can read and write
*                       O_RDONLY, can only read
*                       O_CREAT, opens the file, if it exists.
*                       O_TRUNC, file size truncated to 0 when open the file.
*                       For other values, please refer to <_default_fcntl.h>
* Return:
*               If the function succeeds, the return value specifies a file handle.
*               If the function fails, the return value is an error codes.
******************************************************************************/
s32 MG_FS_Open(const char *filePathName, u32 opt);
/******************************************************************************
* Function:     MG_FS_Close
*
* Description:
*               Closes the file associated with the file handle and makes
*               the file unavailable for reading or writing.
*
* Parameters:
*               fd:
*                   [in] A file handle, which was returned by calling 'MG_FS_Open'.
* Return:
*               If the function succeeds,the return value is MG_RET_OK
*               If the function fails, the return value is an error codes.
******************************************************************************/
s32 MG_FS_Close(s32 fd);
/******************************************************************************
* Function:     MG_FS_Read
*
* Description:
*               Reads data from the specified file, starting at the position
*               indicated by the file pointer. After the read operation has been
*               completed, the file pointer is auto adjusted by the number of bytes actually read.
*
* Parameters:
*               fd:
*                   [in] A handle to the file to be read, which is the return value
*                        of the function MG_FS_Open.
*
*               readBuff:
*                   [out] Point to the buffer that receives the data read from the file.
*
*               size:
*                   [in] Number of bytes to be read from the file.
* Return:
*               If the function succeeds, the return the number of bytes has been read.
*               If the function fails, the return value is an error codes.
******************************************************************************/
s32 MG_FS_Read(s32 fd, char *readBuff, u32 size);
/******************************************************************************
* Function:     MG_FS_Write
*
* Description:
*               This function writes data to a file. MG_FS_Write starts writing
*               data to the file at the position indicated by the file pointer
*               After the write operation has been completed, the file pointer
*               is auto adjusted by the number of bytes actually written.
*
* Parameters:
*               fd:
*                   [in] A handle to the file to be read, which is the return value
*                        of the function MG_FS_Open.
*
*               writeBuff:
*                   [in] Point to the buffer containing the data to be written to the file.
*
*               len:
*                   [in] Number of bytes to be write to the file.
*
* Return:
*               If the function succeeds, the return the number of bytes has been written.
*               If the function fails, the return value is an error codes.
******************************************************************************/
s32 MG_FS_Write(s32 fd, const char *writeBuff, u32 len);
/******************************************************************************
* Function:     MG_FS_GetFilePosition
*
* Description:
*               Gets the current value of the file pointer.
*
* Parameters:
*               fd:
*                   [in] A file handle, which was returned by calling 'MG_FS_Open'.
*
* Return:
*               If the function succeeds,the return value is the current offset from the beginning of the file
*               If the function fails, the return value is an error codes.
******************************************************************************/
s32 MG_FS_GetFilePosition(s32 fd);
/******************************************************************************
* Function:     MG_FS_GetSize
*
* Description:
*               Retrieves the size, in bytes, of the specified file.
*
* Parameters:
*               filePathName:
*                   [in] The name of the file.
*
* Return:
*               If the function succeeds,the return value is the bytes of the file
*               If the function fails, the return value is an error codes.
******************************************************************************/
s32 MG_FS_GetFileSize(const char *filePathName);
/******************************************************************************
* Function:     MG_FS_Seek
*
* Description:
*               Repositions the pointer in the previously opened file.
*
* Parameters:
*               fd:
*                   [in] A handle to the file to be read, which is the return value
*                        of the function MG_FS_Open.
*
*               offset:
*                   [in] Offset according to the directive whence.
*
*               opt:
*                   [in] SEEK_SET, SEEK_CUR, SEEK_END.
* Return:
*               If the function succeeds, the return value is the current offset from the beginning of the file
*               If the function fails, the return value is an error codes.
******************************************************************************/
s32 MG_FS_Seek(s32 fd, s32 offset, u32 opt);
/******************************************************************************
* Function:     MG_FS_Rename
*
* Description:
*               Rename a file.
*
* Parameters:
*               filePathName:
*                  [in] File to be renamed.
*
*               newfilePathName:
*                  [in] New name of file.
* Return:
*               If the function succeeds, the return value is MG_RET_OK
*               If the function fails, the return value is an error codes.
******************************************************************************/
s32 MG_FS_Rename(const char *oldfilePathName, const char *newfilePathName);
/******************************************************************************
* Function:     MG_FS_Truncate
*
* Description:
*               This function truncate a file to a specified length
*
* Parameters:
*               filePathName:
*                   [in] The name of the file.
*
*               len:
*                   [in] Truncation length.
* Return:
*               If the function succeeds, the return value is MG_RET_OK
*               If the function fails, the return value is an error codes.
******************************************************************************/
s32 MG_FS_Truncate(const char *filePathName, u32 len);
/******************************************************************************
* Function:     MG_FS_TruncateEX
*
* Description:
*               This function truncate a file to a specified length
*
* Parameters:
*               fd:
*                   [in] A handle to the file to be read, which is the return value
*                        of the function MG_FS_Open.
*
*               len:
*                   [in] Truncation length.
* Return:
*               If the function succeeds, the return value is MG_RET_OK
*               If the function fails, the return value is an error codes.
******************************************************************************/
s32 MG_FS_TruncateEX(s32 fd, u32 len);
/******************************************************************************
* Function:     MG_FS_Check
*
* Description:
*               Check whether the file exists or not.
*
* Parameters:
*               filePathName:
*                   [in] The name of the file.
* Return:
*               If the function succeeds, the return value is MG_RET_OK
*               If the function fails, the return value is an error codes.
******************************************************************************/
s32 MG_FS_Check(const char *filePathName);
/******************************************************************************
* Function:     MG_FS_Delete
*
* Description:
*               This function deletes an existing file.
*
* Parameters:
*               filePathName:
*                   [in]The name of the file to be deleted.
* Return:
*               If the function succeeds, the return value is MG_RET_OK
*               If the function fails, the return value is an error codes.
******************************************************************************/
s32 MG_FS_Delete(const char *filePathName);
/******************************************************************************
* Function:     MG_FS_Sync
*
* Description:
*               synchronize a file's in-core state with storage device
*
* Parameters:
*               fd:
*                   [in] file descriptor
* Return:
*               If the function succeeds, the return value is MG_RET_OK
*               If the function fails, the return value is an error codes.
******************************************************************************/
s32 MG_FS_Sync(s32 fd);

/******************************************************************************
* Function:     MG_FS_State
*
* Description:
*               get file status
*
* Parameters:
*               filePathName:
*                   [in] file path
*               st:
*                   [in] file state
* Return:
*               If the function succeeds, the return value is MG_RET_OK
*               If the function fails, the return value is an error codes.
******************************************************************************/
s32 MG_FS_State(const char *filePathName, struct stat *st);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MG_FS_H__ */