/**
  ******************************************************************************
  * @file    bsp_custom.c
  * @author  MEIG Development Team
  * @version V1.1.0
  * @date    26-Apr-2023
  * @brief   Set peripherals to the determinate state beford starting the system.
  ******************************************************************************
  * @attention
  *
  *     Date            Version     Comments
  *     2022-12-17      V1.0.0      Add wakeup out function(TaskID 30081).
  *     2023-01-10      V1.0.1      Add system status LED function(TaskID 30374).
  *     2023-01-13      V1.0.2      ADD AT+QSIMDET Voltage-polarity(TaskID 30755).
  *     2023-01-13      V1.0.3      modify for PIR.(TaskID 30896).
  *     2023-02-06      V1.0.4      Modify SIM card hot swap configuration.(TaskID 30861).
  *     2023-02-06      V1.0.5      Turn off the GPIO function and select internal drop-down(TaskID 39826).
  *     2023-02-17      V1.0.6      Modify the nv storage(BugID 41254).
  *     2023-02-21      V1.0.7      Debug GPIO function of WAKEUP pad(TaskID 30366).
  *     2023-02-24      V1.0.8      configure WAKEUP_PAD_4 as GPIO to control power of GPS in SLM325Y(TaskID 32438).
  *     2023-03-21      V1.0.9      Modify AT+ECPIR (TaskId 30896).
  *     2023-04-26      V1.1.0      Replace AGPIO (TaskId 35507).
  *
  * @note
  *
  * <h2><center>&copy; COPYRIGHT 2023 MEIG SMART</center></h2>
  ******************************************************************************
  */

/*----------------------------------------------------------------------------*
 *                    INCLUDES                                                *
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "clock.h"
#include "bsp_custom.h"
#include "slpman.h"
#include "plat_config.h"
#include DEBUG_LOG_HEADER_FILE
#include "exception_process.h"
#include "gpio.h"
#include "timer.h"
#include "reset.h"
#include "apmu_external.h"
#include "uart_device.h"
#include "hal_charge.h"
#include "apmu_external.h"
#if (WDT_FEATURE_ENABLE==1)
#include "wdt.h"
#define WDT_TIMEOUT_VALUE     (20)            // in unit of second, shall be less than 256s
#endif

#include "hal_pwrkey.h"
#include "hal_misc.h"
#include "unilog.h"
#include "usb_ext_inc.h"
#include "hal_alarm.h"
#include "usbd_errinfo.h"

//<!--[ODM][TaskID25305]fw@2022-09-14 add getDebugDVersion
#include "dbversion.h"
//end-->
#include "led.h"

#include "meig_nvm_config.h"

#ifdef CONFIG_MG_CUSTOMER_XRS
#include "atec_meig_xrs.h"
#endif

/*----------------------------------------------------------------------------*
 *                    MACROS                                                  *
 *----------------------------------------------------------------------------*/

 #ifndef BSP_USB_BOOT_INIT_EN
//#define BSP_USB_BOOT_INIT_EN 0
#define BSP_USB_BOOT_INIT_EN 1
#endif

//<!--[ODM][TaskID29162]fw@2022-09-14 add WAKEUP IN PAD MACROS
#ifdef CONFIG_MG_WAKEUP_IN_PAD_INDEX
#define WAKEUP_IN_PAD_INDEX                      CONFIG_MG_WAKEUP_IN_PAD_INDEX
#else
#define WAKEUP_IN_PAD_INDEX                      0
#endif
//end-->

//<!--[ODM][TaskID29162]fw@2022-12_12 add wake up process
#ifdef FEATURE_OPENCPU_ENABLE
extern void MCI_SendPwrKeyEvent(unsigned char keyStatus);
extern void MCI_SendWakeupInEvent(unsigned char index, unsigned char pin_level);
#endif
//end-->

/*----------------------------------------------------------------------------*
 *                    DATA TYPE DEFINITION                                    *
 *----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*
 *                      GLOBAL VARIABLES                                      *
 *----------------------------------------------------------------------------*/

extern ARM_DRIVER_USART Driver_USART1;

//<!--[ODM][RAM][NULL]zcxz@2022-01-11 Change the size of the RAM reserved array
//#ifdef LOW_SPEED_SERVICE_ONLY
//__USED static uint8_t rsvUsrRam[400*1024];    // 400KB ram reserved for customer
//#else
//__USED static uint8_t rsvUsrRam[176*1024];    // 176KB ram reserved for customer
//#endif
__USED static uint8_t rsvUsrRam[1];    // 200KB ram reserved for customer
//end-->[ODM][RAM][NULL]zcxz@2022-01-11 Change the size of the RAM reserved array

/*----------------------------------------------------------------------------*
 *                      PRIVATE FUNCTION DECLEARATION                         *
 *----------------------------------------------------------------------------*/

typedef int (*pfn_PadWakeupHook)(uint32_t pad_num);
extern BOOL excepIsInExcephandler(void);
extern void apmuSetCPFastBoot(bool force_on);

extern void BSP_InitUartDriver(ARM_DRIVER_USART *drvHandler,
                               ARM_POWER_STATE powerMode,
                               uint32_t settings,
                               uint32_t baudRate,
                               ARM_USART_SignalEvent_t cb_event);
extern void BSP_DeinitUartDriver(ARM_DRIVER_USART *drvHandler);

extern void usbstack_top_var_clear(void);
extern uint8_t usbstack_clear_ctx_stat(void);
extern uint8_t usbstack_ctx_stat_ison(void);
extern void usbstack_set_ctx_vbus_mode(uint8_t vbus_mode_en, uint8_t vbus_pad_idx);
extern void usbc_call_usbmst_slp1_retothwk_earlyinit(void);


/*----------------------------------------------------------------------------*
 *                      PRIVATE FUNCTIONS                                     *
 *----------------------------------------------------------------------------*/

void srvWakeupInit(void);
bool srvWakeupOut(bool level);

/*----------------------------------------------------------------------------*
 *                      GLOBAL FUNCTIONS                                      *
 *----------------------------------------------------------------------------*/

//<!--[ODM][TaskID29162]fw@2022-12_12 add wake up process
//<!--[ODM][TaskID25305]fw@2022-09-14 add getDebugDVersion
#ifdef FEATURE_OPENCPU_ENABLE
uint8_t* getDebugDVersion(void)
{
    return (uint8_t*)DB_VERSION_UNIQ_ID;
}

void wakeup_in_process(unsigned char index)
{
    unsigned char pin_level = 0;
    if (index == WAKEUP_IN_PAD_INDEX)
    {
        if((slpManGetWakeupPinValue() & (0x1<<index)) == (0x1<<index))
        {
            pin_level = 1;
        }
        MCI_SendWakeupInEvent(index, pin_level);
    }
}
#endif
//end-->
//end-->

/**
  \brief Init uart handler for AT CMD
  \return
 */
void InitAtUartHandler(uint8_t uartIndex, uint32_t baudRate, uint32_t frameFormat)
{
    atPortFrameFormat_t    uartFrameFmt;
    UartDevConf_t           uartDevConf;
    UartHwConf_t            *uartHwConf = &uartDevConf.hwConf;

    memset(&uartDevConf, 0, sizeof(UartDevConf_t));

    uartHwConf->powerMode   = ARM_POWER_FULL;
    uartHwConf->ctrlSetting = ARM_USART_MODE_ASYNCHRONOUS;
    uartHwConf->baudRate    = baudRate;

    uartFrameFmt.wholeValue = frameFormat;
    switch(uartFrameFmt.config.dataBits)
    {
        case 1:
            uartHwConf->ctrlSetting |= ARM_USART_DATA_BITS_7;
            break;
        default:
            uartHwConf->ctrlSetting |= ARM_USART_DATA_BITS_8;
            break;
    }

    switch(uartFrameFmt.config.parity)
    {
        case 1:
            uartHwConf->ctrlSetting |= ARM_USART_PARITY_ODD;
            break;
        case 2:
            uartHwConf->ctrlSetting |= ARM_USART_PARITY_EVEN;
            break;
        default:
            uartHwConf->ctrlSetting |= ARM_USART_PARITY_NONE;
            break;
    }

    switch(uartFrameFmt.config.stopBits)
    {
        case 2:
            uartHwConf->ctrlSetting |= ARM_USART_STOP_BITS_2;
            break;
        default:
            uartHwConf->ctrlSetting |= ARM_USART_STOP_BITS_1;
            break;
    }

    switch(uartFrameFmt.config.flowControl)
    {
        case 3:
            uartHwConf->ctrlSetting |= ARM_USART_FLOW_CONTROL_RTS_CTS;
            break;
        case 2:
            uartHwConf->ctrlSetting |= ARM_USART_FLOW_CONTROL_CTS;
            break;
        case 1:
            uartHwConf->ctrlSetting |= ARM_USART_FLOW_CONTROL_RTS;
            break;
        case 0:
        default:
            uartHwConf->ctrlSetting |= ARM_USART_FLOW_CONTROL_NONE;
            break;
    }

    switch(baudRate)
    {
        case 9600:
        case 4800:
        case 2400:
        case 1200:
        case 600:
            uartHwConf->powerMode = ARM_POWER_LOW;
            break;
        default:
            break;
    }


    if(uartIndex == 0)
    {
#if (RTE_UART0)
        extern ARM_DRIVER_USART Driver_USART0;
        uartDevConf.drvHandler = &Driver_USART0;
#endif
    }
    else if(uartIndex == 1)
    {
#if (RTE_UART1)
        extern ARM_DRIVER_USART Driver_LPUSART1;
        uartDevConf.drvHandler  = &Driver_LPUSART1;
        uartDevConf.isDftAtPort = 1;
#endif
    }
    else if (uartIndex == 2)
    {
#if (RTE_UART2)
        extern ARM_DRIVER_USART Driver_USART2;
        uartDevConf.drvHandler = &Driver_USART2;
#endif
    }

    EC_ASSERT(uartDevConf.drvHandler, uartDevConf.drvHandler, 0, 0);

    uartDevConf.mainUsage    = CSIO_DT_AT;
    uartDevConf.speedType    = CCIO_ST_SLOW;
    uartDevConf.bmCreateFlag = CCIO_TASK_FLAG_RX | CCIO_TASK_FLAG_TX;

    // open printf throught AT port
    // UsartPrintHandle = uartDevConf.drvHandler;
    uartDevCreate(uartIndex, &uartDevConf);
}

