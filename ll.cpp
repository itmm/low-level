
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

#line 912 "start.x"

	number,

#line 1145 "start.x"

	t_and,
	t_or,

#line 1542 "start.x"

	t_if,
	t_colon,

#line 1772 "start.x"

	t_equals,
	t_not_equals,

#line 1912 "start.x"

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

#line 918 "start.x"

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

#line 942 "start.x"

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
		
#line 1549 "start.x"

	if (_name == "if") {
		_type = Token_Type::t_if;
	}
	if (_name == "and") {
		_type = Token_Type::t_and;
	}
	if (_name == "or") {
		_type = Token_Type::t_or;
	}

#line 488 "start.x"
;
		_cur = c;
		break;
	}

#line 924 "start.x"

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

#line 1113 "start.x"

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

#line 1136 "start.x"

	if (*_cur == '#') {
		_type = Token_Type::end;
		break;
	}

#line 1563 "start.x"

	if (*_cur == ':') {
		_type = Token_Type::t_colon;
		++_cur;
		break;
	}

#line 1779 "start.x"

	if (*_cur == '=') {
		_type = Token_Type::t_equals;
		++_cur;
		break;
	}

#line 1789 "start.x"

	if (*_cur == '!' && _cur + 1 < _end && _cur[1] == '=') {
		_type = Token_Type::t_not_equals;
		_cur += 2;
		break;
	}

#line 1919 "start.x"

	if (*_cur == '[') {
		_type = Token_Type::t_open_bracket;
		++_cur;
		break;
	}

#line 1929 "start.x"

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

#line 501 "start.x"

	#include <memory>

#line 508 "start.x"

	class Expression {
		public:
			virtual ~Expression() {}
			virtual std::unique_ptr<Expression> clone() {
				return std::unique_ptr<Expression> {};
			}
	};

#line 521 "start.x"

	using Expression_Ptr =
		std::unique_ptr<Expression>;

#line 529 "start.x"

	class BinaryExpression:
		public Expression
	{
			Expression_Ptr _first;
			Expression_Ptr _second;
		public:
			
#line 543 "start.x"

	BinaryExpression(
		Expression_Ptr first,
		Expression_Ptr second
	):
		_first { std::move(first) },
		_second { std::move(second) }
	{ }

#line 556 "start.x"

	const Expression_Ptr &first() const {
		return _first;
	}
	const Expression_Ptr &second() const {
		return _second;
	}

#line 536 "start.x"
;
	};

#line 568 "start.x"

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

#line 586 "start.x"

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

#line 604 "start.x"

	class Gen_Register: public Expression {
			
#line 616 "start.x"

	const int _nr;

#line 606 "start.x"
;
		public:
			
#line 626 "start.x"

	Gen_Register(int nr):
		_nr { nr }
	{ }

#line 635 "start.x"

	int nr() const { return _nr; }
	Expression_Ptr clone() override {
		return std::make_unique<Gen_Register>(_nr);
	}

#line 608 "start.x"
;
	};

#line 646 "start.x"

	class Pc_Register: public Expression {
		public:
			Pc_Register() {}
			Expression_Ptr clone() override {
				return std::make_unique<Pc_Register>();
			}
	};

#line 658 "start.x"

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

#line 676 "start.x"

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

#line 698 "start.x"

	Expression_Ptr parse(Tokenizer &t);
	
#line 713 "start.x"

	#include <map>

#line 719 "start.x"

	static std::map<std::string, Expression_Ptr> _symbols;

#line 725 "start.x"

	void clear_symbols() {
		_symbols.clear();
		
#line 746 "start.x"
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
#line 765 "start.x"

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

#line 728 "start.x"
;
	}

#line 1958 "start.x"

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

#line 700 "start.x"
;
	Expression_Ptr parse_factor(
		Tokenizer &t
	) {
		
#line 805 "start.x"

	if (t.type() == Token_Type::ident) {
		auto found { _symbols.find(t.name()) };
		if (found != _symbols.end()) {
			t.next();
			return found->second->clone();
		}
	}

#line 948 "start.x"

	if (t.type() == Token_Type::number) {
		auto res =
			std::make_unique<Number>(
				t.value()
			);
		t.next();
		return res;
	}

#line 1939 "start.x"

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

#line 704 "start.x"
;
		std::cerr << "no factor\n";
		return Expression_Ptr {};
	}

#line 818 "start.x"

	
#line 1152 "start.x"

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

#line 1170 "start.x"

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

#line 1573 "start.x"

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

#line 1615 "start.x"

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

#line 1799 "start.x"

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

#line 1816 "start.x"

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

#line 819 "start.x"
;
	Expression_Ptr parse(Tokenizer &t) {
		
#line 1591 "start.x"

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

#line 821 "start.x"
;
		auto dst = parse_factor(t);
		do {
			
#line 834 "start.x"

	if (t.type() == Token_Type::becomes) {
		t.next();
		auto src = parse(t);
		if (! src) {
			std::cerr << "no expression after <-\n";
			return Expression_Ptr { };
		}
		return std::make_unique<Assignment>(std::move(dst), std::move(src));
	}

#line 848 "start.x"

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

#line 1188 "start.x"

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

#line 1203 "start.x"

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

#line 1633 "start.x"

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

#line 1833 "start.x"

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

#line 1848 "start.x"

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

#line 824 "start.x"
;
		} while (false);
		return dst;
	}

