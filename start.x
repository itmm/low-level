# Parse RISC-V assembly
* start low-level with the first unit-test

```
@Def(file: ll.cpp)
	#include <iostream>
	@Put(needed by main);
	int main(
		int argc, const char *argv[]
	) {
		@Put(unit-tests)
		@Put(main)
	}
@End(file: ll.cpp)
```
* the start is easy in C++
* a naked `@f(main)` function with arguments array
* it will call some other things that must be declared before the
  `@f(main)` function
* so there is a fragment for prerequisites and the content

```
@Def(needed by main)
	@put(needed by main)
@End(needed by main)
```
* group local additions into a fragment

```
@def(needed by add line)
	#include <cassert>
@end(needed by add line)
```
* the unit-tests need the `@f(assert)`-macro

## Parsing a single line
* the first test focuses on parsing a single line
* other code is responsible for reading files and extract the low-level
  lines from it

The parsing is part of a `State` object that contains all generated
machine instructions.

```
@def(needed by main)
	@put(needed by state);
	class State {
			@put(private state)
		public:
			@put(public state)
	};
	@put(state impl);
@end(needed by main)
```
* the `State` class can contain private and public members
* also a fragment is reserved for containing the implementation of
  non-inlined methods

```
@def(needed by state)
	#include <string>
	#include <vector>
	#include <sstream>
@end(needed by state)
```
* `@f(add_line)` needs `std::string`

```
@def(public state)
	void add_line(
		const std::string &line
	);
@end(public state)
```
* method to consume one line with one instruction

```
@def(state impl)
	@put(needed by add line)
	void State::add_line(
		const std::string &line
	) {
		@put(add line);
	}
@end(state impl)
```
* populate the empty implementation when dependencies are present


Where will the machine instructions be written?
To abstract the storage away from the code generation a new method stores
the machine code.

```
@def(private state)
	void add_machine(int instr);
@end(private state)
```
* adding one machine instruction to the current output

```
@add(state impl)
	void State::add_machine(int instr) {
		@put(add machine)
	}
@end(state impl)
```
* populate empty implementation when dependencies are present

```
@add(public state)
	@put(code storage impl)
@end(public state)
```
* first a simple code storage mechanism will be used

```
@def(code storage impl)
	private:
		std::vector<int> code;
@end(code storage impl)
```
* code is stored in one big array

```
@add(code storage impl)
	public:
		int code_size() const {
			return static_cast<int>(
				code.size()
			);
		}
@end(code storage impl)
```
* code size is the size of the array

```
@add(code storage impl)
	public:
		int get_code(int pos) const {
			return code[pos];
		}
		int &mod_code(int pos) {
			return code[pos];
		}
@end(code storage impl)
```
* machine instructions can be accessed directly from the array

```
@def(add machine)
	code.push_back(instr);
@end(add machine)
```
* simple implementation just saves the data

```
@add(needed by main)
	void assert_line(
		const char *line,
		int expected
	) {
		@put(assert line);
	}
@end(needed by main)
```
* simple helper function to check that some source code generates the
  expected instruction

```
@Def(unit-tests)
	@put(unit-tests)
@End(unit-tests)
```
* group local tests in a fragment

```
@def(unit-tests)
	assert_line(
		"raw $87654321", 0x87654321
	);
	assert_line(
		"raw (1000 - 200)", 800
	);
	assert_line(
		"raw (200 - 1000)", -800
	);
	assert_line(
		"raw ($20010020 - $20010010)", 0x10
	);
	assert_line(
		"raw ($20010010 - $20010020)", -0x10
	);
@end(unit-tests)
```

```
@def(assert line)
	State s;
	s.add_line(line);
	if (s.code_size() != 1) {
		std::cerr << "assert failed in [" << line << "]\n";
		std::cerr << "got " << s.code_size() << " elements\n";
	}
	assert(s.code_size() == 1);
	if (s.get_code(0) != expected) {
		std::cerr << "assert failed in [" << line << "]\n";
		std::cerr << "EXP " << std::hex << expected << "\n";
		std::cerr << "GOT " << s.get_code(0) << std::dec << "\n";
	}
	assert(s.get_code(0) == expected);
@end(assert line)
```
* instantiate a state and let it parse the line
* sadly this test fails right now

