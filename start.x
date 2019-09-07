
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
	void assert_line(const char *line, int expected) {
		std::cerr << "ignoring " << line << "\n";
	}
	void assert_line_2(
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
	assert_line_2(
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
	assert_line_2(
		"%x4 <- %x2 + %x3",
		0x00310233
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
	class Item;
	
	using Item_Ptr = std::unique_ptr<Item>;
	using Items = std::vector<Item_Ptr>;
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
@add(private state)
	std::vector<Macro> _macros;
	void setup_symbols();
@end(private state)
```

```
@add(state impl)
	void State::setup_symbols() {
		@put(setup symbols);
	}
@end(state impl)
```

```
@add(public state)
	State() { setup_symbols(); }
@end(public state)
```

```
@def(setup symbols)
	#include "default.h"
	std::istringstream in { setup };
	std::string l;
	while (std::getline(in, l)) {
		if (l.empty()) { continue; }
		add_line(l);
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
	std::vector<std::unique_ptr<Item>> items;
	auto end { line.end() };
	auto cur { line.begin() };
	for (;;) {
		while (cur < end && *cur <= ' ') {
			++cur;
		}
		if (cur == end) { break; }

		auto begin { cur };
		if (isalpha(*cur) || *cur == '_' || *cur == '%') {
			while (cur < end && (isalnum(*cur) || *cur == '_' || *cur == '%')) {
				++cur;
			}
			items.emplace_back(new Named_Item { { begin, cur } } );
		} else if (isdigit(*cur)) {
			int value { 0 };
			while (cur < end && isdigit(*cur)) {
				value = value * 10 + (*cur++ - '0');
			}
			items.emplace_back(new Type_Instance_Item { "num", value });
		} else if (*cur == '#') {
			break;
		} else if (*cur == '@') {
			++cur;
			std::string name;
			while (cur < end && isalnum(*cur)) {
				name += *cur++;
			}
			items.emplace_back(new Type_Item { name });
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
			items.emplace_back(new Type_Instance_Item { "num", value });
		} else if (ispunct(*cur)) {
			while (cur < end && ispunct(*cur) && *cur != '$' && *cur != '#' && *cur != '@' && *cur != '_' && *cur != '%') {
				++cur;
			}
			items.emplace_back(new Named_Item { { begin, cur } });
		}
	}
	@put(expand);
@end(add line)
```

```
@add(unit-tests)
	assert_line_2(
		"%pc <- %pc",
		0x0000006f
	);
@end(unit-tests)
```
* check for simple endless loop


```
@add(unit-tests)
	assert_line_2(
		"%pc <- %pc - 28",
		0xfe5ff06f
	);
@end(unit-tests)
```
* check for simple endless loop

```
@add(unit-tests)
	assert_line_2(
		"%pc <- %pc - 32",
		0xfe1ff06f
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
	assert_line_2(
		"%x5 <- %x5 and $ff",
		0x0ff2f293
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line_2(
		"%x5 <- %x5 or $1",
		0x0012e293
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line_2(
		"%x6 <- %x6 or $1",
		0x00136313
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line_2(
		"%x11 <- $0d",
		0x00d00593
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line_2(
		"%x12 <- $0a",
		0x00a00613
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line_2(
		"%x10 <- $1013000",
		0x1013537
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"%x5 <- %pc",
		0x00000297
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"%mtvec <- %x5",
		0x30529073
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line_2(
		"%x5 <- %mhartid",
		0xf14022f3
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"if %x5 < 0: %pc <- %pc + -4",
		0xfe02cee3
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"if %x5 = 0: %pc <- %pc + -12",
		0xfe028ae3
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"if %x5 != %x11: %pc <- %pc + -28",
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
@add(needed by state)
	class Item {
		public:
			virtual ~Item() {};
			virtual Item *clone() const = 0;
			virtual void write(std::ostream &out) const = 0;
			virtual bool matches(const Item &in) const {
				return false;
			}
	};
	std::ostream &operator<<(std::ostream &out, const Item &item) {
		item.write(out);
		return out;
	}
@end(needed by state)
```

```
@add(needed by state)
	class Named_Item: public Item {
		private:
			std::string _name;
		public:
			Named_Item(const std::string &name): _name { name } { }
			const std::string &name() const { return _name; }
			Item *clone() const override {
				return new Named_Item { _name };
			}
			void write(std::ostream &out) const override {
				out << _name;
			}
			bool matches(const Item &in) const override {
				auto o { dynamic_cast<const Named_Item *>(&in) };
				return o && (o->name() == _name || _name.empty());
			};
	};
	class Type_Instance_Item: public Item {
		private:
			std::string _type;
			int _value;
		public:
			Type_Instance_Item(const std::string &type, int value):
				_type { type }, _value { value }
			{ }
			const std::string &type() const { return _type; }
			int value() const { return _value; }
			Item *clone() const override {
				return new Type_Instance_Item { _type, _value };
			}
			void write(std::ostream &out) const override {
				out << '@' << _type << ":$" << std::hex << _value << std::dec;
			}
			bool matches(const Item &in) const override {
				auto o { dynamic_cast<const Type_Instance_Item *>(&in) };
				return o && _type == o->type() && o->value() == _value;
			}
	};
	class Type_Item: public Item {
		private:
			std::string _type;
		public:
			Type_Item(const std::string &type): _type { type } { }
			const std::string &type() const { return _type; }
			Item *clone() const override {
				return new Type_Item { _type };
			}
			void write(std::ostream &out) const override {
				out << '@' << _type;
			}
			bool matches(const Item &in) const override {
				auto o { dynamic_cast<const Type_Item *>(&in) };
				if (o && o->type() == _type) { return true; };
				auto i { dynamic_cast<const Type_Instance_Item *>(&in) };
				if (i && i->type() == _type) { return true; }
				return false;
			};
	};
@end(needed by state)
```

```
@add(needed by state)
	class I_Type_Item: public Item {
		private:
			int _immediate;
			int _rs1;
			int _func3;
			int _rd;
			int _opcode;
		public:
			I_Type_Item(
				int immediate, int rs1, int func3,
				int rd, int opcode
			):
				_immediate { immediate },
				_rs1 { rs1 },
				_func3 { func3 },
				_rd { rd },
				_opcode { opcode }
			{ }
			int immediate() const { return _immediate; }
			int rs1() const { return _rs1; }
			int func3() const { return _func3; }
			int rd() const { return _rd; }
			int opcode() const { return _opcode; }
			Item *clone() const override {
				return new I_Type_Item {
					_immediate, _rs1, _func3,
					_rd, _opcode
				};
			}
			void write(std::ostream &out) const override {
				out << "#i_type(" << _immediate << ", " << _rs1 <<
					", " << _func3 << ", " << _rd << ", " <<
					_opcode << ')';
			}
	};
@end(needed by state)
```

```
@add(needed by state)
	class U_Type_Item: public Item {
		private:
			int _immediate;
			int _rd;
			int _opcode;
		public:
			U_Type_Item(
				int immediate,
				int rd, int opcode
			):
				_immediate { immediate },
				_rd { rd },
				_opcode { opcode }
			{ }
			int immediate() const { return _immediate; }
			int rd() const { return _rd; }
			int opcode() const { return _opcode; }
			Item *clone() const override {
				return new U_Type_Item {
					_immediate,
					_rd, _opcode
				};
			}
			void write(std::ostream &out) const override {
				out << "#u_type(" << _immediate << ", " << _rd << ", " << _opcode << ')';
			}
	};
@end(needed by state)
```

```
@def(expand)
restart:
	#if 0
		std::cerr << "LINE {";
		for (const auto &i : items) {
			std::cerr << '[' << *i << "], ";
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
			line << "]\n";
	}
@end(expand)
```

```
@def(transform)
	if (i + 2 < items.size()) {
		auto t { dynamic_cast<Type_Item *>(&*items[i]) };
		auto c { dynamic_cast<Named_Item *>(&*items[i + 1]) };
		if (t && c && c->name() == ":") {
			auto v { dynamic_cast<Type_Instance_Item *>(&*items[i + 2]) };
			if (v && v->type() == "num") {
				std::string type { t->type() };
				int value { v->value() };
				items.erase(items.begin() + i, items.begin() + i + 3);
				items.emplace(items.begin() + i, new Type_Instance_Item { type, value });
				goto restart;
			}
		}
		auto o { dynamic_cast<Type_Instance_Item *>(&*items[i]) };
		if (o && c && c->name() == ":") {
			auto n { dynamic_cast<Named_Item *>(&*items[i + 2]) };
			if (n && n->name() == "value") {
				int value { o->value() };
				items.erase(items.begin() + i, items.begin() + i + 3);
				items.emplace(items.begin() + i, new Type_Instance_Item { "num", value });
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
		matches = matches && (**p).matches(*items[i + j]);
	}
	if (matches) {
		auto k { i };
		i += macro->pattern().size();
		for (const auto &e : macro->replacement()) {
			auto para { dynamic_cast<Type_Item *>(&*e) };
			if (para && ! para->type().empty()) {
				if (isdigit(para->type()[0])) {
					int idx { std::stoi(para->type()) };
					if (idx >= 0 && idx < (int) macro->pattern().size()) {
						items.emplace(
							items.begin() + i, items[k + idx]->clone()
						);
						++i;
						continue;
					}
				} else if (para->type() == "arithmetic") {
					auto n1 { dynamic_cast<Type_Instance_Item *>(&*items[k + 0]) };
					auto op { dynamic_cast<Named_Item *>(&*items[k + 1]) };
					auto n2 { dynamic_cast<Type_Instance_Item *>(&*items[k + 2]) };
					if (n1 && n1->type() == "num" && op && n2 && n2->type() == "num") {
						if (op->name() == "+") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() + n2->value() }
							);
							++i;
							continue;
						} else if (op->name() == "-") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() - n2->value() }
							);
							++i;
							continue;
						} else if (op->name() == "*") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() * n2->value() }
							);
							++i;
							continue;
						} else if (op->name() == "/" && n2->value() != 0) {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() / n2->value() }
							);
							++i;
							continue;
						} else if (op->name() == "and") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() & n2->value() }
							);
							++i;
							continue;
						} else if (op->name() == "or") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() | n2->value() }
							);
							++i;
							continue;
						} else if (op->name() == "<<") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() << n2->value() }
							);
							++i;
							continue;
						} else if (op->name() == ">>") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() >> n2->value() }
							);
							++i;
							continue;
						} else if (op->name() == "xor") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() ^ n2->value() }
							);
							++i;
							continue;
						}
					}
				}
			}
			items.emplace(
				items.begin() + i, e->clone()
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
	auto *ni {
		dynamic_cast<Named_Item *>(
			&*items[i]
	) };
	if (ni) {
		@put(transform named);
	}
} @end(transform)
```

```
@def(transform named)
	if (ni->name() == "raw") {
		@put(transform raw);
	}
