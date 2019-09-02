
* start low-level with the first unit-test

```
@Def(file: ll.cpp)
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
@def(needed by main)
	#include <cassert>
@end(needed by main)
```
* the unit-tests need the `@f(assert)`-macro

## Parsing a single line
* the first test focuses on parsing a single line
* other code is responsible for reading files and extract the low-level
  lines from it

The parsing is part of a `State` object that contains all generated
machine instructions.

```
@add(needed by main)
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
		@put(init assert line);
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
@add(needed by state)
	@put(needed by tokenizer)
	class Tokenizer {
			@put(tokenizer attributes);
		public:
			@put(tokenizer methods);
	};
	@put(tokenizer impl);
@end(needed by state)
```
* defines class for tokenizer

```
@def(needed by tokenizer)
	enum class Token_Type {
		unknown,
		@put(token types)
		end
	};
@end(needed by tokenizer)
```
* type of parsed token

```
@add(needed by tokenizer)
	class Token {
		private:
			Token_Type _type;
			std::string _name = { };
			int _value = { } ;
		public:
			Token(Token_Type type): _type { type } { }
			Token(Token_Type type, std::string &name): _type { type }, _name { name } { }
			Token(Token_Type type, int value): _type { type }, _value { value } { }
			Token_Type type() const { return _type; }
			const std::string &name() const { return _name; }
			int value() const { return _value; }
	};
@end(needed by tokenizer)
```

```
@def(tokenizer attributes)
	Token _token { Token_Type::unknown };
@end(tokenizer attributes)
```
* stores type of parsed token

```
@def(tokenizer methods)
	const Token &token() const {
		return _token;
	}
@end(tokenizer methods)
```
* get type of parsed token

```
@add(tokenizer attributes)
	std::string::const_iterator _cur;
	std::string::const_iterator _end;
@end(tokenizer attributes)
```
* chars not yet parsed

```
@add(tokenizer methods)
	void next();
@end(tokenizer methods)
```
* advances to next token

```
@add(tokenizer methods)
	Tokenizer(const std::string &s):
		_cur { s.begin() },
		_end { s.end() } 
	{
		next();
	}
@end(tokenizer methods)
```
* initialize chars and read first token

```
@def(tokenizer impl)
	void Tokenizer::next() {
		@put(next);
	}
@end(tokenizer impl)
```
* implementation of `@f(next)`

```
@def(next)
	while (
		_cur != _end && *_cur <= ' '
	) {
		++_cur;
	}
@end(next)
```
* skip spaces

```
@add(next)
	if (_cur == _end) {
		_token = Token { Token_Type::end };
		return;
	}
@end(next)
```
* end is reached

```
@add(needed by tokenizer)
	#include <iostream>
@end(needed by tokenizer)
```
* needs `cerr`

```
@add(next)
	do {
		@put(recognize);
		_token = Token { Token_Type::unknown };
		std::cerr <<
			"unrecognized char [" <<
			*_cur << "] == " << (int) *_cur << '\n';
		++_cur;
	} while (false);
@end(next)
```
* recognize individual tokens
* write warning when not recognizing token

```
@def(token types)
	ident,
@end(token types)
```

```
@def(recognize)
	if (isalpha(*_cur) || *_cur == '%' || *_cur == '_') {
		auto c = _cur;
		std::string name {};
		while (c != _end && (
			isalnum(*c) || *c == '_' ||
			*c == '%'
		)) {
			name += *c++;
		}
		Token_Type type { Token_Type::ident };
		@put(recognize keywords);
		_token = Token { type, name };
		_cur = c;
		break;
	}
@end(recognize)
```
* recognize register

```
@add(token types)
	t_raw,
@end(token types)
```

```
@def(recognize keywords)
	if (name == "raw") {
		type = Token_Type::t_raw;
	}
@end(recognize keywords)
```

```
@add(recognize)
	if (*_cur == '#') {
		_token = Token { Token_Type::end };
		break;
	}
@end(recognize)
```

```
@add(token types)
	number,
@end(token types)
```

