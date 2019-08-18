
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

#line 391 "start.x"

	plus,

#line 416 "start.x"

	ident,

#line 875 "start.x"

	number,

#line 1108 "start.x"

	t_and,
	t_or,

#line 1525 "start.x"

	t_if,
	t_colon,

#line 1749 "start.x"

	t_equals,
	t_not_equals,

#line 1889 "start.x"

	t_open_bracket,
	t_close_bracket,

#line 226 "start.x"

		end
	};

#line 308 "start.x"

	#include <iostream>

#line 470 "start.x"

	#include <cctype>

#line 211 "start.x"

	class Tokenizer {
			
#line 234 "start.x"

	Token_Type _type =
		Token_Type::unknown;

#line 251 "start.x"

	std::string::const_iterator _cur;
	std::string::const_iterator _end;

#line 423 "start.x"

	std::string _name = {};

#line 881 "start.x"

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

#line 430 "start.x"

	const std::string &name() const {
		return _name;
	}

#line 905 "start.x"

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
		
#line 371 "start.x"

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

#line 405 "start.x"

	if (*_cur == '+') {
		_type = Token_Type::plus;
		++_cur;
		break;
	}

#line 477 "start.x"

	if (isalpha(*_cur) || *_cur == '%') {
		auto c = _cur;
		_name = {};
		while (c != _end && (
			isalnum(*c) || *c == '_' ||
			*c == '%'
		)) {
			_name += *c++;
		}
		_type = Token_Type::ident;
		
#line 1532 "start.x"

	if (_name == "if") {
		_type = Token_Type::t_if;
	}

#line 488 "start.x"
;
		if (_name == "if") {
			_type = Token_Type::t_if;
		}
		_cur = c;
		break;
	}

#line 887 "start.x"

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

#line 1076 "start.x"

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

#line 1099 "start.x"

	if (*_cur == '#') {
		_type = Token_Type::end;
		break;
	}

#line 1115 "start.x"

	if (*_cur == '&') {
		_type = Token_Type::t_and;
		++_cur;
		break;
	}

#line 1125 "start.x"

	if (*_cur == '|') {
		_type = Token_Type::t_or;
		++_cur;
		break;
	}

#line 1540 "start.x"

	if (*_cur == ':') {
		_type = Token_Type::t_colon;
		++_cur;
		break;
	}

#line 1756 "start.x"

	if (*_cur == '=') {
		_type = Token_Type::t_equals;
		++_cur;
		break;
	}

#line 1766 "start.x"

	if (*_cur == '!' && _cur + 1 < _end && _cur[1] == '=') {
		_type = Token_Type::t_not_equals;
		_cur += 2;
		break;
	}

#line 1896 "start.x"

	if (*_cur == '[') {
		_type = Token_Type::t_open_bracket;
		++_cur;
		break;
	}

#line 1906 "start.x"

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

#line 504 "start.x"

	#include <memory>

#line 511 "start.x"

	class Expression {
		public:
			virtual ~Expression() {}
			virtual std::unique_ptr<Expression> clone() {
				return std::unique_ptr<Expression> {};
			}
	};

#line 524 "start.x"

	using Expression_Ptr =
		std::unique_ptr<Expression>;

#line 532 "start.x"

	class BinaryExpression:
		public Expression
	{
			Expression_Ptr _first;
			Expression_Ptr _second;
		public:
			
#line 546 "start.x"

	BinaryExpression(
		Expression_Ptr first,
		Expression_Ptr second
	):
		_first { std::move(first) },
		_second { std::move(second) }
	{ }

#line 559 "start.x"

	const Expression_Ptr &first() const {
		return _first;
	}
	const Expression_Ptr &second() const {
		return _second;
	}

#line 539 "start.x"
;
	};

#line 571 "start.x"

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

#line 589 "start.x"

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

#line 607 "start.x"

	class Gen_Register: public Expression {
			
#line 619 "start.x"

	const int _nr;

#line 609 "start.x"
;
		public:
			
#line 629 "start.x"

	Gen_Register(int nr):
		_nr { nr }
	{ }

#line 638 "start.x"

	int nr() const { return _nr; }
	Expression_Ptr clone() override {
		return std::make_unique<Gen_Register>(_nr);
	}

#line 611 "start.x"
;
	};

#line 649 "start.x"

	class Pc_Register: public Expression {
		public:
			Pc_Register() {}
			Expression_Ptr clone() override {
				return std::make_unique<Pc_Register>();
			}
	};

#line 661 "start.x"

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

#line 679 "start.x"

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

#line 701 "start.x"

	Expression_Ptr parse(Tokenizer &t);
	
#line 716 "start.x"

	#include <map>

#line 722 "start.x"

	static std::map<std::string, Expression_Ptr> _symbols;

#line 728 "start.x"

	void clear_symbols() {
		_symbols.clear();
		
#line 749 "start.x"

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

#line 731 "start.x"
;
	}

#line 1935 "start.x"

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

#line 703 "start.x"
;
	Expression_Ptr parse_factor(
		Tokenizer &t
	) {
		
#line 768 "start.x"

	if (t.type() == Token_Type::ident) {
		auto found { _symbols.find(t.name()) };
		if (found != _symbols.end()) {
			t.next();
			return found->second->clone();
		}
	}

#line 911 "start.x"

	if (t.type() == Token_Type::number) {
		auto res =
			std::make_unique<Number>(
				t.value()
			);
		t.next();
		return res;
	}

#line 1916 "start.x"

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

#line 707 "start.x"
;
		std::cerr << "no factor\n";
		return Expression_Ptr {};
	}

#line 781 "start.x"

	
#line 1135 "start.x"

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

#line 1153 "start.x"

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

#line 1550 "start.x"

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

#line 1592 "start.x"

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

#line 1776 "start.x"

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

#line 1793 "start.x"

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

#line 782 "start.x"
;
	Expression_Ptr parse(Tokenizer &t) {
		
#line 1568 "start.x"

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

#line 784 "start.x"
;
		auto dst = parse_factor(t);
		do {
			
#line 797 "start.x"

	if (t.type() == Token_Type::becomes) {
		t.next();
		auto src = parse(t);
		if (! src) {
			std::cerr << "no expression after <-\n";
			return Expression_Ptr { };
		}
		return std::make_unique<Assignment>(std::move(dst), std::move(src));
	}

#line 811 "start.x"

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

#line 1171 "start.x"

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

#line 1186 "start.x"

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

#line 1610 "start.x"

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

#line 1810 "start.x"

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

#line 1825 "start.x"

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

#line 787 "start.x"
;
		} while (false);
		return dst;
	}

#line 826 "start.x"

	int build_r_cmd(
		int funct7, char src2, char src1,
		int funct3, char dst, int opcode
	) {
		return (funct7 << 25) |
			(src2 << 20) | (src1 << 15) |
			(funct3 << 12) | (dst << 7) |
			opcode;
	}

#line 840 "start.x"

	int build_i_cmd(
		int imm, char src1, int funct3, char dst, int opcode
	) {
		return (imm << 20) | (src1 << 15) | (funct3 << 12) | (dst << 7) | opcode;
	}

#line 850 "start.x"

	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}

