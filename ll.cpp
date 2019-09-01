
#line 5 "start.x"

	
#line 22 "start.x"

	
#line 29 "start.x"

	#include <cassert>

#line 44 "start.x"

	
#line 59 "start.x"

	#include <string>

#line 115 "start.x"

	#include <vector>

#line 212 "start.x"

	
#line 225 "start.x"

	enum class Token_Type {
		unknown,
		
#line 349 "start.x"

	ident,

#line 376 "start.x"

	t_raw,

#line 399 "start.x"

	number,

#line 443 "start.x"

	becomes,
	t_less,

#line 504 "start.x"

	plus, minus,

#line 1140 "start.x"

	t_and,
	t_or,

#line 1398 "start.x"

	t_if,
	t_colon,

#line 1555 "start.x"

	t_equals,
	t_not_equals,

#line 1673 "start.x"

	t_open_bracket,
	t_close_bracket,

#line 1811 "start.x"

	t_open_parenthesis,
	t_close_parenthesis,

#line 1878 "start.x"

	t_times,

#line 228 "start.x"

		end
	};

#line 236 "start.x"

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

#line 327 "start.x"

	#include <iostream>

#line 566 "start.x"

	#include <cctype>

#line 213 "start.x"

	class Tokenizer {
			
#line 254 "start.x"

	Token _token { Token_Type::unknown };

#line 270 "start.x"

	std::string::const_iterator _cur;
	std::string::const_iterator _end;

#line 215 "start.x"
;
		public:
			
#line 261 "start.x"

	const Token &token() const {
		return _token;
	}

#line 278 "start.x"

	void next();

#line 285 "start.x"

	Tokenizer(const std::string &s):
		_cur { s.begin() },
		_end { s.end() } 
	{
		next();
	}

#line 217 "start.x"
;
	};
	
#line 297 "start.x"

	void Tokenizer::next() {
		
#line 306 "start.x"

	while (
		_cur != _end && *_cur <= ' '
	) {
		++_cur;
	}

#line 317 "start.x"

	if (_cur == _end) {
		_token = Token { Token_Type::end };
		return;
	}

#line 334 "start.x"

	do {
		
#line 355 "start.x"

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
		
#line 382 "start.x"

	if (name == "raw") {
		type = Token_Type::t_raw;
	}

#line 1405 "start.x"

	if (name == "if") {
		type = Token_Type::t_if;
	}
	if (name == "and") {
		type = Token_Type::t_and;
	}
	if (name == "or") {
		type = Token_Type::t_or;
	}

#line 366 "start.x"
;
		_token = Token { type, name };
		_cur = c;
		break;
	}

#line 390 "start.x"

	if (*_cur == '#') {
		_token = Token { Token_Type::end };
		break;
	}

#line 405 "start.x"

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

#line 420 "start.x"

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

#line 484 "start.x"

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

#line 519 "start.x"

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

#line 1419 "start.x"

	if (*_cur == ':') {
		_token = Token { Token_Type::t_colon };
		++_cur;
		break;
	}

#line 1562 "start.x"

	if (*_cur == '=') {
		_token = Token { Token_Type::t_equals };
		++_cur;
		break;
	}

#line 1572 "start.x"

	if (*_cur == '!' && _cur + 1 < _end && _cur[1] == '=') {
		_token = Token { Token_Type::t_not_equals };
		_cur += 2;
		break;
	}

#line 1680 "start.x"

	if (*_cur == '[') {
		_token = Token_Type::t_open_bracket;
		++_cur;
		break;
	}

#line 1690 "start.x"

	if (*_cur == ']') {
		_token = Token_Type::t_close_bracket;
		++_cur;
		break;
	}

#line 1818 "start.x"

	if (*_cur == '(') {
		_token = Token { Token_Type::t_open_parenthesis };
		++_cur;
		break;
	}

#line 1828 "start.x"

	if (*_cur == ')') {
		_token = Token { Token_Type::t_close_parenthesis };
		++_cur;
		break;
	}

#line 1884 "start.x"

	if (*_cur == '*') {
		_token = Token { Token_Type::t_times };
		++_cur;
		break;
	}

#line 336 "start.x"
;
		_token = Token { Token_Type::unknown };
		std::cerr <<
			"unrecognized char [" <<
			*_cur << "] == " << (int) *_cur << '\n';
		++_cur;
	} while (false);