@end(transform named)
```

```
@def(transform raw)
	if (i + 1 < items.size()) {
		auto *n2 {
			dynamic_cast<Type_Instance_Item *>(
				&*items[i + 1]
		) };
		if (n2 && n2->type() == "num") {
			@put(do transform raw);
		}
	}
@end(transform raw)
```

```
@def(do transform raw)
	int value { n2->value() };
	items.erase( items.begin() + i,
		items.begin() + i + 2
	);
	items.emplace(items.begin() + i,
		new Type_Instance_Item { "raw", value }
	);
	goto restart;
@end(do transform raw)
```

```
@add(transform named)
	if (ni->name() == "*") {
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
		new Type_Instance_Item { "num", addr }
	);
	goto restart;
@end(transform cur addr)
```

```
@add(transform) {
	auto *ri {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i]
	) };
	if (ri && ri->type() == "reg") {
		int rd { ri->value() };
		@put(transform reg);
	}
} @end(transform)
```

```
@def(transform reg)
	if (i + 2 < items.size()) {
		auto *t2 {
			dynamic_cast<Named_Item *>(
				&*items[i + 1]
		) };
		if (t2 && t2->name() == "<-") {
			@put(transform reg assign);
		}
	}
@end(transform reg)
```

```
@def(transform reg assign) {
	auto *n3 {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i + 2]
	) };
	if (n3 && n3->type() == "num") {
		@put(transform reg assign num);
		i = 0; continue;
	}
} @end(transform reg assign)
```

```
@def(transform reg assign num)
	int v { n3->value() };
	items.erase(items.begin() + i,
		items.begin() + i + 3
	);
