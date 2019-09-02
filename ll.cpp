
#line 5 "start.x"

	
#line 22 "start.x"

	
#line 29 "start.x"

	#include <cassert>

#line 44 "start.x"

	
#line 59 "start.x"

	#include <string>
	#include <vector>

#line 208 "start.x"

	
#line 221 "start.x"

	enum class Token_Type {
		unknown,
		
#line 345 "start.x"

	ident,

#line 372 "start.x"

	t_raw,

#line 395 "start.x"

	number,

#line 439 "start.x"

	becomes,
	t_less,

#line 500 "start.x"

	plus, minus,

#line 832 "start.x"

	t_and,
	t_or,

#line 1024 "start.x"

	t_if,
	t_colon,

#line 1091 "start.x"

	t_equals,
	t_not_equals,

#line 1145 "start.x"

	t_open_bracket,
	t_close_bracket,

#line 1247 "start.x"

	t_open_parenthesis,
	t_close_parenthesis,

#line 1274 "start.x"

	t_times,

#line 224 "start.x"

		end
	};

#line 232 "start.x"

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

#line 323 "start.x"

	#include <iostream>

#line 562 "start.x"

	#include <cctype>

#line 209 "start.x"

	class Tokenizer {
			
#line 250 "start.x"

	Token _token { Token_Type::unknown };

#line 266 "start.x"

	std::string::const_iterator _cur;
	std::string::const_iterator _end;

#line 211 "start.x"
;
		public:
			
#line 257 "start.x"

	const Token &token() const {
		return _token;
	}

#line 274 "start.x"

	void next();

#line 281 "start.x"

	Tokenizer(const std::string &s):
		_cur { s.begin() },
		_end { s.end() } 
	{
		next();
	}

#line 213 "start.x"
;
	};
	
#line 293 "start.x"

	void Tokenizer::next() {
		
#line 302 "start.x"

	while (
		_cur != _end && *_cur <= ' '
	) {
		++_cur;
	}

#line 313 "start.x"

	if (_cur == _end) {
		_token = Token { Token_Type::end };
		return;
	}

#line 330 "start.x"

	do {
		
#line 351 "start.x"

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
		
#line 378 "start.x"

	if (name == "raw") {
		type = Token_Type::t_raw;
	}

#line 1031 "start.x"

	if (name == "if") {
		type = Token_Type::t_if;
	}
	if (name == "and") {
		type = Token_Type::t_and;
	}
	if (name == "or") {
		type = Token_Type::t_or;
	}

#line 362 "start.x"
;
		_token = Token { type, name };
		_cur = c;
		break;
	}

#line 386 "start.x"

	if (*_cur == '#') {
		_token = Token { Token_Type::end };
		break;
	}

#line 401 "start.x"

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

#line 416 "start.x"

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

#line 480 "start.x"

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

#line 515 "start.x"

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

#line 1045 "start.x"

	if (*_cur == ':') {
		_token = Token { Token_Type::t_colon };
		++_cur;
		break;
	}

#line 1098 "start.x"

	if (*_cur == '=') {
		_token = Token { Token_Type::t_equals };
		++_cur;
		break;
	}

#line 1108 "start.x"

	if (*_cur == '!' && _cur + 1 < _end && _cur[1] == '=') {
		_token = Token { Token_Type::t_not_equals };
		_cur += 2;
		break;
	}

#line 1152 "start.x"

	if (*_cur == '[') {
		_token = Token_Type::t_open_bracket;
		++_cur;
		break;
	}

#line 1162 "start.x"

	if (*_cur == ']') {
		_token = Token_Type::t_close_bracket;
		++_cur;
		break;
	}

#line 1254 "start.x"

	if (*_cur == '(') {
		_token = Token { Token_Type::t_open_parenthesis };
		++_cur;
		break;
	}

#line 1264 "start.x"

	if (*_cur == ')') {
		_token = Token { Token_Type::t_close_parenthesis };
		++_cur;
		break;
	}

#line 1280 "start.x"

	if (*_cur == '*') {
		_token = Token { Token_Type::t_times };
		++_cur;
		break;
	}

#line 332 "start.x"
;
		_token = Token { Token_Type::unknown };
		std::cerr <<
			"unrecognized char [" <<
			*_cur << "] == " << (int) *_cur << '\n';
		++_cur;
	} while (false);

#line 295 "start.x"
;
	}

#line 215 "start.x"
;

#line 572 "start.x"

	#include <memory>

