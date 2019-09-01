
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
	@put(needed by expand);
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
@add(needed by state)
	#include <vector>
@end(needed by state)
```
* simple code storage needs `std::vector`

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
@add(needed by state)
	class Expression {
		public:
			virtual ~Expression() {}
			virtual std::unique_ptr<Expression> clone() {
				return std::unique_ptr<Expression> {};
			}
	};
@end(needed by state)
```
* a lot of structure is encoded in an expression

```
@add(needed by state)
	using Expression_Ptr =
		std::unique_ptr<Expression>;
@end(needed by state)
```
* shortcut for expression pointer

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
@add(needed by state)
	class BinaryExpression:
		public Expression
	{
			Expression_Ptr _first;
			Expression_Ptr _second;
		public:
			@put(bin expr methods);
	};
@end(needed by state)
```
* a binary expression has two operands

```
@def(bin expr methods)
	BinaryExpression(
		Expression_Ptr first,
		Expression_Ptr second
	):
		_first { std::move(first) },
		_second { std::move(second) }
	{ }
@end(bin expr methods)
```
* the constructor consumes the operands

```
@add(bin expr methods)
	const Expression_Ptr &first() const {
		return _first;
	}
	const Expression_Ptr &second() const {
		return _second;
	}
@end(bin expr methods)
```
* accessors for the operands

```
@add(needed by state)
	class Assignment:
		public BinaryExpression
	{
		public:
			Assignment(
				Expression_Ptr dst,
				Expression_Ptr src
			): BinaryExpression(
				std::move(dst),
				std::move(src)
			) { }
	};
@end(needed by state)
```
* assignment is a special binary expression

```
@add(needed by state)
	class Addition:
		public BinaryExpression
	{
		public:
			Addition(
				Expression_Ptr first,
				Expression_Ptr second
			): BinaryExpression(
				std::move(first),
				std::move(second)
			) { }
			Expression_Ptr clone() override {
				return std::make_unique<Addition>(
					first()->clone(), second()->clone()
				);
			}
	};
@end(needed by state)
```
* addition is a special binary expression

```
@add(needed by state)
	class Subtraction:
		public BinaryExpression
	{
		public:
			Subtraction(
				Expression_Ptr first,
				Expression_Ptr second
			): BinaryExpression(
				std::move(first),
				std::move(second)
			) { }
	};
@end(needed by state)
```
* subtraction is a special binary expression

```
@add(needed by state)
	class Gen_Register: public Expression {
			@put(register privates);
		public:
			@put(register methods);
	};
@end(needed by state)
```
* register access is an expression


```
@def(register privates)
	const int _nr;
@end(register privates)
```
* register has a name
* and a number if it is a general purpose register
* otherwise `_nr` is negative


```
@def(register methods)
	Gen_Register(int nr):
		_nr { nr }
	{ }
@end(register methods)
```
* init with name and register number

```
@add(register methods)
	int nr() const { return _nr; }
	Expression_Ptr clone() override {
		return std::make_unique<Gen_Register>(_nr);
	}
@end(register methods)
```
* accessors for attributes
* and shortcut to decide if the register is a general purpose register

```
@add(needed by state)
	class Pc_Register: public Expression {
		public:
			Pc_Register() {}
			Expression_Ptr clone() override {
				return std::make_unique<Pc_Register>();
			}
	};
@end(needed by state)
```

```
@add(needed by state)
	class Csr_Register: public Expression {
			int _addr;
		public:
			Csr_Register(int addr):
				_addr { addr }
			{ }
			Expression_Ptr clone() override {
				return std::make_unique<Csr_Register>(_addr);
			}
			int addr() const {
				return _addr; 
			}
	};
@end(needed by state)
```

```
@add(needed by state)
	class Number: public Expression {
			const int _value;

		public:
			Number(int value):
				_value { value }
			{ }

			Expression_Ptr clone() override {
				return std::make_unique<Number>(_value);
			}

			int value() const {
				return _value;
			}
	};
@end(needed by state)
```
* number is a special expression

```
@add(needed by state)
	#include <vector>
	Expression_Ptr parse(std::vector<Token>::iterator &cur, std::vector<Token>::iterator end, int addr);
	@put(needed by parse factor);
	Expression_Ptr parse_factor(
		std::vector<Token>::iterator &cur, std::vector<Token>::iterator end, int addr
	) {
		@put(parse factor);
		return Expression_Ptr {};
	}
@end(needed by state)
```
* parses a arithmetic factor

```
@def(needed by parse factor)
	#include <map>
@end(needed by parse factor)
```

```
@add(needed by parse factor)
	static std::map<std::string, Expression_Ptr> _symbols;
@end(needed by parse factor)
```

```
@add(needed by parse factor)
	void clear_symbols() {
		_symbols.clear();
		@put(clear symbols);
	}