```
@add(needed by main)
	void assert_line_2(const char *line, int exp1, int exp2) {
		State s;
		s.add_line(line);
		assert(s.code_size() == 2);
		if (s.get_code(0) != exp1 || s.get_code(1) != exp2) {
	 		std::cerr << "EXP " << std::hex << exp1 << ", " << exp2 << "\n";
			std::cerr << "GOT " << s.get_code(0) << ", " << s.get_code(1) << std::dec << "\n";
	 	}
		assert(s.get_code(0) == exp1);
		assert(s.get_code(1) == exp2);
	}
@end(needed by main)
```

Before this test can pass the state must have the ability to parse the
source line.

For the parsing a first step is to split the input line into tokens.
From these tokens a syntax tree will be derived.
These syntax trees are then transformed into machine code.

## Tokenizing
* split the input line into token

```
@add(needed by add line)
	#include <cctype>
@end(needed by add line)
```
* needs `@f(isalnum)` and `@f(isdigit)`

```
@add(unit-tests)
	assert_line(
		"%x4 <- %x2 + %x3", 0x00310233
	);
	assert_line(
		"%a0 <- %a1 - %a2", 0x40c58533
	);
@end(unit-tests)
```
* the first test checks that a simple register add can be parsed

```
@add(needed by add line)
	#include <map>
@end(needed by add line)
```

```
@inc(macros.x)
```

```
@add(private state)
	Macros _macros;
	static Macros *setup_symbols();
@end(private state)
```

```
@add(state impl)
	Macros *State::setup_symbols() {
		static State s { nullptr };
		static bool initialized { false };
		if (! initialized) {
			@put(setup symbols);
			initialized = true;
		}
		return &s._macros;
	}
@end(state impl)
```

```
@add(public state)
	State(): _macros { setup_symbols() } { }
	State(Macros *parent): _macros { parent } { }
@end(public state)
```

```
@def(setup symbols)
	#include "default.h"
	std::istringstream in { setup };
	std::string l;
	while (std::getline(in, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}
@end(setup symbols)
```

```
@def(add line)
	std::vector<Item> items;
	auto end { line.end() };
	auto cur { line.begin() };
	bool log { false };
	if (cur < end && *cur == '!') {
		log = true;
		++cur;
	}
	for (;;) {
		while (cur < end && *cur <= ' ') {
			++cur;
		}
		if (cur == end) { break; }

		int escapes { 0 };
		while (cur < end && *cur == '`') {
			++escapes;
			++cur;
		}
		auto begin { cur };
		if (isalpha(*cur) || *cur == '_' || *cur == '%') {
			while (cur < end && (isalnum(*cur) || *cur == '_' || *cur == '%')) {
				++cur;
			}
			items.emplace_back(Item_Type::t_string, std::string { begin, cur }, 0, escapes);
		} else if (isdigit(*cur)) {
			int value { 0 };
			while (cur < end && isdigit(*cur)) {
				value = value * 10 + (*cur++ - '0');
			}
			items.emplace_back(Item_Type::t_instance, "num", value, escapes);
		} else if (*cur == '#') {
			break;
		} else if (*cur == '@') {
			++cur;
			std::string name;
			while (cur < end && isalnum(*cur)) {
				name += *cur++;
			}
			items.emplace_back(Item_Type::t_type, name, 0, escapes);
		} else if (*cur == '$') {
			++cur;
			int value { 0 };
			while (cur < end && isxdigit(*cur)) {
				int digit;
				if (isdigit(*cur)) {
					digit = *cur - '0';
				} else if (*cur <= 'F') {
					digit = *cur - 'A' + 10;
				} else {
					digit = *cur - 'a' + 10;
				}
				value = value * 16 + digit;
				++cur;
			}
			items.emplace_back(Item_Type::t_instance, "num", value, escapes);
		} else if (ispunct(*cur)) {
			while (cur < end && ispunct(*cur) && *cur != '$' && *cur != '#' && *cur != '@' && *cur != '_' && *cur != '%' && *cur != '`') {
				++cur;
			}
			items.emplace_back(Item_Type::t_string, std::string { begin, cur }, 0, escapes);
		}
	}
	items.emplace_back(Item_Type::t_string, ";", 0, 0);
	@put(expand);
@end(add line)
```

```
@add(unit-tests)
	assert_line(
		"%pc <- %pc", 0x0000006f
	);
@end(unit-tests)
```
* check for simple endless loop


```
@add(unit-tests)
	assert_line(
		"%pc <- %pc - 28", 0xfe5ff06f
	);
@end(unit-tests)
```
* check for simple endless loop

```
@add(unit-tests)
	assert_line(
		"%pc <- %pc - 32", 0xfe1ff06f
	);
@end(unit-tests)
```
* check for simple endless loop

```
@Def(main)
	@put(read stdin)
