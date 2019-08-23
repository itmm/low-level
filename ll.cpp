
#line 5 "start.x"

	
#line 22 "start.x"

	
#line 29 "start.x"

	#include <cassert>

#line 44 "start.x"

	
#line 59 "start.x"

	#include <string>

#line 114 "start.x"

	#include <vector>

#line 218 "start.x"

	
#line 231 "start.x"

	enum class Token_Type {
		unknown,
		
#line 338 "start.x"

	ident,

#line 380 "start.x"

	t_raw,

#line 403 "start.x"

	number,

#line 459 "start.x"

	becomes,
	t_less,

#line 520 "start.x"

	plus, minus,

#line 1229 "start.x"

	t_and,
	t_or,

#line 1626 "start.x"

	t_if,
	t_colon,

#line 1870 "start.x"

	t_equals,
	t_not_equals,

#line 2010 "start.x"

	t_open_bracket,
	t_close_bracket,

#line 2215 "start.x"

	t_open_parenthesis,
	t_close_parenthesis,

#line 2282 "start.x"

	t_times,

#line 234 "start.x"

		end
	};

#line 316 "start.x"

	#include <iostream>

#line 582 "start.x"

	#include <cctype>

#line 219 "start.x"

	class Tokenizer {
			
#line 242 "start.x"

	Token_Type _type =
		Token_Type::unknown;

#line 259 "start.x"

	std::string::const_iterator _cur;
	std::string::const_iterator _end;

#line 344 "start.x"

	std::string _name = {};

#line 409 "start.x"

	int _value = 0;

#line 221 "start.x"
;
		public:
			
#line 250 "start.x"

	Token_Type type() const {
		return _type;
	}

#line 267 "start.x"

	void next();

#line 274 "start.x"

	Tokenizer(const std::string &s):
		_cur { s.begin() },
		_end { s.end() } 
	{
		next();
	}

#line 351 "start.x"

	const std::string &name() const {
		return _name;
	}

#line 430 "start.x"

	int value() const { return _value; }

#line 223 "start.x"
;
	};
	
#line 286 "start.x"

	void Tokenizer::next() {
		
#line 295 "start.x"

	while (
		_cur != _end && *_cur <= ' '
	) {
		++_cur;
	}

#line 306 "start.x"

	if (_cur == _end) {
		_type = Token_Type::end;
		return;
	}

#line 323 "start.x"

	do {
		
#line 360 "start.x"

	if (isalpha(*_cur) || *_cur == '%' || *_cur == '_') {
		auto c = _cur;
		_name = {};
		while (c != _end && (
			isalnum(*c) || *c == '_' ||
			*c == '%'
		)) {
			_name += *c++;
		}
		_type = Token_Type::ident;
		
#line 386 "start.x"

	if (_name == "raw") {
		_type = Token_Type::t_raw;
	}

#line 1633 "start.x"

	if (_name == "if") {
		_type = Token_Type::t_if;
	}
	if (_name == "and") {
		_type = Token_Type::t_and;
	}
	if (_name == "or") {
		_type = Token_Type::t_or;
	}

#line 371 "start.x"
;
		_cur = c;
		break;
	}

#line 394 "start.x"

	if (*_cur == '#') {
		_type = Token_Type::end;
		break;
	}

#line 415 "start.x"

	if (isdigit(*_cur)) {
		_value = 0;
		while (isdigit(*_cur)) {
			_value = _value * 10 +
				(*_cur - '0');
			++_cur;
		}
		_type = Token_Type::number;
		break;
	}

#line 436 "start.x"

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

#line 500 "start.x"

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

#line 535 "start.x"

	if (*_cur == '+') {
		_type = Token_Type::plus;
		++_cur;
		break;
	}
	if (*_cur == '-') {
		_type = Token_Type::minus;
		++_cur;
		break;
	}

#line 1647 "start.x"

	if (*_cur == ':') {
		_type = Token_Type::t_colon;
		++_cur;
		break;
	}

#line 1877 "start.x"

	if (*_cur == '=') {
		_type = Token_Type::t_equals;
		++_cur;
		break;
	}

#line 1887 "start.x"

	if (*_cur == '!' && _cur + 1 < _end && _cur[1] == '=') {
		_type = Token_Type::t_not_equals;
		_cur += 2;
		break;
	}

#line 2017 "start.x"

	if (*_cur == '[') {
		_type = Token_Type::t_open_bracket;
		++_cur;
		break;
	}

#line 2027 "start.x"

	if (*_cur == ']') {
		_type = Token_Type::t_close_bracket;
		++_cur;
		break;
	}

#line 2222 "start.x"

	if (*_cur == '(') {
		_type = Token_Type::t_open_parenthesis;
		++_cur;
		break;
	}

#line 2232 "start.x"

	if (*_cur == ')') {
		_type = Token_Type::t_close_parenthesis;
		++_cur;
		break;
	}

#line 2288 "start.x"

	if (*_cur == '*') {
		_type = Token_Type::t_times;
		++_cur;
		break;
	}

#line 325 "start.x"
;
		_type = Token_Type::unknown;
		std::cerr <<
			"unrecognized char [" <<
			*_cur << "] == " << (int) *_cur << '\n';
		++_cur;
	} while (false);

#line 288 "start.x"
;
	}