#line 299 "start.x"
;
	}

#line 219 "start.x"
;

#line 576 "start.x"

	#include <memory>

#line 583 "start.x"

	class Expression {
		public:
			virtual ~Expression() {}
			virtual std::unique_ptr<Expression> clone() {
				return std::unique_ptr<Expression> {};
			}
	};

#line 596 "start.x"

	using Expression_Ptr =
		std::unique_ptr<Expression>;

#line 614 "start.x"

	class BinaryExpression:
		public Expression
	{
			Expression_Ptr _first;
			Expression_Ptr _second;
		public:
			
#line 628 "start.x"

	BinaryExpression(
		Expression_Ptr first,
		Expression_Ptr second
	):
		_first { std::move(first) },
		_second { std::move(second) }
	{ }

#line 641 "start.x"

	const Expression_Ptr &first() const {
		return _first;
	}
	const Expression_Ptr &second() const {
		return _second;
	}

#line 621 "start.x"
;
	};

#line 653 "start.x"

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

#line 671 "start.x"

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

#line 694 "start.x"

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

#line 712 "start.x"

	class Gen_Register: public Expression {
			
#line 724 "start.x"

	const int _nr;

#line 714 "start.x"
;
		public:
			
#line 734 "start.x"

	Gen_Register(int nr):
		_nr { nr }
	{ }

#line 743 "start.x"

	int nr() const { return _nr; }
	Expression_Ptr clone() override {
		return std::make_unique<Gen_Register>(_nr);
	}

#line 716 "start.x"
;
	};

#line 754 "start.x"

	class Pc_Register: public Expression {
		public:
			Pc_Register() {}
			Expression_Ptr clone() override {
				return std::make_unique<Pc_Register>();
			}
	};

#line 766 "start.x"

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

#line 784 "start.x"

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

#line 806 "start.x"

	#include <vector>
	Expression_Ptr parse(std::vector<Token>::iterator &cur, std::vector<Token>::iterator end, int addr);
	
#line 821 "start.x"

	#include <map>

#line 827 "start.x"

	static std::map<std::string, Expression_Ptr> _symbols;

#line 833 "start.x"

	void clear_symbols() {
		_symbols.clear();
		
#line 854 "start.x"
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
#line 873 "start.x"

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

#line 836 "start.x"
;
	}

#line 1719 "start.x"

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

#line 809 "start.x"
;
	Expression_Ptr parse_factor(
		std::vector<Token>::iterator &cur, std::vector<Token>::iterator end, int addr
	) {
		
#line 913 "start.x"

	if (cur == end) { return Expression_Ptr { }; };
	if (cur->type() == Token_Type::ident) {
		auto found { _symbols.find(cur->name()) };
		if (found != _symbols.end()) {
			++cur;
			return found->second->clone();
		}
	}

#line 1048 "start.x"

	if (cur->type() == Token_Type::number) {
		auto res =
			std::make_unique<Number>(
				cur->value()
			);
		++cur;
		return res;
	}

#line 1061 "start.x"

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

#line 1700 "start.x"

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

#line 1838 "start.x"

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

#line 1857 "start.x"

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

#line 1894 "start.x"

	if (cur->type() == Token_Type::t_times) {
		++cur;
		return std::make_unique<Number>(addr);
	}

#line 813 "start.x"
;
		return Expression_Ptr {};
	}

#line 927 "start.x"

	
#line 1147 "start.x"

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

#line 1165 "start.x"

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

#line 1429 "start.x"

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

#line 1486 "start.x"

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

#line 1582 "start.x"

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

#line 1599 "start.x"

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

#line 928 "start.x"
;
	Expression_Ptr parse(std::vector<Token>::iterator &cur, std::vector<Token>::iterator end, int addr) {
		
#line 1447 "start.x"

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

#line 1462 "start.x"

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

#line 930 "start.x"
;
		auto dst = parse_factor(cur, end, addr);
		do {
			
#line 943 "start.x"

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

#line 958 "start.x"

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

#line 979 "start.x"

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

#line 1183 "start.x"

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

#line 1198 "start.x"

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

#line 1504 "start.x"

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

#line 1616 "start.x"

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

#line 1631 "start.x"

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

#line 933 "start.x"
;
		} while (false);
		return dst;
	}

