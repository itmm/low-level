
#line 5 "start.x"

	
#line 22 "start.x"

	
#line 29 "start.x"

	#include <cassert>

#line 44 "start.x"

	
#line 59 "start.x"

	#include <string>

#line 114 "start.x"

	#include <vector>

#line 210 "start.x"

	
#line 223 "start.x"

	enum class Token_Type {
		unknown,
		
#line 330 "start.x"

	becomes,
	t_less,

#line 390 "start.x"

	plus,

#line 415 "start.x"

	reg,

#line 850 "start.x"

	number,

#line 1107 "start.x"

	t_and,
	t_or,

#line 1542 "start.x"

	t_if,
	t_colon,

#line 1768 "start.x"

	t_equals,
	t_not_equals,

#line 1908 "start.x"

	t_open_bracket,
	t_close_bracket,

#line 226 "start.x"

		end
	};

#line 308 "start.x"

	#include <iostream>

#line 469 "start.x"

	#include <cctype>

#line 211 "start.x"

	class Tokenizer {
			
#line 234 "start.x"

	Token_Type _type =
		Token_Type::unknown;

#line 251 "start.x"

	std::string::const_iterator _cur;
	std::string::const_iterator _end;

#line 422 "start.x"

	std::string _name = {};

#line 856 "start.x"

	int _value = 0;

#line 213 "start.x"
;
		public:
			
#line 242 "start.x"

	Token_Type type() const {
		return _type;
	}

#line 259 "start.x"

	void next();

#line 266 "start.x"

	Tokenizer(const std::string &s):
		_cur { s.begin() },
		_end { s.end() } 
	{
		next();
	}

#line 429 "start.x"

	const std::string &name() const {
		return _name;
	}

#line 880 "start.x"

	int value() const { return _value; }

#line 215 "start.x"
;
	};
	
#line 278 "start.x"

	void Tokenizer::next() {
		
#line 287 "start.x"

	while (
		_cur != _end && *_cur <= ' '
	) {
		++_cur;
	}

#line 298 "start.x"

	if (_cur == _end) {
		_type = Token_Type::end;
		return;
	}

#line 315 "start.x"

	do {
		
#line 370 "start.x"

	if (*_cur == '<') {
		if (
			_cur  + 1 != _end &&
				_cur[1] == '-'
		) {
			_type = Token_Type::becomes;
			_cur += 2;
			break;
		} else {
			_type = Token_Type::t_less;
			++_cur;
			break;
		}
	}

#line 404 "start.x"

	if (*_cur == '+') {
		_type = Token_Type::plus;
		++_cur;
		break;
	}

#line 476 "start.x"

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

#line 862 "start.x"

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

#line 1075 "start.x"

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

#line 1098 "start.x"

	if (*_cur == '#') {
		_type = Token_Type::end;
		break;
	}

#line 1114 "start.x"

	if (*_cur == '&') {
		_type = Token_Type::t_and;
		++_cur;
		break;
	}

#line 1124 "start.x"

	if (*_cur == '|') {
		_type = Token_Type::t_or;
		++_cur;
		break;
	}

#line 1549 "start.x"

	if (*_cur == 'i' && _cur[1] == 'f' && _cur[2] <= ' ') {
		_type = Token_Type::t_if;
		_cur += 2;
		break;
	}

#line 1559 "start.x"

	if (*_cur == ':') {
		_type = Token_Type::t_colon;
		++_cur;
		break;
	}

#line 1775 "start.x"

	if (*_cur == '=') {
		_type = Token_Type::t_equals;
		++_cur;
		break;
	}

#line 1785 "start.x"

	if (*_cur == '!' && _cur + 1 < _end && _cur[1] == '=') {
		_type = Token_Type::t_not_equals;
		_cur += 2;
		break;
	}

#line 1915 "start.x"

	if (*_cur == '[') {
		_type = Token_Type::t_open_bracket;
		++_cur;
		break;
	}

#line 1925 "start.x"

	if (*_cur == ']') {
		_type = Token_Type::t_close_bracket;
		++_cur;
		break;
	}

#line 317 "start.x"
;
		_type = Token_Type::unknown;
		std::cerr <<
			"unrecognized char [" <<
			*_cur << "] == " << (int) *_cur << '\n';
		++_cur;
	} while (false);

#line 280 "start.x"
;
	}