#line 225 "start.x"
;

#line 592 "start.x"

	#include <memory>

#line 599 "start.x"

	class Expression {
		public:
			virtual ~Expression() {}
			virtual std::unique_ptr<Expression> clone() {
				return std::unique_ptr<Expression> {};
			}
	};

#line 612 "start.x"

	using Expression_Ptr =
		std::unique_ptr<Expression>;

#line 620 "start.x"

	class BinaryExpression:
		public Expression
	{
			Expression_Ptr _first;
			Expression_Ptr _second;
		public:
			
#line 634 "start.x"

	BinaryExpression(
		Expression_Ptr first,
		Expression_Ptr second
	):
		_first { std::move(first) },
		_second { std::move(second) }
	{ }

#line 647 "start.x"

	const Expression_Ptr &first() const {
		return _first;
	}
	const Expression_Ptr &second() const {
		return _second;
	}

#line 627 "start.x"
;
	};

#line 659 "start.x"

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

#line 677 "start.x"

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

#line 700 "start.x"

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

#line 718 "start.x"

	class Gen_Register: public Expression {
			
#line 730 "start.x"

	const int _nr;

#line 720 "start.x"
;
		public:
			
#line 740 "start.x"

	Gen_Register(int nr):
		_nr { nr }
	{ }

#line 749 "start.x"

	int nr() const { return _nr; }
	Expression_Ptr clone() override {
		return std::make_unique<Gen_Register>(_nr);
	}

#line 722 "start.x"
;
	};

#line 760 "start.x"

	class Pc_Register: public Expression {
		public:
			Pc_Register() {}
			Expression_Ptr clone() override {
				return std::make_unique<Pc_Register>();
			}
	};

#line 772 "start.x"

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

#line 790 "start.x"

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

#line 812 "start.x"

	Expression_Ptr parse(Tokenizer &t, int addr);
	
#line 826 "start.x"

	#include <map>

#line 832 "start.x"

	static std::map<std::string, Expression_Ptr> _symbols;

#line 838 "start.x"

	void clear_symbols() {
		_symbols.clear();
		
#line 859 "start.x"
 {
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
} 
#line 878 "start.x"

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

#line 841 "start.x"
;
	}

#line 2056 "start.x"

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

