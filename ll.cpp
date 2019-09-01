
#line 5 "start.x"

	
#line 22 "start.x"

	
#line 29 "start.x"

	#include <cassert>

#line 44 "start.x"

	
#line 59 "start.x"

	#include <string>

#line 204 "start.x"

	
#line 217 "start.x"

	enum class Token_Type {
		unknown,
		
#line 341 "start.x"

	ident,

#line 368 "start.x"

	t_raw,

#line 391 "start.x"

	number,

#line 435 "start.x"

	becomes,
	t_less,

#line 496 "start.x"

	plus, minus,

#line 1165 "start.x"

	t_and,
	t_or,

#line 1423 "start.x"

	t_if,
	t_colon,

#line 1580 "start.x"

	t_equals,
	t_not_equals,

#line 1698 "start.x"

	t_open_bracket,
	t_close_bracket,

#line 1836 "start.x"

	t_open_parenthesis,
	t_close_parenthesis,

#line 1882 "start.x"

	t_times,

#line 220 "start.x"

		end
	};

#line 228 "start.x"

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

#line 319 "start.x"

	#include <iostream>

#line 558 "start.x"

	#include <cctype>

#line 205 "start.x"

	class Tokenizer {
			
#line 246 "start.x"

	Token _token { Token_Type::unknown };

#line 262 "start.x"

	std::string::const_iterator _cur;
	std::string::const_iterator _end;

#line 207 "start.x"
;
		public:
			
#line 253 "start.x"

	const Token &token() const {
		return _token;
	}

#line 270 "start.x"

	void next();

#line 277 "start.x"

	Tokenizer(const std::string &s):
		_cur { s.begin() },
		_end { s.end() } 
	{
		next();
	}

#line 209 "start.x"
;
	};
	
#line 289 "start.x"

	void Tokenizer::next() {
		
#line 298 "start.x"

	while (
		_cur != _end && *_cur <= ' '
	) {
		++_cur;
	}

#line 309 "start.x"

	if (_cur == _end) {
		_token = Token { Token_Type::end };
		return;
	}

#line 326 "start.x"

	do {
		
#line 347 "start.x"

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
		
#line 374 "start.x"

	if (name == "raw") {
		type = Token_Type::t_raw;
	}

#line 1430 "start.x"

	if (name == "if") {
		type = Token_Type::t_if;
	}
	if (name == "and") {
		type = Token_Type::t_and;
	}
	if (name == "or") {
		type = Token_Type::t_or;
	}

#line 358 "start.x"
;
		_token = Token { type, name };
		_cur = c;
		break;
	}

#line 382 "start.x"

	if (*_cur == '#') {
		_token = Token { Token_Type::end };
		break;
	}

#line 397 "start.x"

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

#line 412 "start.x"

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

#line 476 "start.x"

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

#line 511 "start.x"

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

#line 1444 "start.x"

	if (*_cur == ':') {
		_token = Token { Token_Type::t_colon };
		++_cur;
		break;
	}

#line 1587 "start.x"

	if (*_cur == '=') {
		_token = Token { Token_Type::t_equals };
		++_cur;
		break;
	}

#line 1597 "start.x"

	if (*_cur == '!' && _cur + 1 < _end && _cur[1] == '=') {
		_token = Token { Token_Type::t_not_equals };
		_cur += 2;
		break;
	}

#line 1705 "start.x"

	if (*_cur == '[') {
		_token = Token_Type::t_open_bracket;
		++_cur;
		break;
	}

#line 1715 "start.x"

	if (*_cur == ']') {
		_token = Token_Type::t_close_bracket;
		++_cur;
		break;
	}

#line 1843 "start.x"

	if (*_cur == '(') {
		_token = Token { Token_Type::t_open_parenthesis };
		++_cur;
		break;
	}

#line 1853 "start.x"

	if (*_cur == ')') {
		_token = Token { Token_Type::t_close_parenthesis };
		++_cur;
		break;
	}

#line 1888 "start.x"

	if (*_cur == '*') {
		_token = Token { Token_Type::t_times };
		++_cur;
		break;
	}

#line 328 "start.x"
;
		_token = Token { Token_Type::unknown };
		std::cerr <<
			"unrecognized char [" <<
			*_cur << "] == " << (int) *_cur << '\n';
		++_cur;
	} while (false);

#line 291 "start.x"
;
	}

#line 211 "start.x"
;

#line 568 "start.x"

	#include <memory>

#line 575 "start.x"

	class Expression {
		public:
			virtual ~Expression() {}
			virtual std::unique_ptr<Expression> clone() {
				return std::unique_ptr<Expression> {};
			}
	};

#line 588 "start.x"

	using Expression_Ptr =
		std::unique_ptr<Expression>;