/*
 *  set printf and at command uart port
 *  Parameter:      port: port for at cmd
 */
void SetAtUart(usart_port_t port)
{
    uint32_t baudRate = BSP_GetPlatConfigItemValue(PLAT_CONFIG_ITEM_AT_PORT_BAUDRATE);
    uint32_t frameFormat = BSP_GetPlatConfigItemValue(PLAT_CONFIG_ITEM_AT_PORT_FRAME_FORMAT);

    // check if auto baud rate flag is set
    if(baudRate & 0x80000000UL)
    {
        // re-enable auto baud detection on POR, otherwise keep setting if wakeup from other low power modes
        if(slpManGetLastSlpState() == SLP_ACTIVE_STATE)
        {
            // Shall overwrite previous detection value for corner case:
            // previous detection e.g 9600 is recorded, when power on again, the baud rate is going to change to 115200, however device enter
            // lower poer mode before auto detection is done, after wakeup, it'll mess up, so we need to keep this state sync by saving setting
            if(baudRate & 0x7FFFFFFFUL)
            {
                BSP_SetPlatConfigItemValue(PLAT_CONFIG_ITEM_AT_PORT_BAUDRATE, 0x80000000UL);
                BSP_SavePlatConfigToFs();
                /* ensure the baud to be identical! */
                BSP_SavePlatConfigToRawFlash();
            }

            baudRate = 0; // auto baud rate

        }
        else
        {
            baudRate &= 0x7FFFFFFFUL;
        }
    }
    else
    {
        baudRate &= 0x7FFFFFFFUL;
    }

    InitAtUartHandler(port, baudRate, frameFormat);
}

/*
 *  unset uart port
 *  Parameter: port: port for atcmd/unilog
 */
void UnsetAtOrLogUart(usart_port_t port)
{
    uint32_t uartIdx = 0xff;

    if(port == PORT_USART_0)
    {
#if (RTE_UART0)
        uartIdx = 0;
#endif
    }
    else if(port == PORT_USART_1)
    {
#if (RTE_UART1)
        uartIdx = 1;
#endif
    }
    else if (port == PORT_USART_2)
    {
#if (RTE_UART2)
        uartIdx = 2;
#endif
    }

    uartDevDestroy(uartIdx);
}


void GPR_SetUartClk(void)
{
    GPR_clockDisable(FCLK_UART0);
    GPR_clockDisable(FCLK_UART1);

    GPR_setClockSrc(FCLK_UART0, FCLK_UART0_SEL_26M);
    GPR_setClockSrc(FCLK_UART1, FCLK_UART1_SEL_26M);

    GPR_clockEnable(FCLK_UART0);
    GPR_clockEnable(FCLK_UART1);

    GPR_swReset(RST_FCLK_UART0);
    GPR_swReset(RST_FCLK_UART1);
}



#if (WDT_FEATURE_ENABLE == 1)

/*
 *  WDT Initialize, wdt timeout value is 20s
 *  Parameter:   none
 */
void BSP_WdtInit(void)
{
    // Config WDT clock, source from 32.768KHz and divide by WDT_TIMEOUT_VALUE
    GPR_setClockSrc(FCLK_WDG, FCLK_WDG_SEL_32K);
    GPR_setClockDiv(FCLK_WDG, WDT_TIMEOUT_VALUE);

    WdtConfig_t wdtConfig;
    wdtConfig.mode = WDT_INTERRUPT_RESET_MODE;
    wdtConfig.timeoutValue = 32768U;
    WDT_init(&wdtConfig);
}

#endif

void NMI_Handler()
{
    slpManAonWdtStop();

    ECPLAT_PRINTF(UNILOG_PLA_APP, enter_NMI_handler, P_ERROR, "WDT timeout!!! Enter NMI Handler!!!");

    // If we have been in exception handler excecution, we shall resume it.
    if(excepIsInExcephandler())
    {
        WDT_stop();
    }
    else
    {
        ResetReasonWriteAP(RESET_REASON_WDT);
        if(BSP_GetPlatConfigItemValue(PLAT_CONFIG_ITEM_FAULT_ACTION) == EXCEP_OPTION_SILENT_RESET)
        {
            ResetECSystemReset();
            while(1);
        }
        else
        {
            EC_ASSERT(0, 0, 0, 0);
        }
    }
}



void NetLightInit(void)
{
    slpManAONIOPowerOn();			// power on AONIO if we use aonio

    PadConfig_t padConfig;
    PAD_getDefaultConfig(&padConfig);

    padConfig.mux = NETLIGHT_PAD_ALT_FUNC;
    PAD_setPinConfig(NETLIGHT_PAD_INDEX, &padConfig);

    // Config PWM clock, source from 26MHz and divide by 1
    CLOCK_setClockSrc(FCLK_TIMER3, FCLK_TIMER3_SEL_26M);
    CLOCK_setClockDiv(FCLK_TIMER3, 1);

    TIMER_driverInit();

    TIMER_netlightEnable(NETLIGHT_PWM_INSTANCE);

}

#define UART_PAD_PAD_INDEX(id,tag)      (UART##id##_##tag##_PAD_INDEX)
#define UART_PAD_GPIO_INSTANCE(id,tag)  (UART##id##_##tag##_GPIO_INSTANCE)
#define UART_PAD_GPIO_PIN(id,tag)       (UART##id##_##tag##_GPIO_PIN)
#define UART_PAD_PWM_INSTANCE(id,tag)   (UART##id##_##tag##_PWM_INSTANCE)
#define UART_PAD_PWM_CLK_ID(id,tag)     (UART##id##_##tag##_PWM_CLK_ID)
#define UART_PAD_PWM_CLK_SEL(id,tag)    (UART##id##_##tag##_PWM_CLK_SEL)

typedef struct
{
    uint8_t   hasInit;
    uint8_t   pinId;    /* pin@gpio */
    uint16_t  instId;
    uint32_t  data1;    /* initVal@gpio or dutyCyclePct@pwm */
    uint32_t  data2;    /* handleCb@gpio, cycleTimeMs@pwm */
}UartPadInst_t;

typedef struct
{
    uint8_t  hasUsed;
    uint8_t  altFunc;  /* PadMux_e */
    uint8_t  rsvd[2];
    UartPadInst_t inst[2];
}UartPadInstSel_t;

static void UartGpioInit(uint16_t padIdx, uint32_t port, uint16_t pin, uint16_t pinDir, uint16_t initVal)
{
    PadConfig_t      padConfig;
    GpioPinConfig_t  pinConfig;

    // gpio pin out config
    PAD_getDefaultConfig(&padConfig);
    padConfig.mux = PAD_MUX_ALT0;
    PAD_setPinConfig(padIdx, &padConfig);
    PAD_setPinPullConfig(padIdx, PAD_INTERNAL_PULL_UP);

    pinConfig.pinDirection = pinDir;
    if(pinDir == GPIO_DIRECTION_INPUT)
    {
        pinConfig.misc.interruptConfig = initVal;
    }
    else
    {
        pinConfig.misc.initOutput = initVal;
    }

    GPIO_pinConfig(port, pin, &pinConfig);
}

static void UartPwmInit(uint16_t padIdx, uint32_t clkId, uint32_t clkSel)
{
    PadConfig_t      padConfig;

    // PWM out pin config
    PAD_getDefaultConfig(&padConfig);
    padConfig.mux = PAD_MUX_ALT5;
    PAD_setPinConfig(padIdx, &padConfig);

    // Config PWM clock, source from 26MHz and divide by 1
    CLOCK_setClockSrc(clkId, clkSel);
    CLOCK_setClockDiv(clkId, 1);

    TIMER_driverInit();
}

static int32_t UartPwmSetup(uint32_t pwmInst, uint32_t clkFreq, uint16_t dutyCyclePct, uint32_t cycleTimeMs)
{
    uint32_t          pwmFreqHz = 0;
    uint32_t          clkMagnif = 0;
    TimerPwmConfig_t  pwmConfig;

    if(cycleTimeMs <= 1000)
    {
        pwmFreqHz = 1000 / cycleTimeMs;
        clkMagnif = 1;
    }
    else
    {
        pwmFreqHz = 1;
        clkMagnif = cycleTimeMs / 1000;
    }

    pwmConfig.pwmFreq_HZ       = pwmFreqHz;
    pwmConfig.srcClock_HZ      = clkFreq * clkMagnif;  // 26MHz * clkMagnif: to support pwmFreq down to (1/clkMagnif)Hz
    pwmConfig.dutyCyclePercent = dutyCyclePct;
    return TIMER_setupPwm(pwmInst, &pwmConfig);
}