@end(transform reg assign num)
```

```
@add(transform reg assign num) {
	int up { v & ~ 0xfff };
	if (up != 0 && up != ~ 0xfff) {
		items.emplace(items.begin() + i,
			new U_Type_Item {
				up, rd, 0x37
		});
		++i;
	}
} @end(transform reg assign num)
```

```
@add(transform reg assign num) {
	int low { v & 0xfff };
	if (
		(low && (low != 0xfff)) || v == 0
	) {
		items.emplace(items.begin() + i,
			new I_Type_Item {
				low, 0, 0x0, rd, 0x13
			}
		);
	}
	goto restart;
} @end(transform reg assign num)
```

```
@add(transform reg assign) {
	auto c3 {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i + 2]
	) };
	if (c3 && c3->type() == "csr") {
		@put(transform reg assign csr);
	}
} @end(transform reg assign)
```

```
@def(transform reg assign csr)
	int cv { c3->value() };
	items.erase(items.begin() + i,
		items.begin() + i + 3
	);
	items.emplace(items.begin() + i,
		new I_Type_Item {
			cv, 0, 0x2, rd, 0x73
		}
	);
	goto restart;
@end(transform reg assign csr)
```

```
@add(transform reg assign) {
	auto rs1 {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i + 2]
		)
	};
	if (rs1 && rs1->type() == "reg") {
		int rs1_nr { rs1->value() };
		@put(transform reg assign reg);
	}
} @end(transform reg assign)
```

```
@def(transform reg assign reg)
	if (i + 4 < items.size()) {
		auto n3 {
			dynamic_cast<Named_Item *>(
				&*items[i + 3]
			)
		};
		if (n3) {
			@put(transform reg assign op);
		}
	}
