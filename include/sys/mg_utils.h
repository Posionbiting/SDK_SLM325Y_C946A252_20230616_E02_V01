/**
  ******************************************************************************
  * @file    mg_utils.h
  * @author  MEIG Development Team
  * @version V1.0.0
  * @date    28-Mar-2022
  * @brief   This file provides all the utils function APIs.
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

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef _MG_UTILS_H_
#define _MG_UTILS_H_
#include "mg_type.h"

/******************************************************************************
* Function:     MG_UTILS_Memset
*
* Description:
*               memset
*
* Parameters:
*               dest:
*                       [IN] the dest pointer.
*               value:
*                       [IN] the set value.
*               size:
*                       [IN] the memory size.
* Return:
*               the dest pointer.
******************************************************************************/
void* MG_UTILS_Memset(void* dest, u8 value, u32 size);

/******************************************************************************
* Function:     MG_UTILS_Memcpy
*
* Description:
*               memcpy
*
* Parameters:
*               dest:
*                       [IN] the dest pointer.
*               src:
*                       [IN] the src pointer.
*               size:
*                       [IN] the memory size.
* Return:
*               the dest pointer.
******************************************************************************/
void* MG_UTILS_Memcpy(void* dest, const void* src, u32 size);

/******************************************************************************
* Function:     MG_UTILS_Memcmp
*
* Description:
*               memcmp
*
* Parameters:
*               s1:
*                       [IN] the s1 pointer.
*               s2:
*                       [IN] the s2 pointer.
*               size:
*                       [IN] the memory size.
* Return:
*               <0, s1 < s2
*               >0, s1 > s2
*               =0, s1 == s2
******************************************************************************/
s32 MG_UTILS_Memcmp(const void* s1, const void*s2, u32 size);

/******************************************************************************
* Function:     MG_UTILS_Memmove
*
* Description:
*               memmove
*
* Parameters:
*               dest:
*                       [IN] the dest pointer.
*               src:
*                       [IN] the src pointer.
*               size:
*                       [IN] the memory size.
* Return:
*               the dest pointer.
******************************************************************************/
void* MG_UTILS_Memmove(void* dest, const void* src, u32 size);

/******************************************************************************
* Function:     MG_UTILS_Strcpy
*
* Description:
*               strcpy
*
* Parameters:
*               dest:
*                       [IN] the dest string pointer.
*               src:
*                       [IN] the src string pointer.
* Return:
*               the dest string pointer
******************************************************************************/
char* MG_UTILS_Strcpy(char* dest, const char* src);

/******************************************************************************
* Function:     MG_UTILS_Strncpy
*
* Description:
*               strncpy
*
* Parameters:
*               dest:
*                       [IN] the dest string pointer.
*               src:
*                       [IN] the src string pointer.
*               size:
*                       [IN] the size.
* Return:
*               the dest string pointer
******************************************************************************/
char* MG_UTILS_Strncpy(char* dest, const char* src, u32 size);

/******************************************************************************
* Function:     MG_UTILS_Strcat
*
* Description:
*               strcat
*
* Parameters:
*               dest:
*                       [IN] the dest string pointer.
*               src:
*                       [IN] the src string pointer.
* Return:
*               the dest string pointer
******************************************************************************/
char* MG_UTILS_Strcat(char* dest, const char* src);

/******************************************************************************
* Function:     MG_UTILS_Strncat
*
* Description:
*               strncat
*
* Parameters:
*               dest:
*                       [IN] the dest string pointer.
*               src:
*                       [IN] the src string pointer.
*               size:
*                       [IN] the size.
* Return:
*               the dest string pointer
******************************************************************************/
char* MG_UTILS_Strncat(char* dest, const char* src, u32 size);

/******************************************************************************
* Function:     MG_UTILS_Strcmp
*
* Description:
*               strcmp
*
* Parameters:
*               s1:
*                       [IN] the s1 pointer.
*               s2:
*                       [IN] the s2 pointer.
* Return:
*               <0, s1 < s2
*               >0, s1 > s2
*               =0, s1 == s2
******************************************************************************/
s32 MG_UTILS_Strcmp(const char*s1, const char*s2);

/******************************************************************************
* Function:     MG_UTILS_Strncmp
*
* Description:
*               strncmp
*
* Parameters:
*               s1:
*                       [IN] the s1 pointer.
*               s2:
*                       [IN] the s2 pointer.
*               size:
*                       [IN] the size.
* Return:
*               <0, s1 < s2
*               >0, s1 > s2
*               =0, s1 == s2
******************************************************************************/
s32 MG_UTILS_Strncmp(const char* s1, const char* s2, u32 size);

