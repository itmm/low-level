
#line 5 "start.x"

	
#line 28 "start.x"

	#include <cassert>

#line 35 "start.x"

	
#line 70 "start.x"

	
#line 56 "start.x"

	int build_r_cmd(
		int funct7, char src2, char src1,
		int funct3, char dst, int opcode
	) {
		return (funct7 << 25) |
			(src2 << 20) | (src1 << 15) |
			(funct3 << 12) | (dst << 7) |
			opcode;
	}

#line 71 "start.x"
;
	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}

#line 36 "start.x"

	int do_riscv(
		const char *src
	) {
		int got = 0x00000000;
		
#line 84 "start.x"

	char dst { 0 };
	char src1 { 0 };
	char src2 { 0 };

#line 92 "start.x"

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

#line 108 "start.x"

	while (*src && *src <= ' ') { ++src; }
	if (*src++ != '<') {
		assert(false); return false;
	}
	if (*src++ != '-') {
		assert(false); return false;
	}

#line 120 "start.x"

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

#line 136 "start.x"

	while (*src && *src <= ' ') { ++src; }
	if (*src++ != '+') {
		assert(false); return false;
	}

#line 145 "start.x"

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

#line 161 "start.x"

	got = build_add(dst, src1, src2);

#line 41 "start.x"
;
		return got;
	}

#line 6 "start.x"
;
	int main(
		int argc, const char *argv[]
	) {
		
#line 21 "start.x"

	
#line 48 "start.x"

	assert(do_riscv(
		"%x4 <- %x2 + %x3"
	) == 0x00310233);

#line 22 "start.x"


#line 10 "start.x"

	}