#line 999 "start.x"

	int build_r_cmd(
		int funct7, char src2, char src1,
		int funct3, char dst, int opcode
	) {
		return (funct7 << 25) |
			(src2 << 20) | (src1 << 15) |
			(funct3 << 12) | (dst << 7) |
			opcode;
	}

#line 1013 "start.x"

	int build_i_cmd(
		int imm, char src1, int funct3, char dst, int opcode
	) {
		return (imm << 20) | (src1 << 15) | (funct3 << 12) | (dst << 7) | opcode;
	}

#line 1023 "start.x"

	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}

#line 1036 "start.x"

	int build_add(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x0, dst, 0x13
		);
	}

#line 1213 "start.x"

	int build_and(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x7, dst, 0x33
		);
	}

#line 1226 "start.x"

	int build_and(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x7, dst, 0x13
		);
	}

#line 1238 "start.x"

	int build_or(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x6, dst, 0x33
		);
	}

#line 1251 "start.x"

	int build_or(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x6, dst, 0x13
		);
	}

#line 1290 "start.x"

	int build_u_cmd(
		int imm, char dst, int opcode
	) {
		return (imm & 0xfffff800) | (dst << 7) | opcode;
	}

#line 1300 "start.x"

	int build_lui(
		char dst, int imm
	) {
		return build_u_cmd(imm, dst, 0x37);
	}

#line 1337 "start.x"

	int build_auipc(
		char dst, int imm
	) {
		return build_u_cmd(imm, dst, 0x17);
	}

#line 1356 "start.x"

	int build_csrrw(
		char dst, int csr, char src
	) {
		return build_i_cmd(
			csr, src, 0x1, dst, 0x73
		);
	}

#line 1377 "start.x"

	int build_csrrs(
		char dst, int csr, char src
	) {
		return build_i_cmd(
			csr, src, 0x2, dst, 0x73
		);
	}

#line 1519 "start.x"

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

#line 1534 "start.x"

	int build_branch(
		int cond, char reg1, char reg2, int offset
	) {
		return build_b_cmd(
			offset, reg2, reg1, cond, 0x63
		);
	}

#line 1736 "start.x"

	int build_load(
		char dst, char src, int imm
	) {
		return build_i_cmd(
			imm, src, 0x2, dst, 0x03
		);
	}

#line 1766 "start.x"

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

#line 1781 "start.x"

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
			
#line 92 "start.x"

	void add_machine(int instr);

#line 47 "start.x"

		public:
			
#line 66 "start.x"

	void add_line(
		const std::string &line
	);

#line 108 "start.x"

	
#line 122 "start.x"

	private:
		std::vector<int> code;

#line 130 "start.x"

	public:
		int code_size() const {
			return static_cast<int>(
				code.size()
			);
		}

#line 142 "start.x"

	public:
		int get_code(int pos) const {
			return code[pos];
		}

#line 109 "start.x"


#line 49 "start.x"

	};
	
#line 75 "start.x"

	
#line 1903 "start.x"

	class Item {
		public:
			virtual ~Item() {};
	};

#line 1912 "start.x"

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

#line 1928 "start.x"

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

#line 76 "start.x"
;
	void State::add_line(
		const std::string &line
	) {
		
#line 1077 "start.x"

	Tokenizer t { line };
	std::vector<Token> ts;
	while (t.token().type() != Token_Type::end) {
		ts.push_back(t.token());
		t.next();
	}
	auto cur { ts.begin() };
	// unsigned addr { code.size() * 4 + 0x20010000 };
	
#line 1944 "start.x"

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

#line 1086 "start.x"
;

#line 80 "start.x"
;
	}

#line 99 "start.x"

	void State::add_machine(int instr) {
		
#line 152 "start.x"

	code.push_back(instr);

#line 101 "start.x"

	}

#line 51 "start.x"
;

#line 159 "start.x"

	void assert_line(const char *line, int expected) {
		std::cerr << "ignoring " << line << "\n";
	}
	void assert_line_2(
		const char *line,
		int expected
	) {
		
#line 842 "start.x"

	clear_symbols();

#line 167 "start.x"
;
		
#line 191 "start.x"

	State s;
	s.add_line(line);
	assert(s.code_size() == 1);
	assert(s.get_code(0) == expected);

#line 168 "start.x"
;
	}