@End(main)
```
* read `State` form standard input

```
@def(read stdin)
	State s;
	std::string l;
	while (std::getline(std::cin, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}
@end(read stdin)
```

```
@add(unit-tests)
	assert_line(
		"%x5 <- %x5 and $ff", 0x0ff2f293
	);
	assert_line(
		"%a0 <- %a1 and %a2", 0x00c5f533
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"%x5 <- %x5 or $1", 0x0012e293
	);
	assert_line(
		"%a0 <- %a1 or %a2", 0x00c5e533
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"%x6 <- %x6 or $1", 0x00136313
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"%a0 <- %a1 xor $ff", 0x0ff5c513
	);
	assert_line(
		"%a0 <- %a1 xor %a2", 0x00c5c533
	);
	assert_line(
		"%a0 <- complement %a1", 0x0005c513
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"%x11 <- $0d", 0x00d00593
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"%x12 <- $0a", 0x00a00613
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"%x10 <- $1013000", 0x1013537
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"%x5 <- %pc", 0x00000297
	);
	assert_line_2(
		"%x5 <- %pc + $20", 0x00000297, 0x02028293
	);
	assert_line_2(
		"%x5 <- %pc - $20", 0x00000297, 0xfe028293
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"%mtvec <- %x5", 0x30529073
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"%x5 <- %mhartid", 0xf14022f3
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"if %x5 < 0: %pc <- %pc - 4",
		0xfe02cee3
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"if %x5 = 0: %pc <- %pc - 12",
		0xfe028ae3
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"if %x5 != %x11: %pc <- %pc - 28",
		0xfeb292e3
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"if %x5 != 0: %pc <- %pc + 0",
		0x00029063
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"%x6 <- [%x10]",
		0x00052303
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"%x5 <- [%x10 + $04]",
		0x00452283
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"[%x10] <- %x12",
		0x00c52023
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"[%x10 + $08] <- %x5",
		0x00552423
	);
@end(unit-tests)
```

```
@def(expand)
restart:
	if (log) {
		std::cerr << "LINE {";
		for (const auto &i : items) {
			std::cerr << i << " ";
		}
		std::cerr << "}\n";
	}
	if (items.size()) {
		auto macro { _macros.begin() };
		while (macro != _macros.end()) {
			unsigned i = 0;
			while (i + macro->pattern().size() <= items.size()) {
				@put(transform);
				++i;
			}
			++macro;
		}
		@put(handle define);
		@put(consume machine instrs);
	}
	if (! items.empty()) {
		std::cerr <<
			"cant expand fully [" <<
			line << "]; got [";
		for (const auto &i: items) {
			std::cerr << i << " ";
		}
		std::cerr << "]\n";
	}
@end(expand)
```

```
@def(transform)
	if (i + 2 < items.size()) {
		const auto &t { items[i] };
		const auto &c { items[i + 1] };
		const auto &v { items[i + 2] };
		if (c.type() == Item_Type::t_string && c.str() == ":") {
			if (t.type() == Item_Type::t_type && v.type() == Item_Type::t_instance && v.str() == "num") {
				std::string type { t.str() };
				int value { v.value() };
				items.erase(items.begin() + i, items.begin() + i + 3);
				items.emplace(items.begin() + i, Item_Type::t_instance, type, value, 0);
				goto restart;
			}
			if (t.type() == Item_Type::t_instance && v.type() == Item_Type::t_string && v.str() == "value") {
				int value { t.value() };
				items.erase(items.begin() + i, items.begin() + i + 3);
				items.emplace(items.begin() + i, Item_Type::t_instance, "num", value, 0);
				goto restart;
			}
		}
	}
@end(transform)
```

```
@add(transform) {
	bool matches { true };
	auto p { macro->pattern().begin() };
	auto e { macro->pattern().end() };
	for (int j = 0; matches && p != e; ++p, ++j) {
		matches = matches && (i + j < items.size());
		matches = matches && p->matches(items[i + j]);
		matches = matches && p->escapes() <= 0;
	}
	if (matches) {
		auto k { i };
		i += macro->pattern().size();
		for (const auto &e : macro->replacement()) {
			if (e.type() == Item_Type::t_type && ! e.str().empty()) {
				if (isdigit(e.str()[0])) {
					int idx { std::stoi(e.str()) };
					if (idx >= 0 && idx < (int) macro->pattern().size()) {
						items.insert(
							items.begin() + i, items[k + idx]
						);
						++i;
						continue;
					}
				} else if (e.str() == "arithmetic") {
					const auto &n1 { items[k + 0] };
					const auto &op { items[k + 1] };
					const auto &n2 { items[k + 2] };
					if (n1.type() == Item_Type::t_instance && n1.str() == "num" && op.type() == Item_Type::t_string && n2.type() == Item_Type::t_instance && n2.str() == "num") {
						if (op.str() == "+") {
							items.emplace(
								items.begin() + i, Item_Type::t_instance, "num", n1.value() + n2.value(), 0
							);
							++i;
							continue;
						} else if (op.str() == "-") {
							items.emplace(
								items.begin() + i, Item_Type::t_instance, "num", n1.value() - n2.value(), 0
							);
							++i;
							continue;
						} else if (op.str() == "*") {
							items.emplace(
								items.begin() + i, Item_Type::t_instance, "num", n1.value() * n2.value(), 0
							);
							++i;
							continue;
						} else if (op.str() == "/" && n2.value() != 0) {
							items.emplace(
								items.begin() + i, Item_Type::t_instance, "num", n1.value() / n2.value(), 0
							);
							++i;
							continue;
						} else if (op.str() == "and") {
							items.emplace(
								items.begin() + i, Item_Type::t_instance, "num", n1.value() & n2.value(), 0
							);
							++i;
							continue;
						} else if (op.str() == "or") {
							items.emplace(
								items.begin() + i, Item_Type::t_instance, "num", n1.value() | n2.value(), 0
							);
							++i;
							continue;
						} else if (op.str() == "<<") {
							items.emplace(
								items.begin() + i, Item_Type::t_instance, "num", n1.value() << n2.value(), 0
							);
							++i;
							continue;
						} else if (op.str() == ">>") {
							items.emplace(
								items.begin() + i, Item_Type::t_instance, "num", n1.value() >> n2.value(), 0
							);
							++i;
							continue;
						} else if (op.str() == "xor") {
							items.emplace(
								items.begin() + i, Item_Type::t_instance, "num", n1.value() ^ n2.value(), 0
							);
							++i;
							continue;
						}
					}
				}
			}
			items.emplace(
				items.begin() + i, e.type(), e.str(), e.value(), e.escapes() - 1
			);
			++i;
		}
		items.erase(items.begin() + k, items.begin() + k + macro->pattern().size());
		goto restart;
	}
} @end(transform)
```

```
@add(transform) {
	const auto &ni { items[i] };
	if (ni.type() == Item_Type::t_string) {
		@put(transform named);
	}
} @end(transform)
```

```
@def(transform named)
	if (ni.str() == "*" && ni.escapes() <= 0) {
		@put(transform cur addr);
	}
@end(transform named)
```

```
@def(transform cur addr)
	items.erase(items.begin() + i,
		items.begin() + i + 1);
	int addr = code.size() * 4 +
		0x20010000;
	items.emplace(items.begin() + i,
		Item_Type::t_instance, "num", addr, 0
	);
	goto restart;
@end(transform cur addr)
```

```
@def(consume machine instrs)
	while (! items.empty()) {
		const auto &mi { *items.begin() };
		if (mi.type() == Item_Type::t_string && mi.str() == ";") {
			items.erase(
				items.begin(),
				items.begin() + 1
			);
			continue;
		}
		if (mi.type() != Item_Type::t_instance || mi.str() != "raw") { break; }
		add_machine(mi.value());
		items.erase(
			items.begin(),
			items.begin() + 1
		);
	}
@end(consume machine instrs)
```

```
@def(handle define)
	for (unsigned i = 1; i < items.size(); ++i) {
		const auto &a { items[i] };
		if (a.type() == Item_Type::t_string && a.str() == "<==" && a.escapes() <= 0) {
			@put(transform sym assign);
		}
	}
@end(handle define)
```

```
@def(transform sym assign)
	Items value;
	unsigned last { items.size() - 1 }; // skip last ;
	for (unsigned j = i + 1;
		j < last; ++j
	) {
		const auto &cur { items[j] };
		if (cur.type() == Item_Type::t_string && cur.str() == "." && cur.escapes() <= 0) {
			last = j; break;
		}
		value.push_back(items[j]);
	}
	Items p;
	for (unsigned j = 0; j < i; ++j) {
		p.push_back(items[j]);
	}
	_macros.emplace_back(std::move(p), std::move(value));
	if (last < items.size()) { ++last; }
	items.erase(
		items.begin(), items.begin() + last
	);
@end(transform sym assign)
```

```
@inc(default.x)
```

```
@inc(forward.x)
```