#line 814 "start.x"
;
	Expression_Ptr parse_factor(
		Tokenizer &t, int addr
	) {
		
#line 918 "start.x"

	if (t.type() == Token_Type::ident) {
		auto found { _symbols.find(t.name()) };
		if (found != _symbols.end()) {
			t.next();
			return found->second->clone();
		}
	}

#line 1051 "start.x"

	if (t.type() == Token_Type::number) {
		auto res =
			std::make_unique<Number>(
				t.value()
			);
		t.next();
		return res;
	}

#line 1064 "start.x"

	if (t.type() == Token_Type::minus) {
		t.next();
		auto res = parse_factor(t, addr);
		auto n = dynamic_cast<const Number *>(&*res);
		if (n) {
			return std::make_unique<Number>(
				-n->value()
			);
		}
		std::cerr << "no number after -\n";
	}

#line 2037 "start.x"

	if (t.type() == Token_Type::t_open_bracket) {
		t.next();
		auto inner = parse(t, addr);
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

#line 2242 "start.x"

	if (t.type() == Token_Type::t_open_parenthesis) {
		t.next();
		auto inner = parse(t, addr);
		if (! inner) {
			std::cerr << "no expr in after (\n";
			return Expression_Ptr { };
		}
		if (t.type() != Token_Type::t_close_parenthesis) {
			std::cerr << "expecting )\n" << (int) t.type() << ", " << (int) Token_Type::t_close_parenthesis << "\n";
			return Expression_Ptr { };
		}
		t.next();
		return inner;
	}

#line 2261 "start.x"

	if (t.type() == Token_Type::ident) {
		std::string name { t.name() };
		t.next();
		if (t.type() == Token_Type::t_equals) {
			t.next();
			auto value { parse(t, addr) };
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

#line 2298 "start.x"

	if (t.type() == Token_Type::t_times) {
		t.next();
		return std::make_unique<Number>(addr);
	}

#line 818 "start.x"
;
		return Expression_Ptr {};
	}

#line 931 "start.x"

	
#line 1236 "start.x"

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

#line 1254 "start.x"

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

#line 1657 "start.x"

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

#line 1713 "start.x"

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

#line 1897 "start.x"

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

#line 1914 "start.x"

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

#line 932 "start.x"
;
	Expression_Ptr parse(Tokenizer &t, int addr) {
		
#line 1675 "start.x"

	if (t.type() == Token_Type::t_raw) {
		t.next();
		auto val { parse(t, addr) };
		if (! val) {
			std::cerr << "no number after raw\n";
			return Expression_Ptr { };
		}
		return val;
	}

#line 1689 "start.x"

	if (t.type() == Token_Type::t_if) {
		t.next();
		auto cond = parse(t, addr);
		if (! cond) {
			std::cerr << "no expression after if\n";
			return Expression_Ptr { };
		}
		if (t.type() != Token_Type::t_colon) {
			std::cerr << "expecting : after if expr\n";
			return Expression_Ptr { };
		}
		t.next();
		auto body = parse(t, addr);
		if (! body) {
			std::cerr << "no if body\n";
			return Expression_Ptr { };
		}
		return std::make_unique<If>(std::move(cond), std::move(body));
	}

#line 934 "start.x"
;
		auto dst = parse_factor(t, addr);
		do {
			
#line 947 "start.x"

	if (t.type() == Token_Type::becomes) {
		t.next();
		auto src = parse(t, addr);
		if (! src) {
			std::cerr << "no expression after <-\n";
			return Expression_Ptr { };
		}
		return std::make_unique<Assignment>(std::move(dst), std::move(src));
	}

#line 961 "start.x"

	if (t.type() == Token_Type::plus) {
		t.next();
		auto src = parse_factor(t, addr);
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

#line 982 "start.x"

	if (t.type() == Token_Type::minus) {
		t.next();
		auto src = parse_factor(t, addr);
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

#line 1272 "start.x"

	if (t.type() == Token_Type::t_and) {
		t.next();
		auto src = parse_factor(t, addr);
		if (! src) {
			std::cerr << "no factor after &\n";
			return Expression_Ptr { };
		}
		dst = std::make_unique<BinaryAnd>(std::move(dst), std::move(src));
		continue;
	}

#line 1287 "start.x"

	if (t.type() == Token_Type::t_or) {
		t.next();
		auto src = parse_factor(t, addr);
		if (! src) {
			std::cerr << "no factor after |\n";
			return Expression_Ptr { };
		}
		dst = std::make_unique<BinaryOr>(std::move(dst), std::move(src));
		continue;
	}

#line 1731 "start.x"

	if (t.type() == Token_Type::t_less) {
		t.next();
		auto src = parse_factor(t, addr);
		if (! src) {
			std::cerr << "no factor after <\n";
			return Expression_Ptr { };
		}
		dst = std::make_unique<Less>(std::move(dst), std::move(src));
		continue;
	}

#line 1931 "start.x"

	if (t.type() == Token_Type::t_equals) {
		t.next();
		auto src = parse_factor(t, addr);
		if (! src) {
			std::cerr << "no factor after =\n";
			return Expression_Ptr { };
		}
		dst = std::make_unique<Equals>(std::move(dst), std::move(src));
		continue;
	}

#line 1946 "start.x"

	if (t.type() == Token_Type::t_not_equals) {
		t.next();
		auto src = parse_factor(t, addr);
		if (! src) {
			std::cerr << "no factor after !=\n";
			return Expression_Ptr { };
		}
		dst = std::make_unique<NotEquals>(std::move(dst), std::move(src));
		continue;
	}

#line 937 "start.x"
;
		} while (false);
		return dst;
	}

#line 1002 "start.x"

	int build_r_cmd(
		int funct7, char src2, char src1,
		int funct3, char dst, int opcode
	) {
		return (funct7 << 25) |
			(src2 << 20) | (src1 << 15) |
			(funct3 << 12) | (dst << 7) |
			opcode;
	}

#line 1016 "start.x"

	int build_i_cmd(
		int imm, char src1, int funct3, char dst, int opcode
	) {
		return (imm << 20) | (src1 << 15) | (funct3 << 12) | (dst << 7) | opcode;
	}

#line 1026 "start.x"

	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}

#line 1039 "start.x"

	int build_add(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x0, dst, 0x13
		);
	}

#line 1356 "start.x"

	int build_and(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x7, dst, 0x33
		);
	}

#line 1369 "start.x"

	int build_and(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x7, dst, 0x13
		);
	}

#line 1381 "start.x"

	int build_or(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x6, dst, 0x33
		);
	}

