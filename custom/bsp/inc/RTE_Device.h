/**
  ******************************************************************************
  * @file    RTE_Device.h
  * @author  MEIG Development Team
  * @version V1.0.1
  * @date    30-May-2023
  * @brief   This file contains all the function configuration for EC618.
  ******************************************************************************
  * @attention
  *     Date            Version         Comments
  *     2023-05-08      V1.0.0          Firt version.
  *     2023-05-30      V1.0.1          Modify Uart1 interrupt mode to DMA_MODE (MeigLinkID: 0012819).
  *
  * <h2><center>&copy; COPYRIGHT 2023 MEIG SMART</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

/* Define to prevent recursive inclusion -------------------------------------*/
#include "ec618.h"

/*  Peripheral IO Mode Select, Must Configure First !!!
    Note, when receiver works in DMA_MODE, interrupt is also enabled to transfer tailing bytes.
*/

/** @addtogroup MODE_definition
  * @{
  */
#define POLLING_MODE                        0x1
#define DMA_MODE                            0x2
#define IRQ_MODE                            0x3
#define UNILOG_MODE                         0x4

#define RTE_UART0_TX_IO_MODE                UNILOG_MODE
#define RTE_UART0_RX_IO_MODE                IRQ_MODE
#define USART0_RX_TRIG_LVL                  (30)

#define RTE_UART1_TX_IO_MODE                DMA_MODE
#define RTE_UART1_RX_IO_MODE                DMA_MODE

#define RTE_UART2_TX_IO_MODE                POLLING_MODE
#define RTE_UART2_RX_IO_MODE                DMA_MODE

#define RTE_SPI0_IO_MODE                    POLLING_MODE

#define RTE_SPI1_IO_MODE                    POLLING_MODE

#define RTE_I2C0_IO_MODE                    POLLING_MODE
#define RTE_I2C1_IO_MODE                    POLLING_MODE

#define EFUSE_INIT_MODE                     POLLING_MODE
#define L2CTLS_INIT_MODE                    POLLING_MODE

#define FLASH_BARE_RW_MODE                  1

/**
  * @}
  */

/** @addtogroup I2C
  * @brief I2C driver modules
  * @{
  */
#if defined(CONFIG_MG_MODULE_NAME_SLM328Y)
#define RTE_I2C0                            1
#define RTE_I2C1                            1

#define RTE_I2C0_SCL_BIT                    28
#define RTE_I2C0_SCL_FUNC                   PAD_MUX_ALT2
#define RTE_I2C0_SDA_BIT                    27
#define RTE_I2C0_SDA_FUNC                   PAD_MUX_ALT2

#define RTE_I2C1_SCL_BIT                    20
#define RTE_I2C1_SCL_FUNC                   PAD_MUX_ALT2
#define RTE_I2C1_SDA_BIT                    19
#define RTE_I2C1_SDA_FUNC                   PAD_MUX_ALT2
#elif defined(CONFIG_MG_MODULE_NAME_SLM332Y)
#define RTE_I2C0                            0
#define RTE_I2C1                            1

#define RTE_I2C1_SCL_BIT                    24
#define RTE_I2C1_SCL_FUNC                   PAD_MUX_ALT2
#define RTE_I2C1_SDA_BIT                    23
#define RTE_I2C1_SDA_FUNC                   PAD_MUX_ALT2
#elif defined(CONFIG_MG_MODULE_NAME_SLM325Y)
#define RTE_I2C0                            1
#define RTE_I2C1                            0

#define RTE_I2C0_SCL_BIT                    13
#define RTE_I2C0_SCL_FUNC                   PAD_MUX_ALT2
#define RTE_I2C0_SDA_BIT                    14
#define RTE_I2C0_SDA_FUNC                   PAD_MUX_ALT2
#endif