@end(needed by parse factor)
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
@def(clear symbols) {
	_symbols["%pc"] = std::move(std::make_unique<Pc_Register>());
	_symbols["%mtvec"] = std::move(std::make_unique<Csr_Register>(0x305));
	_symbols["%mhartid"] = std::move(std::make_unique<Csr_Register>(0xf14));
	char name1[] = "%x#";
	for (int i = 0; i < 10; ++i) {
		name1[2] = '0' + i;
		_symbols[name1] = std::move(std::make_unique<Gen_Register>(i));
	}
	char name2[] = "%x##";
	for (int i = 10; i < 32; ++i) {
		name2[2] = '0' + (i / 10);
		name2[3] = '0' + (i % 10);
		_symbols[name2] = std::move(std::make_unique<Gen_Register>(i));
	}
} @end(clear symbols)
```

```
@add(clear symbols)
	_symbols["%zero"] = _symbols["%x0"]->clone();
	_symbols["%ra"] = _symbols["%x1"]->clone();
	_symbols["%sp"] = _symbols["%x2"]->clone();
	_symbols["%gp"] = _symbols["%x3"]->clone();
	_symbols["%tp"] = _symbols["%x4"]->clone();
	char name1[] = "%t#";
	for (int i = 0; i < 3; ++i) {
		name1[2] = '0' + i;
		_symbols[name1] = std::move(std::make_unique<Gen_Register>(i + 5));
	}
	char name2[] = "%s#";
	for (int i = 0; i < 2; ++i) {
		name2[2] = '0' + i;
		_symbols[name2] = std::move(std::make_unique<Gen_Register>(i + 8));
	}
	_symbols["%fp"] = _symbols["%x8"]->clone();
	char name3[] = "%a#";
	for (int i = 0; i < 8; ++i) {
		name3[2] = '0' + i;
		_symbols[name3] = std::move(std::make_unique<Gen_Register>(i + 10));
	}
	for (int i = 2; i < 10; ++i) {
		name2[2] = '0' + i;
		_symbols[name2] = std::move(std::make_unique<Gen_Register>(i + 16));
	}
	char name4[] = "%s##";
	for (int i = 10; i < 12; ++i) {
		name4[2] = '0' + (i / 10);
		name4[3] = '0' + (i % 10);
		_symbols[name4] = std::move(std::make_unique<Gen_Register>(i + 16));
	}
	for (int i = 3; i < 7; ++i) {
		name1[2] = '0' + i;
		_symbols[name1] = std::move(std::make_unique<Gen_Register>(i + 25));
	}
@end(clear symbols)
```

```
@def(parse factor)
	if (cur == end) { return Expression_Ptr { }; };
	if (cur->type() == Token_Type::ident) {
		auto found { _symbols.find(cur->name()) };
		if (found != _symbols.end()) {
			++cur;
			return found->second->clone();
		}
	}
@end(parse factor)
```
* registers are valid factors

```
@add(needed by state)
	@put(needed by parse);
	Expression_Ptr parse(std::vector<Token>::iterator &cur, std::vector<Token>::iterator end, int addr) {
		@put(parse special);
		auto dst = parse_factor(cur, end, addr);
		do {
			@put(parse binary);
		} while (false);
		return dst;
	}
@end(needed by state)
```
* parses a factor that can be followed by a binary expression
* otherwise returns the factor

```
@def(parse binary)
	if (cur == end) { break; }
	if (cur->type() == Token_Type::becomes) {
		++cur;
		auto src = parse(cur, end, addr);
		if (! src) {
			std::cerr << "no expression after <-\n";
			return Expression_Ptr { };
		}
		return std::make_unique<Assignment>(std::move(dst), std::move(src));
	}
@end(parse binary)
```

```
@add(parse binary)
	if (cur->type() == Token_Type::plus) {
		++cur;
		auto src = parse_factor(cur, end, addr);
		if (! src) {
			std::cerr << "no factor after +\n";
			return Expression_Ptr { };
		}
		auto n1 = dynamic_cast<const Number *>(&*dst);
		auto n2 = dynamic_cast<const Number *>(&*src);
		if (n1 && n2) {
			dst = std::make_unique<Number>(n1->value() + n2->value());
		} else {
			dst = std::make_unique<Addition>(std::move(dst), std::move(src));
		}
		continue;
	}
@end(parse binary);
```

```
@add(parse binary)
	if (cur->type() == Token_Type::minus) {
		++cur;
		auto src = parse_factor(cur, end, addr);
		if (! src) {
			std::cerr << "no factor after -\n";
			return Expression_Ptr { };
		}
		auto n = dynamic_cast<const Number *>(&*src);
		if (n) {
			dst = std::make_unique<Addition>(std::move(dst), std::make_unique<Number>(-n->value()));
		} else {
			dst = std::make_unique<Subtraction>(std::move(dst), std::move(src));
		}
		continue;
	}