#line 1394 "start.x"

	int build_or(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x6, dst, 0x13
		);
	}

#line 1442 "start.x"

	int build_u_cmd(
		int imm, char dst, int opcode
	) {
		return (imm & 0xfffff800) | (dst << 7) | opcode;
	}

#line 1452 "start.x"

	int build_lui(
		char dst, int imm
	) {
		return build_u_cmd(imm, dst, 0x37);
	}

#line 1507 "start.x"

	int build_auipc(
		char dst, int imm
	) {
		return build_u_cmd(imm, dst, 0x17);
	}

#line 1559 "start.x"

	int build_csrrw(
		char dst, int csr, char src
	) {
		return build_i_cmd(
			csr, src, 0x1, dst, 0x73
		);
	}

#line 1595 "start.x"

	int build_csrrs(
		char dst, int csr, char src
	) {
		return build_i_cmd(
			csr, src, 0x2, dst, 0x73
		);
	}

#line 1760 "start.x"

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

#line 1775 "start.x"

	int build_branch(
		int cond, char reg1, char reg2, int offset
	) {
		return build_b_cmd(
			offset, reg2, reg1, cond, 0x63
		);
	}

#line 2082 "start.x"

	int build_load(
		char dst, char src, int imm
	) {
		return build_i_cmd(
			imm, src, 0x2, dst, 0x03
		);
	}

#line 2146 "start.x"

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

#line 2161 "start.x"

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
		