#line 217 "start.x"
;

#line 498 "start.x"

	class Expression {
		public:
			virtual ~Expression() {}
	};

#line 508 "start.x"

	#include <memory>

#line 515 "start.x"

	using Expression_Ptr =
		std::unique_ptr<Expression>;

#line 523 "start.x"

	class BinaryExpression:
		public Expression
	{
			Expression_Ptr _first;
			Expression_Ptr _second;
		public:
			
#line 537 "start.x"

	BinaryExpression(
		Expression_Ptr first,
		Expression_Ptr second
	):
		_first { std::move(first) },
		_second { std::move(second) }
	{ }

#line 550 "start.x"

	const Expression_Ptr &first() const {
		return _first;
	}
	const Expression_Ptr &second() const {
		return _second;
	}

#line 530 "start.x"
;
	};

#line 562 "start.x"

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

#line 580 "start.x"

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

#line 598 "start.x"

	class Register: public Expression {
			
#line 610 "start.x"

	const std::string _name;
	const int _nr;

#line 621 "start.x"

	static int nr_from_name(
		const std::string &name
	) {
		int nr { 0 };
		
#line 636 "start.x"

	if (name.empty()) { return -1; }

#line 643 "start.x"

	if (name == "x") { return -1; }

#line 650 "start.x"

	if (name[0] != 'x') { return -1; }

#line 658 "start.x"

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

#line 676 "start.x"

	if (nr < 0 || nr > 31) {
		return -1;
	}

#line 626 "start.x"
;
		return nr;
	}

#line 600 "start.x"
;
		public:
			
#line 685 "start.x"

	Register(const std::string &name):
		_name { name },
		_nr { nr_from_name(name) }
	{ }

#line 695 "start.x"

	const std::string &name() const {
		return _name;
	}
	int nr() const { return _nr; }
	bool is_general() const {
		return _nr >= 0;
	}

#line 602 "start.x"
;
	};

#line 709 "start.x"

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

#line 727 "start.x"

	Expression_Ptr parse(Tokenizer &t);
	
#line 1954 "start.x"

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

#line 729 "start.x"
;
	Expression_Ptr parse_factor(
		Tokenizer &t
	) {
		
#line 742 "start.x"

	if (t.type() == Token_Type::reg) {
		auto res =
			std::make_unique<Register>(
				t.name()
			);
		t.next();
		return res;
	}

#line 886 "start.x"

	if (t.type() == Token_Type::number) {
		auto res =
			std::make_unique<Number>(
				t.value()
			);
		t.next();
		return res;
	}

#line 1935 "start.x"

	if (t.type() == Token_Type::t_open_bracket) {
		t.next();
		auto inner = parse(t);
		if (! inner) {
			std::cerr << "no expr in memory access\n";
			return Expression_Ptr { };
		}
		if (t.type() != Token_Type::t_close_bracket) {
			std::cerr << "expecting ]\n";
			return Expression_Ptr { };
		}
		t.next();
		return std::make_unique<Access>(inner);
	}

#line 733 "start.x"
;
		std::cerr << "no factor\n";
		return Expression_Ptr {};
	}

#line 756 "start.x"

	
#line 1134 "start.x"

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

#line 1152 "start.x"

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

#line 1569 "start.x"

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

#line 1611 "start.x"

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

#line 1795 "start.x"

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

#line 1812 "start.x"

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

