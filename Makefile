CXXFLAGS += -Wall -g

.PHONY: all clean tests

all: tests echo.list

tests: ll echo-as.hex
	@echo run unit-tests
	@./ll <echo.ll >echo.hex
	@diff echo.hex echo-as.hex

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

echo.o: echo.s
	riscv32-unknown-elf-gcc -Wall -march=rv32g -c $^

echo.elf: echo.o
	riscv32-unknown-elf-ld -T memory.lds $^  -o $@

echo-as.hex: echo.elf
	riscv32-unknown-elf-objcopy $^ -O ihex $@

echo.list: echo.hex
	@echo LIST
	@riscv32-unknown-elf-objdump -EL -D -mriscv $^ >$@