#define RTE_I2C0_DMA_TX_EN                  0
#define RTE_I2C0_DMA_TX_REQID               DMA_REQUEST_I2C0_TX
#define RTE_I2C0_DMA_RX_EN                  0
#define RTE_I2C0_DMA_RX_REQID               DMA_REQUEST_I2C0_RX
#define RTE_I2C1_DMA_TX_EN                  1
#define RTE_I2C1_DMA_TX_REQID               DMA_REQUEST_I2C1_TX
#define RTE_I2C1_DMA_RX_EN                  1
#define RTE_I2C1_DMA_RX_REQID               DMA_REQUEST_I2C1_RX
/**
  * @}
  */

/** @addtogroup UART
  * @brief UART driver modules
  * @{
  */
//Driver_USART0
#define RTE_UART0_CTS_PIN_EN                0
#define RTE_UART0_RTS_PIN_EN                0

#define RTE_UART0_RTS_BIT                   27
#define RTE_UART0_RTS_FUNC                  PAD_MUX_ALT3

#define RTE_UART0_CTS_BIT                   28
#define RTE_UART0_CTS_FUNC                  PAD_MUX_ALT3

#define RTE_UART0_RX_BIT                    29
#define RTE_UART0_RX_FUNC                   PAD_MUX_ALT3

#define RTE_UART0_TX_BIT                    30
#define RTE_UART0_TX_FUNC                   PAD_MUX_ALT3

#define RTE_UART0_DMA_TX_REQID              DMA_REQUEST_USART0_TX
#define RTE_UART0_DMA_RX_REQID              DMA_REQUEST_USART0_RX

//Driver_USART1
#define RTE_UART1_CTS_PIN_EN                1
#define RTE_UART1_RTS_PIN_EN                1

#define RTE_UART1_RTS_BIT                   31
#define RTE_UART1_RTS_FUNC                  PAD_MUX_ALT1

#define RTE_UART1_CTS_BIT                   32
#define RTE_UART1_CTS_FUNC                  PAD_MUX_ALT1

#define RTE_UART1_RX_BIT                    33
#define RTE_UART1_RX_FUNC                   PAD_MUX_ALT1

#define RTE_UART1_TX_BIT                    34
#define RTE_UART1_TX_FUNC                   PAD_MUX_ALT1

#define RTE_UART1_DMA_TX_REQID              DMA_REQUEST_USART1_TX
#define RTE_UART1_DMA_RX_REQID              DMA_REQUEST_USART1_RX

//Driver_USART2
#define RTE_UART2_CTS_PIN_EN                0
#define RTE_UART2_RTS_PIN_EN                0

#if defined(CONFIG_MG_MODULE_NAME_SLM325Y)  || defined(CONFIG_MG_MODULE_NAME_SLM325Y_YS)
#define RTE_UART2_RX_BIT                    21
#define RTE_UART2_RX_FUNC                   PAD_MUX_ALT3

#define RTE_UART2_TX_BIT                    22
#define RTE_UART2_TX_FUNC                   PAD_MUX_ALT3
#else
#define RTE_UART2_RX_BIT                    25
#define RTE_UART2_RX_FUNC                   PAD_MUX_ALT3

#define RTE_UART2_TX_BIT                    26
#define RTE_UART2_TX_FUNC                   PAD_MUX_ALT3
#endif

#define RTE_UART2_DMA_TX_REQID              DMA_REQUEST_USART2_TX
#define RTE_UART2_DMA_RX_REQID              DMA_REQUEST_USART2_RX
/**
  * @}
  */

/** @addtogroup SPI
  * @brief SPI driver modules
  * @{
  */

#if defined(CONFIG_MG_MODULE_NAME_SLM328Y)
#define RTE_SPI0                            1
#define RTE_SPI1                            0

#define RTE_SPI0_SSN_BIT                    23
#define RTE_SPI0_SSN_FUNC                   PAD_MUX_ALT1

#define RTE_SPI0_MOSI_BIT                   24
#define RTE_SPI0_MOSI_FUNC                  PAD_MUX_ALT1

#define RTE_SPI0_MISO_BIT                   25
#define RTE_SPI0_MISO_FUNC                  PAD_MUX_ALT1

#define RTE_SPI0_SCLK_BIT                   26
#define RTE_SPI0_SCLK_FUNC                  PAD_MUX_ALT1