#line 721 "start.x"

	int build_r_cmd(
		int funct7, char src2, char src1,
		int funct3, char dst, int opcode
	) {
		return (funct7 << 25) |
			(src2 << 20) | (src1 << 15) |
			(funct3 << 12) | (dst << 7) |
			opcode;
	}

#line 735 "start.x"

	int build_i_cmd(
		int imm, char src1, int funct3, char dst, int opcode
	) {
		return (imm << 20) | (src1 << 15) | (funct3 << 12) | (dst << 7) | opcode;
	}

#line 745 "start.x"

	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}

#line 758 "start.x"

	int build_add(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x0, dst, 0x13
		);
	}

#line 839 "start.x"

	int build_and(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x7, dst, 0x33
		);
	}

#line 852 "start.x"

	int build_and(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x7, dst, 0x13
		);
	}

#line 864 "start.x"

	int build_or(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x6, dst, 0x33
		);
	}

#line 877 "start.x"

	int build_or(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x6, dst, 0x13
		);
	}

#line 916 "start.x"

	int build_u_cmd(
		int imm, char dst, int opcode
	) {
		return (imm & 0xfffff800) | (dst << 7) | opcode;
	}

#line 926 "start.x"

	int build_lui(
		char dst, int imm
	) {
		return build_u_cmd(imm, dst, 0x37);
	}

#line 963 "start.x"

	int build_auipc(
		char dst, int imm
	) {
		return build_u_cmd(imm, dst, 0x17);
	}

#line 982 "start.x"

	int build_csrrw(
		char dst, int csr, char src
	) {
		return build_i_cmd(
			csr, src, 0x1, dst, 0x73
		);
	}

#line 1003 "start.x"

	int build_csrrs(
		char dst, int csr, char src
	) {
		return build_i_cmd(
			csr, src, 0x2, dst, 0x73
		);
	}

#line 1055 "start.x"

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

#line 1070 "start.x"

	int build_branch(
		int cond, char reg1, char reg2, int offset
	) {
		return build_b_cmd(
			offset, reg2, reg1, cond, 0x63
		);
	}

#line 1172 "start.x"

	int build_load(
		char dst, char src, int imm
	) {
		return build_i_cmd(
			imm, src, 0x2, dst, 0x03
		);
	}

#line 1202 "start.x"

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

#line 1217 "start.x"

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
			
#line 93 "start.x"

	void add_machine(int instr);

#line 47 "start.x"

		public:
			
#line 67 "start.x"

	void add_line(
		const std::string &line
	);

#line 109 "start.x"

	
#line 116 "start.x"

	private:
		std::vector<int> code;

#line 124 "start.x"

	public:
		int code_size() const {
			return static_cast<int>(
				code.size()
			);
		}

#line 136 "start.x"

	public:
		int get_code(int pos) const {
			return code[pos];
		}

#line 110 "start.x"


#line 49 "start.x"

	};
	
#line 76 "start.x"

	
#line 589 "start.x"

	#include <map>

#line 595 "start.x"

	class Item;
	static std::map<std::string, std::vector<std::unique_ptr<Item>>> _symbols;

#line 602 "start.x"

	
#line 1290 "start.x"

	class Item {
		public:
			virtual ~Item() {};
			virtual Item *clone() const = 0;
	};

#line 1300 "start.x"

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

#line 1319 "start.x"

	class Pc_Item: public Item {
		public:
			Item *clone() const override {
				return new Pc_Item { };
			}
	};

#line 1330 "start.x"

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

#line 1349 "start.x"

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

#line 1368 "start.x"

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

#line 1387 "start.x"

	class I_Type_Item: public Item {
		private:
			int _immediate;
			int _rs1;
			int _func3;
			int _rd;
			int _opcode;
		public:
			I_Type_Item(
				int immediate, int rs1, int func3,
				int rd, int opcode
			):
				_immediate { immediate },
				_rs1 { rs1 },
				_func3 { func3 },
				_rd { rd },
				_opcode { opcode }
			{ }
			int immediate() const { return _immediate; }
			int rs1() const { return _rs1; }
			int func3() const { return _func3; }
			int rd() const { return _rd; }
			int opcode() const { return _opcode; }
			Item *clone() const override {
				return new I_Type_Item {
					_immediate, _rs1, _func3,
					_rd, _opcode
				};
			}
	};

#line 1422 "start.x"

	class U_Type_Item: public Item {
		private:
			int _immediate;
			int _rd;
			int _opcode;
		public:
			U_Type_Item(
				int immediate,
				int rd, int opcode
			):
				_immediate { immediate },
				_rd { rd },
				_opcode { opcode }
			{ }
			int immediate() const { return _immediate; }
			int rd() const { return _rd; }
			int opcode() const { return _opcode; }
			Item *clone() const override {
				return new U_Type_Item {
					_immediate,
					_rd, _opcode
				};
			}
	};

