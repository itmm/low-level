
#line 5 "start.x"

	
#line 22 "start.x"

	
#line 29 "start.x"

	#include <cassert>

#line 44 "start.x"

	
#line 59 "start.x"

	#include <string>

#line 114 "start.x"

	#include <vector>

#line 209 "start.x"

	
#line 222 "start.x"

	enum class Token_Type {
		unknown,
		
#line 328 "start.x"

	becomes,

#line 383 "start.x"

	plus,

#line 408 "start.x"

	reg,

#line 767 "start.x"

	number,

#line 225 "start.x"

		end
	};

#line 307 "start.x"

	#include <iostream>

#line 462 "start.x"

	#include <cctype>

#line 210 "start.x"

	class Tokenizer {
			
#line 233 "start.x"

	Token_Type _type =
		Token_Type::unknown;

#line 250 "start.x"

	std::string::const_iterator _cur;
	std::string::const_iterator _end;

#line 415 "start.x"

	std::string _name = {};

#line 773 "start.x"

	int _value = 0;

#line 212 "start.x"
;
		public:
			
#line 241 "start.x"

	Token_Type type() const {
		return _type;
	}

#line 258 "start.x"

	void next();

#line 265 "start.x"

	Tokenizer(const std::string &s):
		_cur { s.begin() },
		_end { s.end() } 
	{
		next();
	}

#line 422 "start.x"

	const std::string &name() const {
		return _name;
	}

#line 797 "start.x"

	int value() const { return _value; }

#line 214 "start.x"
;
	};
	
#line 277 "start.x"

	void Tokenizer::next() {
		
#line 286 "start.x"

	while (
		_cur != _end && *_cur <= ' '
	) {
		++_cur;
	}

#line 297 "start.x"

	if (_cur == _end) {
		_type = Token_Type::end;
		return;
	}

#line 314 "start.x"

	do {
		
#line 367 "start.x"

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

#line 397 "start.x"

	if (*_cur == '+') {
		_type = Token_Type::plus;
		++_cur;
		break;
	}

#line 469 "start.x"

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

#line 779 "start.x"

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

#line 316 "start.x"
;
		_type = Token_Type::unknown;
		std::cerr <<
			"unrecognized char [" <<
			*_cur++ << "] \n" ;
	} while (false);

#line 279 "start.x"
;
	}

#line 216 "start.x"
;

#line 491 "start.x"

	class Expression {
		public:
			virtual ~Expression() {}
	};

#line 501 "start.x"

	#include <memory>

#line 508 "start.x"

	using Expression_Ptr =
		std::unique_ptr<Expression>;

#line 515 "start.x"

	class BinaryExpression:
		public Expression
	{
			Expression_Ptr _first;
			Expression_Ptr _second;
		public:
			
#line 528 "start.x"

	BinaryExpression(
		Expression_Ptr first,
		Expression_Ptr second
	):
		_first { std::move(first) },
		_second { std::move(second) }
	{ }

#line 540 "start.x"

	const Expression_Ptr &first() const {
		return _first;
	}
	const Expression_Ptr &second() const {
		return _second;
	}

#line 522 "start.x"
;
	};

#line 551 "start.x"

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

#line 568 "start.x"

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

#line 585 "start.x"

	class Register: public Expression {
			
#line 595 "start.x"

	const std::string _name;
	const int _nr;

#line 602 "start.x"

	static int nr_from_name(
		const std::string &name
	) {
		int nr { 0 };
		
#line 614 "start.x"

	if (name.empty()) { return -1; }
	if (name == "x") { return -1; }
	if (name[0] != 'x') { return -1; }

#line 622 "start.x"

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

#line 638 "start.x"

	if (nr < 0 || nr > 31) {
		return -1;
	}

#line 607 "start.x"
;
		return nr;
	}

#line 587 "start.x"
;
		public:
			
#line 646 "start.x"

	Register(const std::string &name):
		_name { name },
		_nr { nr_from_name(name) }
	{ }

#line 655 "start.x"

	const std::string &name() const {
		return _name;
	}
	int nr() const { return _nr; }
	bool is_general() const {
		return _nr >= 0;
	}

#line 589 "start.x"
;
	};

#line 667 "start.x"

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

#line 684 "start.x"

	Expression_Ptr parse_factor(
		Tokenizer &t
	) {
		
#line 696 "start.x"

	if (t.type() == Token_Type::reg) {
		auto res =
			std::make_unique<Register>(
				t.name()
			);
		t.next();
		return res;
	}

#line 803 "start.x"

	if (t.type() == Token_Type::number) {
		auto res =
			std::make_unique<Number>(
				t.value()
			);
		t.next();
		return res;
	}

#line 688 "start.x"
;
		std::cerr << "no factor\n";
		return Expression_Ptr {};
	}

#line 709 "start.x"

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

#line 740 "start.x"

	int build_r_cmd(
		int funct7, char src2, char src1,
		int funct3, char dst, int opcode
	) {
		return (funct7 << 25) |
			(src2 << 20) | (src1 << 15) |
			(funct3 << 12) | (dst << 7) |
			opcode;
	}