#line 863 "start.x"

	int build_add(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x0, dst, 0x13
		);
	}

#line 1255 "start.x"

	int build_and(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x7, dst, 0x33
		);
	}

#line 1268 "start.x"

	int build_and(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x7, dst, 0x13
		);
	}

#line 1280 "start.x"

	int build_or(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x6, dst, 0x33
		);
	}

#line 1293 "start.x"

	int build_or(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x6, dst, 0x13
		);
	}

#line 1341 "start.x"

	int build_u_cmd(
		int imm, char dst, int opcode
	) {
		return (imm & 0xfffff800) | (dst << 7) | opcode;
	}

#line 1351 "start.x"

	int build_lui(
		char dst, int imm
	) {
		return build_u_cmd(imm, dst, 0x37);
	}

#line 1406 "start.x"

	int build_auipc(
		char dst, int imm
	) {
		return build_u_cmd(imm, dst, 0x17);
	}

#line 1458 "start.x"

	int build_csrrw(
		char dst, int csr, char src
	) {
		return build_i_cmd(
			csr, src, 0x1, dst, 0x73
		);
	}

#line 1494 "start.x"

	int build_csrrs(
		char dst, int csr, char src
	) {
		return build_i_cmd(
			csr, src, 0x2, dst, 0x73
		);
	}

