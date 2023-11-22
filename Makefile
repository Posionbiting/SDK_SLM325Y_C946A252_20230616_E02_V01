TOP    :=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
export TOP

.PHONY:gccall
gccall:
	($(MAKE) V=$(V) -f  make/Makefile.prj all)

.PHONY:clean-gccall
clean-gccall:
	($(MAKE) -f make/Makefile.prj cleanall)

.PHONY:clean-gcc
clean-gcc:
	($(MAKE) -f make/Makefile.prj clean)

.PHONY:size
size:
	($(MAKE) -f make/Makefile.prj size)

.PHONY: build-unilog
build-unilog:
	($(MAKE) -f make/Makefile.prj unilog)