@end(transform reg assign reg)
```

```
@def(transform reg assign op)
	if (n3->name() == "or") {
		@put(transform or);
	}
@end(transform reg assign op)
```

```
@def(transform or) {
	auto n4 {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i + 4]
		)
	};
	if (n4 && n4->type() == "num") {
		@put(transform or imm);
	}
} @end(transform or)
```

```
@def(transform or imm)
	int imm { n4->value() };
	items.erase(items.begin() + i,
		items.begin() + i + 5
	);
	items.emplace(items.begin() + i,
		new I_Type_Item {
			imm, rs1_nr, 0x6, rd, 0x13
		}
	);
	goto restart;
@end(transform or imm)
```

```
@add(transform reg assign op)
	if (n3->name() == "and") {
		@put(transform and);
	}
@end(transform reg assign op)
```

```
@def(transform and) {
	auto n4 {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i + 4]
		)
	};
	if (n4 && n4->type() == "num") {
		@put(transform and imm);
	}
} @end(transform and)
```

```
@def(transform and imm)
	int imm { n4->value() };
	items.erase(items.begin() + i,
		items.begin() + i + 5
	);
	items.emplace(items.begin() + i,
		new I_Type_Item {
			imm, rs1_nr, 0x7, rd, 0x13
		}
	);
	goto restart;
