
#line 5 "start.x"

	
#line 28 "start.x"

	#include <cassert>

#line 43 "start.x"

	
#line 58 "start.x"

	#include <string>

#line 113 "start.x"

	#include <vector>

#line 201 "start.x"

	
#line 214 "start.x"

	enum class Token_Type {
		unknown,
		
#line 320 "start.x"

	becomes,

#line 375 "start.x"

	plus,

#line 400 "start.x"

	reg,

#line 759 "start.x"

	number,

#line 217 "start.x"

		end
	};

#line 299 "start.x"

	#include <iostream>

#line 454 "start.x"

	#include <cctype>

#line 202 "start.x"

	class Tokenizer {
			
#line 225 "start.x"

	Token_Type _type =
		Token_Type::unknown;

#line 242 "start.x"

	std::string::const_iterator _cur;
	std::string::const_iterator _end;

#line 407 "start.x"

	std::string _name = {};

#line 765 "start.x"

	int _value = 0;

#line 204 "start.x"
;
		public:
			
#line 233 "start.x"

	Token_Type type() const {
		return _type;
	}

#line 250 "start.x"

	void next();

#line 257 "start.x"

	Tokenizer(const std::string &s):
		_cur { s.begin() },
		_end { s.end() } 
	{
		next();
	}

#line 414 "start.x"

	const std::string &name() const {
		return _name;
	}

#line 789 "start.x"

	int value() const { return _value; }

#line 206 "start.x"
;
	};
	
#line 269 "start.x"

	void Tokenizer::next() {
		
#line 278 "start.x"

	while (
		_cur != _end && *_cur <= ' '
	) {
		++_cur;
	}

#line 289 "start.x"

	if (_cur == _end) {
		_type = Token_Type::end;
		return;
	}

#line 306 "start.x"

	do {
		
#line 359 "start.x"

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

#line 389 "start.x"

	if (*_cur == '+') {
		_type = Token_Type::plus;
		++_cur;
		break;
	}

#line 461 "start.x"

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

#line 771 "start.x"

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

#line 308 "start.x"
;
		_type = Token_Type::unknown;
		std::cerr <<
			"unrecognized char [" <<
			*_cur++ << "] \n" ;
	} while (false);

#line 271 "start.x"
;
	}

#line 208 "start.x"
;

#line 483 "start.x"

	class Expression {
		public:
			virtual ~Expression() {}
	};

#line 493 "start.x"

	#include <memory>

#line 500 "start.x"

	using Expression_Ptr =
		std::unique_ptr<Expression>;

#line 507 "start.x"

	class BinaryExpression:
		public Expression
	{
			Expression_Ptr _first;
			Expression_Ptr _second;
		public:
			
#line 520 "start.x"

	BinaryExpression(
		Expression_Ptr first,
		Expression_Ptr second
	):
		_first { std::move(first) },
		_second { std::move(second) }
	{ }

#line 532 "start.x"

	const Expression_Ptr &first() const {
		return _first;
	}
	const Expression_Ptr &second() const {
		return _second;
	}

#line 514 "start.x"
;
	};

#line 543 "start.x"

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

#line 560 "start.x"

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

#line 577 "start.x"

	class Register: public Expression {
			
#line 587 "start.x"

	const std::string _name;
	const int _nr;

#line 594 "start.x"

	static int nr_from_name(
		const std::string &name
	) {
		int nr { 0 };
		
#line 606 "start.x"

	if (name.empty()) { return -1; }
	if (name == "x") { return -1; }
	if (name[0] != 'x') { return -1; }

#line 614 "start.x"

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

#line 630 "start.x"

	if (nr < 0 || nr > 31) {
		return -1;
	}

#line 599 "start.x"
;
		return nr;
	}

#line 579 "start.x"
;
		public:
			
#line 638 "start.x"

	Register(const std::string &name):
		_name { name },
		_nr { nr_from_name(name) }
	{ }

#line 647 "start.x"

	const std::string &name() const {
		return _name;
	}
	int nr() const { return _nr; }
	bool is_general() const {
		return _nr >= 0;
	}

#line 581 "start.x"
;
	};

#line 659 "start.x"

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

#line 676 "start.x"

	Expression_Ptr parse_factor(
		Tokenizer &t
	) {
		
#line 688 "start.x"

	if (t.type() == Token_Type::reg) {
		auto res =
			std::make_unique<Register>(
				t.name()
			);
		t.next();
		return res;
	}

#line 795 "start.x"

	if (t.type() == Token_Type::number) {
		auto res =
			std::make_unique<Number>(
				t.value()
			);
		t.next();
		return res;
	}

#line 680 "start.x"
;
		std::cerr << "no factor\n";
		return Expression_Ptr {};
	}