#line 1080 "start.x"

	Tokenizer t { line };
	auto e = parse(t, code.size() * 4 + 0x20010000);
	if (e) {
		
#line 1090 "start.x"

	Number *n = dynamic_cast<Number *>(&*e);
	if (n) {
		add_machine(n->value());
		return;
	}

#line 1100 "start.x"

	Assignment *a = dynamic_cast<Assignment *>(&*e);
	if (a) {
		const Gen_Register *dst = dynamic_cast<const Gen_Register *>(&*a->first());
		if (dst) {
			
#line 1113 "start.x"
 {
	const Addition *o = dynamic_cast<const Addition *>(&*a->second());
	if (o) {
		const Gen_Register *src1 = dynamic_cast<const Gen_Register *>(&*o->first());
		if (src1) {
			
#line 1126 "start.x"

	const Gen_Register *src2 = dynamic_cast<const Gen_Register *>(&*o->second());
	if (src2) {
		add_machine(build_add(
			(char) dst->nr(), (char) src1->nr(), (char) src2->nr()
		));
		return;
	}

#line 1138 "start.x"

	const Number *n2 = dynamic_cast<const Number *>(&*o->second());
	if (n2) {
		add_machine(build_add(
			(char) dst->nr(), (char) src1->nr(), n2->value()
		));
		return;
	}

#line 1118 "start.x"
;
		}
		
#line 1536 "start.x"

	if (dynamic_cast<const Pc_Register *>(&*o->first())) {
		
#line 1544 "start.x"

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

#line 1538 "start.x"
;
	}

#line 1120 "start.x"
;
	}
} 
#line 1302 "start.x"
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
#line 1329 "start.x"
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
#line 1433 "start.x"
 {
	const Number *o = dynamic_cast<const Number *>(&*a->second());
	if (o) {
		
#line 1462 "start.x"

	int upper { o->value() & ~ 0xfff };
	if (upper && upper != ~ 0xfff) {
		add_machine(build_lui(dst->nr(), o->value()));
	}

#line 1471 "start.x"

	if (o->value() == 0 || (o->value() & 0xfff)) {
		add_machine(build_add((char) dst->nr(), (char) 0, o->value()));
	}
	return;

#line 1436 "start.x"
;
	}
} 
#line 1517 "start.x"
 {
	const Pc_Register *o = dynamic_cast<const Pc_Register *>(&*a->second());
	if (o) {
		add_machine(build_auipc(dst->nr(), 0));
		return;
	}
} 
#line 1607 "start.x"
 {
	const Csr_Register *o = dynamic_cast<const Csr_Register *>(&*a->second());
	if (o) {
		add_machine(build_csrrs(dst->nr(), o->addr(), '\0'));
		return;
	}
} 
#line 2073 "start.x"
 {
	const Access *x = dynamic_cast<const Access *>(&*a->second());
	if (x) {
		
#line 2094 "start.x"
 {
	const Gen_Register *r = dynamic_cast<const Gen_Register *>(x->inner());
	if (r) {
		add_machine(build_load(dst->nr(), r->nr(), 0));
		return;
	}
} 
#line 2113 "start.x"
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
#line 2076 "start.x"
;
	}
} 
#line 1105 "start.x"
;
		}
		
#line 1150 "start.x"
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
#line 1571 "start.x"
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
#line 2136 "start.x"
 {
	const Access *x = dynamic_cast<const Access *>(&*a->first());
	if (x) {
		const Gen_Register *s = dynamic_cast<const Gen_Register *>(&*a->second());
		
#line 2173 "start.x"
 {
	const Gen_Register *d = dynamic_cast<const Gen_Register *>(x->inner());
	if (d && s) {
		add_machine(build_store(s->nr(), d->nr(), 0));
		return;
	}
} 
#line 2192 "start.x"
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
#line 2140 "start.x"
;
	}
} 
#line 1107 "start.x"
;
	}

#line 1746 "start.x"

	If *i = dynamic_cast<If *>(&*e);
	if (i) {
		int reg1 { -1 };
		int reg2 { -1 };
		int cond { -1 };
		int offset { 0x7fffffff };
		t.next();
		
#line 1787 "start.x"

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
	
#line 1961 "start.x"

	{
		const Equals *e = dynamic_cast<const Equals *>(&*i->first());
		if (e) {
			cond = 0x0;
		}
	}

#line 1972 "start.x"

	{
		const NotEquals *ne = dynamic_cast<const NotEquals *>(&*i->first());
		if (ne) {
			cond = 0x1;
		}
	}

#line 1822 "start.x"
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

#line 1754 "start.x"
;
	}

#line 1084 "start.x"
;
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
		
#line 847 "start.x"

	clear_symbols();

#line 163 "start.x"
;
		
#line 197 "start.x"

	State s;
	s.add_line(line);
	assert(s.code_size() == 1);
	assert(s.get_code(0) == expected);

#line 164 "start.x"
;
	}