@end(transform and imm)
```

```
@add(transform) {
	auto pi {
		dynamic_cast<Named_Item *>(
			&*items[i]
		)
	};
	if (pi && pi->name() == "%pc") {
		@put(transform pc);
	}
} @end(transform)
```

```
@def(transform pc)
	if (i + 2 < items.size()) {
		auto n2 {
			dynamic_cast<Named_Item *>(
				&*items[i + 1]
			)
		};
		if (n2 && n2->name() == "<-") {
			@put(transform pc assgn);
		}
	}
@end(transform pc)
```

```
@def(transform pc assgn)
	auto p3 {
		dynamic_cast<Named_Item *>(
			&*items[i + 2]
		)
	};
	if (p3 && p3->name() == "%pc") {
		@put(transform pc assgn pc);
	}
@end(transform pc assgn)
```

```
@def(transform pc assgn pc)
	if (i + 4 < items.size()) {
		auto n4 {
			dynamic_cast<Named_Item *>(
				&*items[i + 3]
			)
		};
		if (n4 && (
			n4->name() == "+" ||
			n4->name() == "-"
		)) {
			bool neg {
				n4->name() == "-"
			};
			@put(transform pc pm);
		}
	}
@end(transform pc assgn pc)
```

```
@add(needed by state)
	class J_Type_Item: public Item {
		private:
			int _immediate;
			int _rd;
			int _opcode;
		public:
			J_Type_Item(
				int immediate,
				int rd, int opcode
			):
				_immediate { immediate },
				_rd { rd },
				_opcode { opcode }
			{ }
			int immediate() const { return _immediate; }
			int rd() const { return _rd; }
			int opcode() const { return _opcode; }
			Item *clone() const override {
				return new J_Type_Item {
					_immediate,
					_rd, _opcode
				};
			}
			void write(std::ostream &out) const override {
				out << "#j_type(" << _immediate << ", " << _rd << ", " << _opcode << ')';
			}
	};
@end(needed by state)
```

```
@def(transform pc pm)
	auto n5 {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i + 4]
		)
	};
	if (n5 && n5->type() == "num") {
		int value { n5->value() };
		if (neg) { value = -value; }
		items.erase(items.begin() + i,
			items.begin() + i + 5
		);
		items.emplace(items.begin() + i,
			new J_Type_Item(
				value, 0, 0x6f
			)
		);
		goto restart;
	}
@end(transform pc pm)
```

```
@add(transform pc assgn pc)
	if (items.begin() + i + 3 == items.end()) {
		items.emplace(items.begin() + i + 3,
			new Named_Item { "+" }
		);
		items.emplace(items.begin() + i + 4,
			new Type_Instance_Item { "num", 0 }
		);
		goto restart;
	}
@end(transform pc assgn pc)
```

```
@add(transform) {
	auto ii {
		dynamic_cast<I_Type_Item *>(
			&*items[i]
		)
	};
	if (ii) {
		@put(transform i type);
	}
} @end(transform)
```

```
@def(transform i type)
	int result {
		(ii->immediate() << 20) |
		(ii->rs1() << 15) |
		(ii->func3() << 12) |
		(ii->rd() << 7) | ii->opcode()
	};
	items.erase(items.begin() + i,
		items.begin() + i + 1
	);
	items.emplace(items.begin() + i,
		new Type_Instance_Item { "raw", result }
	);
	i = 0; continue;
@end(transform i type)
```

```
@add(transform) {
	auto ui {
		dynamic_cast<U_Type_Item *>(
			&*items[i]
		)
	};
	if (ui) {
		@put(transform u type);
		i = 0; continue;
	}
} @end(transform)
```

```
@def(transform u type)
	int result {
		ui->immediate() |
		(ui->rd() << 7) | ui->opcode()
	};
	items.erase(items.begin() + i,
		items.begin() + i + 1
	);
	items.emplace(items.begin() + i,
		new Type_Instance_Item { "raw", result }
	);