```
@add(recognize)
	if (isdigit(*_cur)) {
		int value { 0 };
		while (isdigit(*_cur)) {
			value = value * 10 +
				(*_cur - '0');
			++_cur;
		}
		_token = Token { Token_Type::number, value };
		break;
	}
@end(recognize)
```

```
@add(recognize)
	if (*_cur == '$') {
		int value { 0 };
		++_cur;
		while (isxdigit(*_cur)) {
			int digit;
			if (*_cur <= '9') {
				digit = *_cur - '0';
			} else if (*_cur <= 'F') {
				digit = *_cur - 'A' + 10;
			} else {
				digit = *_cur - 'a' + 10;
			}
			value = (value << 4) + digit;
			++_cur;
		}
		_token = Token { Token_Type::number, value };
		break;
	}
@end(recognize)
```

```
@add(token types)
	becomes,
	t_less,
@end(token types)
```
* add token for assignment

```
@add(needed by main)
	void assert_token(
		const char *line,
		Token_Type token
	) {
		@put(assert token);
	}
@end(needed by main)
```
* check for a single token

```
@add(unit-tests)
	assert_token(
		"<-", Token_Type::becomes
	);
@end(unit-tests)
```
* try to recognize assignment

```
@def(assert token)
	@put(init assert token);
	Tokenizer t(line);
	assert(t.token().type() == token);
	t.next();
	assert(t.token().type() == Token_Type::end);
@end(assert token)
```
* initialize tokenizer and verify that it only contains the one expected
  token

```
@add(recognize)
	if (*_cur == '<') {
		if (
			_cur  + 1 != _end &&
				_cur[1] == '-'
		) {
			_token = Token { Token_Type::becomes };
			_cur += 2;
			break;
		} else {
			_token = Token { Token_Type::t_less };
			++_cur;
			break;
		}
	}
@end(recognize)
```
* recognize assignment

```
@add(token types)
	plus, minus,
@end(token types)
```
* add token for addition

```
@add(unit-tests)
	assert_token("+", Token_Type::plus);
	assert_token("-", Token_Type::minus);
@end(unit-tests)
```
* try to recognize addition

```
@add(recognize)
	if (*_cur == '+') {
		_token = Token { Token_Type::plus };
		++_cur;
		break;
	}
	if (*_cur == '-') {
		_token = Token { Token_Type::minus };
		++_cur;
		break;
	}
@end(recognize)
```
* recognize addition

```
@add(needed by main)
	void assert_register(
		const char *line,
		const char *name
	) {
		@put(assert register);
	}
@end(needed by main)
```
* check for a register token

```
@def(assert register)
	Tokenizer t(line);
	assert(t.token().type() == Token_Type::ident);
	assert(t.token().name() == name);
	t.next();
	assert(t.token().type() == Token_Type::end);
@end(assert register)
```
* check for a register token with specific name

```
@add(unit-tests)
	assert_register("%x10", "%x10");
	assert_register("%pc", "%pc");
@end(unit-tests)
```
* verify some register names

```
@add(needed by tokenizer)
	#include <cctype>
@end(needed by tokenizer)
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
		"%x4 <- %x2 + %x3",
		0x00310233
	);
@end(unit-tests)
```
* the first test checks that a simple register add can be parsed

```
@def(needed by add line)
	#include <map>
@end(needed by add line)
```

```
@add(needed by add line)
	class Item;
	static std::map<std::string, std::vector<std::unique_ptr<Item>>> _symbols;
@end(needed by add line)
```

```
@add(needed by add line)
	@put(needed by clear symbols);
	void clear_symbols() {
		_symbols.clear();
		@put(clear symbols);
	}
@end(needed by add line)
```

```
@def(init assert line)
	clear_symbols();
@end(init assert line)
```

```
@def(init assert token)
	clear_symbols();
@end(init assert token)
```

