# The Low-Level Programming Language

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
	bool do_riscv(
		const char *src,
		const char *obj
	) {
		return false;
	}
@end(needed by main)
```

```
@def(unit-tests)
	assert(do_riscv(
		"%r4 <- %r2 + %r3", 
		"\x01\x02\x03\x04"
	));
@end(unit-tests)
```