#line 606 "start.x"

	class BinaryExpression:
		public Expression
	{
			Expression_Ptr _first;
			Expression_Ptr _second;
		public:
			
#line 620 "start.x"

	BinaryExpression(
		Expression_Ptr first,
		Expression_Ptr second
	):
		_first { std::move(first) },
		_second { std::move(second) }
	{ }

#line 633 "start.x"

	const Expression_Ptr &first() const {
		return _first;
	}
	const Expression_Ptr &second() const {
		return _second;
	}

#line 613 "start.x"
;
	};

#line 645 "start.x"

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

#line 663 "start.x"

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

#line 686 "start.x"

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

#line 704 "start.x"

	class Gen_Register: public Expression {
			
#line 716 "start.x"

	const int _nr;

#line 706 "start.x"
;
		public:
			
#line 726 "start.x"

	Gen_Register(int nr):
		_nr { nr }
	{ }

#line 735 "start.x"

	int nr() const { return _nr; }
	Expression_Ptr clone() override {
		return std::make_unique<Gen_Register>(_nr);
	}

#line 708 "start.x"
;
	};

#line 746 "start.x"

	class Pc_Register: public Expression {
		public:
			Pc_Register() {}
			Expression_Ptr clone() override {
				return std::make_unique<Pc_Register>();
			}
	};

#line 758 "start.x"

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

#line 776 "start.x"

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

#line 798 "start.x"

	#include <vector>
	Expression_Ptr parse(std::vector<Token>::iterator &cur, std::vector<Token>::iterator end, int addr);
	
#line 813 "start.x"

	#include <map>
	#include <vector>

#line 820 "start.x"

	class Item;
	static std::map<std::string, std::vector<std::unique_ptr<Item>>> _symbols;

#line 827 "start.x"

	
#line 1907 "start.x"

	class Item {
		public:
			virtual ~Item() {};
			virtual Item *clone() const = 0;
	};

#line 1917 "start.x"

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

#line 1936 "start.x"

	class Pc_Item: public Item {
		public:
			Item *clone() const override {
				return new Pc_Item { };
			}
	};

#line 1947 "start.x"

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

#line 1966 "start.x"

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

#line 1985 "start.x"

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

#line 828 "start.x"
;
	void clear_symbols() {
		_symbols.clear();
		
#line 849 "start.x"

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

#line 880 "start.x"

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

#line 831 "start.x"
;
	}

#line 1744 "start.x"

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

#line 801 "start.x"
;
	Expression_Ptr parse_factor(
		std::vector<Token>::iterator &cur, std::vector<Token>::iterator end, int addr
	) {
		
#line 946 "start.x"

	if (cur == end) { return Expression_Ptr { }; };

#line 1074 "start.x"

	if (cur->type() == Token_Type::number) {
		auto res =
			std::make_unique<Number>(
				cur->value()
			);
		++cur;
		return res;
	}

#line 1087 "start.x"

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

#line 1725 "start.x"

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

#line 1863 "start.x"

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

#line 1898 "start.x"

	if (cur->type() == Token_Type::t_times) {
		++cur;
		return std::make_unique<Number>(addr);
	}

#line 805 "start.x"
;
		return Expression_Ptr {};
	}

#line 953 "start.x"

	
#line 1172 "start.x"

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

#line 1190 "start.x"

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

#line 1454 "start.x"

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

#line 1511 "start.x"

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

#line 1607 "start.x"

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

#line 1624 "start.x"

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

#line 954 "start.x"
;
	Expression_Ptr parse(std::vector<Token>::iterator &cur, std::vector<Token>::iterator end, int addr) {
		
#line 1472 "start.x"

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

#line 1487 "start.x"

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

#line 956 "start.x"
;
		auto dst = parse_factor(cur, end, addr);
		do {
			
#line 969 "start.x"

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

#line 984 "start.x"

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

#line 1005 "start.x"

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

#line 1208 "start.x"

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

#line 1223 "start.x"

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

#line 1529 "start.x"

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

#line 1641 "start.x"

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

#line 1656 "start.x"

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

#line 959 "start.x"
;
		} while (false);
		return dst;
	}

#line 1025 "start.x"

	int build_r_cmd(
		int funct7, char src2, char src1,
		int funct3, char dst, int opcode
	) {
		return (funct7 << 25) |
			(src2 << 20) | (src1 << 15) |
			(funct3 << 12) | (dst << 7) |
			opcode;
	}

#line 1039 "start.x"

	int build_i_cmd(
		int imm, char src1, int funct3, char dst, int opcode
	) {
		return (imm << 20) | (src1 << 15) | (funct3 << 12) | (dst << 7) | opcode;
	}

#line 1049 "start.x"

	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}