```
@def(clear symbols)
	{
		auto &l { _symbols["%pc"] };
		l.emplace_back(new Pc_Item { });
	} {
		auto &l { _symbols["%mtvec"] };
		l.emplace_back(new Csr_Item { 0x305 });
	} {
		auto &l { _symbols["%mhartid"] };
		l.emplace_back(new Csr_Item { 0xf14 });
	} {
		char name[] = "%x#";
		for (int i = 0; i < 10; ++i) {
			name[2] = '0' + i;
			auto &l { _symbols[name] };
			l.emplace_back(new Register_Item { i });
		}

	} {
		char name[] = "%x##";
		for (int i = 10; i < 32; ++i) {
			name[2] = '0' + (i / 10);
			name[3] = '0' + (i % 10);
			auto &l { _symbols[name] };
			l.emplace_back(new Register_Item { i });
		}
	}
@end(clear symbols)
```

```
@add(clear symbols)
	{
		auto &l { _symbols["%zero"] };
		l.emplace_back(new Register_Item { 0 });
	} {
		auto &l { _symbols["%ra"] };
		l.emplace_back(new Register_Item { 1 });
	} {
		auto &l { _symbols["%sp"] };
		l.emplace_back(new Register_Item { 2 });
	} {
		auto &l { _symbols["%gp"] };
		l.emplace_back(new Register_Item { 3 });
	} {
		auto &l { _symbols["%tp"] };
		l.emplace_back(new Register_Item { 4 });
	} {
		char name[] = "%t#";
		for (int i = 0; i < 3; ++i) {
			name[2] = '0' + i;
			auto &l { _symbols[name] };
			l.emplace_back(new Register_Item { i + 5 });
		}
	} {
		char name[] = "%s#";
		for (int i = 0; i < 2; ++i) {
			name[2] = '0' + i;
			auto &l { _symbols[name] };
			l.emplace_back(new Register_Item { i + 8 });
		}
	} {
		auto &l { _symbols["%fp"] };
		l.emplace_back(new Register_Item { 8 });
	} {
		char name[] = "%a#";
		for (int i = 0; i < 8; ++i) {
			name[2] = '0' + i;
			auto &l { _symbols[name] };
			l.emplace_back(new Register_Item { i + 10 });
		}
	} {
		char name[] = "%s#";
		for (int i = 2; i < 10; ++i) {
			name[2] = '0' + i;
			auto &l { _symbols[name] };
			l.emplace_back(new Register_Item { i + 16 });
		}
	} {
		char name[] = "%s1#";
		for (int i = 10; i < 12; ++i) {
			name[3] = '0' + (i % 10);
			auto &l { _symbols[name] };
			l.emplace_back(new Register_Item { i + 16 });
		}
	} {
		char name[] = "%t#";
		for (int i = 3; i < 7; ++i) {
			name[2] = '0' + i;
			auto &l { _symbols[name] };
			l.emplace_back(new Register_Item { i + 25 });
		}
	}
@end(clear symbols)
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
	Tokenizer t { line };
	std::vector<Token> ts;
	while (t.token().type() != Token_Type::end) {
		ts.push_back(t.token());
		t.next();
	}
	auto cur { ts.begin() };
	@put(expand);
@end(add line)
```

```
@add(unit-tests)
	assert_line(
		"%pc <- %pc + 0",
		0x0000006f
	);
@end(unit-tests)
```
* check for simple endless loop


```
@add(unit-tests)
	assert_line(
		"%pc <- %pc + -28",
		0xfe5ff06f
	);
@end(unit-tests)
```
* check for simple endless loop

```
@add(unit-tests)
	assert_line(
		"%pc <- %pc + -32",
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
@add(token types)
	t_and,
	t_or,
@end(token types)
```

```
@add(needed by state)
	int build_and(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x7, dst, 0x33
		);
	}
@end(needed by state)
```

```
@add(needed by state)
	int build_and(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x7, dst, 0x13
		);
	}
@end(needed by state)
```

```
@add(needed by state)
	int build_or(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x6, dst, 0x33
		);
	}
@end(needed by state)
```

```
@add(needed by state)
	int build_or(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x6, dst, 0x13
		);
	}
@end(needed by state)
```

