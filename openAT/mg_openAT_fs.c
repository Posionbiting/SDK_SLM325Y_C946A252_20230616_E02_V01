
#include "at_util.h"
#include "atc_decoder.h"

#include "mg_openAT_param.h"
#include "mg_openAT_reply.h"
#include "mg_openAT_engine.h"
#include "mg_openAT_fs.h"
#include "mg_common.h"
#include "mg_trace.h"
#include "mg_utils.h"
#include "mg_fs.h"
#include "mg_mem.h"

#define LOG_TAG MAKE_LOG_TAG('A', 'T', 'F', 'S')
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_LOGD(LOG_TAG, FORMAT, ##__VA_ARGS__)


#define OPEN_AT_FS_BUF_SIZE          1024

typedef struct _fsdwnfileAsyncCtx
{
    char *file_name;

    u32 size;
    u32 pos;
    char *mem;

    UINT32 atHandle;
} fsdwnfileAsyncCtx_t;

#define OPENAT_LIST_STRUCT_MEMBERS          \
  /** Marks the beginning of a list */     \
  s32 is_header;                           \
  /** fileName */     \
  char name[OPEN_AT_FS_FILE_NAME_MAX_LEN +1 ];       \
  /** Pointer to the next list node */ \
  struct _fileInfo *next;

typedef struct _fileInfo
{
    OPENAT_LIST_STRUCT_MEMBERS
} openAt_fileInfo;

openAt_fileInfo *openFileInfoList = NULL;

static openAt_fileInfo * _open_at_list_new()
{
    openAt_fileInfo *list = (openAt_fileInfo *)MG_MEM_Calloc(1, sizeof(openAt_fileInfo));

    if (NULL != list)
    {
        list->is_header = 1;
        MG_UTILS_Memset(list->name, 0, sizeof(list->name));
        list->next = NULL;
    }

    APP_DEBUG( " headerlist : %x", list);
    return list;
}

static openAt_fileInfo *_open_at_node_new(char *fileName)
{
    openAt_fileInfo *node = NULL;
    u32 len = 0;
    node = (openAt_fileInfo *)MG_MEM_Calloc(1, sizeof(openAt_fileInfo));

    if (NULL != node)
    {
        node->is_header = 0;
        node->next = NULL;
        len = MG_UTILS_Strlen(fileName);
        if (len > OPEN_AT_FS_FILE_NAME_MAX_LEN)
            len = OPEN_AT_FS_FILE_NAME_MAX_LEN;
        MG_UTILS_Memcpy(node->name, fileName, len);
    }

    return node;
}

static openAt_fileInfo *_open_at_list_find(openAt_fileInfo *list, char *fileName)
{
    openAt_fileInfo *node = NULL;
    openAt_fileInfo *pCurNode = NULL;

    if (list == NULL || fileName == NULL)
    {
        return NULL;
    }
    pCurNode = (openAt_fileInfo *)list;
    do
    {
        if (pCurNode->is_header == 0)
        {
            APP_DEBUG("list :%x, pCurNode : %x,  headFlag:%d", list, pCurNode, pCurNode->is_header);
            if(MG_UTILS_Strcmp((CHAR*)(pCurNode->name), (CHAR*)fileName) == 0)
            {
                node = pCurNode;
                break;
            }
        }

        pCurNode = (openAt_fileInfo *)pCurNode->next;
    }while (pCurNode != NULL);


    return node;
}


static openAt_fileInfo *_open_at_get_next_node(openAt_fileInfo *list)
{

    if (list == NULL)
    {
        return NULL;
    }

    return list->next;
}

static BOOL _open_at_is_header(openAt_fileInfo *list)
{

    if (list == NULL)
    {
        return FALSE;
    }

    return (list->is_header == 1) ? (TRUE) : (FALSE);
}


static char *_open_at_get_file_name(openAt_fileInfo *list, char *fileName, s32 size)
{
    s32 len = 0;
    if (list == NULL || fileName == NULL)
    {
        return NULL;
    }

    len = size;
    if (len > OPEN_AT_FS_FILE_NAME_MAX_LEN)
        len = OPEN_AT_FS_FILE_NAME_MAX_LEN;

    MG_UTILS_Memcpy(fileName, list->name, len);
    return fileName;
}

static s32 _open_at_list_insert(openAt_fileInfo *list, char *name)
{
    openAt_fileInfo *newNode = NULL;
    u32 nCritical = 0;

    if ((NULL == list) || name == NULL)
        return -1;

    newNode = _open_at_list_find(list, name);
    if(newNode == NULL)
    {
        newNode = _open_at_node_new(name);
        if (newNode == NULL)
            return -1;

        nCritical = MG_OS_EnterCritical();
        newNode->next = list->next;
        list->next = newNode;
        MG_OS_ExitCritical(nCritical);
    }

    return 0;
}

static void _open_at_list_remove(openAt_fileInfo *list, char *fileName)
{
    openAt_fileInfo *pPreNode = NULL;
    openAt_fileInfo *pCurNode = NULL;
    u32 nCritical = 0;
    u32 isFound = 0;

    if (list == NULL || fileName == NULL)
        return;

    nCritical = MG_OS_EnterCritical();
    pCurNode = (openAt_fileInfo *)list;
    pPreNode = pCurNode;
    do
    {
        if (pCurNode->is_header == 0)
        {
            APP_DEBUG("list :%x, pCurNode : %x,  headFlag:%d", list, pCurNode, pCurNode->is_header);
            if(MG_UTILS_Strcmp((CHAR*)(pCurNode->name), (CHAR*)fileName) == 0)
            {
                isFound = 1;
                break;
            }
        }

        pPreNode = (openAt_fileInfo *)pCurNode;
        pCurNode = (openAt_fileInfo *)pCurNode->next;
    }while (pCurNode != NULL);

    if (isFound)
    {
        pPreNode->next = pCurNode->next;
        MG_MEM_Free(pCurNode);
    }
    MG_OS_ExitCritical(nCritical);
}

static void _add_path(char *name)
{
    if(openFileInfoList == NULL)
    {
        openFileInfoList = _open_at_list_new();
    }

    _open_at_list_insert(openFileInfoList, name);
}

static void _remove_path(char *name)
{
    _open_at_list_remove(openFileInfoList, name);
}

static void _dwnfileAsyncFree(fsdwnfileAsyncCtx_t *async)
{
    if (async)
    {
        if (async->file_name)
            MG_MEM_Free(async->file_name);

        MG_MEM_Free(async);
    }
}

static s32 _dwnfile(const char *file_name, const void *data, u32 size)
{
    s32 fd = -1;
    s32 wlen = -1;

    if (file_name == NULL || data == NULL || size == 0)
        return -1;

    fd = MG_FS_Open(file_name, O_RDWR | O_CREAT | O_TRUNC);
    if (fd >= 0)
    {
        wlen = MG_FS_Write(fd, (char *)data, size);
        MG_FS_Close(fd);

        _add_path((char *)file_name);
    }

    return wlen;
}

static s32 _dwnfilePushData(void *ctx, u8 chanId, const void *data, u32 size)
{
    s32 iRet = -1;
    char *pMem = NULL;
    s32 len = 0;
    s32 isEof = 0;
    fsdwnfileAsyncCtx_t *async = (fsdwnfileAsyncCtx_t *)ctx;

    do
    {
        if (async == NULL || data == NULL || size == 0)
            break;

        if (async->pos >= async->size)
           break;

        len = async->size - async->pos;
        if ( len > size)
            len = (s32)size;

        pMem = async->mem;
        MG_UTILS_Memcpy(pMem + async->pos, (char *)data, (u32)len);
        async->pos += len;

        if (async->pos >= async->size)
        {
            len = _dwnfile(async->file_name, async->mem, async->size);
            if (len == async->size)
            {
                mg_openAT_SetLineMode(chanId);
                mg_openAT_cmdRespOK(async->atHandle);
            }
            else
            {
                mg_openAT_SetLineMode(chanId);
                mg_openAT_cmdRespError(async->atHandle, OPEN_AT_ERROR);
            }

            isEof = 1;
        }
        else if(pMem[async->pos-1] == AT_ASCI_CTRL_Z)
        {
            len = _dwnfile(async->file_name, async->mem, async->pos-1);
            if (len == async->pos-1)
            {
                mg_openAT_SetLineMode(chanId);
                mg_openAT_cmdRespOK(async->atHandle);
            }
            else
            {
                mg_openAT_SetLineMode(chanId);
                mg_openAT_cmdRespError(async->atHandle, OPEN_AT_ERROR);
            }

            isEof = 1;
        }
        else if(pMem[async->pos-1] == AT_ASCI_ESC)
        {
            mg_openAT_SetLineMode(chanId);
            mg_openAT_cmdRespOK(async->atHandle);
            isEof = 1;
        }

        iRet = 0;
    } while (0);

    if (iRet < 0 || isEof == 1)
    {
        _dwnfileAsyncFree(async);
    }

    return iRet;
}

CmsRetId openFSDWNFILE(const AtCmdInputContext *pAtCmdReq)
{
    CmsRetId    rc = CMS_FAIL;
    UINT32      atHandle = AT_SET_SRC_HANDLER(pAtCmdReq->tid, CMS_REF_SUB_AT_5_ID, pAtCmdReq->chanId);
    UINT32      operaType = (UINT32)pAtCmdReq->operaType;
    UINT8       chanId = (UINT8)AT_GET_HANDLER_CHAN_ID(atHandle);
    CHAR        rspBuf[ATEC_IND_RESP_128_STR_LEN] = {0};
    BOOL        paramok = TRUE;

    switch (operaType)
    {
        case AT_TEST_REQ:             /* AT+FSDWNFILE=? */
        {
            MG_UTILS_Snprintf((u8 *)rspBuf, ATEC_IND_RESP_128_STR_LEN, "+FSDWNFILE: filename,size");
            rc = mg_openAT_cmdRespTextAndOK(atHandle, rspBuf);
            break;
        }

        case AT_SET_REQ:              /* AT+FSDWNFILE=filename,size */
        {
            char *fileName = NULL;
            s32  fileNameLen = 0;
            s32  length = 0;

            /* parameter count */
            if (pAtCmdReq->paramRealNum != 2)
            {
                rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_PARAM);
                break; 
            }

            fileName = (char *)MG_MEM_Calloc(1, OPEN_AT_FS_FILE_NAME_MAX_LEN + 1 );
            if (fileName == NULL)
            {
                rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_NO_MEMORY);
                break;
            }

            fileNameLen = mg_openAT_cmdParamStr(pAtCmdReq->pParamList, 0, (u8 *)fileName, OPEN_AT_FS_FILE_NAME_MAX_LEN, &paramok);
            length = mg_openAT_cmdParamUintInRange(pAtCmdReq->pParamList, 1, 0, OPEN_AT_FS_SIZE_MAX, 0, &paramok);
            if( ( !paramok ) || ( fileNameLen <= 0 ) ||
                ( MG_UTILS_Strlen( (char *)fileName ) > OPEN_AT_FS_FILE_NAME_MAX_LEN ) ||
                ( length <= 0 ) )
            {
                MG_MEM_Free(fileName);
                rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_PARAM);
                break;
            }

            fsdwnfileAsyncCtx_t *async = (fsdwnfileAsyncCtx_t *)MG_MEM_Calloc(1, sizeof(fsdwnfileAsyncCtx_t) + length);
            if (async == NULL)
            {
                MG_MEM_Free(fileName);
                rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_NO_MEMORY);
                break;
            }

            async->size = length;
            async->pos = 0;
            async->file_name = fileName;
            async->mem = (char *)async + sizeof(fsdwnfileAsyncCtx_t);
            async->atHandle = atHandle;

            mg_openAT_cmdRespOutputPrompt(atHandle);
            rc = mg_openAT_SetBypassMode(chanId, _dwnfilePushData, (void *)async);
            break;
        }

        default:
        {
            rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_NOT_SUPPORT);
            break;
        }
    }

    return rc;
}


