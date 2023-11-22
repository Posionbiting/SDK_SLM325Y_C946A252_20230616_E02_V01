OPENCPU_AT_DIR=$(TOP)/openAT

ifeq ($(FEATURE_OPEN_AT_ENABLE),y)

CFLAGS_INC                     +=  -I $(OPENCPU_AT_DIR)/include
OPENCPU_AT_TABLE_CFILES        +=  $(OPENCPU_AT_DIR)/mg_openAT_cmd_table.c
OPENCPU_AT_EXCLUDE_FILES       :=

CFLAGS += -DFEATURE_OPENCPU_AT_ENABLE

OPENCPU_AT_CSRC = $(foreach dir, $(OPENCPU_AT_DIR), $(wildcard $(dir)/*.c))
OPENCPU_AT_CFILES = $(filter-out $(OPENCPU_AT_EXCLUDE_FILES), $(OPENCPU_AT_CSRC))
OPENCPU_AT_COBJSTEMP := $(patsubst %.c, %.o, $(OPENCPU_AT_CFILES))
OPENCPU_AT_COBJSTEMP :=$(subst $(OPENCPU_AT_DIR),openAT,$(OPENCPU_AT_COBJSTEMP))
OPENCPU_AT_COBJS := $(addprefix $(BUILDDIR)/, $(OPENCPU_AT_COBJSTEMP))
OPENCPU_AT_PPFILES := $(patsubst %.o, %.pp, $(OPENCPU_AT_COBJS))

PPFILES += $(OPENCPU_AT_PPFILES)


OPENCPU_AT_TABLE_COBJSTEMP := $(patsubst %.c, %.o, $(OPENCPU_AT_TABLE_CFILES))
OPENCPU_AT_TABLE_OBJS := $(subst $(OPENCPU_AT_DIR), $(BUILDDIR)/openAT,$(OPENCPU_AT_TABLE_COBJSTEMP))

OPENCPU_AT_TABLE_GPERF_CFILES := $(addsuffix _gperf.c, $(basename $(OPENCPU_AT_TABLE_OBJS)))
OPENCPU_AT_TABLE_GPERF_OBJS := $(patsubst %.c, %.o, $(OPENCPU_AT_TABLE_GPERF_CFILES))

STRIP_OBJS += $(patsubst %.o, %.sc, $(OPENCPU_AT_TABLE_OBJS))

OPENCPU_AT_COBJS += $(OPENCPU_AT_TABLE_GPERF_OBJS)

$(BUILDDIR)/openAT/mg_openAT_cmd_table_gperf.c : $(BUILDDIR)/openAT/mg_openAT_cmd_table.sc
	$(Q)$(TOP)/tools/LogPrePass.exe -G $< AtCmdPreDefInfoMap $(basename $<).gperf;[ $$? -eq 0 ]
	$(Q)$(TOP)/tools/gperf/bin/gperf.exe -L ANSI-C -t -CGD --ignore-case -K pName -H atOpenAtCmdHash -N atOpenAtCmdLookUp -W openAtCmdTableGp "$(abspath $(basename $<).gperf)" --output-file="$(abspath $@)"

else
CFLAGS_INC           +=  -I $(OPENCPU_AT_DIR)/include

OPENCPU_AT_CSRC      += $(OPENCPU_AT_DIR)/mg_openAT_cmd_table.c
OPENCPU_AT_CSRC      += $(OPENCPU_AT_DIR)/mg_openAT_engine.c

OPENCPU_AT_COBJSTEMP := $(patsubst %.c, %.o, $(OPENCPU_AT_CSRC))
OPENCPU_AT_COBJSTEMP :=$(subst $(OPENCPU_AT_DIR),openAT,$(OPENCPU_AT_COBJSTEMP))
OPENCPU_AT_COBJS := $(addprefix $(BUILDDIR)/, $(OPENCPU_AT_COBJSTEMP))
OPENCPU_AT_PPFILES := $(patsubst %.o, %.pp, $(OPENCPU_AT_COBJS))

PPFILES += $(OPENCPU_AT_PPFILES)

endif

ifneq ($(MAKECMDGOALS), unilog)
-include $(OPENCPU_AT_COBJS:.o=.d)
endif

-include $(OPENCPU_AT_COBJS:.o=.dd)


ifeq ($(TOOLCHAIN),GCC)

lib-y += libopenAt.a

$(BUILDDIR)/lib/libopenAt.a: $(OPENCPU_AT_COBJS)
	@mkdir -p $(dir $@)
	$(ECHO) AR $@
	$(Q)$(AR) -cr $@ $^

endif
