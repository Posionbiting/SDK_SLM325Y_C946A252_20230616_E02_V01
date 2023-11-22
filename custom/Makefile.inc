CUS_INC +=  $(TOP)/custom/bsp/inc
CUS_SRCS += $(wildcard $(TOP)/custom/bsp/src/*.c)

ifeq ($(strip $(C_APP_PROJECT)),CUSTOMER_CODE)
CFLAGS += -D__CUSTOMER_CODE__
CUS_INC += $(TOP)/custom
CUS_SRCS += $(wildcard  $(TOP)/custom/*.c)
endif

# load app
include $(TOP)/custom/app_zj/Makefile.inc
