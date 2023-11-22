#ifndef _MG_UNITY_STRING_H_
#define _MG_UNITY_STRING_H_
#include "mg_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Container for string.
 */
typedef struct _UnityString {
    char *value;
    s32 memSize;
    s32 valueSize;
} ST_UnityString;

/**
 * \brief  This function new string context.
 *
 * \param  None.
 *
 * \return The string context.
 */
ST_UnityString *MG_UNITY_StringNew();

/**
 * \brief  This function delete string context.
 *
 * \param  str     The string context.
 *
 * \return None.
 */
void MG_UNITY_StringDelete(ST_UnityString *str);

/**
 * \brief  This function clear string context.
 *
 * \param  str     The string context.
 *
 * \return None.
 */
void MG_UNITY_StringClear(ST_UnityString *str);

/**
 * \brief  This function get sring length.
 *
 * \param  str     The string context.
 *
 * \return The length of the string context or a negative error code.
 */
s32 MG_UNITY_StringLength(ST_UnityString *str);

/**
 * \brief  This function get sring value pointer.
 *
 * \param  str     The string context.
 *
 * \return The sring value pointer.
 */
char *MG_UNITY_StringGetvalue(ST_UnityString *str);

/**
 * \brief  This function set sring value pointer.
 *
 * \param  str     The string context.
 * \param  value   The value pointer.
 * \param  len     The value length.
 *
* \return The length of the string context or a negative error code.
 */
s32 MG_UNITY_StringSetvalue(ST_UnityString *str, const char *value, s32 len);

/**
 * \brief  This function add sring value pointer.
 *
 * \param  str     The string context.
 * \param  value   The value pointer.
 * \param  len     The value length.
 *
* \return The length of the string context or a negative error code.
 */
char *MG_UNITY_StringAddvalue(ST_UnityString *str, const char *value, s32 len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of _MG_UNITY_STRING_H_