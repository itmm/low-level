# The Low-Level Programming Language
* defines a language to generate RISC-V machine code
* uses a high-level syntax
* but keeps the ability to generate direct machine code instructions

## Overview
* a small motivation for this project

This project is an answer to the different syntax versions of assembly
languages that are around in the wild.
Most are build around a simple syntactic structure for each instruction:

```
@k(mnemonic) [arg1 [, arg2 [, arg3]]]
```
* the `@k(mnemonic)` describes the kind of the operation
* depending on the operation `0` to `3` arguments are present

An example from the basic RISC-V instruction set is:

```
@k(add) @k(x8), @k(x9), @k(x10)
```
* adds the contents of the general purpose registers `@k(x9)` and
  `@k(x10)`
* and store the result in `@k(x8)`

Sounds easy enough, right?
Wrong!

Even this statement is not as clear as possible.
It is not clear what are the arguments (`@k(x9)` and `@k(x10)`) and
where the result is stored (`@k(x8)`).

In RISC-V assembly language the target is specified most often as the
first argument.
But sadly not always.
One often used instruction is the store to memory.

A load from memory follows the general pattern:

```
@k(lw) @k(x8), (@k(x9))
```
* loads one word of memory at the address stored in `@k(x9)` into
  register `@k(x8)`
* the parenthesis indicate an indirect access

The store operation has the same ordering as the load:

```
@k(sw) @k(x8), (@k(x9))
```
* stores the contents of `@k(x8)` in memory at the address stored in
  register `@k(x9)`

And it is not clear that `@k(x8)` to `@k(x10)` are registers instead
of some predefined constants, labels or variable names stored in
registers or on the stack.

Also it is not clear, what the different mnemonics mean (for example
`@k(auipc)`? `@k(shaiw)`? `@k(lui)`?).

In low-level these three instructions are encoded in the following form:

```
@k(%x8) <- @k(%x9) + @k(%x10)
@k(%x8) <- [@k(%x9)]
[@k(%x9)] <- @k(%x8)
```
* addition is performed with an infix operator
* assignment is also an operator
* memory lookup is indicated with brackets
* special names for registers are prefixed with `@k(%)`

### Jumps
* implementation of jumps in low-level

Even jumps can be encoded this way by updating the program counter
`@k(%pc)`:

```
@t(infinite_loop) = *
	@k(%pc) <- @k(%pc) + \
		@t(inifinite_loop) - *
```
* `*` is the address of the current instruction
* in RISC-V jumps are relative to the current position
* the `\` signals that the instruction is continued on the next line

Of course this is a bit cumbersome.
But due to the powerful meta-programming functionality of low-level
a shortcut exist:

```
@t(infinite_loop) = *
	@k(jump) @t(inifinite_loop)
```
* `@k(jump)` is a meta-function that low-level expands to the addition
  to the program counter

This meta-function expands to the same code as the direct assignment
of the program counter.

### Conditional Jumps
* implementation of conditional jumps in low-level

The implementation of conditional jumps is a bit more verbose than
in normal assembly languages.
Inheriting from other high-level languages an `if` statement can check
an expression and perform a jump only if the condition is `true`.

Here is an example for computing the sum over the first `100` integers:

```
	result = @k(%x8)
	i = @k(%x9)
	result <- 0
	i <- 100
@t(loop) = *
	result <- result + i
	i <- i - 1
	if (i > 0) { @k(jump) @t(loop) }
```
* variables are defined as synonyms for register names to ease
  understanding
* the `if` statement do not have the flexibility of high-level languages
* each `if`-`@k(jump)`-combination expands to a conditional jump

### Expected output
* low-level directly generates hex-files to upload to the SiFive HiFive1
  Rev. B experimental board

```
@inc(goal.x)
```
* complete low-level program to implement a simple echo-client over the
  UART serial interface
* this is the reference to guide the development of low-level

```
@Def(file: ll.cpp)
	@put(needed by main);
	int main(
		int argc, const char *argv[]
	) {
		@put(unit-tests)
	}
@End(file: ll.cpp)
```

```
@def(needed by main)
	#include <cassert>
@end(needed by main)
```

```
@add(needed by main)
	@put(needed by do riscv)
	int do_riscv(
		const char *src
	) {
		int got = 0x00000000;
		@put(do riscv);
		return got;
	}
@end(needed by main)
```

```
@def(unit-tests)
	assert(do_riscv(
		"%x4 <- %x2 + %x3"
	) == 0x00310233);
@end(unit-tests)
```

```
@def(needed by build add)
	int build_r_cmd(
		int funct7, char src2, char src1,
		int funct3, char dst, int opcode
	) {
		return (funct7 << 25) |
			(src2 << 20) | (src1 << 15) |
			(funct3 << 12) | (dst << 7) |
			opcode;
	}
@end(needed by build add)
```

```
@def(needed by do riscv)
	@put(needed by build add);
	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}
@end(needed by do riscv)
```

```
@def(do riscv)
	char dst { 0 };
	char src1 { 0 };
	char src2 { 0 };
@end(do riscv)
```

```
@add(do riscv)
	while (*src && *src <= ' ') { ++src; }
	if (*src++ != '%') {
		assert(false); return false;
	}
	if (*src++ != 'x') {
		assert(false); return false;
	}
	dst = *src++ - '0';
	if ((unsigned) dst >= 32) {
		assert(false); return false;
	}
@end(do riscv)
```

```
@add(do riscv)
	while (*src && *src <= ' ') { ++src; }
	if (*src++ != '<') {
		assert(false); return false;
	}
	if (*src++ != '-') {
		assert(false); return false;
	}
@end(do riscv)
```

```
@add(do riscv)
	while (*src && *src <= ' ') { ++src; }
	if (*src++ != '%') {
		assert(false); return false;
	}
	if (*src++ != 'x') {
		assert(false); return false;
	}
	src1 = *src++ - '0';
	if ((unsigned) src1 >= 32) {
		assert(false); return false;
	}
@end(do riscv)
```

```
@add(do riscv)
	while (*src && *src <= ' ') { ++src; }
	if (*src++ != '+') {
		assert(false); return false;
	}
@end(do riscv)
```

```
@add(do riscv)
	while (*src && *src <= ' ') { ++src; }
	if (*src++ != '%') {
		assert(false); return false;
	}
	if (*src++ != 'x') {
		assert(false); return false;
	}
	src2 = *src++ - '0';
	if ((unsigned) src2 >= 32) {
		assert(false); return false;
	}
@end(do riscv)
```

```
@add(do riscv)
	got = build_add(dst, src1, src2);
@end(do riscv)
```