@end(transform u type)
```

```
@add(transform) {
	auto ji {
		dynamic_cast<J_Type_Item *>(
			&*items[i]
		)
	};
	if (ji) {
		@put(transform j type);
	}
} @end(transform)
```

```
@def(transform j type)
	int imm { ji->immediate() };
	int result = 
		((imm << (31 - 20)) & 0x80000000) |
		((imm << (21 - 1)) & 0x7fe00000) |
		((imm << (20 - 11)) & 0x00100000) |
		(imm & 0x000ff000) |
		(ji->rd() << 7) | ji->opcode();
	items.erase(items.begin() + i,
		items.begin() + i + 1
	);
	items.emplace(items.begin() + i,
		new Type_Instance_Item { "raw", result }
	);
@end(transform j type)
```

```
@add(transform named)
	if (ni->name() == "goto") {
		if (i + 1 < items.size()) {
			@put(transform goto);
		}
	}
@end(transform named)
```

```
@def(transform goto)
	auto n2 {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i + 1]
		)
	};
	if (n2 && n2->type() == "num") {
		int target {
			n2->value()
		};
		items.erase(items.begin() + i,
			items.begin() + i + 2
		);
		items.emplace(items.begin() + i,
			new Named_Item { "%pc" }
		);
		items.emplace(items.begin() + i + 1,
			new Named_Item { "<-" }
		);
		items.emplace(items.begin() + i + 2,
			new Named_Item { "%pc" }
		);
		items.emplace(items.begin() + i + 3,
			new Named_Item { "+" }
		);
		items.emplace(items.begin() + i + 4,
			new Named_Item { "(" }
		);
		items.emplace(items.begin() + i + 5,
			new Type_Instance_Item { "num", target }
		);
		items.emplace(items.begin() + i + 6,
			new Named_Item { "-" }
		);
		items.emplace(items.begin() + i + 7,
			new Named_Item { "*" }
		);
		items.emplace(items.begin() + i + 8,
			new Named_Item { ")" }
		);
		goto restart;
	}
@end(transform goto)
```

```
@add(transform named)
	if (i + 1 < items.size()) {
		auto ci {
			dynamic_cast<Named_Item *>(
				&*items[i + 1]
			)
		};
		if (ci && ci->name() == ":") {
			std::string name {
				ni->name()
			};
			items.erase(
				items.begin() + i,
				items.begin() + i + 2
			);
			items.emplace(items.begin() + i,
				new Named_Item { name }
			);
			items.emplace(items.begin() + i + 1,
				new Named_Item { "=" }
			);
			items.emplace(items.begin() + i + 2,
				new Named_Item { "*" }
			);
		}
	}
@end(transform named)
```

```
@def(consume machine instrs)
	while (! items.empty() &&
		dynamic_cast<Type_Instance_Item *>(
			&**items.begin()
	)) {
		auto &mi {
			dynamic_cast<Type_Instance_Item &>(
				**items.begin()
		)};
		if (mi.type() != "raw") { break; }
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
		auto a { dynamic_cast<Named_Item *>(&*items[i]) };
		if (a && a->name() == "=") {
			@put(transform sym assign);
		}
	}
@end(handle define)
```

```
@def(transform sym assign)
	Items value;
	for (unsigned j = i + 1;
		j < items.size(); ++j
	) {
		value.push_back(std::move(items[j]));
	}
	Items p;
	for (unsigned j = 0; j < i; ++j) {
		p.push_back(std::move(items[j]));
	}
	_macros.emplace_back(std::move(p), std::move(value));
	items.erase(
		items.begin(), items.end()
	);
@end(transform sym assign)
```

