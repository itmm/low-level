CXXFLAGS += -Wall

.PHONY: all clean tests

all: tests echo_small.list

tests: ll
	@echo run unit-tests
	@./ll <echo_small.ll >echo_small.hex
	@diff echo.hex echo_small.hex

SOURCEs := $(wildcard *.x)
GENs := $(shell hx-files.sh $(SOURCEs))
CPPs := $(filter %.cpp, $(GENs))

$(GENs) .hx-run: $(SOURCEs)
	@echo HX
	@hx
	@touch .hx-run

ll: $(CPPs)
	@echo C++ ll
	@$(CXX) $(CXXFLAGS) $(CPPs) -o ll

clean:
	@echo RM
	@rm -f .hx-run $(GENs)

echo_small.o: echo_small.s
	riscv32-unknown-elf-gcc -Wall -march=rv32g -c $^

echo_small.elf: echo_small.o
	riscv32-unknown-elf-ld -T memory.lds $^  -o $@

echo_small.hex: echo_small.elf
	riscv32-unknown-elf-objcopy $^ -O ihex $@

echo_small.list: echo_small.hex
	@echo LIST
	@riscv32-unknown-elf-objdump -EL -D -mriscv $^ >$@
