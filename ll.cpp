
#line 4 "index.x"

	
#line 15 "index.x"

	#include <cassert>

#line 21 "index.x"

	
#line 56 "index.x"

	
#line 42 "index.x"

	int build_r_cmd(
		int funct7, char src2, char src1,
		int funct3, char dst, int opcode
	) {
		return (funct7 << 25) |
			(src2 << 20) | (src1 << 15) |
			(funct3 << 12) | (dst << 7) |
			opcode;
	}

#line 57 "index.x"
;
	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}

#line 22 "index.x"

	int do_riscv(
		const char *src
	) {
		int got = 0x00000000;
		
#line 70 "index.x"

	char dst { 0 };
	char src1 { 0 };
	char src2 { 0 };

#line 78 "index.x"

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

#line 94 "index.x"

	while (*src && *src <= ' ') { ++src; }
	if (*src++ != '<') {
		assert(false); return false;
	}
	if (*src++ != '-') {
		assert(false); return false;
	}

#line 106 "index.x"

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

#line 122 "index.x"

	while (*src && *src <= ' ') { ++src; }
	if (*src++ != '+') {
		assert(false); return false;
	}

#line 131 "index.x"

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

#line 147 "index.x"

	got = build_add(dst, src1, src2);

#line 27 "index.x"
;
		return got;
	}

#line 5 "index.x"
;
	int main(
		int argc, const char *argv[]
	) {
		
#line 34 "index.x"

	assert(do_riscv(
		"%x4 <- %x2 + %x3"
	) == 0x00310233);

#line 9 "index.x"

	}