CmsRetId openFSRDFILE(const AtCmdInputContext *pAtCmdReq)
{
    CmsRetId    rc = CMS_FAIL;
    UINT32      atHandle = AT_SET_SRC_HANDLER(pAtCmdReq->tid, CMS_REF_SUB_AT_5_ID, pAtCmdReq->chanId);
    UINT32      operaType = (UINT32)pAtCmdReq->operaType;
    CHAR        rspBuf[ATEC_IND_RESP_128_STR_LEN] = {0};
    BOOL        paramok = TRUE;

    switch (operaType)
    {
        case AT_TEST_REQ:             /* AT+FSRDFILE=? */
        {
            MG_UTILS_Snprintf((u8 *)rspBuf, ATEC_IND_RESP_128_STR_LEN, "+FSRDFILE: filename");
            rc = mg_openAT_cmdRespTextAndOK(atHandle, rspBuf);
            break;
        }

        case AT_SET_REQ:              /* AT+FSRDFILE=0 */
        {
            s32  file_size = 0;
            s32  fileNameLen = 0;
            CHAR fileName[OPEN_AT_FS_FILE_NAME_MAX_LEN+1] = {0};
            char *pBuffer = NULL;
            char *pRspBuffer = NULL;
            s32 fd = -1;
            s32 pos = 0;
            s32 len = 0;
            s32 rz = 0;
            s32 nRspBufSize = 0;

            fileNameLen = mg_openAT_cmdParamStr(pAtCmdReq->pParamList, 0, (UINT8 *)fileName, OPEN_AT_FS_FILE_NAME_MAX_LEN, &paramok);
            if(!paramok || fileNameLen <= 0)
            {
                rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_PARAM);
                break;
            }

            file_size = MG_FS_GetFileSize((char *)fileName);
            if (file_size > OPEN_AT_FS_SIZE_MAX || file_size <= 0)
            {
                rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR);
                break;
            }

            pBuffer = MG_MEM_Calloc(1, OPEN_AT_FS_BUF_SIZE+1);
            if (pBuffer == NULL)
            {
                rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_PARAM);
                break;
            }

            nRspBufSize = OPEN_AT_FS_BUF_SIZE+32;
            pRspBuffer = MG_MEM_Calloc(1, nRspBufSize);
            if (pRspBuffer == NULL)
            {
                MG_MEM_Free(pBuffer);
                rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_PARAM);
                break;
            }

            fd = MG_FS_Open((char *)fileName, O_RDONLY);
            if (fd < 0)
            {
                MG_MEM_Free(pBuffer);
                MG_MEM_Free(pRspBuffer);
                rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR);
                break;
            }

            while (pos < file_size)
            {
                MG_UTILS_Memset(pBuffer, 0, OPEN_AT_FS_BUF_SIZE+1);
                MG_UTILS_Memset(pRspBuffer, 0, nRspBufSize);
                len = file_size - pos;
                if (len > OPEN_AT_FS_BUF_SIZE)
                {
                    len = OPEN_AT_FS_BUF_SIZE;
                }

                rz = MG_FS_Read(fd, pBuffer, len);
                if (rz <= 0)
                    break;

                MG_UTILS_Snprintf((u8 *)pRspBuffer, nRspBufSize, "+FSRDFILE:%s,%d,%s", fileName, rz, pBuffer);
                mg_openAT_cmdRespUrcText(atHandle, pRspBuffer);
                mg_openAT_cmdRespOutputLineBreak(atHandle);
                pos += rz;
            }

            MG_FS_Close(fd);
            MG_MEM_Free(pBuffer);
            MG_MEM_Free(pRspBuffer);

            if (pos != file_size)
            {
                rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR);
                break;                
            }

            rc = mg_openAT_cmdRespOK(atHandle);
            break;
        }

        default:
        {
            rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_NOT_SUPPORT);
            break;
        }
    }

    return rc;
}

