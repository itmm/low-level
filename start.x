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
@end(unit-tests)
```

```
@def(assert line)
	State s;
	s.add_line(line);
	assert(s.code_size() == 1);
	// std::cerr << "EXP " << std::hex << expected << "\n";
	// std::cerr << "GOT " << s.get_code(0) << std::dec << "\n";
	assert(s.get_code(0) == expected);
@end(assert line)
```
* instantiate a state and let it parse the line
* sadly this test fails right now

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

## Parsing
* parses simple additions

```
@add(needed by state)
	#include <memory>
@end(needed by state)
```
* needs `unique_ptr`

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
@add(needed by state)
	enum class Item_Type {
		t_type, t_instance, t_string
	};
	class Item {
		private:
			Item_Type _type;
			int _escapes;
			int _value;
			std::string _str;
		public:
			Item(Item_Type type, const std::string &str, int value, int escapes):
				_type { type }, 
				_escapes { escapes },
				_value { value },
				_str { str }
			{ }
			void write(std::ostream &out) const {
				switch (_type) {
					case Item_Type::t_type:
						out << '@' << _str;
						break;
					case Item_Type::t_instance:
						out << '@' << _str << ":$" << std::hex << _value << std::dec;
						break;
					case Item_Type::t_string:
						out << "@str:" << _str;
				}
			}
			virtual bool matches(const Item &in) const {
				switch (_type) {
					case Item_Type::t_type: {
						if (in._type == Item_Type::t_type && in._str == _str) {
							return true;
						}
						if (in._type == Item_Type::t_instance && in._str == _str) {
							return true;
						}
						if (in._type == Item_Type::t_string && _str == "str") {
							return true;
						}
						return false;
					}
					case Item_Type::t_instance: {
						return in._type == Item_Type::t_instance && in._str == _str && in._value == _value;
					}
					case Item_Type::t_string: {
						return in._type == Item_Type::t_string && in._str == _str;
					}
				}
				return false;
			}
			int escapes() const { return _escapes; }
			Item_Type type() const { return _type; }
			const std::string &str() const { return _str; }
			int value() const { return _value; }
	};
	std::ostream &operator<<(std::ostream &out, const Item &item) {
		item.write(out);
		return out;
	}
@end(needed by state)
```

```
@add(needed by state)
	using Items = std::vector<Item>;
	class Macro {
		private:
			Items _pattern;
			Items _replacement;
		public:
			Macro(
				Items &&pattern,
				Items &&replacement
			):
				_pattern { std::move(pattern) },
				_replacement { std::move(replacement) }
			{ }
			const Items &pattern() const {
				return _pattern;
			}
			const Items &replacement() const {
				return _replacement;
			}
	};
@end(needed by state)
```

```
@add(needed by state)
	class Macros {
		private:
			Macros *_parent;
			using Container = std::vector<Macro>;
			Container _macros;
		public:
			Macros(Macros *parent): _parent { parent } {}
			
			class Iterator {
				private:
					Macros *_macros;
					Container::iterator _cur;
					void fix() {
						if (_macros && _cur == _macros->_macros.end()) {
							if (_macros->_parent) {
								_macros = _macros->_parent;
								_cur = _macros->_macros.begin();
							} else {
								_macros = nullptr;
							}
						}
					}
				public:
					Iterator(Macros *macros, Container::iterator cur):
						_macros { macros },
						_cur { cur }
					{ fix() ;}
					Macro &operator*() { return *_cur; }
					Macro *operator->() { return &*_cur; }
					const Macro &operator*() const { return *_cur; }
					const Macro *operator->() const { return &*_cur; }
					Iterator &operator++() {
						++_cur;
						fix();
						return *this;
					}
					bool operator!=(const Iterator &o) const {
						return o._macros != _macros || (_macros != nullptr && o._cur != _cur);
					}
			};

			Iterator begin() {
				return { this , _macros.begin() };
			}

			Iterator end() {
				return { nullptr, Container::iterator { } };
			}

			void emplace_back(Items &&pattern, Items &&replacement) {
				_macros.emplace_back(std::move(pattern), std::move(replacement));
			}
	};
@end(needed by state)
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
@add(needed by state)
	int build_r_cmd(
		int funct7, char src2, char src1,
		int funct3, char dst, int opcode
	) {
		return (funct7 << 25) |
			(src2 << 20) | (src1 << 15) |
			(funct3 << 12) | (dst << 7) |
			opcode;
	}
@end(needed by state)
```

