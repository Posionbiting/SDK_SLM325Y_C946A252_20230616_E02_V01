#ifndef __CAMERA_DRV_H__
#define __CAMERA_DRV_H__

#include "cspi.h"
#include "sp0A39.h"
#include "sp0821.h"
#include "gc6123.h"
#include "gc032A.h"

/**
  \addtogroup cam_interface_gr
  \{
 */


////////////////////////////////////////////////////////////////
// For a picture 320*240=76800, since each DMA descriptor data size should less than 8k, so here we define each DMA
// descriptor data size to 7680bytes, that we need 10 descriptors. If you want to receive 30w pixel picture, modify
// CAM_CHAIN_COUNT to 40. 
////////////////////////////////////////////////////////////////
#define CAMERA_ENABLE_SP0A39            0       ///< SP0A39 enable
	#define SP0A39_2SDR 				0		///< SP0A39 2 wire
	#define SP0A39_1SDR 				0		///< SP0A39 1 wire
#if (CAMERA_ENABLE_SP0A39 == 1)
	#define CAM_CHAIN_COUNT             CAM_30W
#endif

#define CAMERA_ENABLE_SP0821            0       ///< SP0821 enable	
	#define SP0821_2SDR 				0		///< SP0821 2 wire
	#define SP0821_1SDR 				0		///< SP0821 1 wire
#if (CAMERA_ENABLE_SP0821 == 1)
	#define CAM_CHAIN_COUNT             CAM_8W
#endif	


#define CAMERA_ENABLE_GC6123            1       ///< GC6123 enable    
	#define GC6123_2SDR                 1       ///< GC6123 2 wire
    #define GC6123_1SDR                 0       ///< GC6123 1 wire
#if (CAMERA_ENABLE_GC6123 == 1)
	#define CAM_CHAIN_COUNT             CAM_8W
#endif

#define CAMERA_ENABLE_GC032A            0       ///< GC032A enable
	#define GC032A_2SDR 				1		///< GC6123 2 wire
	#define GC032A_1SDR 				0		///< GC6123 1 wire
#if (CAMERA_ENABLE_GC032A == 1)
	#define CAM_CHAIN_COUNT             CAM_30W
#endif	


#ifdef CAM_I2C_USE_GPIO
#include "i2cGpio.h"
#endif

typedef struct
{
    uint8_t regAddr;                            ///< Sensor I2C register address
    uint8_t regVal;                             ///< Sensor I2C register value
}camI2cCfg_t;

typedef enum
{
    LSB_MODE    = 0,                            ///< Little endian
    MSB_MODE    = 1,                            ///< Big endian
}endianMode_e;

typedef enum
{
    WIRE_1      = 0,                            ///< 1 wire
    WIRE_2      = 1,                            ///< 2 wire
}wireNum_e;


typedef enum
{
    SEQ_0       = 0,                            ///< rxd[0] 6 4 2 0
                                                ///< rxd[1] 7 5 3 1    
    SEQ_1       = 1,                            ///< rxd[1] 6 4 2 0
                                                ///< rxd[0] 7 5 3 1
}rxSeq_e;

typedef enum
{
	CSPI_0		= 0,
	CSPI_1		= 1,
}cspiInstance_e;

typedef enum
{
	CSPI_START	 = 1,			///< cspi enable
	CSPI_STOP    = 0,			///< Cspi disable
}cspiStartStop_e;

typedef enum
{
	CSPI_INT_ENABLE	    = 1,		///< cspi interrupt enable
	CSPI_INT_DISABLE    = 0,		///< Cspi interrupt disable
}cspiIntEnable_e;


typedef struct
{
    endianMode_e    endianMode;                 ///< Endian mode
    wireNum_e       wireNum;                    ///< Wire numbers
    rxSeq_e         rxSeq;                      ///< Bit sequence in 2 wire mode
    uint8_t 		cpol;
    uint8_t			cpha;
}camParamCfg_t;

typedef void (*camCbEvent_fn) (uint32_t event); ///< Camera callback event.
typedef void (*camIrq_fn)(void); 					///< Camera irq


/**
  \brief Init camera, include pinMux, and enable clock.  
  \param[in] dataAddr     Mem addr to store picture.
  \param[in] cb           Indicate that a picture has been taken.
  \return              
*/
void camInit(void* dataAddr, camCbEvent_fn cb);

/**
  \brief Receive the picture has been taken.
  \param[out] dataIn     The buffer which is used to store the picture.
  \return              
*/
void camRecv(uint8_t * dataIn);

/**
  \brief Init sensor's registers.
  \return              
*/
void camRegCfg(void);

/**
  \brief Write some parameters into the sensor.
  \param[in] regInfo     Sensor I2C addr and value.
  \return              
*/
void camWriteReg(camI2cCfg_t* regInfo);

/**
  \brief Read from the sensor's I2C address.
  \param[in] regAddr     Sensor's I2C register address.
  \return              
*/
uint8_t camReadReg(uint8_t regAddr);

/**
  \brief Start or stop Camera controller.
  \param[in] startStop     If true, start camera controller. If false, stop camera controller.
  \return              
*/
void camStartStop(cspiStartStop_e startStop);

/**
  \brief Register irq for cspi.
  \param[in] instance     cspi0 or cspi1.
  \param[in] irqCb        irq cb.
  \return              
*/
void camRegisterIRQ(cspiInstance_e instance, camIrq_fn irqCb);

/**
  \brief Get cspi status.
  \param[in] instance     cspi0 or cspi1.
  \return              
*/
uint32_t camGetCspiStats(cspiInstance_e instance);

/**
  \brief Clear cspi interrupt status.
  \param[in] instance     cspi0 or cspi1.
  \param[in] mask         which bit needs to clear.
  \return              
*/
void camClearIntStats(cspiInstance_e instance, uint32_t mask);

/**
  \brief Set memory addr which is used to store picture of camera.
  \param[in] dataAddr     data addr.
  \return              
*/
void camSetMemAddr(uint32_t dataAddr);

/**
  \brief Enable or disable interrupt of cspi.
  \param[in] intEnable     interrupt enable or not.
  \return              
*/
void cspiIntEnable(cspiIntEnable_e intEnable);


/** \} */

#endif