#define RTE_SPI0_SSN_GPIO_INSTANCE          0
#define RTE_SPI0_SSN_GPIO_INDEX             8
#elif defined(CONFIG_MG_MODULE_NAME_SLM332Y)
#define RTE_SPI0                            0
#define RTE_SPI1                            0
#elif defined(CONFIG_MG_MODULE_NAME_SLM325Y)
#define RTE_SPI0                            1
#define RTE_SPI1                            1

#define RTE_SPI0_SSN_BIT                    23
#define RTE_SPI0_SSN_FUNC                   PAD_MUX_ALT1

#define RTE_SPI0_MOSI_BIT                   24
#define RTE_SPI0_MOSI_FUNC                  PAD_MUX_ALT1

#define RTE_SPI0_MISO_BIT                   25
#define RTE_SPI0_MISO_FUNC                  PAD_MUX_ALT1

#define RTE_SPI0_SCLK_BIT                   26
#define RTE_SPI0_SCLK_FUNC                  PAD_MUX_ALT1

#define RTE_SPI0_SSN_GPIO_INSTANCE          0
#define RTE_SPI0_SSN_GPIO_INDEX             8

#define RTE_SPI1_SSN_BIT                    27
#define RTE_SPI1_SSN_FUNC                   PAD_MUX_ALT1

#define RTE_SPI1_MOSI_BIT                   28
#define RTE_SPI1_MOSI_FUNC                  PAD_MUX_ALT1

#define RTE_SPI1_MISO_BIT                   29
#define RTE_SPI1_MISO_FUNC                  PAD_MUX_ALT1

#define RTE_SPI1_SCLK_BIT                   30
#define RTE_SPI1_SCLK_FUNC                  PAD_MUX_ALT1

#define RTE_SPI1_SSN_GPIO_INSTANCE          0
#define RTE_SPI1_SSN_GPIO_INDEX             12
#endif

#define RTE_SPI0_DMA_TX_REQID               DMA_REQUEST_SPI0_TX
#define RTE_SPI0_DMA_RX_REQID               DMA_REQUEST_SPI0_RX
#define RTE_SPI1_DMA_TX_REQID               DMA_REQUEST_SPI1_TX
#define RTE_SPI1_DMA_RX_REQID               DMA_REQUEST_SPI1_RX
/**
  * @}
  */

/** @addtogroup PWM
  * @brief PWM driver modules
  * @{
  */
//Driver_PWM0
#define RTE_PWM                             1
/**
  * @}
  */

/** @addtogroup SYS
  * @brief SYS driver modules
  * @{
  */
/*
static configuration for USB/UART relatded feature
    RTE_USB_EN:      whether init USB stack
    RTE_ONE_UART_AT: enable one UART AT
    RTE_ONE_UART_AT: enable two UART AT
    RTE_ETHER_EN   : whehter RNDIS/ECM feature is enabled
    RTE_PPP_EN     : whehter PPPOS feature is enabled
    RTE_OPAQ_EN    : whehter OPAQOS feature is enabled
*/
#ifdef LOW_SPEED_SERVICE_ONLY
/* device */
#define RTE_USB_EN                          1
#define RTE_ONE_UART_AT                     1
#define RTE_TWO_UART_AT                     0

/* feature */
#define RTE_ETHER_EN                        0
#define RTE_PPP_EN                          0
#define RTE_OPAQ_EN                         0

#else
/* device */
#define RTE_USB_EN                          1
#define RTE_ONE_UART_AT                     1
#define RTE_TWO_UART_AT                     0

/* feature */
#if (RTE_USB_EN == 1)
#define RTE_ETHER_EN                        1              /* 0/1: to ctrl eth(rndis/ecm) independently! */
#else
#define RTE_ETHER_EN                        RTE_USB_EN     /* must be the same(disabled) */
#endif
#define RTE_PPP_EN                          1

#define RTE_OPAQ_EN                         1
#endif

/* to be compatible with old style */
#define RTE_RNDIS_EN                        RTE_ETHER_EN


#if (RTE_ONE_UART_AT == 1)
#define RTE_UART0                           1
#define RTE_UART1                           1
#define RTE_UART2                           1
#elif (RTE_TWO_UART_AT == 1)
#define RTE_UART0                           1
#define RTE_UART1                           1
#define RTE_UART2                           1
#endif

