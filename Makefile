CXXFLAGS += -Wall -g

.PHONY: all clean

SOURCEs := $(shell hx-srcs.sh *.x)
GENs := $(shell hx-files.sh $(SOURCEs))

all: $(GENs)
	@make -f Makefile.build all

$(GENs) .hx-run: $(SOURCEs)
	@echo HX
	@hx
	@touch .hx-run

clean:
	@echo RM
	@rm -f .hx-run $(GENs)
	@make -f Makefile.build clean