#define UART_DTR_PAD_INDEX(id)          UART_PAD_PAD_INDEX(id,DTR)
#define UART_DTR_GPIO_INSTANCE(id)      UART_PAD_GPIO_INSTANCE(id,DTR)
#define UART_DTR_GPIO_PIN(id)           UART_PAD_GPIO_PIN(id,DTR)

static UartPadInst_t  gUartDtrInst[PORT_USART_MAX] = {0};

static void UartDtrGpioIsr(uint8_t uartIdx)
{
    EC_ASSERT(uartIdx < PORT_USART_MAX && gUartDtrInst[uartIdx].hasInit, uartIdx, gUartDtrInst[uartIdx].hasInit, 0);

    uint16_t portIrqMask = GPIO_saveAndSetIrqMask(gUartDtrInst[uartIdx].instId);

    if (GPIO_getInterruptFlags(gUartDtrInst[uartIdx].instId) & (1 << gUartDtrInst[uartIdx].pinId))
    {
        if(gUartDtrInst[uartIdx].data2)
        {
            ((void(*)(uint32_t))gUartDtrInst[uartIdx].data2)(uartIdx);
        }
        else
        {
            ECPLAT_PRINTF(UNILOG_PLA_APP, UART_DTR_ISR, P_WARNING, "uart(%d): DTR GPIO is not inited!!!", uartIdx);
        }
        GPIO_clearInterruptFlags(gUartDtrInst[uartIdx].instId, 1 << gUartDtrInst[uartIdx].pinId);
    }

    GPIO_restoreIrqMask(gUartDtrInst[uartIdx].instId, portIrqMask);
}

#if (RTE_UART1 == 1)
static void Uart1DtrGpioIsr(void)
{
    UartDtrGpioIsr(1);
}
#endif

#if (RTE_UART2 == 1)
static void Uart2DtrGpioIsr(void)
{
    UartDtrGpioIsr(2);
}
#endif

int32_t UartDtrGpioEnable(uint8_t uartIdx, void(*statusCb)(uint32_t))
{
    void (*handleCb)(void);
    uint16_t           padIndex = 0;
    uint16_t           gpioInst = 0;
    uint16_t            gpioPin = 0;

    switch(uartIdx)
    {
    #if 0
        case PORT_USART_0:
        {
            padIndex = UART_DTR_PAD_INDEX(0);
            gpioInst = UART_DTR_GPIO_INSTANCE(0);
            gpioPin  = UART_DTR_GPIO_PIN(0);
            handleCb = Uart0DtrGpioIsr;
            break;
        }
    #endif
    #if (RTE_UART1 == 1)
        case PORT_USART_1:
        {
            padIndex = UART_DTR_PAD_INDEX(1);
            gpioInst = UART_DTR_GPIO_INSTANCE(1);
            gpioPin  = UART_DTR_GPIO_PIN(1);
            handleCb = Uart1DtrGpioIsr;
            break;
        }
    #endif
    #if (RTE_UART2 == 1)
        case PORT_USART_2:
        {
            padIndex = UART_DTR_PAD_INDEX(2);
            gpioInst = UART_DTR_GPIO_INSTANCE(2);
            gpioPin  = UART_DTR_GPIO_PIN(2);
            handleCb = Uart2DtrGpioIsr;
            break;
        }
    #endif

        default:
            return -1;
    }

    if(gUartDtrInst[uartIdx].hasInit == 0)
    {
        gUartDtrInst[uartIdx].hasInit = 1;
        gUartDtrInst[uartIdx].pinId   = gpioPin;
        gUartDtrInst[uartIdx].instId  = gpioInst;
        gUartDtrInst[uartIdx].data1   = GPIO_INTERRUPT_RISING_EDGE;
        gUartDtrInst[uartIdx].data2   = (uint32_t)statusCb;

        UartGpioInit(padIndex, gpioInst, gpioPin, GPIO_DIRECTION_INPUT, GPIO_INTERRUPT_RISING_EDGE);
        // Enable IRQ
        XIC_SetVector(PXIC1_GPIO_IRQn, handleCb);
        XIC_EnableIRQ(PXIC1_GPIO_IRQn);
    }

    return 0;
}

#define UART_DCD_PAD_INDEX(id)          UART_PAD_PAD_INDEX(id,DCD)
#define UART_DCD_GPIO_INSTANCE(id)      UART_PAD_GPIO_INSTANCE(id,DCD)
#define UART_DCD_GPIO_PIN(id)           UART_PAD_GPIO_PIN(id,DCD)

static UartPadInst_t  gUartDcdInst[PORT_USART_MAX] = {0};

int32_t UartDcdGpioSet(uint8_t uartIdx, uint16_t outVal)
{
    uint16_t           padIndex = 0;
    uint16_t           gpioInst = 0;
    uint16_t            gpioPin = 0;
    GpioPinConfig_t   pinConfig;

    switch(uartIdx)
    {
    #if 0
        case PORT_USART_0:
        {
            padIndex = UART_DCD_PAD_INDEX(0);
            gpioInst = UART_DCD_GPIO_INSTANCE(0);
            gpioPin  = UART_DCD_GPIO_PIN(0);
            break;
        }
    #endif
    #if (RTE_UART1 == 1)
        case PORT_USART_1:
        {
            padIndex = UART_DCD_PAD_INDEX(1);
            gpioInst = UART_DCD_GPIO_INSTANCE(1);
            gpioPin  = UART_DCD_GPIO_PIN(1);
            break;
        }
    #endif
    #if (RTE_UART2 == 1)
        case PORT_USART_2:
        {
            padIndex = UART_DCD_PAD_INDEX(2);
            gpioInst = UART_DCD_GPIO_INSTANCE(2);
            gpioPin  = UART_DCD_GPIO_PIN(2);
            break;
        }
    #endif

        default:
            return -1;
    }

    if(gUartDcdInst[uartIdx].hasInit == 0)
    {
        gUartDcdInst[uartIdx].hasInit = 1;
        gUartDcdInst[uartIdx].pinId   = gpioPin;
        gUartDcdInst[uartIdx].instId  = gpioInst;
        gUartDcdInst[uartIdx].data1   = outVal;
        gUartDcdInst[uartIdx].data2   = 0;

        slpManAONIOPowerOn();  // power on AONIO if we use aonio
        UartGpioInit(padIndex, gpioInst, gpioPin, GPIO_DIRECTION_OUTPUT, outVal);
    }
    else
    {
        if(gUartDcdInst[uartIdx].data1 == outVal) return 0;

        gUartDcdInst[uartIdx].data1 = outVal;

        pinConfig.pinDirection    = GPIO_DIRECTION_OUTPUT;
        pinConfig.misc.initOutput = outVal;
        GPIO_pinConfig(gpioInst, gpioPin, &pinConfig);
    }

    return 0;
}

#define UART_RI_PAD_INDEX(id)           UART_PAD_PAD_INDEX(id,RI)
#define UART_RI_GPIO_INSTANCE(id)       UART_PAD_GPIO_INSTANCE(id,RI)
#define UART_RI_GPIO_PIN(id)            UART_PAD_GPIO_PIN(id,RI)
#define UART_RI_PWM_INSTANCE(id)        UART_PAD_PWM_INSTANCE(id,RI)
#define UART_RI_PWM_CLK_ID(id)          UART_PAD_PWM_CLK_ID(id,RI)
#define UART_RI_PWM_CLK_SEL(id)         UART_PAD_PWM_CLK_SEL(id,RI)

static UartPadInstSel_t  gUartRiInstSel[PORT_USART_MAX] = {0};

int32_t UartRiGpioSet(uint8_t uartIdx, uint16_t outVal)
{
    uint16_t           padIndex = 0;
    uint16_t           gpioInst = 0;
    uint16_t            gpioPin = 0;

    switch(uartIdx)
    {
    #if 0
        case PORT_USART_0:
        {
            padIndex = UART_RI_PAD_INDEX(0);
            gpioInst = UART_RI_GPIO_INSTANCE(0);
            gpioPin  = UART_RI_GPIO_PIN(0);
            break;
        }
    #endif
    #if (RTE_UART1 == 1)
        case PORT_USART_1:
        {
            padIndex = UART_RI_PAD_INDEX(1);
            gpioInst = UART_RI_GPIO_INSTANCE(1);
            gpioPin  = UART_RI_GPIO_PIN(1);
            break;
        }
    #endif
    #if (RTE_UART2 == 1)
        case PORT_USART_2:
        {
            padIndex = UART_RI_PAD_INDEX(2);
            gpioInst = UART_RI_GPIO_INSTANCE(2);
            gpioPin  = UART_RI_GPIO_PIN(2);
            break;
        }
    #endif

        default:
            return -1;
    }

    if(gUartRiInstSel[uartIdx].hasUsed == 0 ||
       gUartRiInstSel[uartIdx].altFunc != PAD_MUX_ALT0 ||
       gUartRiInstSel[uartIdx].inst[0].hasInit == 0)
    {
        gUartRiInstSel[uartIdx].inst[0].hasInit = 1;
        gUartRiInstSel[uartIdx].inst[0].pinId   = gpioPin;
        gUartRiInstSel[uartIdx].inst[0].instId  = gpioInst;
        gUartRiInstSel[uartIdx].inst[0].data1   = outVal;
        gUartRiInstSel[uartIdx].inst[0].data2   = 0;

        /* reset */
        slpManAONIOPowerOn();  // power on AONIO if we use aonio
        UartGpioInit(padIndex, gpioInst, gpioPin, GPIO_DIRECTION_OUTPUT, outVal);
    }
    else
    {
        if(gUartRiInstSel[uartIdx].inst[0].data1 == outVal) return 0;

        gUartRiInstSel[uartIdx].inst[0].data1 = outVal;

        GpioPinConfig_t   pinConfig;
        pinConfig.pinDirection    = GPIO_DIRECTION_OUTPUT;
        pinConfig.misc.initOutput = outVal;
        GPIO_pinConfig(gpioInst, gpioPin, &pinConfig);
    }

    return 0;
}