#line 1639 "start.x"

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

#line 1654 "start.x"

	int build_branch(
		int cond, char reg1, char reg2, int offset
	) {
		return build_b_cmd(
			offset, reg2, reg1, cond, 0x63
		);
	}

#line 1961 "start.x"

	int build_load(
		char dst, char src, int imm
	) {
		return build_i_cmd(
			imm, src, 0x2, dst, 0x03
		);
	}

#line 2025 "start.x"

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

#line 2040 "start.x"

	int build_store(
		char src, char dst, int imm
	) {
		return build_s_cmd(
			imm, src, dst, 0x2, 0x23
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
		
#line 924 "start.x"

	Tokenizer t { line };
	auto e = parse(t);
	if (! e) {
		std::cerr << "no statement on line " << line << '\n';
		return;
	}
	
#line 937 "start.x"

	Number *n = dynamic_cast<Number *>(&*e);
	if (n) {
		add_machine(n->value());
		return;
	}

#line 947 "start.x"

	Assignment *a = dynamic_cast<Assignment *>(&*e);
	if (a) {
		const Gen_Register *dst = dynamic_cast<const Gen_Register *>(&*a->first());
		if (dst) {
			
#line 960 "start.x"
 {
	const Addition *o = dynamic_cast<const Addition *>(&*a->second());
	if (o) {
		const Gen_Register *src1 = dynamic_cast<const Gen_Register *>(&*o->first());
		if (src1) {
			
#line 973 "start.x"

	const Gen_Register *src2 = dynamic_cast<const Gen_Register *>(&*o->second());
	if (src2) {
		add_machine(build_add(
			(char) dst->nr(), (char) src1->nr(), (char) src2->nr()
		));
		return;
	}

#line 985 "start.x"

	const Number *n2 = dynamic_cast<const Number *>(&*o->second());
	if (n2) {
		add_machine(build_add(
			(char) dst->nr(), (char) src1->nr(), n2->value()
		));
		return;
	}

#line 965 "start.x"
;
		}
		
#line 1435 "start.x"

	if (dynamic_cast<const Pc_Register *>(&*o->first())) {
		
#line 1443 "start.x"

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

#line 1437 "start.x"
;
	}

#line 967 "start.x"
;
	}
} 
#line 1201 "start.x"
 {
	const BinaryAnd *o = dynamic_cast<const BinaryAnd *>(&*a->second());
	if (o) {
		const Gen_Register *src1 = dynamic_cast<const Gen_Register *>(&*o->first());
		if (! src1) {
			std::cerr << "first op of and no register\n";
			return;
		}
		const Gen_Register *src2 = dynamic_cast<const Gen_Register *>(&*o->second());
		if (src2) {
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
#line 1228 "start.x"
 {
	const BinaryOr *o = dynamic_cast<const BinaryOr *>(&*a->second());
	if (o) {
		const Gen_Register *src1 = dynamic_cast<const Gen_Register *>(&*o->first());
		if (! src1) {
			std::cerr << "first op of or no register\n";
			return;
		}
		const Gen_Register *src2 = dynamic_cast<const Gen_Register *>(&*o->second());
		if (src2) {
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
#line 1332 "start.x"
 {
	const Number *o = dynamic_cast<const Number *>(&*a->second());
	if (o) {
		
#line 1361 "start.x"

	int upper { o->value() & ~ 0xfff };
	if (upper && upper != ~ 0xfff) {
		add_machine(build_lui(dst->nr(), o->value()));
	}

#line 1370 "start.x"

	if (o->value() == 0 || (o->value() & 0xfff)) {
		add_machine(build_add((char) dst->nr(), (char) 0, o->value()));
	}
	return;

#line 1335 "start.x"
;
	}
} 
#line 1416 "start.x"
 {
	const Pc_Register *o = dynamic_cast<const Pc_Register *>(&*a->second());
	if (o) {
		add_machine(build_auipc(dst->nr(), 0));
		return;
	}
} 
#line 1506 "start.x"
 {
	const Csr_Register *o = dynamic_cast<const Csr_Register *>(&*a->second());
	if (o) {
		add_machine(build_csrrs(dst->nr(), o->addr(), '\0'));
		return;
	}
} 
#line 1952 "start.x"
 {
	const Access *x = dynamic_cast<const Access *>(&*a->second());
	if (x) {
		
#line 1973 "start.x"
 {
	const Gen_Register *r = dynamic_cast<const Gen_Register *>(x->inner());
	if (r) {
		add_machine(build_load(dst->nr(), r->nr(), 0));
		return;
	}
} 
#line 1992 "start.x"
 {
	const Addition *d = dynamic_cast<const Addition *>(x->inner());
	if (d) {
		const Gen_Register *r = dynamic_cast<const Gen_Register *>(&*d->first());
		const Number *n = dynamic_cast<const Number *>(&*d->second());
		if (r && n) {
			add_machine(build_load(dst->nr(), r->nr(), n->value()));
			return;
		}
	}
} 
#line 1955 "start.x"
;
	}
} 
#line 952 "start.x"
;
		}
		
#line 997 "start.x"
 {
	if (dynamic_cast<const Pc_Register *>(&*a->first())) {
		const Addition *o = dynamic_cast<const Addition *>(&*a->second());
		if (! o) {
			std::cerr << "only addition supported now\n";
			return;
		}
		const Pc_Register *src1 = dynamic_cast<const Pc_Register *>(&*o->first());
		if (! src1) {
			std::cerr << "first op of addition not pc register\n";
			return;
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
} 
#line 1470 "start.x"
 {
	const Csr_Register *x = dynamic_cast<const Csr_Register *>(&*a->first());
	if (x) {
		const Gen_Register *src = dynamic_cast<const Gen_Register *>(&*a->second());
		if (! src) {
			std::cerr << "only assign general register\n";
			return;
		}
		add_machine(build_csrrw('\0', x->addr(), src->nr()));
		return;
	}
} 
#line 2015 "start.x"
 {
	const Access *x = dynamic_cast<const Access *>(&*a->first());
	if (x) {
		const Gen_Register *s = dynamic_cast<const Gen_Register *>(&*a->second());
		
#line 2052 "start.x"
 {
	const Gen_Register *d = dynamic_cast<const Gen_Register *>(x->inner());
	if (d && s) {
		add_machine(build_store(s->nr(), d->nr(), 0));
		return;
	}
} 
#line 2071 "start.x"
 {
	const Addition *p = dynamic_cast<const Addition *>(x->inner());
	if (p) {
		const Gen_Register *d = dynamic_cast<const Gen_Register *>(&*p->first());
		const Number *n = dynamic_cast<const Number *>(&*p->second());
		if (d && s && n) {
			add_machine(build_store(s->nr(), d->nr(), n->value()));
			return;
		}
	}
} 
#line 2019 "start.x"
;
	}
} 
#line 954 "start.x"
;
	}

#line 1625 "start.x"

	If *i = dynamic_cast<If *>(&*e);
	if (i) {
		int reg1 { -1 };
		int reg2 { -1 };
		int cond { -1 };
		int offset { 0x7fffffff };
		t.next();
		
#line 1666 "start.x"

	const Assignment *a = dynamic_cast<const Assignment *>(&*i->second());
	if (! a) {
		std::cerr << "no assignment in if\n";
		return;
	}
	{
		const Pc_Register *r = dynamic_cast<const Pc_Register *>(&*a->first());
		if (! r) {
			std::cerr << "expect %pc in if assignment\n";
			return;
		}
	}
	{
		const Addition *d = dynamic_cast<const Addition *>(&*a->second());
		if (d) {
			const Pc_Register *r = dynamic_cast<const Pc_Register *>(&*d->first());
			if (! r) {
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
	
#line 1840 "start.x"

	{
		const Equals *e = dynamic_cast<const Equals *>(&*i->first());
		if (e) {
			cond = 0x0;
		}
	}

#line 1851 "start.x"

	{
		const NotEquals *ne = dynamic_cast<const NotEquals *>(&*i->first());
		if (ne) {
			cond = 0x1;
		}
	}

#line 1701 "start.x"
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
				const Gen_Register *r = dynamic_cast<const Gen_Register *>(&*b->first());
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
				const Gen_Register *r = dynamic_cast<const Gen_Register *>(&*b->second());
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

#line 1633 "start.x"
;
	}

#line 931 "start.x"
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
		
#line 737 "start.x"

	clear_symbols();

#line 163 "start.x"
;
		
#line 189 "start.x"

	State s;
	s.add_line(line);
	assert(s.code_size() == 1);
	assert(s.get_code(0) == expected);

#line 164 "start.x"
;
	}

#line 338 "start.x"

	void assert_token(
		const char *line,
		Token_Type token
	) {
		
#line 359 "start.x"

	
#line 743 "start.x"

	clear_symbols();

#line 360 "start.x"
;
	Tokenizer t(line);
	assert(t.type() == token);
	t.next();
	assert(t.type() == Token_Type::end);

#line 343 "start.x"
;
	}

#line 439 "start.x"

	void assert_register(
		const char *line,
		const char *name
	) {
		
#line 451 "start.x"

	Tokenizer t(line);
	assert(t.type() == Token_Type::ident);
	assert(t.name() == name);
	t.next();
	assert(t.type() == Token_Type::end);

#line 444 "start.x"
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
		
#line 172 "start.x"

	
#line 179 "start.x"

	assert_line(
		"%x4 <- %x2 + %x3",
		0x00310233
	);

#line 350 "start.x"

	assert_token(
		"<-", Token_Type::becomes
	);

#line 398 "start.x"

	assert_token("+", Token_Type::plus);

#line 462 "start.x"

	assert_register("%x10", "%x10");
	assert_register("%pc", "%pc");

#line 1027 "start.x"

	assert_line(
		"%pc <- %pc + 0",
		0x0000006f
	);

#line 1038 "start.x"

	assert_line(
		"%pc <- %pc + -28",
		0xfe5ff06f
	);

#line 1048 "start.x"

	assert_line(
		"%pc <- %pc + -32",
		0xfe1ff06f
	);

#line 1305 "start.x"

	assert_line(
		"%x5 <- %x5 & $ff",
		0x0ff2f293
	);

#line 1314 "start.x"

	assert_line(
		"%x5 <- %x5 | $1",
		0x0012e293
	);

#line 1323 "start.x"

	assert_line(
		"%x6 <- %x6 | $1",
		0x00136313
	);

#line 1379 "start.x"

	assert_line(
		"%x11 <- $0d",
		0x00d00593
	);

#line 1388 "start.x"

	assert_line(
		"%x12 <- $0a",
		0x00a00613
	);

#line 1397 "start.x"

	assert_line(
		"%x10 <- $1013000",
		0x1013537
	);

#line 1426 "start.x"

	assert_line(
		"%x5 <- %pc",
		0x00000297
	);

#line 1485 "start.x"

	assert_line(
		"%mtvec <- %x5",
		0x30529073
	);

#line 1516 "start.x"

	assert_line(
		"%x5 <- %mhartid",
		0xf14022f3
	);

#line 1740 "start.x"

	assert_line(
		"if %x5 < 0: %pc <- %pc + -4",
		0xfe02cee3
	);

#line 1862 "start.x"

	assert_line(
		"if %x5 = 0: %pc <- %pc + -12",
		0xfe028ae3
	);

#line 1871 "start.x"

	assert_line(
		"if %x5 != %x11: %pc <- %pc + -28",
		0xfeb292e3
	);

#line 1880 "start.x"

	assert_line(
		"if %x5 != 0: %pc <- %pc + 0",
		0x00029063
	);

#line 1983 "start.x"

	assert_line(
		"%x6 <- [%x10]",
		0x00052303
	);

#line 2006 "start.x"

	assert_line(
		"%x5 <- [%x10 + $04]",
		0x00452283
	);

#line 2062 "start.x"

	assert_line(
		"[%x10] <- %x12",
		0x00c52023
	);

#line 2085 "start.x"

	assert_line(
		"[%x10 + $08] <- %x5",
		0x00552423
	);

#line 173 "start.x"


#line 10 "start.x"

		
#line 1058 "start.x"

	
#line 1065 "start.x"

	State s;
	std::string l;
	while (std::getline(std::cin, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}

#line 1059 "start.x"


#line 16 "hex.x"

	std::cout << s;

#line 11 "start.x"

	}