@end(parse binary)
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
@add(parse factor)
	if (cur->type() == Token_Type::number) {
		auto res =
			std::make_unique<Number>(
				cur->value()
			);
		++cur;
		return res;
	}
@end(parse factor)
```

```
@add(parse factor)
	if (cur->type() == Token_Type::minus) {
		++cur;
		auto res = parse_factor(cur, end, addr);
		auto n = dynamic_cast<const Number *>(&*res);
		if (n) {
			return std::make_unique<Number>(
				-n->value()
			);
		}
		std::cerr << "no number after -\n";
	}
@end(parse factor)
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
	// unsigned addr { code.size() * 4 + 0x20010000 };
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
@def(needed by parse)
	class BinaryAnd:
		public BinaryExpression
	{
		public:
			BinaryAnd(
				Expression_Ptr first,
				Expression_Ptr second
			): BinaryExpression(
				std::move(first),
				std::move(second)
			) { }
	};
@end(needed by parse)
```
* and is a special binary expression

```
@add(needed by parse)
	class BinaryOr:
		public BinaryExpression
	{
		public:
			BinaryOr(
				Expression_Ptr first,
				Expression_Ptr second
			): BinaryExpression(
				std::move(first),
				std::move(second)
			) { }
	};
@end(needed by parse)
```
* and is a special binary expression

```
@add(parse binary)
	if (cur->type() == Token_Type::t_and) {
		++cur;
		auto src = parse_factor(cur, end, addr);
		if (! src) {
			std::cerr << "no factor after &\n";
			return Expression_Ptr { };
		}
		dst = std::make_unique<BinaryAnd>(std::move(dst), std::move(src));
		continue;
	}
@end(parse binary);
```

```
@add(parse binary)
	if (cur->type() == Token_Type::t_or) {
		++cur;
		auto src = parse_factor(cur, end, addr);
		if (! src) {
			std::cerr << "no factor after |\n";
			return Expression_Ptr { };
		}
		dst = std::make_unique<BinaryOr>(std::move(dst), std::move(src));
		continue;
	}