#line 754 "start.x"

	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}

#line 45 "start.x"
;
	class State {
			
#line 91 "start.x"

	void add_machine(int instr);

#line 47 "start.x"

		public:
			
#line 66 "start.x"

	void add_line(
		const std::string &line
	);

#line 107 "start.x"

	
#line 121 "start.x"

	private:
		std::vector<int> code;

#line 129 "start.x"

	public:
		int code_size() const {
			return static_cast<int>(
				code.size()
			);
		}

#line 141 "start.x"

	public:
		int get_code(int pos) const {
			return code[pos];
		}

#line 108 "start.x"


#line 49 "start.x"

	};
	
#line 75 "start.x"

	void State::add_line(
		const std::string &line
	) {
		
#line 816 "start.x"

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

#line 79 "start.x"
;
	}

#line 98 "start.x"

	void State::add_machine(int instr) {
		
#line 151 "start.x"

	code.push_back(instr);

#line 100 "start.x"

	}

#line 51 "start.x"
;

#line 158 "start.x"

	void assert_line(
		const char *line,
		int expected
	) {
		
#line 188 "start.x"

	State s;
	s.add_line(line);
	assert(s.code_size() == 1);
	assert(s.get_code(0) == expected);

#line 163 "start.x"
;
	}

#line 335 "start.x"

	void assert_token(
		const char *line,
		Token_Type token
	) {
		
#line 356 "start.x"

	Tokenizer t(line);
	assert(t.type() == token);
	t.next();
	assert(t.type() == Token_Type::end);

#line 340 "start.x"
;
	}

#line 431 "start.x"

	void assert_register(
		const char *line,
		const char *name
	) {
		
#line 443 "start.x"

	Tokenizer t(line);
	assert(t.type() == Token_Type::reg);
	assert(t.name() == name);
	t.next();
	assert(t.type() == Token_Type::end);

#line 436 "start.x"
;
	}

#line 23 "start.x"


#line 4 "hex.x"

	
#line 30 "hex.x"

	void write_byte(
		std::ostream &out, int b
	) {
		
#line 99 "hex.x"

	static const char map[] {
		"0123456789ABCDEF"
	};
	out << map[(b >> 4) & 0xf];
	out << map[b & 0xf];

#line 34 "hex.x"
;
	}

#line 40 "hex.x"

	void write_word(
		std::ostream &out, int w
	) {
		write_byte(out, w >> 8);
		write_byte(out, w);
	}

#line 5 "hex.x"
;
	std::ostream &operator<<(
		std::ostream &out, const State &s
	) {
		
#line 22 "hex.x"

	
#line 51 "hex.x"

	int base = 0x20010000;
	out << ":02000004";
	int sum = 0x06;
	sum += (base >> 24) + (base >> 16);
	write_word(out, base >> 16);
	write_byte(out, -sum);
	out << '\n';
	

#line 23 "hex.x"
;
	
#line 64 "hex.x"

	int len = s.code_size();
	int c = 0;
	for (; c < len; c += 4) {
		out << ':';
		int l = ((c + 4 > len) ? (len - c) : 4);
		write_byte(out, l * 4);
		int sum = l * 4;
		int addr = base + c * 4;
		sum += (addr >> 8) + addr;
		write_word(out, addr);
		out << "00";
		for (int i = 0; i < l; ++i) {
			int m = s.get_code(c + i);
			write_byte(out, m); sum += m;
			m = m >> 8;
			write_byte(out, m); sum += m;
			m = m >> 8;
			write_byte(out, m); sum += m;
			m = m >> 8;
			write_byte(out, m); sum += m;
		}
		write_byte(out, -sum);
		out << '\n';
	}

#line 24 "hex.x"
;
	
#line 93 "hex.x"

	out << ":00000001FF\n";

#line 25 "hex.x"
;

#line 9 "hex.x"
;
		return out;
	}

#line 6 "start.x"
;
	int main(
		int argc, const char *argv[]
	) {
		
#line 171 "start.x"

	
#line 178 "start.x"

	assert_line(
		"%x4 <- %x2 + %x3",
		0x00310233
	);

#line 347 "start.x"

	assert_token(
		"<-", Token_Type::becomes
	);

#line 390 "start.x"

	assert_token("+", Token_Type::plus);

#line 454 "start.x"

	assert_register("%x10", "x10");
	assert_register("%pc", "pc");

#line 892 "start.x"

	assert_line(
		"%pc <- %pc + 0",
		0x0000006f
	);

#line 903 "start.x"

	assert_line(
		"%pc <- %pc + -28",
		0xfe5ff06f
	);

#line 913 "start.x"

	assert_line(
		"%pc <- %pc + -32",
		0xfe1ff06f
	);

#line 172 "start.x"


#line 10 "start.x"

		
#line 923 "start.x"

	
#line 930 "start.x"

	State s;
	std::string l;
	while (std::getline(std::cin, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}

#line 924 "start.x"


#line 16 "hex.x"

	std::cout << s;

#line 11 "start.x"

	}