```
@add(unit-tests)
	assert_line(
		"%x5 <- %x5 and $ff",
		0x0ff2f293
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"%x5 <- %x5 or $1",
		0x0012e293
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"%x6 <- %x6 or $1",
		0x00136313
	);
@end(unit-tests)
```

```
@add(needed by state)
	int build_u_cmd(
		int imm, char dst, int opcode
	) {
		return (imm & 0xfffff800) | (dst << 7) | opcode;
	}
@end(needed by state)
```

```
@add(needed by state)
	int build_lui(
		char dst, int imm
	) {
		return build_u_cmd(imm, dst, 0x37);
	}
@end(needed by state)
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
@add(needed by state)
	int build_auipc(
		char dst, int imm
	) {
		return build_u_cmd(imm, dst, 0x17);
	}
@end(needed by state)
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
@add(needed by state)
	int build_csrrw(
		char dst, int csr, char src
	) {
		return build_i_cmd(
			csr, src, 0x1, dst, 0x73
		);
	}
@end(needed by state)
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
@add(needed by state)
	int build_csrrs(
		char dst, int csr, char src
	) {
		return build_i_cmd(
			csr, src, 0x2, dst, 0x73
		);
	}
@end(needed by state)
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
@add(token types)
	t_if,
	t_colon,
@end(token types)
```

```
@add(recognize keywords)
	if (name == "if") {
		type = Token_Type::t_if;
	}
	if (name == "and") {
		type = Token_Type::t_and;
	}
	if (name == "or") {
		type = Token_Type::t_or;
	}
@end(recognize keywords)
```

```
@add(recognize)
	if (*_cur == ':') {
		_token = Token { Token_Type::t_colon };
		++_cur;
		break;
	}
@end(recognize)
```

```
@add(needed by state)
	int build_b_cmd(
		int offset, int reg2, int reg1, int cond, int opcode
	) {
		return ((offset & 0x1000) << (31 - 12)) |
			(offset & 0x07e0) << (25 - 5) |
			(reg2 << 20) | (reg1 << 15) | (cond << 12) |
			(offset & 0x1e) << (8 - 1) |
			(offset & 0x0800) >> (11 - 7) |
			opcode;
	}
@end(needed by state)
```

```
@add(needed by state)
	int build_branch(
		int cond, char reg1, char reg2, int offset
	) {
		return build_b_cmd(
			offset, reg2, reg1, cond, 0x63
		);
	}
@end(needed by state)
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
@add(token types)
	t_equals,
	t_not_equals,
@end(token types)
```

```
@add(recognize)
	if (*_cur == '=') {
		_token = Token { Token_Type::t_equals };
		++_cur;
		break;
	}
@end(recognize)
```

```
@add(recognize)
	if (*_cur == '!' && _cur + 1 < _end && _cur[1] == '=') {
		_token = Token { Token_Type::t_not_equals };
		_cur += 2;
		break;
	}
@end(recognize)
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
@add(token types)
	t_open_bracket,
	t_close_bracket,
@end(token types)
```

```
@add(recognize)
	if (*_cur == '[') {
		_token = Token_Type::t_open_bracket;
		++_cur;
		break;
	}
@end(recognize)
```

```
@add(recognize)
	if (*_cur == ']') {
		_token = Token_Type::t_close_bracket;
		++_cur;
		break;
	}
@end(recognize)
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
@add(needed by state)
	int build_s_cmd(
		int imm, char rs2, char rs1, int type, int opcode
	) {
		return
			((imm & 0xfe0) << (25 - 5)) |
			(rs2 << 20) | (rs1 << 15) |
			(type << 12) |
			((imm & 0x1f) << 7) |
			opcode;
	}
@end(needed by state)
```

```
@add(needed by state)
	int build_store(
		char src, char dst, int imm
	) {
		return build_s_cmd(
			imm, src, dst, 0x2, 0x23
		);
	}
@end(needed by state)
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
@add(token types)
	t_open_parenthesis,
	t_close_parenthesis,
@end(token types)
```

```
@add(recognize)
	if (*_cur == '(') {
		_token = Token { Token_Type::t_open_parenthesis };
		++_cur;
		break;
	}
@end(recognize)
```