CmsRetId openFSDELFILE(const AtCmdInputContext *pAtCmdReq)
{
    CmsRetId    rc = CMS_FAIL;
    UINT32      atHandle = AT_SET_SRC_HANDLER(pAtCmdReq->tid, CMS_REF_SUB_AT_5_ID, pAtCmdReq->chanId);
    UINT32      operaType = (UINT32)pAtCmdReq->operaType;
    CHAR        rspBuf[ATEC_IND_RESP_128_STR_LEN] = {0};
    BOOL        paramok = TRUE;

    switch (operaType)
    {
        case AT_TEST_REQ:             /* AT+FSDELFILE=? */
        {
            MG_UTILS_Snprintf((u8 *)rspBuf, ATEC_IND_RESP_128_STR_LEN, "+FSDELFILE: filename[,tag]");
            rc = mg_openAT_cmdRespTextAndOK(atHandle, rspBuf);
            break;
        }

        case AT_SET_REQ:              /* AT+FSDELFILE= "filename" */
        {
            s32  fileNameLen = 0;
            CHAR fileName[OPEN_AT_FS_FILE_NAME_MAX_LEN+1] = {0};
            fileNameLen = mg_openAT_cmdParamStr(pAtCmdReq->pParamList, 0, (UINT8 *)fileName, OPEN_AT_FS_FILE_NAME_MAX_LEN, &paramok);
            if(!paramok || fileNameLen <= 0)
            {
                rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_PARAM);
                break;
            }

            if(MG_FS_Delete((char *)fileName) < 0)
            {
                rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR);
                break;
            }

            _remove_path(fileName);

            rc = mg_openAT_cmdRespOK(atHandle);
            break;
        }

        default:
        {
            rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_NOT_SUPPORT);
            break;
        }
    }

    return rc;
}


