.PHONY: all clean

SOURCEs := $(shell hx-srcs.sh *.x)
GENs := $(shell hx-files.sh $(SOURCEs))
CPPs := $(filter %.cpp, $(GENs))

all: .hx_run
	@make -f Makefile.build all

.hx_run: $(SOURCES)
	@echo HX
	@hx
	@touch .hx-run

clean:
	@echo RM
	@rm -f .hx-run $(GENs)
	@make -f Makefile.build clean

%.list: %.hex
	@make -f Makefile.build $@

%.hex: %.ll
	@make -f Makefile.build $@
