#ifndef  BSP_CODECDRV_H
#define  BSP_CODECDRV_H

#include "i2s.h"


typedef enum
{
    ES8388,                             ///< Codec ES8388
    NAU88C22,                           ///< Codec NAU88C22
//<!--[ODM][CODEC][TaskID 17994]wbh@2022-10-28 Add codec es8311.
    ES8311,                             ///< Codec ES8311
//end-->
}codecType_e;

typedef struct
{
    uint8_t                 regAddr;    ///< Register addr
    uint16_t                regVal;     ///< Register value
}i2sI2cCfg_t;


/**
  \brief Write value to codec via I2C.
  \param[in] codecType     The codec type you use.
  \param[in] regAddr         I2C register addr.
  \param[in] regVal           I2C register value that need to write.
  \return              
*/
void codecWriteVal(codecType_e codecType, uint8_t regAddr, uint16_t regVal);
uint8_t codecReadVal(codecType_e codecType, uint8_t regAddr);
//void codecCtrlVolume(codecType_e codecType, bool raise, uint8_t step);


#endif

