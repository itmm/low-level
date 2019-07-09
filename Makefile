CXXFLAGS += -Wall

.PHONY: all clean

all: ll

SOURCEs := $(wildcard *.x)
GENs := $(shell hx-files.sh $(SOURCEs))

$(GENs) .hx-run: $(SOURCEs)
	@echo HX
	@hx
	@touch .hx-run

ll: .hx-run
	@echo C++ ll
	@$(CXX) $(CXXFLAGS) $(GENs) -o ll

clean:
	@echo RM
	@rm -f .hx-run $(GENs)

