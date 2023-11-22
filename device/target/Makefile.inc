ifeq ($(TOOLCHAIN),GCC)
PRECINIT_FILE_PATH ?= device/target/board/$(TARGET)/$(CORE)/gcc
SYSCALLS_FILE_PATH ?= device/target/board/$(TARGET)/$(CORE)/gcc
SYSTEM_FILE_PATH   ?= device/target/board/$(TARGET)/$(CORE)/src
STARTUP_FILE_PATH  ?= device/target/board/$(TARGET)/$(CORE)/gcc
LINK_FILE_PATH     ?= device/target/board/$(TARGET)/$(CORE)/gcc
endif

ifeq ($(TOOLCHAIN),ARMCC)
PRECINIT_FILE_PATH ?= device/target/board/$(TARGET)/$(CORE)/keil
SYSTEM_FILE_PATH   ?= device/target/board/$(TARGET)/$(CORE)/src
STARTUP_FILE_PATH  ?= device/target/board/$(TARGET)/$(CORE)/keil
LINK_FILE_PATH     ?= device/target/board/$(TARGET)/$(CORE)/keil
endif

##$(error "error  $(MAIN_FILE_PATH)")

include $(TOP)/device/target/board/$(TARGET)/$(CORE)/$(TARGET)_$(CORE).mk
include $(TOP)/device/target/board/common/macro.mk

CFLAGS_INC += -I$(TOP)/device/target/board/common/ARMCM3/inc  \
              -I$(TOP)/device/target/board/$(TARGET)/common/inc


ifeq ($(TOOLCHAIN),GCC)
libstartup-y := $(PRECINIT_FILE_PATH)/prec_init.o        \
                $(SYSCALLS_FILE_PATH)/syscalls.o         \
                $(SYSTEM_FILE_PATH)/system_$(CHIP).o       \
                $(STARTUP_FILE_PATH)/startup_$(CHIP)_gcc.o

STARTUP_OBJS := $(addprefix $(BUILDDIR)/,$(libstartup-y))

ifneq ($(MAKECMDGOALS), unilog)
-include $(STARTUP_OBJS:.o=.d)
endif

CFLAGS_INC += -I$(TOP)/device/target/board/$(TARGET)/$(CORE)/gcc \
              -I$(TOP)/device/target/board/$(TARGET)/$(CORE)/inc
lib-y += libstartup.a

ifeq ($(LOW_SPEED_SERVICE_ONLY_ENABLE), y)
linker-script-y := $(TOP)/$(LINK_FILE_PATH)/$(TARGET)_flash_low_spd.ld
else
linker-script-y := $(TOP)/$(LINK_FILE_PATH)/$(TARGET)_flash.ld
endif

$(BUILDDIR)/lib/libstartup.a: $(STARTUP_OBJS)
	@mkdir -p $(dir $@)
	$(ECHO) AR $@
	$(Q)$(AR) -cr $@ $^

endif

ifeq ($(TOOLCHAIN),ARMCC)
libstartup-y += $(PRECINIT_FILE_PATH)/prec_init.o  \
                $(SYSTEM_FILE_PATH)/system_$(CHIP).o \
				$(STARTUP_FILE_PATH)/startup_$(CHIP).o

STARTUP_OBJS := $(addprefix $(BUILDDIR)/,$(libstartup-y))

ifneq ($(MAKECMDGOALS), unilog)
-include $(STARTUP_OBJS:.o=.d)
endif


##obj-y        += $(STARTUP_FILE_PATH)/startup_$(CHIP).o

CFLAGS_INC += -I$(TOP)/device/target/board/$(TARGET)/$(CORE)/keil \
              -I$(TOP)/device/target/board/$(TARGET)/$(CORE)/inc
lib-y += libstartup.lib

ifeq ($(FPGA_DEBUG),y)
linker-script-y := $(TOP)/$(LINK_FILE_PATH)/$(TARGET)_flash_fpga.sct
else
linker-script-y := $(TOP)/$(LINK_FILE_PATH)/$(TARGET)_flash.sct
endif


$(BUILDDIR)/lib/libstartup.lib: $(STARTUP_OBJS)
	@mkdir -p $(dir $@)
	$(ECHO) AR $@
	$(Q)$(AR) $(ARFLAGS) $@ $^

endif