static int32_t UartRiPwmInit(uint8_t uartIdx, uint16_t dutyCyclePct, uint32_t cycleTimeMs)
{
    uint32_t          padIndex = 0;
    uint32_t           pwmInst = 0;
    uint32_t          pwmClkId = 0;
    uint32_t         pwmClkSel = 0;

    switch(uartIdx)
    {
    #if 0
        case PORT_USART_0:
        {
            padIndex  = UART_RI_PAD_INDEX(0);
            pwmInst   = UART_RI_PWM_INSTANCE(0);
            pwmClkId  = UART_RI_PWM_CLK_ID(0);
            pwmClkSel = UART_RI_PWM_CLK_SEL(0);
            break;
        }
    #endif
    #if (RTE_UART1 == 1)
        case PORT_USART_1:
        {
            padIndex  = UART_RI_PAD_INDEX(1);
            pwmInst   = UART_RI_PWM_INSTANCE(1);
            pwmClkId  = UART_RI_PWM_CLK_ID(1);
            pwmClkSel = UART_RI_PWM_CLK_SEL(1);
            break;
        }
    #endif
    #if (RTE_UART2 == 1)
        case PORT_USART_2:
        {
            padIndex  = UART_RI_PAD_INDEX(2);
            pwmInst   = UART_RI_PWM_INSTANCE(2);
            pwmClkId  = UART_RI_PWM_CLK_ID(2);
            pwmClkSel = UART_RI_PWM_CLK_SEL(2);
            break;
        }
    #endif

        default:
            return -1;
    }

    gUartRiInstSel[uartIdx].inst[1].hasInit = 1;
    gUartRiInstSel[uartIdx].inst[1].instId  = pwmInst;
    gUartRiInstSel[uartIdx].inst[1].data1   = dutyCyclePct;
    gUartRiInstSel[uartIdx].inst[1].data2   = cycleTimeMs;

    /* reset */
    slpManAONIOPowerOn();  // power on AONIO if we use aonio
    UartPwmInit(padIndex, pwmClkId, pwmClkSel);
    UartPwmSetup(pwmInst, GPR_getClockFreq(pwmClkId), dutyCyclePct, cycleTimeMs);

    return 0;
}

int32_t UartRiPwmDisable(uint8_t uartIdx)
{
    if(uartIdx >= PORT_USART_MAX || gUartRiInstSel[uartIdx].inst[1].hasInit == 0) return -1;

    TIMER_stop(gUartRiInstSel[uartIdx].inst[1].instId);

    return 0;
}

int32_t UartRiPwmEnable(uint8_t uartIdx, uint16_t dutyCyclePct, uint32_t cycleTimeMs)
{
    if(uartIdx >= PORT_USART_MAX) return -1;

    if(gUartRiInstSel[uartIdx].hasUsed == 0 ||
       gUartRiInstSel[uartIdx].altFunc != PAD_MUX_ALT5 ||
       gUartRiInstSel[uartIdx].inst[1].hasInit == 0 ||
       gUartRiInstSel[uartIdx].inst[1].data2 != cycleTimeMs)
    {
        if(gUartRiInstSel[uartIdx].inst[1].data2 != cycleTimeMs)
        {
            UartRiPwmDisable(uartIdx);
        }

        if(0 != UartRiPwmInit(uartIdx, dutyCyclePct, cycleTimeMs)) return -1;
    }
    else
    {
        if(gUartRiInstSel[uartIdx].inst[1].data1 != dutyCyclePct)
        {
            gUartRiInstSel[uartIdx].inst[1].data1 = dutyCyclePct;
            TIMER_updatePwmDutyCycle(gUartRiInstSel[uartIdx].inst[1].instId, dutyCyclePct);
        }
    }

    TIMER_start(gUartRiInstSel[uartIdx].inst[1].instId);

    return 0;
}

void WakeupPadInit(void)
{
    APmuWakeupPadSettings_t wakeupPadSetting;

#ifdef CONFIG_MG_CUSTOMER_XRS
    wakeupPadSetting.negEdgeEn = false;
    wakeupPadSetting.posEdgeEn = true;
    wakeupPadSetting.pullDownEn = true;
    wakeupPadSetting.pullUpEn = false;
#else
    wakeupPadSetting.negEdgeEn = true;
//<--[ODM][WAKEUP][TaskID 24755]wbh@2022-08-19 Configure both rising and falling edge.
    wakeupPadSetting.posEdgeEn = true;
//end-->

    wakeupPadSetting.pullDownEn = false;
    wakeupPadSetting.pullUpEn = true;
#endif
    apmuSetWakeupPadCfg(WAKEUP_PAD_0, true, &wakeupPadSetting);
    apmuSetWakeupPadCfg(WAKEUP_PAD_1, true, &wakeupPadSetting);
    apmuSetWakeupPadCfg(WAKEUP_PAD_2, true, &wakeupPadSetting);
    apmuSetWakeupPadCfg(WAKEUP_PAD_3, true, &wakeupPadSetting);
    apmuSetWakeupPadCfg(WAKEUP_PAD_5, true, &wakeupPadSetting);

    NVIC_EnableIRQ(PadWakeup0_IRQn);
    NVIC_EnableIRQ(PadWakeup1_IRQn);
    NVIC_EnableIRQ(PadWakeup2_IRQn);
    NVIC_EnableIRQ(PadWakeup3_IRQn);
    NVIC_EnableIRQ(PadWakeup5_IRQn);

    //WAKEUP_PAD_4(J6) is used to control power of GPS in SLM325Y, so we should set it to low level after system startup.
#if defined(CONFIG_MG_GPS_EN_PAD_INDEX) && (CONFIG_MG_GPS_EN_PAD_INDEX == 41)
    wakeupPadSetting.pullUpEn = false;
    wakeupPadSetting.pullDownEn = false;
    apmuSetWakeupPadCfg(WAKEUP_PAD_4, false, &wakeupPadSetting);
    NVIC_DisableIRQ(PadWakeup4_IRQn);
#else
    apmuSetWakeupPadCfg(WAKEUP_PAD_3, true, &wakeupPadSetting);
    NVIC_EnableIRQ(PadWakeup4_IRQn);
#endif
    srvWakeupInit();
}

void reConfig(void)
{
    APmuWakeupPadSettings_t wakeupPadSetting;
    wakeupPadSetting.negEdgeEn = true;
    wakeupPadSetting.posEdgeEn = true;

    MGNvmConfig mg_nvmConfig = {0};
    MG_NV_GetMgSysParam(&mg_nvmConfig);
    if (mg_nvmConfig.bSimVoltTrigMode == false)
    {
        wakeupPadSetting.pullDownEn = false;
        wakeupPadSetting.pullUpEn = true;
        apmuSetWakeupPadCfg(WAKEUP_PAD_2, true, &wakeupPadSetting);
    }
    else
    {
        wakeupPadSetting.pullDownEn = true;
        wakeupPadSetting.pullUpEn = false;
        apmuSetWakeupPadCfg(WAKEUP_PAD_2, true, &wakeupPadSetting);
    }

    NVIC_EnableIRQ(PadWakeup2_IRQn);

    srvWakeupInit();
}

void chargeStatusCallbackFunc(chargeStatus_e status)
{
    ECPLAT_PRINTF(UNILOG_PMU, chargeStatusCb_1, P_VALUE, "Charge Status update = %d", status);
}

void powerKeyStatusUpdate(pwrKeyPressStatus status)
{
    ECPLAT_PRINTF(UNILOG_PMU, powerKeyStatusUpdate_1, P_VALUE, "PowerKey Status update to = %d", status);

//<!--[ODM][TaskID29162]fw@2022-12_12 add wake up process
#ifdef FEATURE_OPENCPU_ENABLE
    if (status == PWRKEY_LONGPRESS || status == PWRKEY_PRESS)
    {
        MCI_SendPwrKeyEvent(1);
    }
    else if (status == PWRKEY_RELEASE)
    {
        MCI_SendPwrKeyEvent(0);
    }
#endif
//end-->

    if(status == PWRKEY_LONGPRESS)
    {
        ECPLAT_PRINTF(UNILOG_PMU, powerKeyStatusUpdate_2, P_WARNING, "PowerKey Start Power Off");
        uniLogFlushOut();
        pwrKeyStartPowerOff();
    }
}


void powerKeyFuncInit(void)
{
    if(BSP_GetPlatConfigItemValue(PLAT_CONFIG_ITEM_PWRKEY_MODE) == 2)       // power key always connect to GND
    {
        pwrKeyDeinit(false);
    }
    else
    {
        pwrKeyDly_t pwrKeyDlyCfg;
        pwrKeyDlyCfg.pwrOffTimeout = 2500;
        pwrKeyDlyCfg.longPressTimeout = 2000;
        pwrKeyDlyCfg.repeatTimeout = 500;
        pwrKeyInit(PWRKEY_WAKEUP_LOWACTIVE_MODE, true, pwrKeyDlyCfg, powerKeyStatusUpdate);
    }
}


uint8_t usbstack_ctx_stat_issucc(void);

