CXXFLAGS += -Wall -g

.PHONY: all clean tests

all: tests

tests: ll echo-as.hex
	@echo run unit-tests
	@./ll <echo.ll >echo.hex
	@diff echo.hex echo-as.hex

default.h: default.ll
	@echo default.ll
	@(echo "std::string setup {"; \
		sed -e 's/#.*//' -e 's/^\s*//' <$^ | \
			grep -v '^$$' | \
			sed -e 's/\(.*\)/\t"\1\\n"/'; \
		echo "};") >$@

ll: default.h

SOURCEs := $(shell hx-srcs.sh *.x)
GENs := $(shell hx-files.sh $(SOURCEs))
CPPs := $(filter %.cpp, $(GENs))

.hx_run: $(SOURCES)
	@echo HX
	@hx
	@touch .hx-run

$(CPPs): .hx_run

ll: $(CPPs)
	@echo C++ $@
	@$(CXX) $(CXXFLAGS) $(CPPs) -o $@

clean:
	@echo RM
	@rm -f ll *.hex *.o *.elf default.h
	@rm -f .hx-run $(GENs)

echo.o: echo.s
ref.o: ref.s

%.o: %.s
	@echo RV32-CC $@
	@riscv32-unknown-elf-gcc -Wall -march=rv32g -c $^

echo.elf: echo.o
	@echo RV32-ELF $@
	@riscv32-unknown-elf-ld -T memory.lds $^  -o $@

echo-as.hex: echo.elf
	@echo RV32-HEX $@
	@riscv32-unknown-elf-objcopy $^ -O ihex $@

echo.list: echo.hex
	@echo RV32-LIST $@
	@riscv32-unknown-elf-objdump -EL -D -mriscv $^ >$@

ref.list: ref.o
	@echo RV32-LIST $@
	@riscv32-unknown-elf-objdump -EL -D -mriscv $^ >$@
