ifeq ($(TOOLCHAIN),GCC)
ifeq ($(BUILD_USE_PREBUILD_LIB),y)

PREBUILDLIBS_INC  := $(LIBDIR)/PLAT/inc
PREBUILDLIBS_PATH := $(LIBDIR)/PLAT/lib/gcc

CFLAGS_INC += -I $(PREBUILDLIBS_INC)
PREBUILDLIBS += $(PREBUILDLIBS_PATH)/libosa.a \
                $(PREBUILDLIBS_PATH)/libmiddleware_ec_private.a \
                $(PREBUILDLIBS_PATH)/libccio.a \
                $(PREBUILDLIBS_PATH)/libdeltapatch.a \
                $(PREBUILDLIBS_PATH)/libfota.a


PREBUILDLIBS_CORE_INC   := $(PREBUILDLIBS_INC)/core
PREBUILDLIBS_CORE_PATH  := $(PREBUILDLIBS_PATH)/core

# psnv
PSNV_DIR      := $(PREBUILDLIBS_CORE_INC)/developed/psnv
CFLAGS_INC    += -I $(PSNV_DIR)/inc
PREBUILDLIBS  += $(PREBUILDLIBS_CORE_PATH)/libpsnv.a

# tcpipmgr
TCPIPMGRDIR   := $(PREBUILDLIBS_CORE_INC)/developed/tcpipmgr
CFLAGS_INC    += -I $(TCPIPMGRDIR)/app/inc  \
			     -I $(TCPIPMGRDIR)/common/inc
PREBUILDLIBS  += $(PREBUILDLIBS_CORE_PATH)/libtcpipmgr.a

# yrcompress
YRCOMPRESS_DIR := $(PREBUILDLIBS_CORE_INC)/developed/yrcompress
CFLAGS_INC    += -I $(YRCOMPRESS_DIR)
#CFLAGS_INC    += -I $(CJSON_DIR)
PREBUILDLIBS  += $(PREBUILDLIBS_CORE_PATH)/libyrcompress.a

# os
FREERTOS_DIR := $(PREBUILDLIBS_CORE_INC)/freertos
CFLAGS_INC   += -I $(FREERTOS_DIR)/inc          \
                -I $(FREERTOS_DIR)/CMSIS/common/inc   \
                -I $(FREERTOS_DIR)/CMSIS/ap/inc   \
                -I $(FREERTOS_DIR)/portable/mem/tlsf

CFLAGS_INC += -I $(FREERTOS_DIR)/portable/gcc
CFLAGS += -DFEATURE_OS_ENABLE
CFLAGS += -DconfigUSE_NEWLIB_REENTRANT=1
LDFLAGS += -Wl,--wrap=_malloc_r -Wl,--wrap=_free_r -Wl,--wrap=_realloc_r
PREBUILDLIBS  += $(PREBUILDLIBS_CORE_PATH)/libfreertos.a

# driver
DRIVER_DIR := $(PREBUILDLIBS_CORE_INC)/driver
CFLAGS_INC += -I $(DRIVER_DIR)/board/ec618_0h00/inc \
              -I $(DRIVER_DIR)/board/ec618_0h00/inc/eeprom \
              -I $(DRIVER_DIR)/board/ec618_0h00/inc/camera \
              -I $(DRIVER_DIR)/board/ec618_0h00/inc/camera/sp0A39 \
              -I $(DRIVER_DIR)/board/ec618_0h00/inc/camera/sp0821 \
              -I $(DRIVER_DIR)/board/ec618_0h00/inc/camera/gc6123 \
              -I $(DRIVER_DIR)/board/ec618_0h00/inc/camera/gc032A \
              -I  $(DRIVER_DIR)/board/ec618_0h00/inc/audio \
              -I $(DRIVER_DIR)/board/ec618_0h00/inc/lcd \
              -I $(DRIVER_DIR)/board/ec618_0h00/inc/lcd/ST7789V2 \
              -I $(DRIVER_DIR)/board/ec618_0h00/inc/lcd/ST7571 \
              -I $(DRIVER_DIR)/board/ec618_0h00/inc/ntc
CFLAGS_INC += -I $(DRIVER_DIR)/board/ec618_0h00/inc/meig

CFLAGS_INC += -I $(DRIVER_DIR)/chip/ec618/ap/inc \
              -I $(DRIVER_DIR)/chip/ec618/ap/inc_cmsis

CFLAGS_INC += -I $(DRIVER_DIR)/hal/common/inc \
              -I $(DRIVER_DIR)/hal/ec618/ap/inc

PREBUILDLIBS  += $(PREBUILDLIBS_CORE_PATH)/libdriver.a