```
@add(recognize)
	if (*_cur == ')') {
		_token = Token { Token_Type::t_close_parenthesis };
		++_cur;
		break;
	}
@end(recognize)
```

```
@add(token types)
	t_times,
@end(token types)
```

```
@add(recognize)
	if (*_cur == '*') {
		_token = Token { Token_Type::t_times };
		++_cur;
		break;
	}
@end(recognize)
```

```
@def(needed by clear symbols)
	class Item {
		public:
			virtual ~Item() {};
			virtual Item *clone() const = 0;
	};
@end(needed by clear symbols)
```

```
@add(needed by clear symbols)
	class Token_Item: public Item {
		private:
			Token _token;
		public:
			Token_Item(const Token &t):
				_token { t }
			{ }
			const Token &token() const {
				return _token;
			}
			Item *clone() const override {
				return new Token_Item { _token };
			}
	};
@end(needed by clear symbols)
```

```
@add(needed by clear symbols)
	class Pc_Item: public Item {
		public:
			Item *clone() const override {
				return new Pc_Item { };
			}
	};
@end(needed by clear symbols)
```

```
@add(needed by clear symbols)
	class Register_Item: public Item {
		private:
			int _nr;
		public:
			Register_Item(int nr):
				_nr { nr }
			{ }
			int nr() const {
				return _nr;
			}
			Item *clone() const override {
				return new Register_Item { _nr };
			}
	};
@end(needed by clear symbols)
```

```
@add(needed by clear symbols)
	class Csr_Item: public Item {
		private:
			int _nr;
		public:
			Csr_Item(int nr):
				_nr { nr }
			{ }
			int nr() const {
				return _nr;
			}
			Item *clone() const override {
				return new Csr_Item { _nr };
			}
	};
@end(needed by clear symbols)
```

```
@add(needed by clear symbols)
	class Machine_Item: public Item {
		private:
			int _instruction;
		public:
			Machine_Item(int i):
				_instruction { i }
			{ }
			int instruction() const {
				return _instruction;
			}
			Item *clone() const override {
				return new Machine_Item { _instruction };
			}
	};
@end(needed by clear symbols)
```

```
@add(needed by clear symbols)
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
	};
@end(needed by clear symbols)
```

```
@add(needed by clear symbols)
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
	};
@end(needed by clear symbols)
```

```
@def(expand)
	std::vector<std::unique_ptr<Item>>
		items;
	@put(build items);
	unsigned i = 0;
	while (i < items.size()) {
		@put(transform);
		++i;
	}
	@put(consume machine instrs);
	if (! items.empty()) {
		std::cerr <<
			"can expand fully [" <<
			line << "]\n";
	}
@end(expand)
```

```
@def(build items)
	for (; cur != ts.end(); ++cur) {
		items.emplace_back(
			new Token_Item { *cur }
		);
	}
@end(build items)
```

```
@def(transform) {
	auto *ti {
		dynamic_cast<Token_Item *>(
			&*items[i]
	) };
	if (ti) {
		@put(transform tok);
	}
} @end(transform)
```

```
@def(transform tok)
	if (ti->token().type() ==
		Token_Type::ident
	) {
		@put(transform tok ident);
	}
@end(transform tok)
```

```
@def(transform tok ident)
	auto s {
		_symbols.find(ti->token().name())
	};
	if (s != _symbols.end()) {
		@put(transform tok lookup);
		i = 0; continue;
	}
@end(transform tok ident)
```

```
@def(transform tok lookup)
	auto &ss { s->second };
	items.erase(items.begin() + i,
		items.begin() + i + 1
	);
	for (const auto &e : ss) {
		items.emplace(
			items.begin() + i, e->clone()
		);
		++i;
	}
@end(transform tok lookup)
```

```
@add(transform tok ident)
	if (i < items.size() - 1) {
		auto *ta {
			dynamic_cast<Token_Item *>(
				&*items[i + 1]
		) };
		if (ta && ta->token().type() ==
			Token_Type::t_equals
		) {
			@put(transform sym assign);
			i = 0; continue;
		}
	}
@end(transform tok ident)
```

