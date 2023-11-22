#include "mg_common.h"
#include "mg_type.h"
#include "mg_error.h"
#include "mg_os.h"
#include "mg_trace.h"
#include "mg_utils.h"
#include "mg_opt_fs.h"
#include "mg_fs.h"

#define LOG_TAG MAKE_LOG_TAG('E', 'X', 'F', 'S')
#define DEBUG_ENABLE 1
#if DEBUG_ENABLE
#define APP_DEBUG(FORMAT, ...)  MG_TRACE_Printf(LOG_TAG, FORMAT, ##__VA_ARGS__)
#else
#define APP_DEBUG(FORMAT, ...)
#endif

void examples_file(void);

static void prvThreadEntry(void *arg)
{
    MG_OS_ThreadSleep(10000);
    APP_DEBUG("mgfs example entry line[%d]", __LINE__);

    examples_file();
    MG_OS_ThreadSleep(1000);

    APP_DEBUG("mgfs example exit line[%d]", __LINE__);
    MG_OS_ThreadSleep(1000);
    MG_OS_ThreadExit();
}

void examples_file(void)
{
    s32 ret;

    //create a file
    char *file = "/test_fd_wr.txt";
    ret = MG_FS_Open(file, O_RDWR | O_CREAT | O_TRUNC);
    if (ret < 0)
    {
        APP_DEBUG("MG_FS_Open fail line[%d]", __LINE__);
        return;
    }
    APP_DEBUG("open file %s", file);
    MG_OS_ThreadSleep(100);

    s32 fd = ret;

    //write data to fs
    ret = MG_FS_Write(fd, "12345678910", sizeof("12345678910"));
    if (ret < 0)
    {
        APP_DEBUG("MG_FS_Write fail line[%d]", __LINE__);
        MG_FS_Close(fd);
        MG_FS_Delete(file);
        return;
    }
    else
    {
        APP_DEBUG("%s write data size %d", file, ret);
    }
    MG_OS_ThreadSleep(100);

    //read file size
    ret = MG_FS_GetFileSize(file);
    if (ret < 0)
    {
        APP_DEBUG("%s write fail", file);
    }
    else
    {
        APP_DEBUG("%s write data size %d", file, ret);
    }
    MG_OS_ThreadSleep(100);

    //get file pos
    ret = MG_FS_GetFilePosition(fd);
    if (ret < 0)
    {
        APP_DEBUG("%s Get File Position fail", file);
    }
    else
    {
        APP_DEBUG("%s Get File Position %d", file, ret);
    }
    MG_OS_ThreadSleep(100);

    //set file pos
    ret = MG_FS_Seek(fd, 2, SEEK_SET);
    if (ret < 0)
    {
        APP_DEBUG("%s set pos fail", file);
        MG_FS_Close(fd);
        MG_FS_Delete(file);
        return;
    }
    else
    {
        APP_DEBUG("%s set pos %d", file, ret);
    }
    MG_OS_ThreadSleep(100);

    //read data from fs
    char buf[64] = {0};
    ret = MG_FS_Read(fd, buf, sizeof(buf));
    if (ret < 0)
    {
        APP_DEBUG("%s read fail line[%d]", __LINE__);
        MG_FS_Close(fd);
        MG_FS_Delete(file);
        return;
    }
    else
    {
        APP_DEBUG("%s read size %d, data %s", file, ret, buf);
    }
    MG_OS_ThreadSleep(100);
    MG_FS_Close(fd);

    //file rename
    char *newfile = "/test_fd_wr_new.txt";
    ret = MG_FS_Rename(file, newfile);
    if (ret < 0)
    {
        APP_DEBUG("%s rename fail", file);
        MG_FS_Delete(file);
        return;
    }
    APP_DEBUG("%s rename %s", file, newfile);
    MG_OS_ThreadSleep(100);

    //file check
    ret = MG_FS_Check(file);
    if (ret < 0)
    {
        APP_DEBUG("%s no exits", file);
    }
    else
    {
        APP_DEBUG("%s exits", file);
        MG_FS_Delete(file);
        return;
    }
    MG_OS_ThreadSleep(100);

    ret = MG_FS_Check(newfile);
    if (ret < 0)
    {
        APP_DEBUG("%s no exits", newfile);
        return;
    }
    APP_DEBUG("%s exits", newfile);
    MG_OS_ThreadSleep(100);

    //file truncate
    ret = MG_FS_Truncate(newfile, 5);
    if (ret < 0)
    {
        APP_DEBUG("%s Truncate fail", newfile);
        MG_FS_Delete(newfile);
        return;
    }
    APP_DEBUG("%s Truncate length %d", newfile, 5);
    MG_OS_ThreadSleep(100);

    //read file size
    ret = MG_FS_GetFileSize(newfile);
    if (ret < 0)
    {
        APP_DEBUG("%s write fail", newfile);
    }
    else
    {
        APP_DEBUG("%s size %d", newfile, ret);
    }
    MG_FS_Delete(newfile);
    MG_OS_ThreadSleep(100);

}

void proc_main_task(void *param)
{
    APP_DEBUG("mg example enter line[%d]", __LINE__);
    MG_OS_ThreadCreate("mgfs", prvThreadEntry, NULL, Thread_PRIORITY_NORMAL, 1024 * 2, 4);
}

