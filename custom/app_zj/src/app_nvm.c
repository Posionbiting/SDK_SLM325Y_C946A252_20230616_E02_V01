#include "app_include.h"
#include "mg_fs.h"

#define LOG_TAG MAKE_LOG_TAG('U', 'S', 'E', 'R')

#define APP_CONFIG_FILE    "app_config.nvm"
#define APP_CONFIG_FILE_CURRENT_VERSION           (1)

typedef struct _config_file_header
{
    uint16_t fileBodySize;     /**< size of file body, in unit of byte */
    uint8_t  version;          /**< file version, this field shall be updated when file structure is changed */
    uint8_t  checkSum;         /**< check sum value of file body */
} app_config_file_header_t;

void app_SetDefaultConfig(void)
{
    g_app_cfg.nvConfig.nReportCycle = 180;
    g_app_cfg.nvConfig.batteryCount = 0;
    g_app_cfg.nvConfig.batterySize = 15200000;

}

static uint8_t app_CalcCrcValue(const uint8_t *buf, uint16_t bufSize)
{
    uint32_t i = bufSize;
    uint32_t a = 1, b = 0;

    if(!buf || bufSize <= 0)
        return 0;

    for (i = bufSize; i > 0; )
    {
        a += (uint32_t)(buf[--i]);
        b += a;
    }

    return (uint8_t)(((a>>24)&0xFF)^((a>>16)&0xFF)^((a>>8)&0xFF)^((a)&0xFF)^
                     ((b>>24)&0xFF)^((b>>16)&0xFF)^((b>>8)&0xFF)^((b)&0xFF)^
                     (bufSize&0xFF));
}

void app_nvmSaveConfig(void)
{
    s32 fd = -1;
    s32 wlen = -1;
    app_config_file_header_t fileHeader;

    /*
     * open the config file
    */
    fd = MG_FS_Open(APP_CONFIG_FILE, O_RDWR | O_CREAT | O_TRUNC);
    if (fd < 0)
    {
        APP_DEBUG("Can't open/create 'app_config' file, save app_config failed");
        return;
    }

    /*
     * write the header
    */
    fileHeader.fileBodySize   = sizeof(app_nv_config);
    fileHeader.version        = APP_CONFIG_FILE_CURRENT_VERSION;
    fileHeader.checkSum       = app_CalcCrcValue((uint8_t *)&g_app_cfg.nvConfig, sizeof(app_nv_config));

    wlen = MG_FS_Write(fd, (const char *)&fileHeader, sizeof(app_config_file_header_t));
    if (wlen != sizeof(app_config_file_header_t))
    {
        APP_DEBUG("Write 'app_config' file header failed");
        MG_FS_Close(fd);
        return;
    }

    /*
     * write the file body
    */
    wlen = MG_FS_Write(fd, (const char *)&g_app_cfg.nvConfig, sizeof(app_nv_config));
    if (wlen != sizeof(app_nv_config))
    {
        APP_DEBUG("Write 'app_config' file body failed");
    }

    MG_FS_Close(fd);
}

void app_nvmLoadConfig(void)
{
    s32 fd = -1;
    app_config_file_header_t fileHeader;
    s32 rz = 0;
    uint8_t crcCheck = 0;

    /*
     * open NVM file
     */
    fd = MG_FS_Open(APP_CONFIG_FILE, O_RDONLY);
    if (fd < 0)
    {
        APP_DEBUG("Can't open 'app_config' file, use the defult value");
        app_SetDefaultConfig();
        app_nvmSaveConfig();
        return;
    }

    /*
     * read file header
     */
    rz = MG_FS_Read(fd, (char *)&fileHeader, sizeof(app_config_file_header_t));
    if (rz <= 0)
    {
        APP_DEBUG("Can't read 'app_config' file header, use the defult value");

        MG_FS_Close(fd);
        app_SetDefaultConfig();
        app_nvmSaveConfig();
        return;
    }

    /*
     * read file body, check validation and handle compatiblity issue
     */
    if(fileHeader.version != APP_CONFIG_FILE_CURRENT_VERSION
    || fileHeader.fileBodySize != sizeof(app_nv_config))
    {
        APP_DEBUG("'app_config' version:%d not right, use the defult value", fileHeader.version);

        MG_FS_Close(fd);

        app_SetDefaultConfig();
        app_nvmSaveConfig();
    }
    else
    {
        rz = MG_FS_Read(fd, (char *)&g_app_cfg.nvConfig, sizeof(app_nv_config));
        crcCheck = app_CalcCrcValue((uint8_t *)&g_app_cfg.nvConfig, sizeof(app_nv_config));

        MG_FS_Close(fd);
        if (rz <=0 || crcCheck != fileHeader.checkSum)
        {
            APP_DEBUG("Can't read 'app_config' version:%d file body, or body not right, (%u/%u), use the defult value",
                        fileHeader.version, crcCheck, fileHeader.checkSum);

            app_SetDefaultConfig();
            app_nvmSaveConfig();
        }
    }
}