void BSP_UsbDeInit(void)
{
    uint32_t ret ;
    usbd_set_mod_last_err(0, 0);

    ret = usbstack_deinit();
    EC_ASSERT((ret==0), usbd_get_mod_last_err(), 0, 0);
}

uint32_t BSP_UsbGetVBUSMode(void)
{
    if(BSP_GetPlatConfigItemValue(PLAT_CONFIG_ITEM_USB_VBUS_MODE_EN) == 1)
    {
        return 1;
    }
    return 0;
}

uint32_t BSP_UsbGetVBUSWkupPad(void)
{
    return BSP_GetPlatConfigItemValue(PLAT_CONFIG_ITEM_USB_VBUS_WKUP_PAD);
}


uint8_t usblpw_get_retothwk_proc_stat(void);

uint8_t usblpw_rd_reg_retwkup_ctxstat(void);

void BSP_UsbInit(void)
{
#if (RTE_USB_EN == 1)
    uint32_t xTaskGetTickCount( void );
#if ((USBC_CTRL_HIB_LITE_MODE==0) )
    uint32_t save_irqflag;
#endif

    uint8_t usbinit_mode = 0;
    uint8_t usb_pwr_state = 0;
    uint8_t usb_retwkup_bootstat = 0;
    uint8_t usb_lastwkup_type = 0;
    uint32_t startTick, endTick;
    uint8_t ret;

    startTick = xTaskGetTickCount();
    ret = 1;
    usbc_trace_cfg(BSP_GetPlatConfigItemValue(PLAT_CONFIG_ITEM_USB_SW_TRACE_FLAG));
    usblpw_clear_retothwk_proc_stat();
#if (USBC_CTRL_HIB_LITE_MODE==0)
    usb_wkmon_cmmon_clear();
#endif
    if(BSP_GetPlatConfigItemValue(PLAT_CONFIG_ITEM_USB_CTRL) != 2)
    {
        ECOMM_TRACE(UNILOG_PMU, usblpw_wkup_or_init_pre_0, P_WARNING, 1, "ctx stat reg %d", usblpw_rd_reg_retwkup_ctxstat());

        if (usblpw_is_retwkup_support())
        {
            usblpw_get_pwr_info(&usb_pwr_state, &usb_retwkup_bootstat, &usb_lastwkup_type);
            ECPLAT_PRINTF(UNILOG_PMU, usblpw_wkup_or_init_0, P_VALUE, "pwr state 0x%x, bootstat 0x%x, lastwkup 0x%x, othwk stg 0x%x ", \
                    (uint32_t)usb_pwr_state, (uint32_t)usb_retwkup_bootstat, (uint32_t)usb_lastwkup_type, usblpw_retothwk_get_cur_stg() );

            if(usblpw_is_retwkups2_start())
            {
                //while(*(volatile uint32_t*)0x4d0201a0);
                slpManDrvVoteSleep(SLP_VOTE_LPUSB, SLP_IDLE_STATE);

                ret = usblpw_retwkup_stack_restore();
                usbinit_mode  = 0x11;
                if (ret==0)
                {
                    usblpw_retwkup_stack_enable();
                }

                EC_ASSERT((ret==0), 0, 0, 0);
            }
            else if (usblpw_is_retothwks2_start())
            {
#if (USBC_CTRL_HIB_LITE_MODE==1)
                usblpw_clear_lpusbwkup_src();
                usbc_call_usbmst_slp1_retothwk_earlyinit();

                ret = usblpw_retothwk_hibslp2_stack_restore(usblpw_retothwk_stg_bt_later_idle);

                //anyway let allow sleep
                slpManDrvVoteSleep(SLP_VOTE_LPUSB, SLP_SLP1_STATE);
                usbinit_mode  = 0x14;

                EC_ASSERT((ret==0), usbd_get_mod_last_err(), usblpw_retothwk_get_cur_stg(), usblpw_get_retothwk_proc_stat());

#endif

#if (USBC_CTRL_HIB_LITE_MODE==0)

                ECPLAT_PRINTF(UNILOG_PMU, usblpw_is_retothwks2_start_1, P_VALUE, "cur_stg 0x%x, ", usblpw_retothwk_get_cur_stg());
                //only bt pre idle or bt actv are valid here
                ret = 0;
                save_irqflag = SaveAndSetIRQMask();
                //for now, the LpusbWakeup_IRQn may enabled, so check triggered irq flag
                //usblpw_wr_reg_usb_wkdetflag(1); not called , the LpusbWakeup_IRQn is not enabled  when othwk recovery

                if (NVIC_GetEnableIRQ(LpusbWakeup_IRQn))
                {
                    ECOMM_TRACE(UNILOG_PLA_DRIVER,usblpw_is_retothwks2_start_2, P_WARNING, 0,"Disable LpusbWakeup_IRQn");
                    NVIC_DisableIRQ(LpusbWakeup_IRQn);
                }

                RestoreIRQMask(save_irqflag);

                if (usblpw_retothwk_cur_stg_bt_pre_idle())
                {
                    //no detect when boot pre idle, try bt later idle proc
                    usblpw_retothwk_set_bt_later_idle_stage();
                }

                switch (usblpw_retothwk_get_cur_stg())
                {
                    case usblpw_retothwk_stg_bt_later_idle:
                        ret = usblpw_retothwk_hibslp2_bt_later_idle_stage_proc();
                        if (ret!=0)
                        {
                            //any why assert
                            break;
                        }

                        if (usblpw_retothwk_cur_stg_hibslp2_wkmon())
                        {
                            // do not clear wkup src to avoid irq drop until wkup succ or terminated
                            //lpusbwkup_irq_flag = usblpw_clear_lpusbwkup_src();

                            //only for hibernate/slp2 mode
                            usb_wkmon_hibslp2_stat_evt_start();
                            usbinit_mode  = 0x14;
                            break;
                        }

                        if (usblpw_retothwk_cur_stg_bt_actv())
                        {
                            ret = usblpw_retothwk_hibslp2_bt_actv_stage_proc();
                            if (ret!=0)
                            {
                                //any why assert
                                break;
                            }
                        }

                        if (usblpw_retothwk_cur_stg_success()==0)
                        {
                            ret = 1;
                            break;
                        }

                        usbinit_mode  = 0x12;
                        break;

                    case usblpw_retothwk_stg_bt_actv:
                        ret = usblpw_retothwk_hibslp2_bt_actv_stage_proc();
                        if (ret!=0)
                        {
                            //any why assert
                            break;
                        }
                        if (usblpw_retothwk_cur_stg_success()==0)
                        {
                            ret = 1;
                            break;
                        }
                        usbinit_mode  = 0x13;
                        break;

                    default:
                        ret = 1;
                        break;
                }
                EC_ASSERT((ret==0), usbd_get_mod_last_err(), usblpw_retothwk_get_cur_stg(), usblpw_get_retothwk_proc_stat());
#endif

            }

        }
        if(usbinit_mode ==0)
        {
            ret = usbstack_init();
            usbinit_mode  = 0x10;
            EC_ASSERT((ret==0), usbd_get_mod_last_err(), 0, 0);
        }
    }

    if (usblpw_is_retwkup_support())
    {
        void usb_lpwkup_step_show();
        uint8_t usbstack_get_ctx_vbus_mode(void);

        usb_lpwkup_step_show();
        ECPLAT_PRINTF(UNILOG_PMU, usblpw_wkup_or_init_1, P_VALUE, "pwr state 0x%x, bootstat 0x%x, lastwkup 0x%x, inimod 0x%x ", \
                (uint32_t)usb_pwr_state, (uint32_t)usb_retwkup_bootstat, (uint32_t)usb_lastwkup_type, (uint32_t)usbinit_mode );
        if (usbstack_get_ctx_vbus_mode()==0)
        {
            ECPLAT_PRINTF(UNILOG_PMU, usblpw_wkup_or_init_2, P_VALUE, "othwk stg 0x%x, proc_stat 0x%x", usblpw_retothwk_get_cur_stg(), usblpw_get_retothwk_proc_stat() );
        }

    }
    endTick = xTaskGetTickCount();

    //check cost time only when wkup boot
    if(usbinit_mode==1)
    {
        EC_ASSERT((endTick-startTick) < 30, startTick, endTick, 0);
    }
#endif

}


#ifdef FEATURE_MEIG_AT_ENABLE
extern BOOL GetUsrdefSlpStatus(void);
#endif
slpManSlpState_t CheckUsrdefSlpStatus(void)
{
    slpManSlpState_t state = SLP_HIB_STATE;

    if(BSP_GetPlatConfigItemValue(PLAT_CONFIG_ITEM_ECSCLK_CFG) == 1)
    {
//<!--[ODM][TaskID29162]fw@2022-09-14 add WAKEUP IN PAD MACROS
//<--[ODM][WAKEUP][TaskID 24755]wbh@2022-08-19 Configure rising edge wakeup and faling edge sleep.
#ifdef  FEATURE_MEIG_AT_ENABLE
        if(GetUsrdefSlpStatus() == TRUE)     // pad0 pin level is high
#else
        if((slpManGetWakeupPinValue() & (0x1<<WAKEUP_IN_PAD_INDEX)) == (0x1<<WAKEUP_IN_PAD_INDEX))     // pad0 pin level is high
#endif
//end-->
        {
            state = SLP_IDLE_STATE;
        }
        else
        {

            state = SLP_HIB_STATE;
        }
    }
	/*
	 if do not sleep when ecsclk = 0
    else
    {
        state = SLP_IDLE_STATE;
    }
	*/
    return state;
}