#line 451 "start.x"

	void assert_token(
		const char *line,
		Token_Type token
	) {
		
#line 472 "start.x"

	
#line 848 "start.x"

	clear_symbols();

#line 473 "start.x"
;
	Tokenizer t(line);
	assert(t.token().type() == token);
	t.next();
	assert(t.token().type() == Token_Type::end);

#line 456 "start.x"
;
	}

#line 535 "start.x"

	void assert_register(
		const char *line,
		const char *name
	) {
		
#line 547 "start.x"

	Tokenizer t(line);
	assert(t.token().type() == Token_Type::ident);
	assert(t.token().name() == name);
	t.next();
	assert(t.token().type() == Token_Type::end);

#line 540 "start.x"
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
		
#line 176 "start.x"

	
#line 183 "start.x"

	assert_line_2(
		"raw $87654321", 0x87654321
	);

#line 463 "start.x"

	assert_token(
		"<-", Token_Type::becomes
	);

#line 511 "start.x"

	assert_token("+", Token_Type::plus);
	assert_token("-", Token_Type::minus);

#line 558 "start.x"

	assert_register("%x10", "%x10");
	assert_register("%pc", "%pc");

#line 604 "start.x"

	assert_line(
		"%x4 <- %x2 + %x3",
		0x00310233
	);

#line 1091 "start.x"

	assert_line(
		"%pc <- %pc + 0",
		0x0000006f
	);

#line 1102 "start.x"

	assert_line(
		"%pc <- %pc + -28",
		0xfe5ff06f
	);

#line 1112 "start.x"

	assert_line(
		"%pc <- %pc + -32",
		0xfe1ff06f
	);

#line 1263 "start.x"

	assert_line(
		"%x5 <- %x5 and $ff",
		0x0ff2f293
	);

#line 1272 "start.x"

	assert_line(
		"%x5 <- %x5 or $1",
		0x0012e293
	);

#line 1281 "start.x"

	assert_line(
		"%x6 <- %x6 or $1",
		0x00136313
	);

#line 1310 "start.x"

	assert_line(
		"%x11 <- $0d",
		0x00d00593
	);

#line 1319 "start.x"

	assert_line(
		"%x12 <- $0a",
		0x00a00613
	);

#line 1328 "start.x"

	assert_line(
		"%x10 <- $1013000",
		0x1013537
	);

#line 1347 "start.x"

	assert_line(
		"%x5 <- %pc",
		0x00000297
	);

#line 1368 "start.x"

	assert_line(
		"%mtvec <- %x5",
		0x30529073
	);

#line 1389 "start.x"

	assert_line(
		"%x5 <- %mhartid",
		0xf14022f3
	);

#line 1546 "start.x"

	assert_line(
		"if %x5 < 0: %pc <- %pc + -4",
		0xfe02cee3
	);

#line 1646 "start.x"

	assert_line(
		"if %x5 = 0: %pc <- %pc + -12",
		0xfe028ae3
	);

#line 1655 "start.x"

	assert_line(
		"if %x5 != %x11: %pc <- %pc + -28",
		0xfeb292e3
	);

#line 1664 "start.x"

	assert_line(
		"if %x5 != 0: %pc <- %pc + 0",
		0x00029063
	);

#line 1748 "start.x"

	assert_line(
		"%x6 <- [%x10]",
		0x00052303
	);

#line 1757 "start.x"

	assert_line(
		"%x5 <- [%x10 + $04]",
		0x00452283
	);

#line 1793 "start.x"

	assert_line(
		"[%x10] <- %x12",
		0x00c52023
	);

#line 1802 "start.x"

	assert_line(
		"[%x10 + $08] <- %x5",
		0x00552423
	);

#line 177 "start.x"


#line 10 "start.x"

		
#line 1122 "start.x"

	
#line 1129 "start.x"

	State s;
	std::string l;
	while (std::getline(std::cin, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}

#line 1123 "start.x"


#line 16 "hex.x"

	std::cout << s;

#line 11 "start.x"

	}