#line 757 "start.x"
;
	Expression_Ptr parse(Tokenizer &t) {
		
#line 1587 "start.x"

	if (t.type() == Token_Type::t_if) {
		t.next();
		auto cond = parse(t);
		if (! cond) {
			std::cerr << "no expression after if\n";
			return Expression_Ptr { };
		}
		if (t.type() != Token_Type::t_colon) {
			std::cerr << "expecting : after if expr\n";
			return Expression_Ptr { };
		}
		t.next();
		auto body = parse(t);
		if (! body) {
			std::cerr << "no if body\n";
			return Expression_Ptr { };
		}
		return std::make_unique<If>(std::move(cond), std::move(body));
	}

#line 759 "start.x"
;
		auto dst = parse_factor(t);
		do {
			
#line 772 "start.x"

	if (t.type() == Token_Type::becomes) {
		t.next();
		auto src = parse(t);
		if (! src) {
			std::cerr << "no expression after <-\n";
			return Expression_Ptr { };
		}
		return std::make_unique<Assignment>(std::move(dst), std::move(src));
	}

#line 786 "start.x"

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

#line 1170 "start.x"

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

#line 1185 "start.x"

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

#line 1629 "start.x"

	if (t.type() == Token_Type::t_less) {
		t.next();
		auto src = parse_factor(t);
		if (! src) {
			std::cerr << "no factor after <\n";
			return Expression_Ptr { };
		}
		dst = std::make_unique<Less>(std::move(dst), std::move(src));
		continue;
	}

#line 1829 "start.x"

	if (t.type() == Token_Type::t_equals) {
		t.next();
		auto src = parse_factor(t);
		if (! src) {
			std::cerr << "no factor after =\n";
			return Expression_Ptr { };
		}
		dst = std::make_unique<Equals>(std::move(dst), std::move(src));
		continue;
	}

#line 1844 "start.x"

	if (t.type() == Token_Type::t_not_equals) {
		t.next();
		auto src = parse_factor(t);
		if (! src) {
			std::cerr << "no factor after !=\n";
			return Expression_Ptr { };
		}
		dst = std::make_unique<NotEquals>(std::move(dst), std::move(src));
		continue;
	}

#line 762 "start.x"
;
		} while (false);
		return dst;
	}

#line 801 "start.x"

	int build_r_cmd(
		int funct7, char src2, char src1,
		int funct3, char dst, int opcode
	) {
		return (funct7 << 25) |
			(src2 << 20) | (src1 << 15) |
			(funct3 << 12) | (dst << 7) |
			opcode;
	}

#line 815 "start.x"

	int build_i_cmd(
		int imm, char src1, int funct3, char dst, int opcode
	) {
		return (imm << 20) | (src1 << 15) | (funct3 << 12) | (dst << 7) | opcode;
	}

#line 825 "start.x"

	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}

#line 838 "start.x"

	int build_add(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x0, dst, 0x13
		);
	}

#line 1266 "start.x"

	int build_and(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x7, dst, 0x33
		);
	}

#line 1279 "start.x"

	int build_and(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x7, dst, 0x13
		);
	}

#line 1291 "start.x"

	int build_or(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x6, dst, 0x33
		);
	}

#line 1304 "start.x"

	int build_or(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x6, dst, 0x13
		);
	}

#line 1352 "start.x"

	int build_u_cmd(
		int imm, char dst, int opcode
	) {
		return (imm & 0xfffff800) | (dst << 7) | opcode;
	}

#line 1362 "start.x"

	int build_lui(
		char dst, int imm
	) {
		return build_u_cmd(imm, dst, 0x37);
	}

#line 1417 "start.x"

	int build_auipc(
		char dst, int imm
	) {
		return build_u_cmd(imm, dst, 0x17);
	}

#line 1477 "start.x"

	int build_csrrw(
		char dst, int csr, char src
	) {
		return build_i_cmd(
			csr, src, 0x1, dst, 0x73
		);
	}

#line 1512 "start.x"

	int build_csrrs(
		char dst, int csr, char src
	) {
		return build_i_cmd(
			csr, src, 0x2, dst, 0x73
		);
	}

#line 1658 "start.x"

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