/* to enable external thermal */
#define EXTERNAL_NTC_EXIST                  0

#if (RTE_UART1 == 1)
#define UART1_DTR_PAD_INDEX                 26     // GPIO11
#define UART1_DTR_GPIO_INSTANCE             0
#define UART1_DTR_GPIO_PIN                  11

#define UART1_RI_PAD_INDEX                  44     // AONIO 4 = GPIO24
#define UART1_RI_GPIO_INSTANCE              1
#define UART1_RI_GPIO_PIN                   8
#define UART1_RI_PWM_INSTANCE               1
#define UART1_RI_PWM_CLK_ID                 FCLK_TIMER1
#define UART1_RI_PWM_CLK_SEL                FCLK_TIMER1_SEL_26M

#define UART1_DCD_PAD_INDEX                 45     // AONIO 5 = GPIO25
#define UART1_DCD_GPIO_INSTANCE             1
#define UART1_DCD_GPIO_PIN                  9
#endif

#if (RTE_UART2 == 1)
#define UART2_DTR_PAD_INDEX                 25     // GPIO10
#define UART2_DTR_GPIO_INSTANCE             0
#define UART2_DTR_GPIO_PIN                  10

#define UART2_RI_PAD_INDEX                  43     // AONIO 3 = GPIO23
#define UART2_RI_GPIO_INSTANCE              1
#define UART2_RI_GPIO_PIN                   7
#define UART2_RI_PWM_INSTANCE               0
#define UART2_RI_PWM_CLK_ID                 FCLK_TIMER0
#define UART2_RI_PWM_CLK_SEL                FCLK_TIMER0_SEL_26M

#define UART2_DCD_PAD_INDEX                 47     // AONIO 7 = GPIO27
#define UART2_DCD_GPIO_INSTANCE             1
#define UART2_DCD_GPIO_PIN                  11
#endif

#define NETLIGHT_PAD_INDEX                  46     // AONIO 6 = GPIO26
#define NETLIGHT_PAD_ALT_FUNC               PAD_MUX_ALT5
#define NETLIGHT_PWM_INSTANCE               3

//USIM1 OPTION1
#define USIM1_URST_OP1_PAD_INDEX            19     // GPIO4
#define USIM1_URST_OP1_GPIO_INSTANCE        0
#define USIM1_URST_OP1_GPIO_PIN             4
#define USIM1_UCLK_OP1_PAD_INDEX            20     // GPIO5
#define USIM1_UCLK_OP1_GPIO_INSTANCE        0
#define USIM1_UCLK_OP1_GPIO_PIN             5
#define USIM1_UIO_OP1_PAD_INDEX             21     // GPIO6
#define USIM1_UIO_OP1_GPIO_INSTANCE         0
#define USIM1_UIO_OP1_GPIO_PIN              6

//USIM1 OPTION2
#define USIM1_UIO_OP2_PAD_INDEX             27     // GPIO12
#define USIM1_UIO_OP2_GPIO_INSTANCE         0
#define USIM1_UIO_OP2_GPIO_PIN              12
#define USIM1_URST_OP2_PAD_INDEX            28     // GPIO13
#define USIM1_URST_OP2_GPIO_INSTANCE        0
#define USIM1_URST_OP2_GPIO_PIN             13
#define USIM1_UCLK_OP2_PAD_INDEX            29     // GPIO14
#define USIM1_UCLK_OP2_GPIO_INSTANCE        0
#define USIM1_UCLK_OP2_GPIO_PIN             14

//USIM1 clock latched by AONIO, for example, use AONIO-6 test on EVB
#define AONIO_6_PAD_INDEX                   46     // AONIO 6 = GPIO26
#define AONIO_6_GPIO_INSTANCE               1
#define AONIO_6_GPIO_PIN                    10

/**
  * @}
  */
#endif  /* __RTE_DEVICE_H */
/******************* (C) COPYRIGHT 2023 MEIG SMART*****END OF FILE*************/