#line 603 "start.x"
;
	void clear_symbols() {
		_symbols.clear();
		
#line 624 "start.x"

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

#line 655 "start.x"

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

#line 606 "start.x"
;
	}

#line 77 "start.x"

	void State::add_line(
		const std::string &line
	) {
		
#line 770 "start.x"

	Tokenizer t { line };
	std::vector<Token> ts;
	while (t.token().type() != Token_Type::end) {
		ts.push_back(t.token());
		t.next();
	}
	auto cur { ts.begin() };
	
#line 1451 "start.x"

	std::vector<std::unique_ptr<Item>>
		items;
	
#line 1470 "start.x"

	for (; cur != ts.end(); ++cur) {
		items.emplace_back(
			new Token_Item { *cur }
		);
	}

#line 1454 "start.x"
;
	unsigned i = 0;
	while (i < items.size()) {
		
#line 1480 "start.x"
 {
	auto *ti {
		dynamic_cast<Token_Item *>(
			&*items[i]
	) };
	if (ti) {
		
#line 1492 "start.x"

	if (ti->token().type() ==
		Token_Type::ident
	) {
		
#line 1502 "start.x"

	auto s {
		_symbols.find(ti->token().name())
	};
	if (s != _symbols.end()) {
		
#line 1514 "start.x"

	auto &ss { s->second };
	items.erase(items.begin() + i,
		items.begin() + i + 1
	);
	for (const auto &e : ss) {
		items.emplace(
			items.begin() + i, e->clone()
		);
		++i;
	}

#line 1507 "start.x"
;
		i = 0; continue;
	}

#line 1529 "start.x"

	if (i < items.size() - 1) {
		auto *ta {
			dynamic_cast<Token_Item *>(
				&*items[i + 1]
		) };
		if (ta && ta->token().type() ==
			Token_Type::t_equals
		) {
			
#line 1546 "start.x"

	auto &l { _symbols[
		ti->token().name()
	] };
	for (unsigned j = i + 2;
		j < items.size(); ++j
	) {
		l.push_back(std::move(items[j]));
	}
	items.erase(
		items.begin() + i, items.end()
	);

#line 1538 "start.x"
;
			i = 0; continue;
		}
	}

#line 1496 "start.x"
;
	}

#line 1562 "start.x"

	if (ti->token().type() ==
		Token_Type::t_raw
	) {
		
#line 1572 "start.x"

	if (i < items.size() - 1) {
		auto *ta {
			dynamic_cast<Token_Item *>(
				&*items[i + 1]
		) };
		if (ta && ta->token().type() ==
			Token_Type::number
		) {
			
#line 1589 "start.x"

	int value { ta->token().value() };
	items.erase( items.begin() + i,
		items.begin() + i + 2
	);
	items.emplace(items.begin() + i,
		new Machine_Item { value }
	);

#line 1581 "start.x"
;
			i = 0; continue;
		}
	}

#line 1566 "start.x"
;
	}

#line 1601 "start.x"

	if (ti->token().type() ==
		Token_Type::t_times
	) {
		
#line 1612 "start.x"

	items.erase(items.begin() + i,
		items.begin() + i + 1);
	int addr = code.size() * 4 +
		0x20010000;
	items.emplace(items.begin() + i,
		new Token_Item({
			Token_Type::number, addr
		})
	);

#line 1605 "start.x"
;
		i = 0; continue;
	}

#line 1486 "start.x"
;
	}
} 
#line 1626 "start.x"
 {
	auto *ri {
		dynamic_cast<Register_Item *>(
			&*items[i]
	) };
	if (ri) {
		int rd { ri->nr() };
		
#line 1639 "start.x"

	if (i < items.size() - 2) {
		auto *t2 {
			dynamic_cast<Token_Item *>(
				&*items[i + 1]
		) };
		if (t2 && t2->token().type() ==
			Token_Type::becomes
		) {
			
#line 1655 "start.x"
 {
	auto *t3 {
		dynamic_cast<Token_Item *>(
			&*items[i + 2]
	) };
	if (t3 && t3->token().type() ==
		Token_Type::number
	) {
		
#line 1670 "start.x"

	int v { t3->token().value() };
	items.erase(items.begin() + i,
		items.begin() + i + 3
	);

#line 1679 "start.x"
 {
	int up { v & ~ 0xfff };
	if (up != 0 && up != ~ 0xfff) {
		items.emplace(items.begin() + i,
			new U_Type_Item {
				up, rd, 0x37
		});
		++i;
	}
} 
#line 1692 "start.x"
 {
	int low { v & 0xfff };
	if (
		(low && (low != 0xfff)) || v == 0
	) {
		items.emplace(items.begin() + i,
			new I_Type_Item {
				low, 0, 0x0, rd, 0x13
			}
		);
	}
} 
#line 1663 "start.x"
;
		i = 0; continue;
	}
} 
#line 1707 "start.x"
 {
	auto c3 {
		dynamic_cast<Csr_Item *>(
			&*items[i + 2]
	) };
	if (c3) {
		
#line 1720 "start.x"

	int cv { c3->nr() };
	items.erase(items.begin() + i,
		items.begin() + i + 3
	);
	items.emplace(items.begin() + i,
		new I_Type_Item {
			cv, 0, 0x2, rd, 0x73
		}
	);

#line 1713 "start.x"
;
		i = 0; continue;
	}
} 
#line 1734 "start.x"
 {
	auto rs1 {
		dynamic_cast<Register_Item *>(
			&*items[i + 2]
		)
	};
	if (rs1) {
		int rs1_nr { rs1->nr() };
		
#line 1748 "start.x"

	if (i < items.size() - 4) {
		auto t3 {
			dynamic_cast<Token_Item *>(
				&*items[i + 3]
			)
		};
		if (t3) {
			
#line 1763 "start.x"

	if (t3->token().type() ==
		Token_Type::t_or
	) {
		
#line 1773 "start.x"
 {
	auto n4 {
		dynamic_cast<Token_Item *>(
			&*items[i + 4]
		)
	};
	if (n4 && n4->token().type() ==
		Token_Type::number
	) {
		
#line 1788 "start.x"

	int imm { n4->token().value() };
	items.erase(items.begin() + i,
		items.begin() + i + 5
	);
	items.emplace(items.begin() + i,
		new I_Type_Item {
			imm, rs1_nr, 0x6, rd, 0x13
		}
	);
	i = 0; continue;

#line 1782 "start.x"
;
	}
} 
#line 1767 "start.x"
;
	}

#line 1756 "start.x"
;
		}
	}

#line 1742 "start.x"
;
	}
} 
#line 1648 "start.x"
;
		}
	}

#line 1633 "start.x"
;
	}
} 
#line 1803 "start.x"
 {
	auto *ii {
		dynamic_cast<I_Type_Item *>(
			&*items[i]
		)
	};
	if (ii) {
		
#line 1816 "start.x"

	int result {
		(ii->immediate() << 20) |
		(ii->rs1() << 15) |
		(ii->func3() << 12) |
		(ii->rd() << 7) | ii->opcode()
	};
	items.erase(items.begin() + i,
		items.begin() + i + 1
	);
	items.emplace(items.begin() + i,
		new Machine_Item { result }
	);
	i = 0; continue;

#line 1810 "start.x"
;
	}
} 
#line 1834 "start.x"
 {
	auto ui {
		dynamic_cast<U_Type_Item *>(
			&*items[i]
		)
	};
	if (ui) {
		
#line 1848 "start.x"

	int result {
		ui->immediate() |
		(ui->rd() << 7) | ui->opcode()
	};
	items.erase(items.begin() + i,
		items.begin() + i + 1
	);
	items.emplace(items.begin() + i,
		new Machine_Item { result }
	);

#line 1841 "start.x"
;
		i = 0; continue;
	}
} 
#line 1457 "start.x"
;
		++i;
	}
	
#line 1863 "start.x"

	while (! items.empty() &&
		dynamic_cast<Machine_Item *>(
			&**items.begin()
	)) {
		auto &mi {
			dynamic_cast<Machine_Item &>(
				**items.begin()
		)};
		add_machine(mi.instruction());
		items.erase(
			items.begin(),
			items.begin() + 1
		);
	}

#line 1460 "start.x"
;
	if (! items.empty()) {
		std::cerr <<
			"can expand fully [" <<
			line << "]\n";
	}

#line 778 "start.x"
;

#line 81 "start.x"
;
	}