#line 1673 "start.x"

	int build_branch(
		int cond, char reg1, char reg2, int offset
	) {
		return build_b_cmd(
			offset, reg2, reg1, cond, 0x63
		);
	}

#line 1980 "start.x"

	int build_load(
		char dst, char src, int imm
	) {
		return build_i_cmd(
			imm, src, 0x2, dst, 0x03
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
		
#line 899 "start.x"

	Tokenizer t { line };
	auto e = parse(t);
	if (! e) {
		std::cerr << "no statement on line " << line << '\n';
		return;
	}
	
#line 912 "start.x"

	Number *n = dynamic_cast<Number *>(&*e);
	if (n) {
		add_machine(n->value());
		return;
	}

#line 922 "start.x"

	Assignment *a = dynamic_cast<Assignment *>(&*e);
	if (a) {
		const Register *dst = dynamic_cast<const Register *>(&*a->first());
		if (! dst) {
			std::cerr << "no assignment to register\n";
			return;
		}
		
#line 936 "start.x"

	if (dst->is_general()) {
		
#line 944 "start.x"
 {
	const Addition *o = dynamic_cast<const Addition *>(&*a->second());
	if (o) {
		const Register *src1 = dynamic_cast<const Register *>(&*o->first());
		if (! src1) {
			std::cerr << "first op of addition no register\n";
			return;
		}
		
#line 958 "start.x"

	if (src1->is_general()) {
		
#line 966 "start.x"

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

#line 981 "start.x"

	const Number *n2 = dynamic_cast<const Number *>(&*o->second());
	if (n2) {
		add_machine(build_add(
			(char) dst->nr(), (char) src1->nr(), n2->value()
		));
		return;
	}

#line 960 "start.x"
;
	}

#line 1454 "start.x"

	if (src1->name() == "pc") {
		
#line 1462 "start.x"

	const Number *n2 = dynamic_cast<const Number *>(&*o->second());
	if (n2) {
		add_machine(build_auipc(
			(char) dst->nr(), n2->value()
		));
		if (n2->value() & 0xfff) {
			add_machine(build_add((char) dst->nr(), (char) dst->nr(), n2->value() & 0xfff));
		}
		return;
	}

#line 1456 "start.x"
;
	}

#line 952 "start.x"
;
	}
} 
#line 1200 "start.x"
 {
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
} 
#line 1233 "start.x"
 {
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
} 
#line 1343 "start.x"
 {
	const Number *o = dynamic_cast<const Number *>(&*a->second());
	if (o) {
		
#line 1372 "start.x"

	int upper { o->value() & ~ 0xfff };
	if (upper && upper != ~ 0xfff) {
		add_machine(build_lui(dst->nr(), o->value()));
	}

#line 1381 "start.x"

	if (o->value() == 0 || (o->value() & 0xfff)) {
		add_machine(build_add((char) dst->nr(), (char) 0, o->value()));
	}
	return;

#line 1346 "start.x"
;
	}
} 
#line 1427 "start.x"
 {
	const Register *o = dynamic_cast<const Register *>(&*a->second());
	if (o) {
		
#line 1436 "start.x"

	if (o->name() == "pc") {
		add_machine(build_auipc(dst->nr(), 0));
		return;
	}

#line 1524 "start.x"

	if (o->name() == "mhartid") {
		add_machine(build_csrrs(dst->nr(), 0xf14, '\0'));
		return;
	}

#line 1430 "start.x"
;
	}
} 
#line 1971 "start.x"
 {
	const Access *x = dynamic_cast<const Access *>(&*a->second());
	if (x) {
		
#line 1992 "start.x"
 {
	const Register *r = dynamic_cast<const Register *>(x->inner());
	if (r && r->is_general()) {
		add_machine(build_load(dst->nr(), r->nr(), 0));
		return;
	}
} 
#line 2011 "start.x"
 {
	const Addition *d = dynamic_cast<const Addition *>(x->inner());
	if (d) {
		const Register *r = dynamic_cast<const Register *>(&*d->first());
		const Number *n = dynamic_cast<const Number *>(&*d->second());
		if (r && r->is_general() && n) {
			add_machine(build_load(dst->nr(), r->nr(), n->value()));
			return;
		}
	}
} 
#line 1974 "start.x"
;
	}
} 
#line 938 "start.x"
;
	}

#line 993 "start.x"

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

#line 1489 "start.x"

	if (dst->name() == "mtvec") {
		const Register *src = dynamic_cast<const Register *>(&*a->second());
		if (! src || ! src->is_general()) {
			std::cerr << "only assign general register\n";
			return;
		}
		add_machine(build_csrrw('\0', 0x305, src->nr()));
		return;
	}

#line 930 "start.x"
;
	}

#line 1644 "start.x"

	If *i = dynamic_cast<If *>(&*e);
	if (i) {
		int reg1 { -1 };
		int reg2 { -1 };
		int cond { -1 };
		int offset { 0x7fffffff };
		t.next();
		
#line 1685 "start.x"

	const Assignment *a = dynamic_cast<const Assignment *>(&*i->second());
	if (! a) {
		std::cerr << "no assignment in if\n";
		return;
	}
	{
		const Register *r = dynamic_cast<const Register *>(&*a->first());
		if (! r || r->name() != "pc") {
			std::cerr << "expect %pc in if assignment\n";
			return;
		}
	}
	{
		const Addition *d = dynamic_cast<const Addition *>(&*a->second());
		if (d) {
			const Register *r = dynamic_cast<const Register *>(&*d->first());
			if (! r || r->name() != "pc") {
				std::cerr << "expect %pc in addition\n";
				return;
			}
			const Number *n = dynamic_cast<const Number *>(&*d->second());
			if (! n) {
				std::cerr << "expected number in addition\n";
				return;
			}
			offset = n->value();
		}
	}
	{
		const Less *l = dynamic_cast<const Less *>(&*i->first());
		if (l) {
			cond = 0x4;
		}
	}
	
#line 1859 "start.x"

	{
		const Equals *e = dynamic_cast<const Equals *>(&*i->first());
		if (e) {
			cond = 0x0;
		}
	}

#line 1870 "start.x"

	{
		const NotEquals *ne = dynamic_cast<const NotEquals *>(&*i->first());
		if (ne) {
			cond = 0x1;
		}
	}

#line 1720 "start.x"
;
	{
		const BinaryExpression *b = dynamic_cast<const BinaryExpression *>(&*i->first());
		if (b) {
			{
				const Number *n = dynamic_cast<const Number *>(&*b->first());
				if (n && n->value() == 0) {
					reg1 = 0;
				}
			}
			{
				const Register *r = dynamic_cast<const Register *>(&*b->first());
				if (r) {
					reg1 = r->nr();
				}
			}
			{
				const Number *n = dynamic_cast<const Number *>(&*b->second());
				if (n && n->value() == 0) {
					reg2 = 0;
				}

			}
			{
				const Register *r = dynamic_cast<const Register *>(&*b->second());
				if (r) {
					reg2 = r->nr();
				}
			}
		}
	}
	if (reg1 >= 0 && reg2 >=0 && offset != 0x7fffffff && cond >= 0) {
		add_machine(build_branch(cond, reg1, reg2, offset));
		return;
	}

#line 1652 "start.x"
;
	}

#line 906 "start.x"
;
	std::cerr << "can't parse " << line << '\n';

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
	// std::cerr << std::hex << expected << ", " << s.get_code(0) << '\n';
	assert(s.get_code(0) == expected);

#line 163 "start.x"
;
	}

#line 338 "start.x"

	void assert_token(
		const char *line,
		Token_Type token
	) {
		
#line 359 "start.x"

	Tokenizer t(line);
	assert(t.type() == token);
	t.next();
	assert(t.type() == Token_Type::end);

#line 343 "start.x"
;
	}

#line 438 "start.x"

	void assert_register(
		const char *line,
		const char *name
	) {
		
#line 450 "start.x"

	Tokenizer t(line);
	assert(t.type() == Token_Type::reg);
	assert(t.name() == name);
	t.next();
	assert(t.type() == Token_Type::end);

#line 443 "start.x"
;
	}

#line 23 "start.x"


#line 4 "hex.x"

	
#line 30 "hex.x"

	void write_byte(
		std::ostream &out, int b
	) {
		
#line 106 "hex.x"

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
	out << "\r\n";

#line 23 "hex.x"
;
	
#line 63 "hex.x"

	int len = s.code_size();
	int c = 0;
	for (; c < len; c += 4) {
		out << ':';
		int l = ((c + 4 > len) ? (len - c) : 4);
		write_byte(out, l * 4);
		sum = l * 4;
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
		out << "\r\n";
	}

#line 24 "hex.x"
;
	
#line 92 "hex.x"

	sum =  9;
	out << ":04000005";
	write_word(out, base >> 16);
	write_word(out, base);
	sum += (base >> 24) + (base >> 16) +
		(base >> 8) + base;
	write_byte(out, -sum);
	out << "\r\n";
	out << ":00000001FF\r\n";

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

#line 350 "start.x"

	assert_token(
		"<-", Token_Type::becomes
	);

#line 397 "start.x"

	assert_token("+", Token_Type::plus);

#line 461 "start.x"

	assert_register("%x10", "x10");
	assert_register("%pc", "pc");

#line 1026 "start.x"

	assert_line(
		"%pc <- %pc + 0",
		0x0000006f
	);

#line 1037 "start.x"

	assert_line(
		"%pc <- %pc + -28",
		0xfe5ff06f
	);

#line 1047 "start.x"

	assert_line(
		"%pc <- %pc + -32",
		0xfe1ff06f
	);

#line 1316 "start.x"

	assert_line(
		"%x5 <- %x5 & $ff",
		0x0ff2f293
	);

#line 1325 "start.x"

	assert_line(
		"%x5 <- %x5 | $1",
		0x0012e293
	);

#line 1334 "start.x"

	assert_line(
		"%x6 <- %x6 | $1",
		0x00136313
	);

#line 1390 "start.x"

	assert_line(
		"%x11 <- $0d",
		0x00d00593
	);

#line 1399 "start.x"

	assert_line(
		"%x12 <- $0a",
		0x00a00613
	);

#line 1408 "start.x"

	assert_line(
		"%x10 <- $1013000",
		0x1013537
	);

#line 1445 "start.x"

	assert_line(
		"%x5 <- %pc",
		0x00000297
	);

#line 1503 "start.x"

	assert_line(
		"%mtvec <- %x5",
		0x30529073
	);

#line 1533 "start.x"

	assert_line(
		"%x5 <- %mhartid",
		0xf14022f3
	);

#line 1759 "start.x"

	assert_line(
		"if %x5 < 0: %pc <- %pc + -4",
		0xfe02cee3
	);

#line 1881 "start.x"

	assert_line(
		"if %x5 = 0: %pc <- %pc + -12",
		0xfe028ae3
	);

#line 1890 "start.x"

	assert_line(
		"if %x5 != %x11: %pc <- %pc + -28",
		0xfeb292e3
	);

#line 1899 "start.x"

	assert_line(
		"if %x5 != 0: %pc <- %pc + 0",
		0x00029063
	);

#line 2002 "start.x"

	assert_line(
		"%x6 <- [%x10]",
		0x00052303
	);

#line 2025 "start.x"

	assert_line(
		"%x5 <- [%x10 + $04]",
		0x00452283
	);

#line 172 "start.x"


#line 10 "start.x"

		
#line 1057 "start.x"

	
#line 1064 "start.x"

	State s;
	std::string l;
	while (std::getline(std::cin, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}

#line 1058 "start.x"


#line 16 "hex.x"

	std::cout << s;

#line 11 "start.x"

	}
