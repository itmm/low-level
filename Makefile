CXXFLAGS += -Wall

.PHONY: all clean tests

all: tests

tests: ll
	@echo run unit-tests
	@./ll

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