#line 701 "start.x"

	Expression_Ptr parse(Tokenizer &t) {
		auto dst = parse_factor(t);
		bool is_reg = !! dynamic_cast<Register *>(&*dst);
		if (t.type() == Token_Type::becomes) {
			if (! is_reg) {
				std::cerr << "assigment to no-reg\n";
			}
			t.next();
			auto src = parse(t);
			if (! src) {
				std::cerr << "no expression after <-\n";
				return Expression_Ptr { };
			}
			return std::make_unique<Assignment>(std::move(dst), std::move(src));
		} else if (t.type() == Token_Type::plus) {
			t.next();
			auto src = parse(t);
			if (! src) {
				std::cerr << "no expression after +\n";
				return Expression_Ptr { };
			}
			return std::make_unique<Addition>(std::move(dst), std::move(src));
		} else {
			return dst;
		}
	}

#line 732 "start.x"

	int build_r_cmd(
		int funct7, char src2, char src1,
		int funct3, char dst, int opcode
	) {
		return (funct7 << 25) |
			(src2 << 20) | (src1 << 15) |
			(funct3 << 12) | (dst << 7) |
			opcode;
	}

#line 746 "start.x"

	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}

#line 44 "start.x"
;
	class State {
			
#line 90 "start.x"

	void add_machine(int instr);

#line 46 "start.x"

		public:
			
#line 65 "start.x"

	void add_line(
		const std::string &line
	);

#line 106 "start.x"

	
#line 120 "start.x"

	private:
		std::vector<int> code;

#line 128 "start.x"

	public:
		int code_size() const {
			return static_cast<int>(
				code.size()
			);
		}

#line 140 "start.x"

	public:
		int get_code(int pos) const {
			return code[pos];
		}

#line 107 "start.x"


#line 48 "start.x"

	};
	
#line 74 "start.x"

	void State::add_line(
		const std::string &line
	) {
		
#line 808 "start.x"

	Tokenizer t { line };
	auto e = parse(t);
	if (! e) {
		std::cerr << "no statement on line " << line << "\n";
		return;
	}
	Assignment *a = dynamic_cast<Assignment *>(&*e);
	if (! a) {
		std::cerr << "assignment expected on line " << line << "\n";
		return;
	}
	const Register *dst = dynamic_cast<const Register *>(&*a->first());
	if (! dst) {
		std::cerr << "no assignment to register\n";
		return;
	}

	if (dst->is_general()) {
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
		if (! src1->is_general()) {
			std::cerr << "first of of addition no general register\n";
		}
		const Register *src2 = dynamic_cast<const Register *>(&*o->second());
		if (! src2) {
			std::cerr << "second op of addition no register\n";
			return;
		}
		if (! src2->is_general()) {
			std::cerr << "second of of addition no general register\n";
		}
		add_machine(build_add(
			(char) dst->nr(), (char) src1->nr(), (char) src2->nr()
		));
	} else if (dst->name() == "pc") {
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
		} else {
			std::cerr << "expected number as second argument of jump\n";
		}
	} else {
		std::cerr << "unknown register " << dst->name() << '\n';
	}

#line 78 "start.x"
;
	}

#line 97 "start.x"

	void State::add_machine(int instr) {
		
#line 150 "start.x"

	code.push_back(instr);

#line 99 "start.x"

	}

#line 50 "start.x"
;

#line 157 "start.x"

	void assert_line(
		const char *line,
		int expected
	) {
		
#line 180 "start.x"

	State s;
	s.add_line(line);
	assert(s.code_size() == 1);
	assert(s.get_code(0) == expected);

#line 162 "start.x"
;
	}

#line 327 "start.x"

	void assert_token(
		const char *line,
		Token_Type token
	) {
		
#line 348 "start.x"

	Tokenizer t(line);
	assert(t.type() == token);
	t.next();
	assert(t.type() == Token_Type::end);

#line 332 "start.x"
;
	}

#line 423 "start.x"

	void assert_register(
		const char *line,
		const char *name
	) {
		
#line 435 "start.x"

	Tokenizer t(line);
	assert(t.type() == Token_Type::reg);
	assert(t.name() == name);
	t.next();
	assert(t.type() == Token_Type::end);

#line 428 "start.x"
;
	}

#line 6 "start.x"
;
	int main(
		int argc, const char *argv[]
	) {
		
#line 21 "start.x"

	
#line 170 "start.x"

	assert_line(
		"%x4 <- %x2 + %x3",
		0x00310233
	);

#line 339 "start.x"

	assert_token(
		"<-", Token_Type::becomes
	);

#line 382 "start.x"

	assert_token("+", Token_Type::plus);

#line 446 "start.x"

	assert_register("%x10", "x10");
	assert_register("%pc", "pc");

#line 884 "start.x"

	assert_line(
		"%pc <- %pc + 0",
		0x0000006f
	);

#line 895 "start.x"

	assert_line(
		"%pc <- %pc + -28",
		0xfe5ff06f
	);

#line 905 "start.x"

	assert_line(
		"%pc <- %pc + -32",
		0xfe1ff06f
	);

#line 22 "start.x"


#line 10 "start.x"

	}
