
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

#line 371 "start.x"

	t_raw,

#line 386 "start.x"

	number,

#line 430 "start.x"

	becomes,
	t_less,

#line 491 "start.x"

	plus, minus,

#line 843 "start.x"

	t_and,
	t_or,

#line 931 "start.x"

	t_if,
	t_colon,

#line 957 "start.x"

	t_equals,
	t_not_equals,

#line 1011 "start.x"

	t_open_bracket,
	t_close_bracket,

#line 1086 "start.x"

	t_open_parenthesis,
	t_close_parenthesis,

#line 1113 "start.x"

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

#line 553 "start.x"

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
		_token = Token { type, name };
		_cur = c;
		break;
	}

#line 377 "start.x"

	if (*_cur == '#') {
		_token = Token { Token_Type::end };
		break;
	}

#line 392 "start.x"

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

#line 407 "start.x"

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

#line 471 "start.x"

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

#line 506 "start.x"

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

#line 938 "start.x"

	if (*_cur == ':') {
		_token = Token { Token_Type::t_colon };
		++_cur;
		break;
	}

#line 964 "start.x"

	if (*_cur == '=') {
		_token = Token { Token_Type::t_equals };
		++_cur;
		break;
	}

#line 974 "start.x"

	if (*_cur == '!' && _cur + 1 < _end && _cur[1] == '=') {
		_token = Token { Token_Type::t_not_equals };
		_cur += 2;
		break;
	}

#line 1018 "start.x"

	if (*_cur == '[') {
		_token = Token_Type::t_open_bracket;
		++_cur;
		break;
	}

#line 1028 "start.x"

	if (*_cur == ']') {
		_token = Token_Type::t_close_bracket;
		++_cur;
		break;
	}

#line 1093 "start.x"

	if (*_cur == '(') {
		_token = Token { Token_Type::t_open_parenthesis };
		++_cur;
		break;
	}

#line 1103 "start.x"

	if (*_cur == ')') {
		_token = Token { Token_Type::t_close_parenthesis };
		++_cur;
		break;
	}

#line 1119 "start.x"

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

#line 563 "start.x"

	#include <memory>

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

	int build_i_cmd(
		int imm, char src1, int funct3, char dst, int opcode
	) {
		return (imm << 20) | (src1 << 15) | (funct3 << 12) | (dst << 7) | opcode;
	}

#line 756 "start.x"

	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}

#line 769 "start.x"

	int build_add(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x0, dst, 0x13
		);
	}

#line 1038 "start.x"

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

	
#line 580 "start.x"

	#include <map>

#line 586 "start.x"

	class Item;
	static std::map<std::string, std::vector<std::unique_ptr<Item>>> _symbols;

#line 593 "start.x"

	
#line 1129 "start.x"

	class Item {
		public:
			virtual ~Item() {};
			virtual Item *clone() const = 0;
	};

#line 1139 "start.x"

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

#line 1158 "start.x"

	class Pc_Item: public Item {
		public:
			Item *clone() const override {
				return new Pc_Item { };
			}
	};

#line 1169 "start.x"

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

#line 1188 "start.x"

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

#line 1207 "start.x"

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

#line 1226 "start.x"

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

#line 1261 "start.x"

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

#line 1717 "start.x"

	class J_Type_Item: public Item {
		private:
			int _immediate;
			int _rd;
			int _opcode;
		public:
			J_Type_Item(
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
				return new J_Type_Item {
					_immediate,
					_rd, _opcode
				};
			}
	};

