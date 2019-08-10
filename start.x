
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
		"%x4 <- %x2 + %x3",
		0x00310233
	);
@end(unit-tests)
```
* the first test checks that a simple register add can be parsed

```
@def(assert line)
	State s;
	s.add_line(line);
	assert(s.code_size() == 1);
	// std::cerr << std::hex << expected << ", " << s.get_code(0) << '\n';
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
@def(tokenizer attributes)
	Token_Type _type =
		Token_Type::unknown;
@end(tokenizer attributes)
```
* stores type of parsed token

```
@def(tokenizer methods)
	Token_Type type() const {
		return _type;
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
		_type = Token_Type::end;
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
		_type = Token_Type::unknown;
		std::cerr <<
			"unrecognized char [" <<
			*_cur++ << "] \n" ;
	} while (false);
@end(next)
```
* recognize individual tokens
* write warning when not recognizing token

```
@def(token types)
	becomes,
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
	Tokenizer t(line);
	assert(t.type() == token);
	t.next();
	assert(t.type() == Token_Type::end);
@end(assert token)
```
* initialize tokenizer and verify that it only contains the one expected
  token

```
@def(recognize)
	if (*_cur == '<') {
		if (
			_cur  + 1 != _end &&
				_cur[1] == '-'
		) {
			_type = Token_Type::becomes;
			_cur += 2;
			break;
		}
	}
@end(recognize)
```
* recognize assignment

```
@add(token types)
	plus,
@end(token types)
```
* add token for addition

```
@add(unit-tests)
	assert_token("+", Token_Type::plus);
@end(unit-tests)
```
* try to recognize addition

```
@add(recognize)
	if (*_cur == '+') {
		_type = Token_Type::plus;
		++_cur;
		break;
	}
@end(recognize)
```
* recognize addition

```
@add(token types)
	reg,
@end(token types)
```
* add token for register

```
@add(tokenizer attributes)
	std::string _name = {};
@end(tokenizer attributes)
```
* store name of register

```
@add(tokenizer methods)
	const std::string &name() const {
		return _name;
	}
@end(tokenizer methods)
```
* return register name

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
	assert(t.type() == Token_Type::reg);
	assert(t.name() == name);
	t.next();
	assert(t.type() == Token_Type::end);
@end(assert register)
```
* check for a register token with specific name

```
@add(unit-tests)
	assert_register("%x10", "x10");
	assert_register("%pc", "pc");
@end(unit-tests)
```
* verify some register names

```
@add(needed by tokenizer)
	#include <cctype>
@end(needed by tokenizer)
```
* needs `@f(isalnum)` and `@f(isdigit)`

```
@add(recognize)
	if (*_cur == '%') {
		auto c = _cur + 1;
		_name = {};
		while (c != _end && isalnum(*c)) {
			_name += *c++;
		}
		if (! _name.empty()) {
			_type = Token_Type::reg;
			_cur = c;
			break;
		}
	}
@end(recognize)
```
* recognize register


## Parsing
* parses simple additions

```
@add(needed by state)
	class Expression {
		public:
			virtual ~Expression() {}
	};
@end(needed by state)
```
* a lot of structure is encoded in an expression

```
@add(needed by state)
	#include <memory>
@end(needed by state)
```
* needs `unique_ptr`

```
@add(needed by state)
	using Expression_Ptr =
		std::unique_ptr<Expression>;
@end(needed by state)
```
* shortcut for expression pointer

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
	};
@end(needed by state)
```
* addition is a special binary expression

```
@add(needed by state)
	class Register: public Expression {
			@put(register privates);
		public:
			@put(register methods);
	};
@end(needed by state)
```
* register access is an expression


```
@def(register privates)
	const std::string _name;
	const int _nr;
@end(register privates)
```
* register has a name
* and a number if it is a general purpose register
* otherwise `_nr` is negative


```
@add(register privates)
	static int nr_from_name(
		const std::string &name
	) {
		int nr { 0 };
		@put(nr from name);
		return nr;
	}
@end(register privates)
```
* get register number from name, if it is a general purpose register
* otherwise return a negitave number


```
@def(nr from name)
	if (name.empty()) { return -1; }
@end(nr from name)
```
* empty name is no general purpose register

```
@add(nr from name)
	if (name == "x") { return -1; }
@end(nr from name)
```
* the name `%x` is no general purpose register

```
@add(nr from name)
	if (name[0] != 'x') { return -1; }
@end(nr from name)
```
* if the name does not start with `%x` it is not a general purpose
  register

```
@add(nr from name)
	for (
		int i { 1 };
		i < (int) name.size();
		++i
	) {
		char ch = name[i];
		if (! isdigit(ch)) {
			return -1;
		}
		nr = nr * 10 + (ch - '0');
	}
@end(nr from name)
```
* parse the register number from the digits
* if some other char occurs, it is no general purpose register

```
@add(nr from name)
	if (nr < 0 || nr > 31) {
		return -1;
	}