# middleware
MW_HOST_DIRS := $(PREBUILDLIBS_CORE_INC)/developed
CFLAGS_INC += -I $(MW_HOST_DIRS)/debug/inc \
              -I $(MW_HOST_DIRS)/nvram/inc \
              -I $(MW_HOST_DIRS)/cms/psdial/inc \
              -I $(MW_HOST_DIRS)/cms/cms/inc \
              -I $(MW_HOST_DIRS)/cms/psil/inc \
              -I $(MW_HOST_DIRS)/cms/pslpp/inc \
              -I $(MW_HOST_DIRS)/cms/pslpp/nbasn1/inc \
              -I $(MW_HOST_DIRS)/cms/psstk/inc \
              -I $(MW_HOST_DIRS)/cms/sockmgr/inc \
              -I $(MW_HOST_DIRS)/cms/cmsnetlight/inc \
              -I $(MW_HOST_DIRS)/ecapi/appmwapi/inc \
              -I $(MW_HOST_DIRS)/ecapi/psapi/inc \
              -I $(MW_HOST_DIRS)/simbip/inc        \
              -I $(MW_HOST_DIRS)/common/inc

CFLAGS_INC  += -I $(MW_HOST_DIRS)/fota/pub \
               -I $(MW_HOST_DIRS)/fota/custom/inc

CFLAGS_INC += -I $(MW_HOST_DIRS)/at/atdecoder/inc \
              -I $(MW_HOST_DIRS)/at/atps/inc \
              -I $(MW_HOST_DIRS)/at/atps/inc/cnfind \
              -I $(MW_HOST_DIRS)/at/atcust/inc \
              -I $(MW_HOST_DIRS)/at/atcust/inc/cnfind \
              -I $(MW_HOST_DIRS)/at/atentity/inc \
              -I $(MW_HOST_DIRS)/at/atreply/inc

CFLAGS_INC += -I $(MW_HOST_DIRS)/ccio/pub\
			  -I $(MW_HOST_DIRS)/ccio/device/inc\
			  -I $(MW_HOST_DIRS)/ccio/service/inc\
			  -I $(MW_HOST_DIRS)/ccio/custom/inc

CFLAGS_INC += -I $(MW_HOST_DIRS)/at/atref/inc \
              -I $(MW_HOST_DIRS)/at/atref/inc/cnfind

CFLAGS_INC  += -I $(MW_HOST_DIRS)/at/atmeig/util
CFLAGS_INC += -I $(LIBDIR)/PS/inc
PREBUILDLIBS  += $(PREBUILDLIBS_CORE_PATH)/libmiddleware_ec.a


PREBUILDLIBS_THIRDPARTY_INC  := $(PREBUILDLIBS_INC)/thirdparty
PREBUILDLIBS_THIRDPARTY_PATH := $(PREBUILDLIBS_PATH)/thirdparty
# thirdparty  cjson
CJSON_DIR         := $(PREBUILDLIBS_THIRDPARTY_INC)/CJSON
CFLAGS_INC        += -I $(CJSON_DIR)
PREBUILDLIBS      += $(PREBUILDLIBS_THIRDPARTY_PATH)/libcjson.a

# thirdparty  ctwing
# CTW_DIR         := $(PREBUILDLIBS_THIRDPARTY_INC)/ctwing
# CFLAGS_INC      += -I $(CTW_DIR)
# PREBUILDLIBS    += $(PREBUILDLIBS_THIRDPARTY_PATH)/libctwing.a

# thirdparty  dm
DM_DIR            := $(PREBUILDLIBS_THIRDPARTY_INC)/dm
CFLAGS_INC        +=  -I $(DM_DIR) \
                      -I $(DM_DIR)/include

CFLAGS_INC        +=  -I $(DM_DIR)/cmcc/ciscore \
                      -I $(DM_DIR)/cmcc/ciscore/std_object \
                      -I $(DM_DIR)/cmcc/ciscore/dm_utils \
                      -I $(DM_DIR)/cmcc/ciscore/er-coap-13 \
                      -I $(DM_DIR)/cmcc/adapter/freeRTOS
PREBUILDLIBS      += $(PREBUILDLIBS_THIRDPARTY_PATH)/libdm.a

# thirdparty  httpclient
HTTPCLIENT_DIR    := $(PREBUILDLIBS_THIRDPARTY_INC)/httpclient
CFLAGS_INC        +=  -I $(HTTPCLIENT_DIR)
PREBUILDLIBS      += $(PREBUILDLIBS_THIRDPARTY_PATH)/libhttpclient.a

# thirdparty  iperf
IPERFDIR          := $(PREBUILDLIBS_THIRDPARTY_INC)/iperf
CFLAGS_INC        +=  -I $(IPERFDIR)
PREBUILDLIBS      += $(PREBUILDLIBS_THIRDPARTY_PATH)/libiperf.a