#line 100 "start.x"

	void State::add_machine(int instr) {
		
#line 146 "start.x"

	code.push_back(instr);

#line 102 "start.x"

	}

#line 51 "start.x"
;

#line 153 "start.x"

	void assert_line(const char *line, int expected) {
		std::cerr << "ignoring " << line << "\n";
	}
	void assert_line_2(
		const char *line,
		int expected
	) {
		
#line 612 "start.x"

	clear_symbols();

#line 161 "start.x"
;
		
#line 185 "start.x"

	State s;
	s.add_line(line);
	assert(s.code_size() == 1);
	// std::cerr << "EXP " << std::hex << expected << "\n";
	// std::cerr << "GOT " << s.get_code(0) << std::dec << "\n";
	assert(s.get_code(0) == expected);

#line 162 "start.x"
;
	}

#line 447 "start.x"

	void assert_token(
		const char *line,
		Token_Type token
	) {
		
#line 468 "start.x"

	
#line 618 "start.x"

	clear_symbols();

#line 469 "start.x"
;
	Tokenizer t(line);
	assert(t.token().type() == token);
	t.next();
	assert(t.token().type() == Token_Type::end);

#line 452 "start.x"
;
	}

#line 531 "start.x"

	void assert_register(
		const char *line,
		const char *name
	) {
		
#line 543 "start.x"

	Tokenizer t(line);
	assert(t.token().type() == Token_Type::ident);
	assert(t.token().name() == name);
	t.next();
	assert(t.token().type() == Token_Type::end);

#line 536 "start.x"
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
		
#line 170 "start.x"

	
#line 177 "start.x"

	assert_line_2(
		"raw $87654321", 0x87654321
	);

#line 459 "start.x"

	assert_token(
		"<-", Token_Type::becomes
	);

#line 507 "start.x"

	assert_token("+", Token_Type::plus);
	assert_token("-", Token_Type::minus);

#line 554 "start.x"

	assert_register("%x10", "%x10");
	assert_register("%pc", "%pc");

#line 579 "start.x"

	assert_line(
		"%x4 <- %x2 + %x3",
		0x00310233
	);

#line 783 "start.x"

	assert_line(
		"%pc <- %pc + 0",
		0x0000006f
	);

#line 794 "start.x"

	assert_line(
		"%pc <- %pc + -28",
		0xfe5ff06f
	);

#line 804 "start.x"

	assert_line(
		"%pc <- %pc + -32",
		0xfe1ff06f
	);

#line 889 "start.x"

	assert_line(
		"%x5 <- %x5 and $ff",
		0x0ff2f293
	);

#line 898 "start.x"

	assert_line_2(
		"%x5 <- %x5 or $1",
		0x0012e293
	);

#line 907 "start.x"

	assert_line_2(
		"%x6 <- %x6 or $1",
		0x00136313
	);

#line 936 "start.x"

	assert_line_2(
		"%x11 <- $0d",
		0x00d00593
	);

#line 945 "start.x"

	assert_line_2(
		"%x12 <- $0a",
		0x00a00613
	);

#line 954 "start.x"

	assert_line_2(
		"%x10 <- $1013000",
		0x1013537
	);

#line 973 "start.x"

	assert_line(
		"%x5 <- %pc",
		0x00000297
	);

#line 994 "start.x"

	assert_line(
		"%mtvec <- %x5",
		0x30529073
	);

#line 1015 "start.x"

	assert_line_2(
		"%x5 <- %mhartid",
		0xf14022f3
	);

#line 1082 "start.x"

	assert_line(
		"if %x5 < 0: %pc <- %pc + -4",
		0xfe02cee3
	);

#line 1118 "start.x"

	assert_line(
		"if %x5 = 0: %pc <- %pc + -12",
		0xfe028ae3
	);

#line 1127 "start.x"

	assert_line(
		"if %x5 != %x11: %pc <- %pc + -28",
		0xfeb292e3
	);

#line 1136 "start.x"

	assert_line(
		"if %x5 != 0: %pc <- %pc + 0",
		0x00029063
	);

#line 1184 "start.x"

	assert_line(
		"%x6 <- [%x10]",
		0x00052303
	);

#line 1193 "start.x"

	assert_line(
		"%x5 <- [%x10 + $04]",
		0x00452283
	);

#line 1229 "start.x"

	assert_line(
		"[%x10] <- %x12",
		0x00c52023
	);

#line 1238 "start.x"

	assert_line(
		"[%x10 + $08] <- %x5",
		0x00552423
	);

#line 171 "start.x"


#line 10 "start.x"

		
#line 814 "start.x"

	
#line 821 "start.x"

	State s;
	std::string l;
	while (std::getline(std::cin, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}

#line 815 "start.x"


#line 16 "hex.x"

	std::cout << s;

#line 11 "start.x"

	}
