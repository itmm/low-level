
	
	#include <cassert>

	bool do_riscv(
		const char *src,
		const char *obj
	) {
		return false;
	}
;
	int main(
		int argc, const char *argv[]
	) {
		
	assert(do_riscv(
		"%r4 <- %r2 + %r3", 
		"\x01\x02\x03\x04"
	));

	}