#line 863 "start.x"

	int build_r_cmd(
		int funct7, char src2, char src1,
		int funct3, char dst, int opcode
	) {
		return (funct7 << 25) |
			(src2 << 20) | (src1 << 15) |
			(funct3 << 12) | (dst << 7) |
			opcode;
	}

#line 877 "start.x"

	int build_i_cmd(
		int imm, char src1, int funct3, char dst, int opcode
	) {
		return (imm << 20) | (src1 << 15) | (funct3 << 12) | (dst << 7) | opcode;
	}

#line 887 "start.x"

	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}

#line 900 "start.x"

	int build_add(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x0, dst, 0x13
		);
	}

#line 1272 "start.x"

	int build_and(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x7, dst, 0x33
		);
	}

#line 1285 "start.x"

	int build_and(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x7, dst, 0x13
		);
	}

#line 1297 "start.x"

	int build_or(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x6, dst, 0x33
		);
	}

#line 1310 "start.x"

	int build_or(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x6, dst, 0x13
		);
	}

#line 1358 "start.x"

	int build_u_cmd(
		int imm, char dst, int opcode
	) {
		return (imm & 0xfffff800) | (dst << 7) | opcode;
	}

#line 1368 "start.x"

	int build_lui(
		char dst, int imm
	) {
		return build_u_cmd(imm, dst, 0x37);
	}

#line 1423 "start.x"

	int build_auipc(
		char dst, int imm
	) {
		return build_u_cmd(imm, dst, 0x17);
	}

#line 1475 "start.x"

	int build_csrrw(
		char dst, int csr, char src
	) {
		return build_i_cmd(
			csr, src, 0x1, dst, 0x73
		);
	}

#line 1511 "start.x"

	int build_csrrs(
		char dst, int csr, char src
	) {
		return build_i_cmd(
			csr, src, 0x2, dst, 0x73
		);
	}

#line 1662 "start.x"

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

#line 1677 "start.x"

	int build_branch(
		int cond, char reg1, char reg2, int offset
	) {
		return build_b_cmd(
			offset, reg2, reg1, cond, 0x63
		);
	}

#line 1984 "start.x"

	int build_load(
		char dst, char src, int imm
	) {
		return build_i_cmd(
			imm, src, 0x2, dst, 0x03
		);
	}

#line 2048 "start.x"

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

#line 2063 "start.x"

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
		
#line 961 "start.x"

	Tokenizer t { line };
	auto e = parse(t);
	if (! e) {
		std::cerr << "no statement on line " << line << '\n';
		return;
	}
	
#line 974 "start.x"

	Number *n = dynamic_cast<Number *>(&*e);
	if (n) {
		add_machine(n->value());
		return;
	}

#line 984 "start.x"

	Assignment *a = dynamic_cast<Assignment *>(&*e);
	if (a) {
		const Gen_Register *dst = dynamic_cast<const Gen_Register *>(&*a->first());
		if (dst) {
			
#line 997 "start.x"
 {
	const Addition *o = dynamic_cast<const Addition *>(&*a->second());
	if (o) {
		const Gen_Register *src1 = dynamic_cast<const Gen_Register *>(&*o->first());
		if (src1) {
			
#line 1010 "start.x"

	const Gen_Register *src2 = dynamic_cast<const Gen_Register *>(&*o->second());
	if (src2) {
		add_machine(build_add(
			(char) dst->nr(), (char) src1->nr(), (char) src2->nr()
		));
		return;
	}

#line 1022 "start.x"

	const Number *n2 = dynamic_cast<const Number *>(&*o->second());
	if (n2) {
		add_machine(build_add(
			(char) dst->nr(), (char) src1->nr(), n2->value()
		));
		return;
	}

#line 1002 "start.x"
;
		}
		
#line 1452 "start.x"

	if (dynamic_cast<const Pc_Register *>(&*o->first())) {
		
#line 1460 "start.x"

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

#line 1454 "start.x"
;
	}

#line 1004 "start.x"
;
	}
} 
#line 1218 "start.x"
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
#line 1245 "start.x"
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
#line 1349 "start.x"
 {
	const Number *o = dynamic_cast<const Number *>(&*a->second());
	if (o) {
		
#line 1378 "start.x"

	int upper { o->value() & ~ 0xfff };
	if (upper && upper != ~ 0xfff) {
		add_machine(build_lui(dst->nr(), o->value()));
	}

#line 1387 "start.x"

	if (o->value() == 0 || (o->value() & 0xfff)) {
		add_machine(build_add((char) dst->nr(), (char) 0, o->value()));
	}
	return;

#line 1352 "start.x"
;
	}
} 
#line 1433 "start.x"
 {
	const Pc_Register *o = dynamic_cast<const Pc_Register *>(&*a->second());
	if (o) {
		add_machine(build_auipc(dst->nr(), 0));
		return;
	}
} 
#line 1523 "start.x"
 {
	const Csr_Register *o = dynamic_cast<const Csr_Register *>(&*a->second());
	if (o) {
		add_machine(build_csrrs(dst->nr(), o->addr(), '\0'));
		return;
	}
} 
#line 1975 "start.x"
 {
	const Access *x = dynamic_cast<const Access *>(&*a->second());
	if (x) {
		
#line 1996 "start.x"
 {
	const Gen_Register *r = dynamic_cast<const Gen_Register *>(x->inner());
	if (r) {
		add_machine(build_load(dst->nr(), r->nr(), 0));
		return;
	}
} 
#line 2015 "start.x"
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
#line 1978 "start.x"
;
	}
} 
#line 989 "start.x"
;
		}
		
#line 1034 "start.x"
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
#line 1487 "start.x"
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
#line 2038 "start.x"
 {
	const Access *x = dynamic_cast<const Access *>(&*a->first());
	if (x) {
		const Gen_Register *s = dynamic_cast<const Gen_Register *>(&*a->second());
		
#line 2075 "start.x"
 {
	const Gen_Register *d = dynamic_cast<const Gen_Register *>(x->inner());
	if (d && s) {
		add_machine(build_store(s->nr(), d->nr(), 0));
		return;
	}
} 
#line 2094 "start.x"
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
#line 2042 "start.x"
;
	}
} 
#line 991 "start.x"
;
	}