#ifdef CONFIG_MG_CUSTOMER_XRS
extern BOOL XRSPirPowerInit(void);
#endif
/*
 *  custom board related init
 *  Parameter:   none
 *  note: this function shall be called in OS task context for dependency of reading configure file
 *        which is implemented based on file system
 */
void BSP_CustomInit(void)
{
    slpManWakeSrc_e wakeupSrc;
    uint8_t DevVer = 0;

    extern void mpu_init(void);
    mpu_init();

    GPR_SetUartClk();

    BSP_LoadPlatConfigFromRawFlash();

    if(BSP_GetPlatConfigItemValue(PLAT_CONFIG_ITEM_LOG_PORT_SEL) == PLAT_CFG_ULG_PORT_USB)
        uniLogSetPherType(USB_FOR_UNILOG);
    else
        uniLogSetPherType(UART_0_FOR_UNILOG);

    if(BSP_GetPlatConfigItemValue(PLAT_CONFIG_ITEM_FAULT_ACTION) == EXCEP_OPTION_SILENT_RESET)
        ResetLockupCfg(true, true);
    else
        ResetLockupCfg(false, false);

//<!--[ODM][openCPU][TaskID 26229]wbh@2022-10-25 Set uart fucnction in the application in opencpu version.
#ifdef CONFIG_UART_AT_SUPPORT
#if (RTE_ONE_UART_AT == 1)
    SetAtUart(PORT_USART_1);
#if (RTE_TWO_UART_AT == 1)
    SetAtUart(PORT_USART_2);
#endif
#endif
#endif
//end-->

    if((BSP_GetPlatConfigItemValue(PLAT_CONFIG_ITEM_LOG_CONTROL) != 0) &&
        ((BSP_GetPlatConfigItemValue(PLAT_CONFIG_ITEM_LOG_PORT_SEL) == PLAT_CFG_ULG_PORT_UART) || (BSP_GetPlatConfigItemValue(PLAT_CONFIG_ITEM_LOG_PORT_SEL) == PLAT_CFG_ULG_PORT_MIX)))
    {
        SetUnilogUart(PORT_USART_0, BSP_GetPlatConfigItemValue(PLAT_CONFIG_ITEM_LOG_BAUDRATE), true);
        uniLogInitStart(UART_0_FOR_UNILOG);
    }

    ECPLAT_PRINTF(UNILOG_PLA_STRING, build_info, P_SIG, "%s", getBuildInfo());

    if(apmuGetAPLLBootFlag()==AP_BOOT_FROM_POWER_ON)
    {
        extern void EFuseGetDevVer(uint8_t *DevVer);
        EFuseGetDevVer(&DevVer);
        ECPLAT_PRINTF(UNILOG_PLA_STRING, build_info_1, P_SIG, "fuse dev 0x%x", DevVer);
    }

    ApmuWakeupProc(apmuGetAPBootFlag(), 1);

    powerKeyFuncInit();
    if (BSP_UsbGetVBUSMode()==0)
    {
        if(apmuGetAPLLBootFlag()==AP_BOOT_FROM_POWER_ON)
        {
            usbstack_top_var_clear();
        }
    }
    usbstack_clear_ctx_stat();
    if (BSP_UsbGetVBUSMode()==0)
    {
#if (USB_WKMON_TASK_EXIST==1)
        usb_wkmon_pre_init();
        usb_wkmon_task_init();
#endif
        usbstack_set_ctx_vbus_mode(0,0xf);
        BSP_UsbInit();
    }
    wakeupSrc = slpManGetWakeupSrc();

    if((wakeupSrc == WAKEUP_FROM_PAD) || (wakeupSrc == WAKEUP_FROM_LPUART))
    {
        slpManStartWaitATTimer();
    }

    ResetReasonInit();

    apmuGetPMUSettings(AP_STATE_IDLE);      /* should be called after log ok, and as early as possible.
                                               before calling slpManSetPmuSleepMode */

    WakeupPadInit();

    alarmFuncInit();

#if (WDT_FEATURE_ENABLE == 1)
    if(BSP_GetPlatConfigItemValue(PLAT_CONFIG_ITEM_START_WDT))
    {
        BSP_WdtInit();
        WDT_start();
    }
#endif
    slpManRegisterUsrSlpDepthCb(CheckUsrdefSlpStatus);

//<!--[ODM][ODM][BugID 45018]hs@2023-05-19 Set uart fucnction
#ifdef CONFIG_MG_CUSTOMER_XRS
    XRSPirPowerInit();
#else
//Turn on SYS_STATUS_LED after system startup.
#ifdef CONFIG_DRIVER_NET_LED_ENABLE
    drvSysStatusLedOn();
#endif
#endif
//end-->
}


uint32_t getAPReservedMem(uint32_t *pBase)
{
    *pBase = (uint32_t)&rsvUsrRam[0];
    return sizeof(rsvUsrRam);
}


/**
  \fn        void NVIC_WakeupIntHandler(void)
  \brief      NVIC wakeup interrupt handler
  \param     void
 */


void Pad0_WakeupIntHandler(void)
{
    if(slpManExtIntPreProcess(PadWakeup0_IRQn)==false)
        return;

    ECPLAT_PRINTF(UNILOG_PMU, Pad0_WakeupIntHandler_1, P_VALUE, "Pad0 Wakeup");
    uniLogFlushOut();
    // add custom code below //

//<!--[ODM][TaskID29162]fw@2022-12_12 add wake up process
#ifdef FEATURE_OPENCPU_ENABLE
    wakeup_in_process(0);
#endif
//end-->
}

pfn_PadWakeupHook p_funcPadWakeupHook = NULL;
int RegPadWakeupIntrHook(pfn_PadWakeupHook pfunc)
{
    if (p_funcPadWakeupHook)
    {
        return -1;
    }
    p_funcPadWakeupHook = pfunc;
    return 0;
}
void Pad1_WakeupIntHandler(void)
{
    if(slpManExtIntPreProcess(PadWakeup1_IRQn)==false)
        return;
    ECPLAT_PRINTF(UNILOG_PMU, Pad1_WakeupIntHandler_1, P_VALUE, "Pad1 Wakeup");
    uniLogFlushOut();
    if (p_funcPadWakeupHook)
    {
        (*p_funcPadWakeupHook)(1);
    }
    // add custom code below //
#if (CONFIG_MG_WAKEUP_IN_PAD_INDEX == 1)
    srvWakeupOut(slpManGetWakeupPinValue() & (0x1 << 1));
#endif

//<!--[ODM][TaskID29162]fw@2022-12_12 add wake up process
#ifdef FEATURE_OPENCPU_ENABLE
    wakeup_in_process(1);
#endif
//end-->

}

//<!--[ODM][BugID 29948]wbh@2022-09-22 Add SIM detect function.
//<!--[ODM][SIM][BugID 38203]wbh@2022-11-08 Fix the issue where SIM detection caused the system to restart.
//<!--[ODM][BugID 38202]wbh@2022-11-11 Set SIM hot swap notify in thread instead of in interrupt handler.
#include "ps_lib_api.h"
#include "semphr.h"

extern osSemaphoreId_t simDetectSem;

/* Hight level indicates plug out and low level indicates plug in */


BOOL BSP_SimGetSimStatus(void)
{
    BOOL bSimPlugIn = false;
    BOOL level = false;

    level = slpManGetWakeupPinValue() & (0x1 << 2);

    MGNvmConfig mg_nvmConfig = {0};
    MG_NV_GetMgSysParam(&mg_nvmConfig);
    if(mg_nvmConfig.bSimVoltTrigMode == false)
        bSimPlugIn = (mg_nvmConfig.bSimVoltTrigMode == level) ? true : false;
    else
        bSimPlugIn = ((mg_nvmConfig.bSimVoltTrigMode << 2) == level) ? true : false;

    return bSimPlugIn;
}

void Pad2_WakeupIntHandler(void)
{
    static uint8_t status = 0xFF;

    if(slpManExtIntPreProcess(PadWakeup2_IRQn)==false)
        return;
    ECPLAT_PRINTF(UNILOG_PMU, Pad2_WakeupIntHandler_1, P_VALUE, "SIM Detect interrupt occurred");
    uniLogFlushOut();

    // add custom code below //
    BOOL bSimPlugIn = BSP_SimGetSimStatus();

    // Process only when sim detect gpio level changed.
    if (status != bSimPlugIn)
    {
        status  = bSimPlugIn;
#ifdef CONFIG_MG_SIM_HOTPLUG_SUPPORT
        {
//<!--[ODM][TaskID 28909]liulei@2022-11-25 add AT+MGSYS
            extern int MEIG_Util_SimHotplugIsEnable(void);
            if (MEIG_Util_SimHotplugIsEnable() && simDetectSem)
            {
                osSemaphoreRelease(simDetectSem);
            }
        }
//end-->
#endif
    }

//<!--[ODM][TaskID29162]fw@2022-12_12 add wake up process
#ifdef FEATURE_OPENCPU_ENABLE
    wakeup_in_process(2);
#endif
//end-->

}
//end-->
//end-->
//end-->

