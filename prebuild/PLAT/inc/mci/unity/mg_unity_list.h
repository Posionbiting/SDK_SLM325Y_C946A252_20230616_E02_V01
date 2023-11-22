#ifndef _MG_UNITY_LIST_H_
#define _MG_UNITY_LIST_H_
#include "mg_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UNITY_LIST_IS_HEADERLIST(list)      (((ST_UnityList *)(list))->headFlag)

#define UNITY_LIST_STRUCT_MEMBERS          \
  /** Marks the beginning of a list */     \
  s32 headFlag;                           \
  /** Pointer to the next list node */     \
  struct _UnityList *prev;                 \
  /** Pointer to the previous list node */ \
  struct _UnityList *next;

/**
 * \brief Container for list.
 */
typedef struct _UnityList {
    UNITY_LIST_STRUCT_MEMBERS
} ST_UnityList;


/**
 * \brief  This function init list header.
 *
 * \param  list      The list pointer.
 *
 * \return None.
 */
void MG_UNITY_ListHeaderInit(ST_UnityList *list);

/**
 * \brief  This function init list node.
 *
 * \param  list      The list pointer.
 *
 * \return None.
 */
void MG_UNITY_ListNodeInit(ST_UnityList *list);

/**
 * \brief  This function gets prev node of the list.
 *
 * \param  list      The list pointer.
 *
 * \return The prev node.
 */
ST_UnityList *MG_UNITY_ListPrev(ST_UnityList *list);

/**
 * \brief  This function gets next node of the list.
 *
 * \param  list      The list pointer.
 *
 * \return The next node.
 */
ST_UnityList *MG_UNITY_ListNext(ST_UnityList *list);


/**
 * \brief  This function insert a new node.
 *
 * \param  list            The list pointer.
 * \param  newListNode     The new node.
 *
 * \return 0 on success or -1 on fail.
 */
s32 MG_UNITY_ListInsert(ST_UnityList *list, ST_UnityList *newListNode);

/**
 * \brief  This function remove a node.
 *
 * \param  removeListNode   The node to be remove.
 *
 * \return the node to be remove.
 */
ST_UnityList *MG_UNITY_ListRemove(ST_UnityList *removeListNode);

/**
 * \brief  This function gets list length.
 *
 * \param  list            The list pointer.
 *
 * \return the length of list.
 */
s32 MG_UNITY_ListLength(ST_UnityList *list);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // End-of _MG_UNITY_LIST_H_