#line 1062 "start.x"

	int build_add(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x0, dst, 0x13
		);
	}

#line 1238 "start.x"

	int build_and(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x7, dst, 0x33
		);
	}

#line 1251 "start.x"

	int build_and(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x7, dst, 0x13
		);
	}

#line 1263 "start.x"

	int build_or(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x6, dst, 0x33
		);
	}

#line 1276 "start.x"

	int build_or(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x6, dst, 0x13
		);
	}

#line 1315 "start.x"

	int build_u_cmd(
		int imm, char dst, int opcode
	) {
		return (imm & 0xfffff800) | (dst << 7) | opcode;
	}

#line 1325 "start.x"

	int build_lui(
		char dst, int imm
	) {
		return build_u_cmd(imm, dst, 0x37);
	}

#line 1362 "start.x"

	int build_auipc(
		char dst, int imm
	) {
		return build_u_cmd(imm, dst, 0x17);
	}

#line 1381 "start.x"

	int build_csrrw(
		char dst, int csr, char src
	) {
		return build_i_cmd(
			csr, src, 0x1, dst, 0x73
		);
	}

#line 1402 "start.x"

	int build_csrrs(
		char dst, int csr, char src
	) {
		return build_i_cmd(
			csr, src, 0x2, dst, 0x73
		);
	}

#line 1544 "start.x"

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

#line 1559 "start.x"

	int build_branch(
		int cond, char reg1, char reg2, int offset
	) {
		return build_b_cmd(
			offset, reg2, reg1, cond, 0x63
		);
	}

#line 1761 "start.x"

	int build_load(
		char dst, char src, int imm
	) {
		return build_i_cmd(
			imm, src, 0x2, dst, 0x03
		);
	}

#line 1791 "start.x"

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

#line 1806 "start.x"

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

	
#line 114 "start.x"

	private:
		std::vector<int> code;

#line 122 "start.x"

	public:
		int code_size() const {
			return static_cast<int>(
				code.size()
			);
		}

#line 134 "start.x"

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
		
#line 1103 "start.x"

	Tokenizer t { line };
	std::vector<Token> ts;
	while (t.token().type() != Token_Type::end) {
		ts.push_back(t.token());
		t.next();
	}
	auto cur { ts.begin() };
	
#line 2004 "start.x"

	std::vector<std::unique_ptr<Item>> items;
	for (; cur != ts.end(); ++cur) {
		items.emplace_back(new Token_Item { *cur });
	}
	unsigned i = 0;
	while (i < items.size()) {
		auto *ti { dynamic_cast<Token_Item *>(&*items[i]) };
		if (ti && ti->token().type() == Token_Type::ident) {
			auto s = _symbols.find(ti->token().name());
			if (s != _symbols.end()) {
				auto &ss { s->second };
				items.erase(items.begin() + i, items.begin() + i + 1);
				for (const auto &e : ss) {
					items.emplace(items.begin() + i, e->clone());
					++i;
				}
				i = 0; continue;
			}
		}
		if (ti && ti->token().type() == Token_Type::ident) {
			if (i < items.size() - 1) {
				auto *ta { dynamic_cast<Token_Item *>(&*items[i + 1]) };
				if (ta && ta->token().type() == Token_Type::t_equals) {
					auto &l { _symbols[ti->token().name()] };
					for (unsigned j = i + 2; j < items.size(); ++j) {
						l.push_back(std::move(items[j]));
					}
					items.erase(items.begin() + i, items.end());
					i = 0; continue;
				}
			}
		}
		if (ti && ti->token().type() == Token_Type::t_raw) {
			if (i < items.size() - 1) {
				auto *ta { dynamic_cast<Token_Item *>(&*items[i + 1]) };
				if (ta && ta->token().type() == Token_Type::number) {
					int value = ta->token().value();
					items.erase(items.begin() + i, items.begin() + i + 2);
					items.emplace(items.begin() + i, new Machine_Item { value });
					i = 0; continue;
				}
			}
		}
		if (ti && ti->token().type() == Token_Type::t_times) {
			items.erase(items.begin() + i, items.begin() + i + 1);
			int addr = code.size() * 4 + 0x20010000;
			items.emplace(items.begin() + i, new Token_Item({ Token_Type::number, addr }));
			i = 0; continue;
		}
		++i;
	}
	while (! items.empty() && dynamic_cast<Machine_Item *>(&**items.begin())) {
		auto &mi { dynamic_cast<Machine_Item &>(**items.begin()) };
		add_machine(mi.instruction());
		items.erase(items.begin(), items.begin() + 1);
	}
	
	if (! items.empty()) {
		std::cerr << "can expand fully [" << line << "]\n";
	}

#line 1111 "start.x"
;

