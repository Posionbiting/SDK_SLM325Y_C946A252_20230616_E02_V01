CFLAGS_INC += -I $(LIBDIR)/PS/inc

ifeq ($(BUILD_PS), y)
ifeq ($(TOOLCHAIN),GCC)
PREBUILDLIBS += $(LIBDIR)/PS/lib/gcc/libps.a   \
                $(LIBDIR)/PS/lib/gcc/libpsl1.a \
                $(LIBDIR)/PS/lib/gcc/libpsif.a 
endif

ifeq ($(TOOLCHAIN),ARMCC)
PREBUILDLIBS += $(LIBDIR)/PS/lib/keil/libps.lib   \
				$(LIBDIR)/PS/lib/keil/libpsl1.lib \
                $(LIBDIR)/PS/lib/keil/libpsif.lib 
endif
endif