# First Code fragments
* start low-level with the first unit-test

```
@Def(file: ll.cpp)
	@put(needed by main);
	int main(
		int argc, const char *argv[]
	) {
		@put(main)
	}
@End(file: ll.cpp)
```
* the start is easy in C++
* a naked `@f(main)` function with arguments array
* it will call some other things that must be declared before the
  `@f(main)` function
* so there is a fragment for prerequisites and the content

```
@def(main)
	@put(unit-tests)
@end(main)
```
* first some unit-tests are performed at every start of low-level

```
@def(needed by main)
	#include <cassert>
@end(needed by main)
```
* the unit-tests need the `@f(assert)`-macro

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