/******************************************************************************
* Function:     MG_UTILS_Strchr
*
* Description:
*               strchr
*
* Parameters:
*               str:
*                       [IN] the string pointer.
*               ch:
*                       [IN] the unsigned character.
* Return:
*               If no error occurs, the function returns the position where
*               the character c first appears in the string. Otherwise, returns NULL.
******************************************************************************/
char* MG_UTILS_Strchr(const char* str, s32 ch);

/******************************************************************************
* Function:     MG_UTILS_Strlen
*
* Description:
*               strlen
*
* Parameters:
*               str:
*                       [IN] the string pointer.
* Return:
*               the string length
******************************************************************************/
u32 MG_UTILS_Strlen(const char* str);

/******************************************************************************
* Function:     MG_UTILS_Strstr
*
* Description:
*               strstr
*
* Parameters:
*               s1:
*                       [IN] the string pointer.
*               s2:
*                       [IN] the unsigned character.
* Return:
*               If no error occurs, the function returns the position where
*               the string s2 first appears in the string. Otherwise, returns NULL.
******************************************************************************/
char* MG_UTILS_Strstr(const char* s1, const char* s2);

/******************************************************************************
* Function:     MG_UTILS_Toupper
*
* Description:
*               toupper
*
* Parameters:
*               c:
*                       [IN] the unsigned character.
* Return:
*               If character c has a corresponding capital letter, the function
*               returns the capital letter of C, otherwise returns c.
******************************************************************************/
s32 MG_UTILS_Toupper(s32 c);

/******************************************************************************
* Function:     MG_UTILS_Tolower
*
* Description:
*               tolower
*
* Parameters:
*               c:
*                       [IN] the unsigned character.
* Return:
*               If character c has a corresponding lowercase letter, the function
*               returns the lowercase letter of C, otherwise returns c.
******************************************************************************/
s32 MG_UTILS_Tolower(s32 c);

/******************************************************************************
* Function:     MG_UTILS_Isdigit
*
* Description:
*               isdigit
*
* Parameters:
*               c:
*                       [IN] the unsigned character.
* Return:
*               If character c is a number, the function returns a non-zero value.
*               otherwise it returns 0.
******************************************************************************/
s32 MG_UTILS_Isdigit(char c);

/******************************************************************************
* Function:     MG_UTILS_Atoi
*
* Description:
*               atoi
*
* Parameters:
*               s:
*                       [IN] the string.
* Return:
*               If no error occurs, the function returns a long integer.
*               otherwise it returns 0.
******************************************************************************/
s32 MG_UTILS_Atoi(const char* s);

/******************************************************************************
* Function:     MG_UTILS_Snprintf
*
* Description:
*               snprintf
*
* Parameters:
*               buf:
*                       [IN] the buffer.
*               size:
*                       [IN] the buffer size.
*               fmt:
*                       [IN] the print format.
* Return:
*               the format print length.
******************************************************************************/
s32 MG_UTILS_Snprintf(u8* buf, u32 size, const char* fmt, ...);

/******************************************************************************
* Function:     MG_UTILS_GetRandom
*
* Description:
*               generate random numbers
*
* Parameters:
*               seed:
*                       [IN] the seed of generating random numbers.
* Return:
*               the generating random numbers.
******************************************************************************/
u32 MG_UTILS_GetRandom(u32 seed);

/******************************************************************************
* Function:     MG_UTILS_Str2Hex
*
* Description:
*               Convert string to hexadecimal
*
* Parameters:
*               input:
*                       [IN] the string.
*               len_of_input:
*                       [IN] the string length.
*               output:
*                       [OUT] the hexadecimal buffer.
*               size_of_output:
*                       [IN] the hexadecimal buffer size.
* Return:
*               the converted hexadecimal data length.
******************************************************************************/
s32 MG_UTILS_Str2Hex(const s8 *input, u32 len_of_input, u8 *output, u32 size_of_output);

/******************************************************************************
* Function:     MG_UTILS_Hex2Str
*
* Description:
*               Convert hexadecimal to string
*
* Parameters:
*               input:
*                       [IN] the hexadecimal buffer.
*               len_of_input:
*                       [IN] the hexadecimal buffer length.
*               output:
*                       [OUT] the string.
*               size_of_output:
*                       [IN] the string size.
* Return:
*               the converted string length.
******************************************************************************/
s32 MG_UTILS_Hex2Str(const u8 *input, u32 len_of_input, s8 *output, u32 size_of_output);

#endif // End-of _MG_UTILS_H_