```
@def(transform sym assign)
	auto &l { _symbols[
		ti->token().name()
	] };
	for (unsigned j = i + 2;
		j < items.size(); ++j
	) {
		l.push_back(std::move(items[j]));
	}
	items.erase(
		items.begin() + i, items.end()
	);
@end(transform sym assign)
```

```
@add(transform tok)
	if (ti->token().type() ==
		Token_Type::t_raw
	) {
		@put(transform raw);
	}
@end(transform tok)
```

```
@def(transform raw)
	if (i < items.size() - 1) {
		auto *ta {
			dynamic_cast<Token_Item *>(
				&*items[i + 1]
		) };
		if (ta && ta->token().type() ==
			Token_Type::number
		) {
			@put(do transform raw);
			i = 0; continue;
		}
	}
@end(transform raw)
```

```
@def(do transform raw)
	int value { ta->token().value() };
	items.erase( items.begin() + i,
		items.begin() + i + 2
	);
	items.emplace(items.begin() + i,
		new Machine_Item { value }
	);
@end(do transform raw)
```

```
@add(transform tok)
	if (ti->token().type() ==
		Token_Type::t_times
	) {
		@put(transform cur addr);
		i = 0; continue;
	}
@end(transform tok)
```

```
@def(transform cur addr)
	items.erase(items.begin() + i,
		items.begin() + i + 1);
	int addr = code.size() * 4 +
		0x20010000;
	items.emplace(items.begin() + i,
		new Token_Item({
			Token_Type::number, addr
		})
	);
@end(transform cur addr)
```

```
@add(transform) {
	auto *ri {
		dynamic_cast<Register_Item *>(
			&*items[i]
	) };
	if (ri) {
		int rd { ri->nr() };
		@put(transform reg);
	}
} @end(transform)
```

```
@def(transform reg)
	if (i < items.size() - 2) {
		auto *t2 {
			dynamic_cast<Token_Item *>(
				&*items[i + 1]
		) };
		if (t2 && t2->token().type() ==
			Token_Type::becomes
		) {
			@put(transform reg assign);
		}
	}
@end(transform reg)
```

```
@def(transform reg assign) {
	auto *t3 {
		dynamic_cast<Token_Item *>(
			&*items[i + 2]
	) };
	if (t3 && t3->token().type() ==
		Token_Type::number
	) {
		@put(transform reg assign num);
		i = 0; continue;
	}
} @end(transform reg assign)
```

```
@def(transform reg assign num)
	int v { t3->token().value() };
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
} @end(transform reg assign num)
```

```
@add(transform reg assign) {
	auto *c3 {
		dynamic_cast<Csr_Item *>(
			&*items[i + 2]
	) };
	if (c3) {
		@put(transform reg assign csr);
		i = 0; continue;
	}
} @end(transform reg assign)
```

```
@def(transform reg assign csr)
	int cv { c3->nr() };
	items.erase(items.begin() + i,
		items.begin() + i + 3
	);
	items.emplace(items.begin() + i,
		new I_Type_Item {
			cv, 0, 0x2, rd, 0x73
		}
	);
@end(transform reg assign csr)
```

```
@add(transform) {
	auto *ii {
		dynamic_cast<I_Type_Item *>(
			&*items[i]
		)
	};
	if (ii) {
		@put(transform i type);
		i = 0; continue;
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
		new Machine_Item { result }
	);
@end(transform i type)
```

```
@add(transform) {
	auto *ui {
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
		new Machine_Item { result }
	);
@end(transform u type)
```

```
@def(consume machine instrs)
	while (! items.empty() &&
		dynamic_cast<Machine_Item *>(
			&**items.begin()
	)) {
		auto &mi {
			dynamic_cast<Machine_Item &>(
				**items.begin()
		)};
		add_machine(mi.instruction());
		items.erase(
			items.begin(),
			items.begin() + 1
		);
	}
@end(consume machine instrs)
```