void Pad3_WakeupIntHandler(void)
{
    if(slpManExtIntPreProcess(PadWakeup3_IRQn)==false)
        return;

    // add custom code below //
    ECPLAT_PRINTF(UNILOG_PMU, Pad3_WakeupIntHandler_1, P_VALUE, "Pad3 Wakeup");
    uniLogFlushOut();

#if (CONFIG_MG_WAKEUP_IN_PAD_INDEX == 3)
#ifdef CONFIG_MG_CUSTOMER_XRS
    xrs_srvWakeupOut(1);
#else
    srvWakeupOut(slpManGetWakeupPinValue() & (0x1 << 3));
#endif
#endif

//<!--[ODM][TaskID29162]fw@2022-12_12 add wake up process
#ifdef FEATURE_OPENCPU_ENABLE
    wakeup_in_process(3);
#endif
//end-->
}

void Pad4_WakeupIntHandler(void)
{
    if(slpManExtIntPreProcess(PadWakeup4_IRQn)==false)
        return;

    // add custom code below //

    ECPLAT_PRINTF(UNILOG_PMU, Pad4_WakeupIntHandler_1, P_VALUE, "Pad4 Wakeup");
    uniLogFlushOut();

//<!--[ODM][TaskID29162]fw@2022-12_12 add wake up process
#ifdef FEATURE_OPENCPU_ENABLE
    wakeup_in_process(4);
#endif
//end-->
}

void Pad5_WakeupIntHandler(void)
{
    if(slpManExtIntPreProcess(PadWakeup5_IRQn)==false)
        return;


    // add custom code below //
    ECPLAT_PRINTF(UNILOG_PMU, Pad5_WakeupIntHandler_1, P_VALUE, "Pad5 Wakeup");
    uniLogFlushOut();

    if (p_funcPadWakeupHook)
    {
        (*p_funcPadWakeupHook)(5);
    }
//<!--[ODM][TaskID29162]fw@2022-12_12 add wake up process
#ifdef FEATURE_OPENCPU_ENABLE
    wakeup_in_process(5);
#endif
//end-->
}

void PwrKey_WakeupIntHandler(void)
{

    if(slpManExtIntPreProcess(PwrkeyWakeup_IRQn)==false)
        return;

    // add custom code below //
    ECPLAT_PRINTF(UNILOG_PMU, PwrKey_WakeupIntHandler_1, P_VALUE, "Pwr key Wakeup");
    uniLogFlushOut();

    pwrKeyIntHandler();


}

void ChrgPad_WakeupIntHandler(void)
{
    if(slpManExtIntPreProcess(ChrgpadWakeup_IRQn)==false)
        return;

    // add custom code below //
    ECPLAT_PRINTF(UNILOG_PMU, ChrgPad_WakeupIntHandler_1, P_VALUE, "charge Wakeup");
    uniLogFlushOut();

    chargeIntHandler();

}

/******************************************************************************
 * Usim1GpioConfig
 * Description: Configure the USIM1 PIN MUX. Called by uiccdrv task if the USIM1 enabled.
 * Input:BOOL bSetUsimFunc, whether set USIM function or not
 * Output: NULL
 * Comment: Customer can modify the actual PINs for USIM1 based on pin mux table. Here shows two options.
 * Warning: The LDOIO shall be limit at 1.8v or 3v and cannot be changed if the USIM1 enabled.
 *       Or else the voltage changed may damage the USIM card.
*******************************************************************************/
void Usim1GpioConfig(BOOL bSetUsimFunc)
{
    PadConfig_t padConfig;
    GpioPinConfig_t config;

    /*
    * Configure PIN MUX for USIM1 or GPIO
    */
#if 1 //opthion 1---GPIO 4/5/6
    if (bSetUsimFunc == TRUE)//USIM func
    {
        // GPIO4--USIM1_URSTn
        PAD_getDefaultConfig(&padConfig);
        padConfig.mux = PAD_MUX_ALT4;
        PAD_setPinConfig(USIM1_URST_OP1_PAD_INDEX, &padConfig);

        // GPIO5--USIM1_UCLK
        PAD_getDefaultConfig(&padConfig);
        padConfig.mux = PAD_MUX_ALT4;
        PAD_setPinConfig(USIM1_UCLK_OP1_PAD_INDEX, &padConfig);

        // GPIO6--USIM1_UIO
        PAD_getDefaultConfig(&padConfig);
        padConfig.mux = PAD_MUX_ALT4;
        PAD_setPinConfig(USIM1_UIO_OP1_PAD_INDEX, &padConfig);
    }
    else// GPIO func set output L as default
    {
        config.pinDirection = GPIO_DIRECTION_OUTPUT;
        config.misc.initOutput = 0;
        PAD_getDefaultConfig(&padConfig);
        padConfig.mux = PAD_MUX_ALT0;

        // GPIO4--USIM1_URSTn
        GPIO_pinConfig(USIM1_URST_OP1_GPIO_INSTANCE, USIM1_URST_OP1_GPIO_PIN, &config);   // GPIO 4
        PAD_setPinConfig(USIM1_URST_OP1_PAD_INDEX, &padConfig);

        // GPIO5--USIM1_UCLK
        GPIO_pinConfig(USIM1_UCLK_OP1_GPIO_INSTANCE, USIM1_UCLK_OP1_GPIO_PIN, &config);   // GPIO 5
        PAD_setPinConfig(USIM1_UCLK_OP1_PAD_INDEX, &padConfig);

        // GPIO6--USIM1_UIO
        GPIO_pinConfig(USIM1_UIO_OP1_GPIO_INSTANCE, USIM1_UIO_OP1_GPIO_PIN, &config);   // GPIO 6
        PAD_setPinConfig(USIM1_UIO_OP1_PAD_INDEX, &padConfig);
    }
#else //option2---GPIO 12/13/14
    if (bSetUsimFunc == TRUE)//USIM func
    {
        // GPIO12--USIM1_UIO
        PAD_getDefaultConfig(&padConfig);
        padConfig.mux = PAD_MUX_ALT4;
        PAD_setPinConfig(USIM1_UIO_OP2_PAD_INDEX, &padConfig);

        // GPIO13--USIM1_URSTn
        PAD_getDefaultConfig(&padConfig);
        padConfig.mux = PAD_MUX_ALT4;
        PAD_setPinConfig(USIM1_URST_OP2_PAD_INDEX, &padConfig);

        // GPIO14--USIM1_UCLK
        PAD_getDefaultConfig(&padConfig);
        padConfig.mux = PAD_MUX_ALT4;
        PAD_setPinConfig(USIM1_UCLK_OP2_PAD_INDEX, &padConfig);
    }
    else// GPIO func
    {
        config.pinDirection = GPIO_DIRECTION_OUTPUT;
        config.misc.initOutput = 0;
        PAD_getDefaultConfig(&padConfig);
        padConfig.mux = PAD_MUX_ALT0;

        // GPIO12--USIM1_UIO
        GPIO_pinConfig(USIM1_UIO_OP2_GPIO_INSTANCE, USIM1_UIO_OP2_GPIO_PIN, &config);   // GPIO 12
        PAD_setPinConfig(USIM1_UIO_OP2_PAD_INDEX, &padConfig);

        // GPIO13--USIM1_URSTn
        GPIO_pinConfig(USIM1_URST_OP2_GPIO_INSTANCE, USIM1_URST_OP2_GPIO_PIN, &config);   // GPIO 13
        PAD_setPinConfig(USIM1_URST_OP2_PAD_INDEX, &padConfig);

        // GPIO14--USIM1_UCLK
        GPIO_pinConfig(USIM1_UCLK_OP2_GPIO_INSTANCE, USIM1_UCLK_OP2_GPIO_PIN, &config);   // GPIO 14
        PAD_setPinConfig(USIM1_UCLK_OP2_PAD_INDEX, &padConfig);
    }
#endif
}