# thirdparty  littlefs
LFS_DIR           := $(PREBUILDLIBS_THIRDPARTY_INC)/littlefs
CFLAGS_INC        +=  -I $(LFS_DIR) \
                      -I $(LFS_DIR)/port
PREBUILDLIBS      += $(PREBUILDLIBS_THIRDPARTY_PATH)/liblfs.a

# thirdparty  lwip
LWIP_HOST_DIRS := $(PREBUILDLIBS_THIRDPARTY_INC)/lwip/src

CFLAGS_INC     += -I $(LWIP_HOST_DIRS)/include      \
                  -I $(LWIP_HOST_DIRS)/include/lwip \
                  -I $(LWIP_HOST_DIRS)/include/posix

LWIPCONFIG_CFLAGS ?= -DLWIP_CONFIG_FILE=\"lwip_config_ec6180h00.h\"
CFLAGS += $(LWIPCONFIG_CFLAGS)

PREBUILDLIBS   += $(PREBUILDLIBS_THIRDPARTY_PATH)/liblwip.a

# thirdparty  mbedtls
MBEDTLS_DIR    := $(PREBUILDLIBS_THIRDPARTY_INC)/mbedtls
CFLAGS_INC     +=  -I $(MBEDTLS_DIR)/include  \
                   -I $(MBEDTLS_DIR)/include/mbedtls  \
                   -I $(MBEDTLS_DIR)/configs  \
                   -I $(MBEDTLS_DIR)/library/ec61x/inc
MBEDTLS_CFLAGS := -DMBEDTLS_CONFIG_FILE=\"config_ec_ssl_comm.h\"
CFLAGS += $(MBEDTLS_CFLAGS)
PREBUILDLIBS  += $(PREBUILDLIBS_THIRDPARTY_PATH)/libmbedtls.a

# thirdparty  mqtt
MQTT_DIR      := $(PREBUILDLIBS_THIRDPARTY_INC)/mqtt
CFLAGS_INC    += -I $(MQTT_DIR)/MQTTPacket/src               \
                 -I $(MQTT_DIR)/MQTTClient-C/src/FreeRTOS    \
                 -I $(MQTT_DIR)/MQTTClient-C/src             \
                 -I $(MQTT_DIR)

CFLAGS_INC    += -I $(MQTT_DIR)/MQTTClient-C/src/eigencomm

#CFLAGS_INC    += -I $(CJSON_DIR)

PREBUILDLIBS  += $(PREBUILDLIBS_THIRDPARTY_PATH)/libmqtt.a

# thirdparty  ping
PINGDIR       := $(PREBUILDLIBS_THIRDPARTY_INC)/ping/src
CFLAGS_INC    += -I $(PINGDIR)
PREBUILDLIBS  += $(PREBUILDLIBS_THIRDPARTY_PATH)/libping.a

# thirdparty  sntp
SNTP_DIR      := $(PREBUILDLIBS_THIRDPARTY_INC)/thirdparty/sntp
CFLAGS_INC    += -I $(SNTP_DIR)
PREBUILDLIBS  += $(PREBUILDLIBS_THIRDPARTY_PATH)/libsntp.a

# opencpu
PREBUILDLIBS_MCI_INC   := $(PREBUILDLIBS_INC)/mci
CFLAGS_INC       += -I $(PREBUILDLIBS_MCI_INC)/unity \
                    -I $(PREBUILDLIBS_MCI_INC)/http  \
                    -I $(PREBUILDLIBS_MCI_INC)/mqtt

PREBUILDLIBS  += $(PREBUILDLIBS_CORE_PATH)/libopencpu.a

ifeq ($(BINNAME),ap_bootloader)
PREBUILDLIBS += $(LIBDIR)/PLAT/lib/gcc/libdriver_private_bl.a
else ifeq ($(BINNAME),ap_driver_example)
PREBUILDLIBS += $(LIBDIR)/PLAT/lib/gcc/libdriver_private_drv.a
else
PREBUILDLIBS += $(LIBDIR)/PLAT/lib/gcc/libdriver_private.a
endif
endif

#qocm also use usb libs and bl/app use different libs
ifeq ($(BUILD_USE_USB_LIB),y)
ifeq ($(BINNAME),ap_bootloader)
PREBUILDLIBS += $(LIBDIR)/PLAT/lib/gcc/libusbbl_priv.a
else
ifeq ($(DRIVER_USB_ENABLE),y)
PREBUILDLIBS += $(LIBDIR)/PLAT/lib/gcc/libusb_private.a
endif
endif
endif

endif