#line 1648 "start.x"

	If *i = dynamic_cast<If *>(&*e);
	if (i) {
		int reg1 { -1 };
		int reg2 { -1 };
		int cond { -1 };
		int offset { 0x7fffffff };
		t.next();
		
#line 1689 "start.x"

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
	
#line 1863 "start.x"

	{
		const Equals *e = dynamic_cast<const Equals *>(&*i->first());
		if (e) {
			cond = 0x0;
		}
	}

#line 1874 "start.x"

	{
		const NotEquals *ne = dynamic_cast<const NotEquals *>(&*i->first());
		if (ne) {
			cond = 0x1;
		}
	}

#line 1724 "start.x"
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

#line 1656 "start.x"
;
	}

#line 968 "start.x"
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
		
#line 734 "start.x"

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

	
#line 740 "start.x"

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

#line 1064 "start.x"

	assert_line(
		"%pc <- %pc + 0",
		0x0000006f
	);

#line 1075 "start.x"

	assert_line(
		"%pc <- %pc + -28",
		0xfe5ff06f
	);

#line 1085 "start.x"

	assert_line(
		"%pc <- %pc + -32",
		0xfe1ff06f
	);

#line 1322 "start.x"

	assert_line(
		"%x5 <- %x5 and $ff",
		0x0ff2f293
	);

#line 1331 "start.x"

	assert_line(
		"%x5 <- %x5 or $1",
		0x0012e293
	);

#line 1340 "start.x"

	assert_line(
		"%x6 <- %x6 or $1",
		0x00136313
	);

#line 1396 "start.x"

	assert_line(
		"%x11 <- $0d",
		0x00d00593
	);

#line 1405 "start.x"

	assert_line(
		"%x12 <- $0a",
		0x00a00613
	);

#line 1414 "start.x"

	assert_line(
		"%x10 <- $1013000",
		0x1013537
	);

#line 1443 "start.x"

	assert_line(
		"%x5 <- %pc",
		0x00000297
	);

#line 1502 "start.x"

	assert_line(
		"%mtvec <- %x5",
		0x30529073
	);

#line 1533 "start.x"

	assert_line(
		"%x5 <- %mhartid",
		0xf14022f3
	);

#line 1763 "start.x"

	assert_line(
		"if %x5 < 0: %pc <- %pc + -4",
		0xfe02cee3
	);

#line 1885 "start.x"

	assert_line(
		"if %x5 = 0: %pc <- %pc + -12",
		0xfe028ae3
	);

#line 1894 "start.x"

	assert_line(
		"if %x5 != %x11: %pc <- %pc + -28",
		0xfeb292e3
	);

#line 1903 "start.x"

	assert_line(
		"if %x5 != 0: %pc <- %pc + 0",
		0x00029063
	);

#line 2006 "start.x"

	assert_line(
		"%x6 <- [%x10]",
		0x00052303
	);

#line 2029 "start.x"

	assert_line(
		"%x5 <- [%x10 + $04]",
		0x00452283
	);

#line 2085 "start.x"

	assert_line(
		"[%x10] <- %x12",
		0x00c52023
	);

#line 2108 "start.x"

	assert_line(
		"[%x10 + $08] <- %x5",
		0x00552423
	);

#line 173 "start.x"


#line 10 "start.x"

		
#line 1095 "start.x"

	
#line 1102 "start.x"

	State s;
	std::string l;
	while (std::getline(std::cin, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}

#line 1096 "start.x"


#line 16 "hex.x"

	std::cout << s;

#line 11 "start.x"

	}