@end(parse binary);
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
	assert_line(
		"%x11 <- $0d",
		0x00d00593
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"%x12 <- $0a",
		0x00a00613
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
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
	assert_line(
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
@add(needed by parse)
	class If:
		public BinaryExpression
	{
		public:
			If(
				Expression_Ptr cond,
				Expression_Ptr body
			): BinaryExpression(
				std::move(cond),
				std::move(body)
			) { }
	};
@end(needed by parse)
```
* if is a special binary expression

```
@def(parse special)
	if (cur == end) { return Expression_Ptr { }; };
	if (cur->type() == Token_Type::t_raw) {
		++cur;
		auto val { parse(cur, end, addr) };
		if (! val) {
			std::cerr << "no number after raw\n";
			return Expression_Ptr { };
		}
		return val;
	}
@end(parse special)
```

```
@add(parse special)
	if (cur->type() == Token_Type::t_if) {
		++cur;
		auto cond = parse(cur, end, addr);
		if (! cond) {
			std::cerr << "no expression after if\n";
			return Expression_Ptr { };
		}
		if (cur == end || cur->type() != Token_Type::t_colon) {
			std::cerr << "expecting : after if expr\n";
			return Expression_Ptr { };
		}
		++cur;
		auto body = parse(cur, end, addr);
		if (! body) {
			std::cerr << "no if body\n";
			return Expression_Ptr { };
		}
		return std::make_unique<If>(std::move(cond), std::move(body));
	}
@end(parse special);
```

```
@add(needed by parse)
	class Less:
		public BinaryExpression
	{
		public:
			Less(
				Expression_Ptr first,
				Expression_Ptr second
			): BinaryExpression(
				std::move(first),
				std::move(second)
			) { }
	};
@end(needed by parse)
```
* if is a special binary expression

```
@add(parse binary)
	if (cur->type() == Token_Type::t_less) {
		++cur;
		auto src = parse_factor(cur, end, addr);
		if (! src) {
			std::cerr << "no factor after <\n";
			return Expression_Ptr { };
		}
		dst = std::make_unique<Less>(std::move(dst), std::move(src));
		continue;
	}
@end(parse binary);
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
@add(needed by parse)
	class Equals:
		public BinaryExpression
	{
		public:
			Equals(
				Expression_Ptr first,
				Expression_Ptr second
			): BinaryExpression(
				std::move(first),
				std::move(second)
			) { }
	};
@end(needed by parse)
```

```
@add(needed by parse)
	class NotEquals:
		public BinaryExpression
	{
		public:
			NotEquals(
				Expression_Ptr first,
				Expression_Ptr second
			): BinaryExpression(
				std::move(first),
				std::move(second)
			) { }
	};
@end(needed by parse)
```

```
@add(parse binary)
	if (cur->type() == Token_Type::t_equals) {
		++cur;
		auto src = parse_factor(cur, end, addr);
		if (! src) {
			std::cerr << "no factor after =\n";
			return Expression_Ptr { };
		}
		dst = std::make_unique<Equals>(std::move(dst), std::move(src));
		continue;
	}
@end(parse binary);
```

```
@add(parse binary)
	if (cur->type() == Token_Type::t_not_equals) {
		++cur;
		auto src = parse_factor(cur, end, addr);
		if (! src) {
			std::cerr << "no factor after !=\n";
			return Expression_Ptr { };
		}
		dst = std::make_unique<NotEquals>(std::move(dst), std::move(src));
		continue;
	}
@end(parse binary);
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
@add(parse factor)
	if (cur->type() == Token_Type::t_open_bracket) {
		++cur;
		auto inner = parse(cur, end, addr);
		if (! inner) {
			std::cerr << "no expr in memory access\n";
			return Expression_Ptr { };
		}
		if (cur == end || cur->type() != Token_Type::t_close_bracket) {
			std::cerr << "expecting ]\n";
			return Expression_Ptr { };
		}
		++cur;
		return std::make_unique<Access>(inner);
	}
@end(parse factor)
```

```
@add(needed by parse factor)
	class Access: public Expression {
			const Expression_Ptr _inner;

		public:
			Access(Expression_Ptr &inner):
				_inner { std::move(inner) }
			{ }

			Expression *inner() const {
				return &*_inner;
			}
	};
@end(needed by parse factor)
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
@add(parse factor)
	if (cur->type() == Token_Type::t_open_parenthesis) {
		++cur;
		auto inner = parse(cur, end, addr);
		if (! inner) {
			std::cerr << "no expr in after (\n";
			return Expression_Ptr { };
		}
		if (cur == end || cur->type() != Token_Type::t_close_parenthesis) {
			std::cerr << "expecting )\n" << (int) cur->type() << ", " << (int) Token_Type::t_close_parenthesis << "\n";
			return Expression_Ptr { };
		}
		++cur;
		return inner;
	}
@end(parse factor)
```

```
@add(parse factor)
	if (cur->type() == Token_Type::ident) {
		std::string name { cur->name() };
		++cur;
		if (cur != end && cur->type() == Token_Type::t_equals) {
			++cur;
			auto value { parse(cur, end, addr) };
			if (! value) {
				std::cerr << "no value on assignment\n";
				return Expression_Ptr { };
			}
			_symbols[name] = std::move(value->clone());
			return Expression_Ptr { };
		}
		std::cerr << "no assignment after unknown ident\n";
		return Expression_Ptr { };
	}
@end(parse factor)
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
@add(parse factor)
	if (cur->type() == Token_Type::t_times) {
		++cur;
		return std::make_unique<Number>(addr);
	}
@end(parse factor)
```

```
@def(needed by expand)
	class Item {
		public:
			virtual ~Item() {};
	};
@end(needed by expand)
```

```
@add(needed by expand)
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
	};
@end(needed by expand)
```

```
@add(needed by expand)
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
	};
@end(needed by expand)
```

```
@def(expand)
	std::vector<std::unique_ptr<Item>> items;
	for (; cur != ts.end(); ++cur) {
		items.emplace_back(new Token_Item { *cur });
	}
	bool modified;
	do {
		modified = false;
		unsigned i = 0;
		while (i < items.size()) {
			auto *ti { dynamic_cast<Token_Item *>(&*items[i]) };
			if (ti && ti->token().type() == Token_Type::t_raw) {
				if (i < items.size() - 1) {
					auto *ta { dynamic_cast<Token_Item *>(&*items[i + 1]) };
					if (ta && ta->token().type() == Token_Type::number) {
						int value = ta->token().value();
						items.erase(items.begin() + i, items.begin() + i + 2);
						items.emplace(items.begin() + i, new Machine_Item { value });
						modified = true;
						i = 0; continue;
					}
				}
			}
			++i;
		}
		while (! items.empty() && dynamic_cast<Machine_Item *>(&**items.begin())) {
			auto &mi { dynamic_cast<Machine_Item &>(**items.begin()) };
			add_machine(mi.instruction());
			items.erase(items.begin(), items.begin() + 1);
			modified = true;
		}
	} while (! items.empty() && modified);
	
	if (! items.empty()) {
		std::cerr << "can expand fully [" << line << "]\n";
	}
@end(expand)
```
