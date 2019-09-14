.PHONY: all clean

SRCs := $(shell hx-srcs.sh)
GENs := $(shell hx-files.sh $(SRCs))
CPPs := $(filter %.cpp, $(GENs))

all: .hx_run
	@make -f Makefile.build all

.hx_run: $(SOURCES)
	@echo HX
	@hx
	@date >$@

clean:
	@echo RM
	@rm -f .hx-run $(GENs)
	@make -f Makefile.build clean

%.list: %.hex
	@make -f Makefile.build $@

%.hex: %.ll
	@make -f Makefile.build $@