@end(nr from name)
```
* only numbers `0` to `31` are valid general purpose registers

```
@def(register methods)
	Register(const std::string &name):
		_name { name },
		_nr { nr_from_name(name) }
	{ }
@end(register methods)
```
* init with name and register number

```
@add(register methods)
	const std::string &name() const {
		return _name;
	}
	int nr() const { return _nr; }
	bool is_general() const {
		return _nr >= 0;
	}
@end(register methods)
```
* accessors for attributes
* and shortcut to decide if the register is a general purpose register

```
@add(needed by state)
	class Number: public Expression {
			const int _value;

		public:
			Number(int value):
				_value { value }
			{ }

			int value() const {
				return _value;
			}
	};
@end(needed by state)
```
* number is a special expression

```
@add(needed by state)
	Expression_Ptr parse_factor(
		Tokenizer &t
	) {
		@put(parse factor);
		std::cerr << "no factor\n";
		return Expression_Ptr {};
	}
@end(needed by state)
```
* parses a arithmetic factor

```
@def(parse factor)
	if (t.type() == Token_Type::reg) {
		auto res =
			std::make_unique<Register>(
				t.name()
			);
		t.next();
		return res;
	}
@end(parse factor)
```
* registers are valid factors

```
@add(needed by state)
	@put(needed by parse);
	Expression_Ptr parse(Tokenizer &t) {
		auto dst = parse_factor(t);
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
	if (t.type() == Token_Type::becomes) {
		t.next();
		auto src = parse(t);
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
	if (t.type() == Token_Type::plus) {
		t.next();
		auto src = parse_factor(t);
		if (! src) {
			std::cerr << "no factor after +\n";
			return Expression_Ptr { };
		}
		dst = std::make_unique<Addition>(std::move(dst), std::move(src));
		continue;
	}
@end(parse binary);
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
@add(token types)
	number,
@end(token types)
```

```
@add(tokenizer attributes)
	int _value = 0;
@end(tokenizer attributes)
```

```
@add(recognize)
	if (*_cur == '-' || isdigit(*_cur)) {
		bool neg = *_cur == '-';
		if (neg) { ++_cur; }
		_value = 0;
		while (isdigit(*_cur)) {
			_value = _value * 10 +
				(*_cur - '0');
			++_cur;
		}
		if (neg) { _value = -_value; }
		_type = Token_Type::number;
		break;
	}
@end(recognize)
```

```
@add(tokenizer methods)
	int value() const { return _value; }
@end(tokenizer methods)
```

```
@add(parse factor)
	if (t.type() == Token_Type::number) {
		auto res =
			std::make_unique<Number>(
				t.value()
			);
		t.next();
		return res;
	}
@end(parse factor)
```

```
@def(add line)
	Tokenizer t { line };
	auto e = parse(t);
	if (! e) {
		std::cerr << "no statement on line " << line << '\n';
		return;
	}
	@put(parse expression);
	std::cerr << "can't parse " << line << '\n';
@end(add line)
```

```
@def(parse expression)
	Number *n = dynamic_cast<Number *>(&*e);
	if (n) {
		add_machine(n->value());
		return;
	}
@end(parse expression)
```

```
@add(parse expression)
	Assignment *a = dynamic_cast<Assignment *>(&*e);
	if (a) {
		const Register *dst = dynamic_cast<const Register *>(&*a->first());
		if (! dst) {
			std::cerr << "no assignment to register\n";
			return;
		}
		@put(parse assignment);
	}
@end(parse expression)
```

```
@def(parse assignment)
	if (dst->is_general()) {
		@put(assign to general);
	}
@end(parse assignment)
```

```
@def(assign to general) {
	const Addition *o = dynamic_cast<const Addition *>(&*a->second());
	if (o) {
		const Register *src1 = dynamic_cast<const Register *>(&*o->first());
		if (! src1) {
			std::cerr << "first op of addition no register\n";
			return;
		}
		if (! src1->is_general()) {
			std::cerr << "first of of addition no general register\n";
		}
		const Register *src2 = dynamic_cast<const Register *>(&*o->second());
		if (src2) {
			if (! src2->is_general()) {
				std::cerr << "second of of addition no general register\n";
			}
			add_machine(build_add(
				(char) dst->nr(), (char) src1->nr(), (char) src2->nr()
			));
			return;
		}
		const Number *n2 = dynamic_cast<const Number *>(&*o->second());
		if (n2) {
			add_machine(build_add(
				(char) dst->nr(), (char) src1->nr(), n2->value()
			));
			return;
		}
	}
} @end(assign to general)
```

```
@add(parse assignment)
	if (dst->name() == "pc") {
		const Addition *o = dynamic_cast<const Addition *>(&*a->second());
		if (! o) {
			std::cerr << "only addition supported now\n";
			return;
		}
		const Register *src1 = dynamic_cast<const Register *>(&*o->first());
		if (! src1) {
			std::cerr << "first op of addition no register\n";
			return;
		}
		if (src1->name() != "pc") {
			std::cerr << "first op of jump is not %pc, but " << dst->name() << '\n';
		}
		const Number *num = dynamic_cast<const Number *>(&*o->second());
		if (num) {
			int val = num->value();
			int word = 0x0000006f;
			word |= ((val >> 12) & 0xff) << 12;
			word |= ((val >> 11) & 0x01) << 20;
			word |= ((val >> 1) & 0x3ff) << 21;
			word |= ((val >> 20) & 0x01) << 31;
			add_machine(word);
			return;
		} else {
			std::cerr << "expected number as second argument of jump\n";
		}
	}
@end(parse assignment)
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
@add(recognize)
	if (*_cur == '$') {
		_value = 0;
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
			_value = (_value << 4) + digit;
			++_cur;
		}
		_type = Token_Type::number;
		break;
	}
@end(recognize)
```

```
@add(recognize)
	if (*_cur == '#') {
		_type = Token_Type::end;
		break;
	}
@end(recognize)
```

```
@add(token types)
	t_and,
	t_or,
@end(token types)
```

```
@add(recognize)
	if (*_cur == '&') {
		_type = Token_Type::t_and;
		++_cur;
		break;
	}
@end(recognize)
```

```
@add(recognize)
	if (*_cur == '|') {
		_type = Token_Type::t_or;
		++_cur;
		break;
	}
@end(recognize)
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
	if (t.type() == Token_Type::t_and) {
		t.next();
		auto src = parse_factor(t);
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
	if (t.type() == Token_Type::t_or) {
		t.next();
		auto src = parse_factor(t);
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
@add(assign to general) {
	const BinaryAnd *o = dynamic_cast<const BinaryAnd *>(&*a->second());
	if (o) {
		const Register *src1 = dynamic_cast<const Register *>(&*o->first());
		if (! src1) {
			std::cerr << "first op of and no register\n";
			return;
		}
		if (! src1->is_general()) {
			std::cerr << "first of of and no general register\n";
		}
		const Register *src2 = dynamic_cast<const Register *>(&*o->second());
		if (src2) {
			if (! src2->is_general()) {
				std::cerr << "second of of and no general register\n";
			}
			add_machine(build_and(
				(char) dst->nr(), (char) src1->nr(), (char) src2->nr()
			));
			return;
		}
		const Number *n2 = dynamic_cast<const Number *>(&*o->second());
		if (n2) {
			add_machine(build_and(
				(char) dst->nr(), (char) src1->nr(), n2->value()
			));
			return;
		}
	}
} @end(assign to general)
```

```
@add(assign to general) {
	const BinaryOr *o = dynamic_cast<const BinaryOr *>(&*a->second());
	if (o) {
		const Register *src1 = dynamic_cast<const Register *>(&*o->first());
		if (! src1) {
			std::cerr << "first op of or no register\n";
			return;
		}
		if (! src1->is_general()) {
			std::cerr << "first of of or no general register\n";
		}
		const Register *src2 = dynamic_cast<const Register *>(&*o->second());
		if (src2) {
			if (! src2->is_general()) {
				std::cerr << "second of of or no general register\n";
			}
			add_machine(build_or(
				(char) dst->nr(), (char) src1->nr(), (char) src2->nr()
			));
			return;
		}
		const Number *n2 = dynamic_cast<const Number *>(&*o->second());
		if (n2) {
			add_machine(build_or(
				(char) dst->nr(), (char) src1->nr(), n2->value()
			));
			return;
		}
	}
} @end(assign to general)
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
		"%x5 <- %x5 & $ff",
		0x0ff2f293
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"%x5 <- %x5 | $1",
		0x0012e293
	);
@end(unit-tests)
```

```
@add(unit-tests)
	assert_line(
		"%x6 <- %x6 | $1",
		0x00136313
	);
@end(unit-tests)
```

```
@add(assign to general) {
	const Number *o = dynamic_cast<const Number *>(&*a->second());
	if (o) {
		@put(load number);
	}
} @end(assign to general)
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
@def(load number)
	int upper { o->value() & ~ 0x7ff };
	if (upper && upper != ~ 0x7ff) {
		add_machine(build_lui(dst->nr(), o->value()));
	}
@end(load number)
```

```
@add(load number)
	if (o->value() == 0 || (o->value() & 0x7ff)) {
		add_machine(build_add((char) dst->nr(), (char) 0, o->value()));
	}
	return;
@end(load number)
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
@add(assign to general) {
	const Register *o = dynamic_cast<const Register *>(&*a->second());
	if (o && o->name() == "pc") {
		add_machine(build_auipc(dst->nr(), 0));
		return;
	}
} @end(assign to general)
```

```
@add(unit-tests)
	assert_line(
		"%x5 <- %pc",
		0x00000297
	);
@end(unit-tests)
```