#line 594 "start.x"
;
	void clear_symbols() {
		_symbols.clear();
		
#line 615 "start.x"

	{
		auto &l { _symbols["raw"] };
		l.emplace_back(new Token_Item { {
			Token_Type::t_raw
		} });
	} {
		auto &l { _symbols["if"] };
		l.emplace_back(new Token_Item { {
			Token_Type::t_if
		} });
	} {
		auto &l { _symbols["and"] };
		l.emplace_back(new Token_Item { {
			Token_Type::t_and
		} });
	} {
		auto &l { _symbols["or"] };
		l.emplace_back(new Token_Item { {
			Token_Type::t_or
		} });
	} {
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

#line 666 "start.x"

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

#line 597 "start.x"
;
	}

#line 77 "start.x"

	void State::add_line(
		const std::string &line
	) {
		
#line 781 "start.x"

	Tokenizer t { line };
	std::vector<Token> ts;
	while (t.token().type() != Token_Type::end) {
		ts.push_back(t.token());
		t.next();
	}
	auto cur { ts.begin() };
	
#line 1290 "start.x"

	std::vector<std::unique_ptr<Item>>
		items;
	
#line 1310 "start.x"

	for (; cur != ts.end(); ++cur) {
		items.emplace_back(
			new Token_Item { *cur }
		);
	}

#line 1293 "start.x"
;
	unsigned i = 0;
	while (i < items.size()) {
		
#line 1320 "start.x"
 {
	auto *ti {
		dynamic_cast<Token_Item *>(
			&*items[i]
	) };
	if (ti) {
		
#line 1332 "start.x"

	if (ti->token().type() ==
		Token_Type::ident
	) {
		
#line 1342 "start.x"

	auto s {
		_symbols.find(ti->token().name())
	};
	if (s != _symbols.end()) {
		
#line 1354 "start.x"

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

#line 1347 "start.x"
;
		i = 0; continue;
	}

#line 1336 "start.x"
;
	}

#line 1369 "start.x"

	if (ti->token().type() ==
		Token_Type::t_raw
	) {
		
#line 1379 "start.x"

	if (i < items.size() - 1) {
		auto *ta {
			dynamic_cast<Token_Item *>(
				&*items[i + 1]
		) };
		if (ta && ta->token().type() ==
			Token_Type::number
		) {
			
#line 1396 "start.x"

	int value { ta->token().value() };
	items.erase( items.begin() + i,
		items.begin() + i + 2
	);
	items.emplace(items.begin() + i,
		new Machine_Item { value }
	);

#line 1388 "start.x"
;
			i = 0; continue;
		}
	}

#line 1373 "start.x"
;
	}

#line 1408 "start.x"

	if (ti->token().type() ==
		Token_Type::t_times
	) {
		
#line 1419 "start.x"

	items.erase(items.begin() + i,
		items.begin() + i + 1);
	int addr = code.size() * 4 +
		0x20010000;
	items.emplace(items.begin() + i,
		new Token_Item({
			Token_Type::number, addr
		})
	);

#line 1412 "start.x"
;
		i = 0; continue;
	}

#line 1880 "start.x"

	if (ti->token().type() ==
		Token_Type::t_open_parenthesis
	) {
		if (i < items.size() - 4) {
			
#line 1892 "start.x"

	auto t2 {
		dynamic_cast<Token_Item *>(
			&*items[i + 1]
		)
	};
	if (t2 && t2->token().type() ==
		Token_Type::number
	) {
		int v1 { t2->token().value() };
		
#line 1908 "start.x"

	auto t3 {
		dynamic_cast<Token_Item *>(
			&*items[i + 2]
		)
	};
	if (t3 && (
		t3->token().type() ==
			Token_Type::plus ||
		t3->token().type() ==
			Token_Type::minus
	)) {
		bool neg { t3->token().type() ==
			Token_Type::minus };
		
#line 1928 "start.x"

	auto t4 {
		dynamic_cast<Token_Item *>(
			&*items[i + 3]
		)
	};
	if (t4 && t4->token().type() ==
		Token_Type::number
	) {
		int v2 { t4->token().value() };
		if (neg) { v2 = -v2; }
		
#line 1945 "start.x"

	auto t5 {
		dynamic_cast<Token_Item *>(
			&*items[i + 4]
		)
	};
	if (t5 && t5->token().type() ==
		Token_Type::t_close_parenthesis
	) {
		items.erase(items.begin() + i,
			items.begin() + i + 5
		);
		items.emplace(items.begin() + i,
			new Token_Item { {
				Token_Type::number,
				v1 + v2
			} }
		);
		i = 0; continue;
	}

#line 1939 "start.x"
;
	}

#line 1922 "start.x"
;
	}

#line 1902 "start.x"
;
	}

#line 1885 "start.x"
;
		}
	}

#line 1969 "start.x"

	if (ti->token().type() ==
		Token_Type::ident &&
			ti->token().name() == "goto"
	) {
		if (i < items.size() - 1) {
			
#line 1982 "start.x"

	auto tt {
		dynamic_cast<Token_Item *>(
			&*items[i + 1]
		)
	};
	if (tt && tt->token().type() ==
		Token_Type::number
	) {
		int target {
			tt->token().value()
		};
		items.erase(items.begin() + i,
			items.begin() + i + 2
		);
		items.emplace(items.begin() + i,
			new Pc_Item { }
		);
		items.emplace(items.begin() + i + 1,
			new Token_Item { {
				Token_Type::becomes
			} }
		);
		items.emplace(items.begin() + i + 2,
			new Pc_Item { }
		);
		items.emplace(items.begin() + i + 3,
			new Token_Item { {
				Token_Type::plus
			} }
		);
		items.emplace(items.begin() + i + 4,
			new Token_Item { {
				Token_Type::t_open_parenthesis
			} }
		);
		items.emplace(items.begin() + i + 5,
			new Token_Item { {
				Token_Type::number, target
			} }
		);
		items.emplace(items.begin() + i + 6,
			new Token_Item { {
				Token_Type::minus
			} }
		);
		items.emplace(items.begin() + i + 7,
			new Token_Item { {
				Token_Type::t_times
			} }
		);
		items.emplace(items.begin() + i + 8,
			new Token_Item { {
				Token_Type::t_close_parenthesis
			} }
		);
		i = 0; continue;
	}

#line 1975 "start.x"
;
		}
	}

#line 2044 "start.x"

	if (ti->token().type() ==
		Token_Type::ident &&
			i < items.size() - 1
	) {
		auto ci {
			dynamic_cast<Token_Item *>(
				&*items[i + 1]
			)
		};
		if (ci && ci->token().type() ==
			Token_Type::t_colon
		) {
			std::string name {
				ti->token().name()
			};
			items.erase(
				items.begin() + i,
				items.begin() + i + 2
			);
			items.emplace(items.begin() + i,
				new Token_Item { {
					Token_Type::ident,
					name
				} }
			);
			items.emplace(items.begin() + i + 1,
				new Token_Item { {
					Token_Type::t_equals
				} }
			);
			items.emplace(items.begin() + i + 2,
				new Token_Item { {
					Token_Type::t_times
				} }
			);
		}
	}

#line 1326 "start.x"
;
	}
} 
#line 1433 "start.x"
 {
	auto *ri {
		dynamic_cast<Register_Item *>(
			&*items[i]
	) };
	if (ri) {
		int rd { ri->nr() };
		
#line 1446 "start.x"

	if (i < items.size() - 2) {
		auto *t2 {
			dynamic_cast<Token_Item *>(
				&*items[i + 1]
		) };
		if (t2 && t2->token().type() ==
			Token_Type::becomes
		) {
			
#line 1462 "start.x"
 {
	auto *t3 {
		dynamic_cast<Token_Item *>(
			&*items[i + 2]
	) };
	if (t3 && t3->token().type() ==
		Token_Type::number
	) {
		
#line 1477 "start.x"

	int v { t3->token().value() };
	items.erase(items.begin() + i,
		items.begin() + i + 3
	);

#line 1486 "start.x"
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
#line 1499 "start.x"
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
#line 1470 "start.x"
;
		i = 0; continue;
	}
} 
#line 1514 "start.x"
 {
	auto c3 {
		dynamic_cast<Csr_Item *>(
			&*items[i + 2]
	) };
	if (c3) {
		
#line 1527 "start.x"

	int cv { c3->nr() };
	items.erase(items.begin() + i,
		items.begin() + i + 3
	);
	items.emplace(items.begin() + i,
		new I_Type_Item {
			cv, 0, 0x2, rd, 0x73
		}
	);

#line 1520 "start.x"
;
		i = 0; continue;
	}
} 
#line 1541 "start.x"
 {
	auto rs1 {
		dynamic_cast<Register_Item *>(
			&*items[i + 2]
		)
	};
	if (rs1) {
		int rs1_nr { rs1->nr() };
		
#line 1555 "start.x"

	if (i < items.size() - 4) {
		auto t3 {
			dynamic_cast<Token_Item *>(
				&*items[i + 3]
			)
		};
		if (t3) {
			
#line 1570 "start.x"

	if (t3->token().type() ==
		Token_Type::t_or
	) {
		
#line 1580 "start.x"
 {
	auto n4 {
		dynamic_cast<Token_Item *>(
			&*items[i + 4]
		)
	};
	if (n4 && n4->token().type() ==
		Token_Type::number
	) {
		
#line 1595 "start.x"

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

#line 1589 "start.x"
;
	}
} 
#line 1574 "start.x"
;
	}

#line 1610 "start.x"

	if (t3->token().type() ==
		Token_Type::t_and
	) {
		
#line 1620 "start.x"
 {
	auto n4 {
		dynamic_cast<Token_Item *>(
			&*items[i + 4]
		)
	};
	if (n4 && n4->token().type() ==
		Token_Type::number
	) {
		
#line 1635 "start.x"

	int imm { n4->token().value() };
	items.erase(items.begin() + i,
		items.begin() + i + 5
	);
	items.emplace(items.begin() + i,
		new I_Type_Item {
			imm, rs1_nr, 0x7, rd, 0x13
		}
	);
	i = 0; continue;

#line 1629 "start.x"
;
	}
} 
#line 1614 "start.x"
;
	}

#line 1563 "start.x"
;
		}
	}

#line 1549 "start.x"
;
	}
} 
#line 1455 "start.x"
;
		}
	}

#line 1440 "start.x"
;
	}
} 
#line 1650 "start.x"
 {
	auto pi {
		dynamic_cast<Pc_Item *>(
			&*items[i]
		)
	};
	if (pi) {
		
#line 1663 "start.x"

	if (i < items.size() - 2) {
		auto t2 {
			dynamic_cast<Token_Item *>(
				&*items[i + 1]
			)
		};
		if (t2 && t2->token().type() ==
			Token_Type::becomes
		) {
			
#line 1680 "start.x"

	auto p3 {
		dynamic_cast<Pc_Item *>(
			&*items[i + 2]
		)
	};
	if (p3) {
		
#line 1693 "start.x"

	if (i < items.size() - 4) {
		auto t4 {
			dynamic_cast<Token_Item *>(
				&*items[i + 3]
			)
		};
		if (t4 && (
			t4->token().type() ==
				Token_Type::plus ||
			t4->token().type() ==
				Token_Type::minus
		)) {
			bool neg {
				t4->token().type() ==
					Token_Type::minus
			};
			
#line 1746 "start.x"

	auto t5 {
		dynamic_cast<Token_Item *>(
			&*items[i + 4]
		)
	};
	if (t5 && t5->token().type() ==
		Token_Type::number
	) {
		int value { t5->token().value() };
		if (neg) { value = -value; }
		items.erase(items.begin() + i,
			items.begin() + i + 5
		);
		items.emplace(items.begin() + i,
			new J_Type_Item(
				value, 0, 0x6f
			)
		);
		i = 0; continue;
	}

#line 1710 "start.x"
;
		}
	}

#line 1771 "start.x"

	if (items.begin() + i + 3 == items.end()) {
		items.emplace(items.begin() + i + 3,
			new Token_Item { {
				Token_Type::plus
			} }
		);
		items.emplace(items.begin() + i + 4,
			new Token_Item { {
				Token_Type::number, 0
			} }
		);
		i = 0; continue;
	}

#line 1687 "start.x"
;
	}

#line 1673 "start.x"
;
		}
	}

#line 1657 "start.x"
;
	}
} 
#line 1789 "start.x"
 {
	auto ii {
		dynamic_cast<I_Type_Item *>(
			&*items[i]
		)
	};
	if (ii) {
		
#line 1802 "start.x"

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

#line 1796 "start.x"
;
	}
} 
#line 1820 "start.x"
 {
	auto ui {
		dynamic_cast<U_Type_Item *>(
			&*items[i]
		)
	};
	if (ui) {
		
#line 1834 "start.x"

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

#line 1827 "start.x"
;
		i = 0; continue;
	}
} 
#line 1849 "start.x"
 {
	auto ji {
		dynamic_cast<J_Type_Item *>(
			&*items[i]
		)
	};
	if (ji) {
		
#line 1862 "start.x"

	int imm { ji->immediate() };
	int result = 
		((imm << (31 - 20)) & 0x80000000) |
		((imm << (21 - 1)) & 0x7fe00000) |
		((imm << (20 - 11)) & 0x00100000) |
		(imm & 0x000ff000) |
		(ji->rd() << 7) | ji->opcode();
	items.erase(items.begin() + i,
		items.begin() + i + 1
	);
	items.emplace(items.begin() + i,
		new Machine_Item { result }
	);

#line 1856 "start.x"
;
	}
} 
#line 1296 "start.x"
;
		++i;
	}
	
#line 2105 "start.x"

	if (items.size() >= 2) {
		auto ii {
			dynamic_cast<Token_Item *>(
				&*items[0]
			)
		};
		auto ai {
			dynamic_cast<Token_Item *>(
				&*items[1]
			)
		};
		if (ii && ai &&
			ii->token().type() ==
				Token_Type::ident &&
			ai->token().type() ==
				Token_Type::t_equals
		) {
			
#line 2130 "start.x"

	auto &l { _symbols[
		ii->token().name()
	] };
	for (unsigned j = 2;
		j < items.size(); ++j
	) {
		l.push_back(std::move(items[j]));
	}
	items.erase(
		items.begin(), items.end()
	);

#line 2123 "start.x"
;
		}
	}

#line 1299 "start.x"
;
	
#line 2086 "start.x"

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

#line 1300 "start.x"
;
	if (! items.empty()) {
		std::cerr <<
			"cant expand fully [" <<
			line << "]\n";
	}

#line 789 "start.x"
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
		
#line 603 "start.x"

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

#line 438 "start.x"

	void assert_token(
		const char *line,
		Token_Type token
	) {
		
#line 459 "start.x"

	
#line 609 "start.x"

	clear_symbols();

#line 460 "start.x"
;
	Tokenizer t(line);
	assert(t.token().type() == token);
	t.next();
	assert(t.token().type() == Token_Type::end);

#line 443 "start.x"
;
	}

#line 522 "start.x"

	void assert_register(
		const char *line,
		const char *name
	) {
		
#line 534 "start.x"

	Tokenizer t(line);
	assert(t.token().type() == Token_Type::ident);
	assert(t.token().name() == name);
	t.next();
	assert(t.token().type() == Token_Type::end);

#line 527 "start.x"
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

#line 450 "start.x"

	assert_token(
		"<-", Token_Type::becomes
	);

#line 498 "start.x"

	assert_token("+", Token_Type::plus);
	assert_token("-", Token_Type::minus);

#line 545 "start.x"

	assert_register("%x10", "%x10");
	assert_register("%pc", "%pc");

#line 570 "start.x"

	assert_line(
		"%x4 <- %x2 + %x3",
		0x00310233
	);

#line 794 "start.x"

	assert_line_2(
		"%pc <- %pc",
		0x0000006f
	);

#line 805 "start.x"

	assert_line_2(
		"%pc <- %pc - 28",
		0xfe5ff06f
	);

#line 815 "start.x"

	assert_line_2(
		"%pc <- %pc - 32",
		0xfe1ff06f
	);

#line 850 "start.x"

	assert_line_2(
		"%x5 <- %x5 and $ff",
		0x0ff2f293
	);

#line 859 "start.x"

	assert_line_2(
		"%x5 <- %x5 or $1",
		0x0012e293
	);

#line 868 "start.x"

	assert_line_2(
		"%x6 <- %x6 or $1",
		0x00136313
	);

#line 877 "start.x"

	assert_line_2(
		"%x11 <- $0d",
		0x00d00593
	);

#line 886 "start.x"

	assert_line_2(
		"%x12 <- $0a",
		0x00a00613
	);

#line 895 "start.x"

	assert_line_2(
		"%x10 <- $1013000",
		0x1013537
	);

#line 904 "start.x"

	assert_line(
		"%x5 <- %pc",
		0x00000297
	);

#line 913 "start.x"

	assert_line(
		"%mtvec <- %x5",
		0x30529073
	);

#line 922 "start.x"

	assert_line_2(
		"%x5 <- %mhartid",
		0xf14022f3
	);

#line 948 "start.x"

	assert_line(
		"if %x5 < 0: %pc <- %pc + -4",
		0xfe02cee3
	);

#line 984 "start.x"

	assert_line(
		"if %x5 = 0: %pc <- %pc + -12",
		0xfe028ae3
	);

#line 993 "start.x"

	assert_line(
		"if %x5 != %x11: %pc <- %pc + -28",
		0xfeb292e3
	);

#line 1002 "start.x"

	assert_line(
		"if %x5 != 0: %pc <- %pc + 0",
		0x00029063
	);

#line 1050 "start.x"

	assert_line(
		"%x6 <- [%x10]",
		0x00052303
	);

#line 1059 "start.x"

	assert_line(
		"%x5 <- [%x10 + $04]",
		0x00452283
	);

#line 1068 "start.x"

	assert_line(
		"[%x10] <- %x12",
		0x00c52023
	);

#line 1077 "start.x"

	assert_line(
		"[%x10 + $08] <- %x5",
		0x00552423
	);

#line 171 "start.x"


#line 10 "start.x"

		
#line 825 "start.x"

	
#line 832 "start.x"

	State s;
	std::string l;
	while (std::getline(std::cin, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}

#line 826 "start.x"


#line 16 "hex.x"

	std::cout << s;

#line 11 "start.x"

	}
