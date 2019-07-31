CXXFLAGS += -Wall

.PHONY: all clean tests

all: tests echo_small.d

tests: ll
	@echo run unit-tests
	@echo | ./ll

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

echo_small.d: echo_small.hex
	riscv32-unknown-elf-objdump -EL -D -mriscv $^ >$@