CmsRetId openFSLSTFILE(const AtCmdInputContext *pAtCmdReq)
{
    CmsRetId    rc = CMS_FAIL;
    UINT32      atHandle = AT_SET_SRC_HANDLER(pAtCmdReq->tid, CMS_REF_SUB_AT_5_ID, pAtCmdReq->chanId);
    UINT32      operaType = (UINT32)pAtCmdReq->operaType;
    CHAR        rspBuf[ATEC_IND_RESP_128_STR_LEN] = {0};
    BOOL        paramok = TRUE;

    switch (operaType)
    {
        case AT_TEST_REQ:             /* AT+FSLSTFILE=? */
        {
            rc = mg_openAT_cmdRespOK(atHandle);
            break;
        }

        case AT_SET_REQ:              /* AT+FSLSTFILE= "/" */
        {
            s32  fileNameLen = 0;
            CHAR fileName[OPEN_AT_FS_FILE_NAME_MAX_LEN+1] = {0};
            CHAR fileNameTemp[OPEN_AT_FS_FILE_NAME_MAX_LEN+1] = {0};
            openAt_fileInfo *node = openFileInfoList;
            fileNameLen = mg_openAT_cmdParamStr(pAtCmdReq->pParamList, 0, (UINT8 *)fileName, OPEN_AT_FS_FILE_NAME_MAX_LEN, &paramok);
            if(!paramok || fileNameLen <= 0)
            {
                rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_PARAM);
                break;
            }

            if(MG_UTILS_Strcmp(fileName,"/") != 0)
            {
                rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_PARAM);
                break;
            }

            if(node == NULL)
            {
                rc = mg_openAT_cmdRespOK(atHandle);
                break;
            }

            do
            {
                if (!_open_at_is_header(node))
                {
                    MG_UTILS_Memset(fileNameTemp, 0, OPEN_AT_FS_FILE_NAME_MAX_LEN+1);
                    _open_at_get_file_name(node, fileNameTemp, OPEN_AT_FS_FILE_NAME_MAX_LEN);
                    MG_UTILS_Snprintf((u8 *)rspBuf, ATEC_IND_RESP_128_STR_LEN, "+FSLSTFILE:%s", fileNameTemp);
                    mg_openAT_cmdRespUrcText(atHandle, rspBuf);
                }
                node = _open_at_get_next_node(node);
            } while(node != NULL);

            mg_openAT_cmdRespOutputLineBreak(atHandle);
            rc = mg_openAT_cmdRespOK(atHandle);
            break;
        }

        default:
        {
            rc = mg_openAT_cmdRespError(atHandle, OPEN_AT_ERROR_NOT_SUPPORT);
            break;
        }
    }

    return rc;
}