#line 79 "start.x"
;
	}

#line 98 "start.x"

	void State::add_machine(int instr) {
		
#line 144 "start.x"

	code.push_back(instr);

#line 100 "start.x"

	}

#line 51 "start.x"
;

#line 151 "start.x"

	void assert_line(const char *line, int expected) {
		std::cerr << "ignoring " << line << "\n";
	}
	void assert_line_2(
		const char *line,
		int expected
	) {
		
#line 837 "start.x"

	clear_symbols();

#line 159 "start.x"
;
		
#line 183 "start.x"

	State s;
	s.add_line(line);
	assert(s.code_size() == 1);
	assert(s.get_code(0) == expected);

#line 160 "start.x"
;
	}

#line 443 "start.x"

	void assert_token(
		const char *line,
		Token_Type token
	) {
		
#line 464 "start.x"

	
#line 843 "start.x"

	clear_symbols();

#line 465 "start.x"
;
	Tokenizer t(line);
	assert(t.token().type() == token);
	t.next();
	assert(t.token().type() == Token_Type::end);

#line 448 "start.x"
;
	}

#line 527 "start.x"

	void assert_register(
		const char *line,
		const char *name
	) {
		
#line 539 "start.x"

	Tokenizer t(line);
	assert(t.token().type() == Token_Type::ident);
	assert(t.token().name() == name);
	t.next();
	assert(t.token().type() == Token_Type::end);

#line 532 "start.x"
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
		
#line 168 "start.x"

	
#line 175 "start.x"

	assert_line_2(
		"raw $87654321", 0x87654321
	);

#line 455 "start.x"

	assert_token(
		"<-", Token_Type::becomes
	);

#line 503 "start.x"

	assert_token("+", Token_Type::plus);
	assert_token("-", Token_Type::minus);

#line 550 "start.x"

	assert_register("%x10", "%x10");
	assert_register("%pc", "%pc");

#line 596 "start.x"

	assert_line(
		"%x4 <- %x2 + %x3",
		0x00310233
	);

#line 1116 "start.x"

	assert_line(
		"%pc <- %pc + 0",
		0x0000006f
	);

#line 1127 "start.x"

	assert_line(
		"%pc <- %pc + -28",
		0xfe5ff06f
	);

#line 1137 "start.x"

	assert_line(
		"%pc <- %pc + -32",
		0xfe1ff06f
	);

#line 1288 "start.x"

	assert_line(
		"%x5 <- %x5 and $ff",
		0x0ff2f293
	);

#line 1297 "start.x"

	assert_line(
		"%x5 <- %x5 or $1",
		0x0012e293
	);

#line 1306 "start.x"

	assert_line(
		"%x6 <- %x6 or $1",
		0x00136313
	);

#line 1335 "start.x"

	assert_line(
		"%x11 <- $0d",
		0x00d00593
	);

#line 1344 "start.x"

	assert_line(
		"%x12 <- $0a",
		0x00a00613
	);

#line 1353 "start.x"

	assert_line(
		"%x10 <- $1013000",
		0x1013537
	);

#line 1372 "start.x"

	assert_line(
		"%x5 <- %pc",
		0x00000297
	);

#line 1393 "start.x"

	assert_line(
		"%mtvec <- %x5",
		0x30529073
	);

#line 1414 "start.x"

	assert_line(
		"%x5 <- %mhartid",
		0xf14022f3
	);

#line 1571 "start.x"

	assert_line(
		"if %x5 < 0: %pc <- %pc + -4",
		0xfe02cee3
	);

#line 1671 "start.x"

	assert_line(
		"if %x5 = 0: %pc <- %pc + -12",
		0xfe028ae3
	);

#line 1680 "start.x"

	assert_line(
		"if %x5 != %x11: %pc <- %pc + -28",
		0xfeb292e3
	);

#line 1689 "start.x"

	assert_line(
		"if %x5 != 0: %pc <- %pc + 0",
		0x00029063
	);

#line 1773 "start.x"

	assert_line(
		"%x6 <- [%x10]",
		0x00052303
	);

#line 1782 "start.x"

	assert_line(
		"%x5 <- [%x10 + $04]",
		0x00452283
	);

#line 1818 "start.x"

	assert_line(
		"[%x10] <- %x12",
		0x00c52023
	);

#line 1827 "start.x"

	assert_line(
		"[%x10 + $08] <- %x5",
		0x00552423
	);

#line 169 "start.x"


#line 10 "start.x"

		
#line 1147 "start.x"

	
#line 1154 "start.x"

	State s;
	std::string l;
	while (std::getline(std::cin, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}

#line 1148 "start.x"


#line 16 "hex.x"

	std::cout << s;

#line 11 "start.x"

	}