#line 467 "start.x"

	void assert_token(
		const char *line,
		Token_Type token
	) {
		
#line 488 "start.x"

	
#line 853 "start.x"

	clear_symbols();

#line 489 "start.x"
;
	Tokenizer t(line);
	assert(t.type() == token);
	t.next();
	assert(t.type() == Token_Type::end);

#line 472 "start.x"
;
	}

#line 551 "start.x"

	void assert_register(
		const char *line,
		const char *name
	) {
		
#line 563 "start.x"

	Tokenizer t(line);
	assert(t.type() == Token_Type::ident);
	assert(t.name() == name);
	t.next();
	assert(t.type() == Token_Type::end);

#line 556 "start.x"
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
		"raw $87654321", 0x87654321
	);

#line 187 "start.x"

	assert_line(
		"%x4 <- %x2 + %x3",
		0x00310233
	);

#line 479 "start.x"

	assert_token(
		"<-", Token_Type::becomes
	);

#line 527 "start.x"

	assert_token("+", Token_Type::plus);
	assert_token("-", Token_Type::minus);

#line 574 "start.x"

	assert_register("%x10", "%x10");
	assert_register("%pc", "%pc");

#line 1180 "start.x"

	assert_line(
		"%pc <- %pc + 0",
		0x0000006f
	);

#line 1191 "start.x"

	assert_line(
		"%pc <- %pc + -28",
		0xfe5ff06f
	);

#line 1201 "start.x"

	assert_line(
		"%pc <- %pc + -32",
		0xfe1ff06f
	);

#line 1406 "start.x"

	assert_line(
		"%x5 <- %x5 and $ff",
		0x0ff2f293
	);

#line 1415 "start.x"

	assert_line(
		"%x5 <- %x5 or $1",
		0x0012e293
	);

#line 1424 "start.x"

	assert_line(
		"%x6 <- %x6 or $1",
		0x00136313
	);

#line 1480 "start.x"

	assert_line(
		"%x11 <- $0d",
		0x00d00593
	);

#line 1489 "start.x"

	assert_line(
		"%x12 <- $0a",
		0x00a00613
	);

#line 1498 "start.x"

	assert_line(
		"%x10 <- $1013000",
		0x1013537
	);

#line 1527 "start.x"

	assert_line(
		"%x5 <- %pc",
		0x00000297
	);

#line 1586 "start.x"

	assert_line(
		"%mtvec <- %x5",
		0x30529073
	);

#line 1617 "start.x"

	assert_line(
		"%x5 <- %mhartid",
		0xf14022f3
	);

#line 1861 "start.x"

	assert_line(
		"if %x5 < 0: %pc <- %pc + -4",
		0xfe02cee3
	);

#line 1983 "start.x"

	assert_line(
		"if %x5 = 0: %pc <- %pc + -12",
		0xfe028ae3
	);

#line 1992 "start.x"

	assert_line(
		"if %x5 != %x11: %pc <- %pc + -28",
		0xfeb292e3
	);

#line 2001 "start.x"

	assert_line(
		"if %x5 != 0: %pc <- %pc + 0",
		0x00029063
	);

#line 2104 "start.x"

	assert_line(
		"%x6 <- [%x10]",
		0x00052303
	);

#line 2127 "start.x"

	assert_line(
		"%x5 <- [%x10 + $04]",
		0x00452283
	);

#line 2183 "start.x"

	assert_line(
		"[%x10] <- %x12",
		0x00c52023
	);

#line 2206 "start.x"

	assert_line(
		"[%x10 + $08] <- %x5",
		0x00552423
	);

#line 173 "start.x"


#line 10 "start.x"

		
#line 1211 "start.x"

	
#line 1218 "start.x"

	State s;
	std::string l;
	while (std::getline(std::cin, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}

#line 1212 "start.x"


#line 16 "hex.x"

	std::cout << s;

#line 11 "start.x"

	}