/******************************************************************************
 * Usim1ClkLatchByAONIO
 * Description: Configure the AON IO used for USIM1 clock latch adn config USIM1 PIN as GPIO output to reduce some pulses.
 *         Called by uiccdrv task if the USIM1 enabled.
 * Input:BOOL bLatchOnHigh, whether latched on high state or not
 * Output: NULL
 * Comment: Customer can modify the actual PINs for USIM1 based on pin mux table. Here shows AONIO-26.
* Warning: The LDOIO shall be limit at 1.8v or 3v and cannot be changed if the USIM1 enabled.
*       Or else the voltage changed may damage the USIM card.
*******************************************************************************/
void Usim1ClkLatchByAONIO(BOOL bLatchOnHigh)
{
//<!--[ODM][SLM325Y_DS][TaskID 35507]liulei@2023-4-26 Supports dual-card configuration.
    PadConfig_t padConfig;
    GpioPinConfig_t config;

    slpManAONIOPowerOn();
    PAD_getDefaultConfig(&padConfig);

#if defined(CONFIG_DUAL_SIM_SUPPORT)
    /*
    * configure AONIO-25
    */
    /* set pull-up */
    padConfig.pullSelect = 1;
    padConfig.pullUpEnable = 1;

    padConfig.mux = PAD_MUX_ALT0;
    PAD_setPinConfig(UART1_DCD_PAD_INDEX, &padConfig);//gpio25
    config.pinDirection = GPIO_DIRECTION_OUTPUT;
    if (bLatchOnHigh == TRUE)
    {
        config.misc.initOutput = 1;
    }
    else
    {
        config.misc.initOutput = 0;
    }
    GPIO_pinConfig(UART1_DCD_GPIO_INSTANCE, UART1_DCD_GPIO_PIN, &config);//26-16
#else 
    /*
    * configure AONIO-26
    */
    padConfig.mux = PAD_MUX_ALT0;
    PAD_setPinConfig(AONIO_6_PAD_INDEX, &padConfig);//gpio26
    config.pinDirection = GPIO_DIRECTION_OUTPUT;
    if (bLatchOnHigh == TRUE)
    {
        config.misc.initOutput = 1;
    }
    else
    {
        config.misc.initOutput = 0;
    }
    GPIO_pinConfig(AONIO_6_GPIO_INSTANCE, AONIO_6_GPIO_PIN, &config);//26-16
#endif
//end-->
    slpManUsim1LatchEn(AonIOLatch_Enable);
    /*
    * set RST-H, CLK-H/L (bLatchOnHigh), IO-H
    */
#if 1 //opthion 1---GPIO 4/5/6
    // GPIO4--USIM1_URSTn
    PAD_getDefaultConfig(&padConfig);
    config.pinDirection = GPIO_DIRECTION_OUTPUT;
    config.misc.initOutput = 1;
    GPIO_pinConfig(USIM1_URST_OP1_GPIO_INSTANCE, USIM1_URST_OP1_GPIO_PIN, &config);   // GPIO 4
    padConfig.mux = PAD_MUX_ALT0;
    PAD_setPinConfig(USIM1_URST_OP1_PAD_INDEX, &padConfig);
    config.pinDirection = GPIO_DIRECTION_INPUT;
    config.misc.interruptConfig = GPIO_INTERRUPT_DISABLED;
    GPIO_pinConfig(USIM1_URST_OP1_GPIO_INSTANCE, USIM1_URST_OP1_GPIO_PIN, &config);   // GPIO 4

    // GPIO5--USIM1_UCLK
    config.pinDirection = GPIO_DIRECTION_OUTPUT;
    if (bLatchOnHigh == TRUE)
    {
        config.misc.initOutput = 1;
    }
    else
    {
        config.misc.initOutput = 0;
    }
    GPIO_pinConfig(USIM1_UCLK_OP1_GPIO_INSTANCE, USIM1_UCLK_OP1_GPIO_PIN, &config);   // GPIO 5
    padConfig.mux = PAD_MUX_ALT0;
    PAD_setPinConfig(USIM1_UCLK_OP1_PAD_INDEX, &padConfig);
    config.pinDirection = GPIO_DIRECTION_INPUT;
    config.misc.interruptConfig = GPIO_INTERRUPT_DISABLED;
    GPIO_pinConfig(USIM1_UCLK_OP1_GPIO_INSTANCE, USIM1_UCLK_OP1_GPIO_PIN, &config);   // GPIO 5

    // GPIO6--USIM1_UIO
    PAD_getDefaultConfig(&padConfig);
    config.pinDirection = GPIO_DIRECTION_OUTPUT;
    config.misc.initOutput = 1;
    GPIO_pinConfig(USIM1_UIO_OP1_GPIO_INSTANCE, USIM1_UIO_OP1_GPIO_PIN, &config);   // GPIO 6
    padConfig.mux = PAD_MUX_ALT0;
    PAD_setPinConfig(USIM1_UIO_OP1_PAD_INDEX, &padConfig);
    config.pinDirection = GPIO_DIRECTION_INPUT;
    config.misc.interruptConfig = GPIO_INTERRUPT_DISABLED;
    GPIO_pinConfig(USIM1_UIO_OP1_GPIO_INSTANCE, USIM1_UIO_OP1_GPIO_PIN, &config);   // GPIO 6
#else//option2---GPIO 12/13/14
    // GPIO13--USIM1_URSTn
    PAD_getDefaultConfig(&padConfig);
    config.pinDirection = GPIO_DIRECTION_OUTPUT;
    config.misc.initOutput = 1;
    GPIO_pinConfig(USIM1_URST_OP2_GPIO_INSTANCE, USIM1_URST_OP2_GPIO_PIN, &config);   // GPIO 13
    padConfig.mux = PAD_MUX_ALT0;
    PAD_setPinConfig(USIM1_URST_OP2_PAD_INDEX, &padConfig);
    config.pinDirection = GPIO_DIRECTION_INPUT;
    config.misc.interruptConfig = GPIO_INTERRUPT_DISABLED;
    GPIO_pinConfig(USIM1_URST_OP2_GPIO_INSTANCE, USIM1_URST_OP2_GPIO_PIN, &config);   // GPIO 13

    // GPIO14--USIM1_UCLK
    config.pinDirection = GPIO_DIRECTION_OUTPUT;
    if (bLatchOnHigh == TRUE)
    {
        config.misc.initOutput = 1;
    }
    else
    {
        config.misc.initOutput = 0;
    }
    GPIO_pinConfig(USIM1_UCLK_OP2_GPIO_INSTANCE, USIM1_UCLK_OP2_GPIO_PIN, &config);   // GPIO 14
    padConfig.mux = PAD_MUX_ALT0;
    PAD_setPinConfig(USIM1_UCLK_OP2_PAD_INDEX, &padConfig);
    config.pinDirection = GPIO_DIRECTION_INPUT;
    config.misc.interruptConfig = GPIO_INTERRUPT_DISABLED;
    GPIO_pinConfig(USIM1_UCLK_OP2_GPIO_INSTANCE, USIM1_UCLK_OP2_GPIO_PIN, &config);   // GPIO 14

    // GPIO12--USIM1_UIO
    PAD_getDefaultConfig(&padConfig);
    config.pinDirection = GPIO_DIRECTION_OUTPUT;
    config.misc.initOutput = 1;
    GPIO_pinConfig(USIM1_UIO_OP2_GPIO_INSTANCE, USIM1_UIO_OP2_GPIO_PIN, &config);   // GPIO 12
    padConfig.mux = PAD_MUX_ALT0;
    PAD_setPinConfig(USIM1_UIO_OP2_PAD_INDEX, &padConfig);
    config.pinDirection = GPIO_DIRECTION_INPUT;
    config.misc.interruptConfig = GPIO_INTERRUPT_DISABLED;
    GPIO_pinConfig(USIM1_UIO_OP2_GPIO_INSTANCE, USIM1_UIO_OP2_GPIO_PIN, &config);   // GPIO 12
#endif
}

void srvWakeupInit(void)
{
#if defined(CONFIG_SRV_WAKEUP_OUT_SUPPORT)
    PadConfig_t padConfig;

    *(uint32_t *)0x4D020170 = 0x01;
    delay_us(1000);

    //If we want to use the GPIO function of the WAKEUP pin, we need to disable the WAKEUP function.
#if (CONFIG_SRV_WAKEUP_OUT_PAD_INDEX == 42)
    apmuSetWakeupPadCfg(WAKEUP_PAD_5, false, NULL);
    NVIC_DisableIRQ(PadWakeup5_IRQn);
#endif

    //SIM detect
    //apmuSetWakeupPadCfg(WAKEUP_PAD_2, false, NULL);
    //NVIC_DisableIRQ(PadWakeup2_IRQn);


    /* GPIO function select */
    PAD_getDefaultConfig(&padConfig);
    //<!--[ODM][SLM332Y][BUGID 39826]liulei@2023-2-6 Do the output function,turn off the internal pull-down.
    padConfig.pullSelect = PAD_PULL_INTERNAL;
    padConfig.pullUpEnable = PAD_PULL_UP_DISABLE;
    padConfig.pullDownEnable = PAD_PULL_DOWN_DISABLE;

    /* Set pull-up */
    //padConfig.pullSelect = 1;
    //padConfig.pullUpEnable = 1;

    PAD_setPinConfig(CONFIG_SRV_WAKEUP_OUT_PAD_INDEX, &padConfig);
    //PAD_setPinPullConfig(CONFIG_SRV_WAKEUP_OUT_PAD_INDEX, PAD_INTERNAL_PULL_UP);
    //end-->

    /* GPIO pin configure */
    GpioPinConfig_t PinConfig;
    PinConfig.pinDirection = GPIO_DIRECTION_OUTPUT;
    PinConfig.misc.initOutput = true;

    GPIO_pinConfig(CONFIG_SRV_WAKEUP_OUT_GPIO / 16, CONFIG_SRV_WAKEUP_OUT_GPIO % 16, &PinConfig);

//<!--[ODM][ANT][Task33295]wj@2023-04-03 ant pin init
#ifdef CONFIG_MG_CUSTOMER_YS
    extern UINT8 gps_ant_pin_init(char *pin_name, INT32 level);
    // gps ant pin init
    gps_ant_pin_init("P63", 1);
#endif
//end-->
#endif
}

bool srvWakeupOut(bool level)
{
#if defined(CONFIG_SRV_WAKEUP_OUT_SUPPORT)
    uint32_t actual_level = 0;

    if (level)
        GPIO_pinWrite(CONFIG_SRV_WAKEUP_OUT_GPIO / 16,
                        1 << (CONFIG_SRV_WAKEUP_OUT_GPIO % 16),
                        1 << (CONFIG_SRV_WAKEUP_OUT_GPIO % 16));
    else
        GPIO_pinWrite(CONFIG_SRV_WAKEUP_OUT_GPIO / 16,
                        1 << (CONFIG_SRV_WAKEUP_OUT_GPIO % 16),
                        0);

    /* Read back to check the actual level */
    actual_level = GPIO_pinRead(CONFIG_SRV_WAKEUP_OUT_GPIO / 16, (CONFIG_SRV_WAKEUP_OUT_GPIO % 16));

    ECPLAT_PRINTF(UNILOG_PLA_DRIVER, srvWakeupOut_1, P_SIG, "wake up out %d/%d", level, actual_level);

    return (actual_level == level ? 1 : 0);
#else
    return false;
#endif
}
/******************* (C) COPYRIGHT 2022 MEIG SMART*****END OF FILE*************/