```
@add(needed by state)
	int build_i_cmd(
		int imm, char src1, int funct3, char dst, int opcode
	) {
		return (imm << 20) | (src1 << 15) | (funct3 << 12) | (dst << 7) | opcode;
	}
@end(needed by state)
```

```
@add(needed by state)
	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}
@end(needed by state)
```

```
@add(needed by state)
	int build_add(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x0, dst, 0x13
		);
	}
@end(needed by state)
```

```
@def(add line)
	std::vector<Item> items;
	auto end { line.end() };
	auto cur { line.begin() };
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
@add(needed by state)
	int build_load(
		char dst, char src, int imm
	) {
		return build_i_cmd(
			imm, src, 0x2, dst, 0x03
		);
	}
@end(needed by state)
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
	#if 0
		std::cerr << "LINE {";
		for (const auto &i : items) {
			std::cerr << '[' << i << "], ";
		}
		std::cerr << "}\n";
	#endif
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
			std::cerr << i << ", ";
		}
		std::cerr << "\n";
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
		if (a.type() == Item_Type::t_string && a.str() == "<=" && a.escapes() <= 0) {
			@put(transform sym assign);
		}
	}
@end(handle define)
```

```
@def(transform sym assign)
	Items value;
	unsigned last { items.size() };
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
@Def(file: default.ll)
# canonical register names

	%x0 <= @reg:0
	%x1 <= @reg:1
	%x2 <= @reg:2
	%x3 <= @reg:3
	%x4 <= @reg:4
	%x5 <= @reg:5
	%x6 <= @reg:6
	%x7 <= @reg:7
	%x8 <= @reg:8
	%x9 <= @reg:9
	%x10 <= @reg:10
	%x11 <= @reg:11
	%x12 <= @reg:12
	%x13 <= @reg:13
	%x14 <= @reg:14
	%x15 <= @reg:15
	%x16 <= @reg:16
	%x17 <= @reg:17
	%x18 <= @reg:18
	%x19 <= @reg:19
	%x20 <= @reg:20
	%x21 <= @reg:21
	%x22 <= @reg:22
	%x23 <= @reg:23
	%x24 <= @reg:24
	%x25 <= @reg:25
	%x26 <= @reg:26
	%x27 <= @reg:27
	%x28 <= @reg:28
	%x29 <= @reg:29
	%x30 <= @reg:30
	%x31 <= @reg:31

# ABI register names

	%zero <= %x0
	%ra <= %x1
	%sp <= %x2
	%gp <= %x3
	%tp <= %x4
	%t0 <= %x5
	%t1 <= %x6
	%t2 <= %x7
	%fp <= %x8
	%s0 <= %x8
	%s1 <= %x9
	%a0 <= %x10
	%a1 <= %x11
	%a2 <= %x12
	%a3 <= %x13
	%a4 <= %x14
	%a5 <= %x15
	%a6 <= %x16
	%a7 <= %x17
	%s2 <= %x18
	%s3 <= %x19
	%s4 <= %x20
	%s5 <= %x21
	%s6 <= %x22
	%s7 <= %x23
	%s8 <= %x24
	%s9 <= %x25
	%s10 <= %x26
	%s11 <= %x27
	%t3 <= %x28
	%t4 <= %x29
	%t5 <= %x30
	%t6 <= %x31

# splitting special chars sequences

	*) <= * )
	:( <= : (
	(( <= ( (
	)) <= ) )

# CSR registers

	%mtvec <= @csr:$305
	%mhartid <= @csr:$f14

# arithemitc operations

	(@num) <= @1
	@num << @num <= @arithmetic
	@num >> @num <= @arithmetic
	@num * @num <= @arithmetic
	@num / @num <= @arithmetic
	@num + @num <= @arithmetic
	@num - @num <= @arithmetic
	@num and @num <= @arithmetic
	@num or @num <= @arithmetic
	@num xor @num <= @arithmetic
	complement @num <= (@1 xor 0)

# instruction types

	r_type(@num, @reg, @reg, @num, @reg, @num) <= @raw: (@2 << 25 or @4:value << 20 or @6:value << 15 or @8 << 12 or @10:value << 7 or @12)
	i_type(@num, @reg, @num, @reg, @num) <= @raw: (@2 << 20 or @4:value << 15 or @6 << 12 or @8:value << 7 or @10)
	b_type(@num, @reg, @reg, @num, @num) <= @raw: ((@2 and $1000) << (31 - 12) or (@2 and $7e0) << (25 - 5) or @4:value << 20 or @6:value << 15 or @8 << 12 or (@2 and $1e) << (8 - 1) or (@2 and $800) >> (11 - 7) or @10)
	u_type(@num, @reg, @num) <= @raw: (@2 or @4:value << 7 or @6)
	j_type(@num, @reg, @num) <= @raw: ((@2 and $100000) << (31 - 20) or (@2 and $7fe) << (21 - 1) or (@2 and $800) << (20 - 11) or (@2 and $ff000) or @4:value << 7 or @6)
	s_type(@num, @reg, @reg, @num, @num) <= @raw: ((@2 and $fe0) << (25 - 5) or @4:value << 20 or @6:value << 15 or @8 << 12 or (@2 and $1f) << 7 or @10)

# instructions

	@reg <- - @num <= @0 <- (0 - @3)
	@reg <- @num <= big_assign (@0, @2 and $fffff800) small_assign(@0, @2)
	big_assign(@reg, $00000000) <=
	big_assign(@reg, $fffff800) <=
	big_assign(@reg, @num) <= u_type(@4 and $fffff000, @2, $37)
	small_assign(@reg, 0) <= @2 <- %zero + 0
	small_assign(@reg, @num) <= small_masked_assign (@2, @4 and $fff)
	small_masked_assign(@reg, 0) <=
	small_masked_assign(@reg, @num) <= @2 <- %zero + @4

	@reg <- @reg + @reg <= r_type($0, @4, @2, $0, @0, $33)
	@reg <- @reg - @reg <= r_type($20, @4, @2, $0, @0, $33)
	@reg <- @reg + @num <= i_type(@4, @2, $0, @0, $13)
	@reg <- @reg - @num <= @0 <- @2 + (0 - @4)
	@reg <- @reg and @num <= i_type(@4, @2, $7, @0, $13)
	@reg <- @reg or @num <= i_type(@4, @2, $6, @0, $13)
	@reg <- @csr <= i_type(@2:value, %zero, $2, @0, $73)
	@csr <- @reg <= i_type(@0:value, @2, $1, %zero, $73)

	@reg <- [@reg + @num] <= i_type(@5, @3, $2, @0, $03)
	@reg <- [@reg] <= @0 <- [@3 + 0]
	@reg <- [@reg - @num] <= @0 <- [@3 + (0 - @5)]
	@reg <-b [@reg + @num] <= i_type(@6, @4, $0, @0, $03)
	@reg <-b [@reg] <= @0 <-b [@4 + 0]
	@reg <-b [@reg - @num] <= @0 <-b [@4 + (0 - @6)]
	@reg <-h [@reg + @num] <= i_type(@6, @4, $1, @0, $03)
	@reg <-h [@reg] <= @0 <-h [@4 + 0]
	@reg <-h [@reg - @num] <= @0 <-h [@4 + (0 - @6)]
	@reg <-bu [@reg + @num] <= i_type(@6, @4, $4, @0, $03)
	@reg <-bu [@reg] <= @0 <-bu [@4 + 0]
	@reg <-bu [@reg - @num] <= @0 <-bu [@4 + (0 - @6)]
	@reg <-hu [@reg + @num] <= i_type(@6, @4, $5, @0, $03)
	@reg <-hu [@reg] <= @0 <-hu [@4 + 0]
	@reg <-hu [@reg - @num] <= @0 <-hu [@4 + (0 - @6)]

	[@reg + @num] <- @reg <= s_type(@3, @6, @1, $2, $23)
	[@reg] <- @reg <= [@1 + 0] <- @4
	[@reg - @num] <- @reg <= [@1 + (0 - @3) <- @6
	[@reg + @num] <-b @reg <= s_type(@3, @7, @1, $0, $23)
	[@reg] <-b @reg <= [@1 + 0] <-b @5
	[@reg - @num] <-b @reg <= [@1 + (0 - @3) <-b @7
	[@reg + @num] <-h @reg <= s_type(@3, @7, @1, $1, $23)
	[@reg] <-h @reg <= [@1 + 0] <-h @5
	[@reg - @num] <-h @reg <= [@1 + (0 - @3) <-h @7

	goto @num <= %pc <- %pc + ( @1 - `* )
	if @reg @str 0: <= if @1 @2 %zero:
	if @reg < @reg: %pc <- %pc + @num <= b_type(@9, @3, @1, $4, $63)
	if @reg > @reg: <= if @3 < @1:
	if @reg >= @reg: %pc <- %pc + @num <= b_type(@9, @3, @1, $5, $63)
	if @reg `<= @reg: <= if @3 >= @1:
	if @reg <u @reg: %pc <- %pc + @num <= b_type(@9, @3, @1, $6, $63)
	if @reg >u @reg: <= if @3 <u @1:
	if @reg >=u @reg: %pc <- %pc + @num <= b_type(@9, @3, @1, $7, $63)
	if @reg `<=u @reg: <= if @3 >=u @1:
	if @reg = @reg: %pc <- %pc + @num <= b_type(@9, @3, @1, $0, $63)
	if @reg != @reg: %pc <- %pc + @num <= b_type(@9, @3, @1, $1, $63)

	%pc <- %pc - @num <= %pc <- %pc + (0 - @4)
	@reg <- %pc, %pc <- %pc + @num <= j_type(@8, @0, $6f)
	%pc <- %pc + @num <= %zero <- %pc, %pc <- %pc + @4
	%pc <- %pc <= %pc <- %pc + 0
	@reg <- %pc, %pc <- @reg + @num <= i_type(@8, @6, $0, @0, $67)
	@reg <- %pc, %pc <- @reg - @num <= @0 <- %pc, %pc <- @6 + (0 - @8)
	@reg <- %pc, %pc <- @reg <= @0 <- %pc, %pc <- @6 + 0
	%pc <- @reg <= %zero <- %pc, %pc <- @2 + 0

	@reg <- %pc + @num <= u_type(@4 and $fffff000, @0, $17) small_pc(@0, @4 and $fff)
	small_pc(@reg, 0) <=
	small_pc(@reg, @num) <= @2 <- @2 + @4
	@reg <- %pc <= @0 <- %pc + 0

	@reg <- @reg < @num <= i_type(@4, @2, $2, @0, $13)
	@reg <- @reg <u @num <= i_type(@5, @2, $3, @0, $13)
	@reg <- @reg xor @num <= i_type(@4, @2, $4, @0, $13)
	@reg <- @reg < @reg <= r_type($0, @4, @2, $2, @0, $33)
	@reg <- @reg <u @reg <= r_type($0, @5, @2, $3, @0, $33)
	@reg <- @reg xor @reg <= r_type($0, @4, @2, $4, @0, $33)
	@reg <- complement @reg <= @0 <- @3 xor 0
	@reg <- @reg != 0 <= @0 <- %zero <u @2
	@reg <- @reg > @reg <= @0 <- @4 < @2
	@reg <- @reg >u @reg <= @0 <- @5 <u @2
	@reg <- @reg and @reg <= r_type($0, @4, @2, $f, @0, $33)
	@reg <- @reg or @reg <= r_type($0, @4, @2, $e, @0, $33)
	@reg <- @reg << @num <= i_type(@4 and $1f, @2, $1, @0, $13)
	@reg <- @reg >> @num <= i_type(@4 and $1f, @2, $5, @0, $13)
	@reg <- @reg >>> @num <= i_type((@4 and $1f) or $200, @2, $5, @0, $13)
	@reg <- @reg << @reg <= r_type($0, @4, @2, $1, @0, $33)
	@reg <- @reg >> @reg <= r_type($0, @4, @2, $5, @0, $33)
	@reg <- @reg >>> @reg <= r_type($20, @4, @2, $5, @0, $33)

	fence(@num, @num) <= i_type(@2 << 4 or @4, %zero, $0, %zero, $0f)
	fence.i <= i_type(0, %zero, $1, %zero, $0f)

	trap <= i_type($0, %zero, 0, %zero, $73)
	break <= i_type($1, %zero, 0, %zero, $73)

# pseudo-instructions

	nop <= %x0 <- %x0 + 0

# basic extensions
	
	@str: <= @0 <= `*
	raw @num <= @raw:@1
	@reg => @str <= @2 <= @0 `. @0
@End(file: default.ll)
```
