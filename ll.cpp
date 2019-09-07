
#line 5 "start.x"

	#include <iostream>
	
#line 23 "start.x"

	
#line 45 "start.x"

	
#line 60 "start.x"

	#include <string>
	#include <vector>

#line 219 "start.x"

	#include <memory>

#line 405 "start.x"

	int build_r_cmd(
		int funct7, char src2, char src1,
		int funct3, char dst, int opcode
	) {
		return (funct7 << 25) |
			(src2 << 20) | (src1 << 15) |
			(funct3 << 12) | (dst << 7) |
			opcode;
	}

#line 419 "start.x"

	int build_i_cmd(
		int imm, char src1, int funct3, char dst, int opcode
	) {
		return (imm << 20) | (src1 << 15) | (funct3 << 12) | (dst << 7) | opcode;
	}

#line 429 "start.x"

	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}

#line 442 "start.x"

	int build_add(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x0, dst, 0x13
		);
	}

#line 672 "start.x"

	int build_load(
		char dst, char src, int imm
	) {
		return build_i_cmd(
			imm, src, 0x2, dst, 0x03
		);
	}

#line 46 "start.x"
;
	class State {
			
#line 94 "start.x"

	void add_machine(int instr);

#line 48 "start.x"

		public:
			
#line 68 "start.x"

	void add_line(
		const std::string &line
	);

#line 110 "start.x"

	
#line 117 "start.x"

	private:
		std::vector<int> code;

#line 125 "start.x"

	public:
		int code_size() const {
			return static_cast<int>(
				code.size()
			);
		}

#line 137 "start.x"

	public:
		int get_code(int pos) const {
			return code[pos];
		}

#line 111 "start.x"


#line 50 "start.x"

	};
	
#line 77 "start.x"

	
#line 30 "start.x"

	#include <cassert>

#line 209 "start.x"

	#include <cctype>

#line 236 "start.x"

	#include <map>

#line 242 "start.x"

	class Item;
	
	using Item_Ptr = std::unique_ptr<Item>;
	using Items = std::vector<Item_Ptr>;
	class Macro {
		private:
			Items _pattern;
			Items _replacement;
		public:
			Macro(
				Items &&pattern,
				Items &&replacement
			):
				_pattern { std::move(pattern) },
				_replacement { std::move(replacement) }
			{ }
			const Items &pattern() const {
				return _pattern;
			}
			const Items &replacement() const {
				return _replacement;
			}
	};
	static std::vector<Macro> _macros;

#line 271 "start.x"

	
#line 720 "start.x"

	class Item {
		public:
			virtual ~Item() {};
			virtual Item *clone() const = 0;
			virtual void write(std::ostream &out) const = 0;
	};
	std::ostream &operator<<(std::ostream &out, const Item &item) {
		item.write(out);
		return out;
	}

#line 735 "start.x"

	class Named_Item: public Item {
		private:
			std::string _name;
		public:
			Named_Item(const std::string &name): _name { name } { }
			const std::string &name() const { return _name; }
			Item *clone() const override {
				return new Named_Item { _name };
			}
			void write(std::ostream &out) const override {
				out << _name;
			}
	};
	class Number_Item: public Item {
		private:
			int _value;
		public:
			Number_Item(int value): _value { value } { }
			int value() const { return _value; }
			Item *clone() const override {
				return new Number_Item { _value };
			}
			void write(std::ostream &out) const override {
				out << '$' << std::hex << _value << std::dec;
			}
	};

#line 766 "start.x"

	class Pc_Item: public Item {
		public:
			Item *clone() const override {
				return new Pc_Item { };
			}
			void write(std::ostream &out) const override {
				out << "%pc";
			}
	};

#line 780 "start.x"

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
			void write(std::ostream &out) const override {
				out << "%x" << _nr;
			}
	};

#line 802 "start.x"

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
			void write(std::ostream &out) const override {
				out << "#csr.$" << std::hex << _nr << std::dec;
			}
	};

#line 824 "start.x"

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
			void write(std::ostream &out) const override {
				out << "#raw.$" << std::hex << _instruction << std::dec;
			}
	};

#line 846 "start.x"

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
			void write(std::ostream &out) const override {
				out << "#i_type(" << _immediate << ", " << _rs1 <<
					", " << _func3 << ", " << _rd << ", " <<
					_opcode << ')';
			}
	};

#line 886 "start.x"

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
			void write(std::ostream &out) const override {
				out << "#u_type(" << _immediate << ", " << _rd << ", " << _opcode << ')';
			}
	};

#line 1301 "start.x"

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
			void write(std::ostream &out) const override {
				out << "#j_type(" << _immediate << ", " << _rd << ", " << _opcode << ')';
			}
	};

#line 272 "start.x"
;
	void clear_symbols() {
		_macros.clear();
		
#line 287 "start.x"

	{
		Items p; p.emplace_back(new Named_Item { "%pc" });
		Items e; e.emplace_back(new Pc_Item { });
		_macros.emplace_back(std::move(p), std::move(e));
	} {
		Items p; p.emplace_back(new Named_Item { "%mtvec" });
		Items e; e.emplace_back(new Csr_Item { 0x305 });
		_macros.emplace_back(std::move(p), std::move(e));
	} {
		Items p; p.emplace_back(new Named_Item { "%mhartid" });
		Items e; e.emplace_back(new Csr_Item { 0xf14 });
		_macros.emplace_back(std::move(p), std::move(e));
	} {
		std::string name { "%x#" };
		for (int i = 0; i < 10; ++i) {
			name[2] = '0' + i;
			Items p; p.emplace_back(new Named_Item { name });
			Items e; e.emplace_back(new Register_Item { i });
			_macros.emplace_back(std::move(p), std::move(e));
		}
	} {
		std::string name { "%x##" };
		for (int i = 10; i < 32; ++i) {
			name[2] = '0' + (i / 10);
			name[3] = '0' + (i % 10);
			Items p; p.emplace_back(new Named_Item { name });
			Items e; e.emplace_back(new Register_Item { i });
			_macros.emplace_back(std::move(p), std::move(e));
		}
	}

#line 322 "start.x"

	{
		Items p; p.emplace_back(new Named_Item { "%zero" });
		Items e; e.emplace_back(new Register_Item { 0 });
		_macros.emplace_back(std::move(p), std::move(e));
	} {
		Items p; p.emplace_back(new Named_Item { "%ra" });
		Items e; e.emplace_back(new Register_Item { 1 });
		_macros.emplace_back(std::move(p), std::move(e));
	} {
		Items p; p.emplace_back(new Named_Item { "%sp" });
		Items e; e.emplace_back(new Register_Item { 1 });
		_macros.emplace_back(std::move(p), std::move(e));
	} {
		Items p; p.emplace_back(new Named_Item { "%gp"});
		Items e; e.emplace_back(new Register_Item { 3 });
		_macros.emplace_back(std::move(p), std::move(e));
	} {
		Items p; p.emplace_back(new Named_Item { "%tp" });
		Items e; e.emplace_back(new Register_Item { 4 });
		_macros.emplace_back(std::move(p), std::move(e));
	} {
		std::string name { "%t#" };
		for (int i = 0; i < 3; ++i) {
			name[2] = '0' + i;
			Items p; p.emplace_back(new Named_Item { name });
			Items e; e.emplace_back(new Register_Item { i + 5 });
			_macros.emplace_back(std::move(p), std::move(e));
		}
	} {
		std::string name { "%s#" };
		for (int i = 0; i < 2; ++i) {
			name[2] = '0' + i;
			Items p; p.emplace_back(new Named_Item { name });
			Items e; e.emplace_back(new Register_Item { i + 8 });
			_macros.emplace_back(std::move(p), std::move(e));
		}
	} {
		Items p; p.emplace_back(new Named_Item { "%fp" });
		Items e; e.emplace_back(new Register_Item { 8 });
		_macros.emplace_back(std::move(p), std::move(e));
	} {
		std::string name { "%a#" };
		for (int i = 0; i < 8; ++i) {
			name[2] = '0' + i;
			Items p; p.emplace_back(new Named_Item { name });
			Items e; e.emplace_back(new Register_Item { i + 10 });
			_macros.emplace_back(std::move(p), std::move(e));
		}
	} {
		std::string name { "%s#" };
		for (int i = 2; i < 10; ++i) {
			name[2] = '0' + i;
			Items p; p.emplace_back(new Named_Item { name });
			Items e; e.emplace_back(new Register_Item { i + 16 });
			_macros.emplace_back(std::move(p), std::move(e));
		}
	} {
		std::string name { "%s1#" };
		for (int i = 10; i < 12; ++i) {
			name[3] = '0' + (i % 10);
			Items p; p.emplace_back(new Named_Item { name });
			Items e; e.emplace_back(new Register_Item { i + 16 });
			_macros.emplace_back(std::move(p), std::move(e));
		}
	} {
		std::string name { "%t#" };
		for (int i = 3; i < 7; ++i) {
			name[2] = '0' + i;
			Items p; p.emplace_back(new Named_Item { name });
			Items e; e.emplace_back(new Register_Item { i + 25 });
			_macros.emplace_back(std::move(p), std::move(e));
		}
	} {
		Items p; p.emplace_back(new Named_Item { "*)" });
		Items e; e.emplace_back(new Named_Item { "*" });
		e.emplace_back(new Named_Item { ")" });
		_macros.emplace_back(std::move(p), std::move(e));
	}

#line 275 "start.x"
;
	}

#line 78 "start.x"

	void State::add_line(
		const std::string &line
	) {
		
#line 454 "start.x"

	std::vector<std::unique_ptr<Item>> items;
	auto end { line.end() };
	auto cur { line.begin() };
	for (;;) {
		while (cur < end && *cur <= ' ') {
			++cur;
		}
		if (cur == end) { break; }

		auto begin { cur };
		if (isalpha(*cur) || *cur == '_' || *cur == '%') {
			while (cur < end && (isalnum(*cur) || *cur == '_' || *cur == '%')) {
				++cur;
			}
			items.emplace_back(new Named_Item { { begin, cur } } );
		} else if (isdigit(*cur)) {
			int value { 0 };
			while (cur < end && isdigit(*cur)) {
				value = value * 10 + (*cur++ - '0');
			}
			items.emplace_back(new Number_Item { value });
		} else if (*cur == '#') {
			break;
		} else if (*cur == '$') {
			++cur;
			int value { 0 };
			while (cur < end && isxdigit(*cur)) {
				int digit;
				if (isdigit(*cur)) {
					digit = *cur - '0';
				} else if (*cur <= 'F') {
					digit = *cur - 'A' + 10;
				} else {
					digit = *cur - 'a' + 10;
				}
				value = value * 16 + digit;
				++cur;
			}
			items.emplace_back(new Number_Item { value });
		} else if (ispunct(*cur)) {
			while (cur < end && ispunct(*cur)) {
				++cur;
			}
			items.emplace_back(new Named_Item { { begin, cur } });
		}
	}
	
#line 918 "start.x"

restart:
	if (items.size()) {
		auto macro { _macros.begin() };
		while (macro != _macros.end()) {
			unsigned i = 0;
			while (i <= items.size() - macro->pattern().size()) {
				
#line 942 "start.x"
 {
	auto *ni {
		dynamic_cast<Named_Item *>(
			&*items[i]
	) };
	if (ni) {
		
#line 954 "start.x"

	if (ni->name() == dynamic_cast<Named_Item *>(&**macro->pattern().begin())->name()) {
		
#line 963 "start.x"

	items.erase(items.begin() + i,
		items.begin() + i + 1
	);
	for (const auto &e : macro->replacement()) {
		items.emplace(
			items.begin() + i, e->clone()
		);
		++i;
	}

#line 956 "start.x"
;
		goto restart;
	}

#line 977 "start.x"

	if (ni->name() == "raw") {
		
#line 985 "start.x"

	if (i < items.size() - 1) {
		auto *n2 {
			dynamic_cast<Number_Item *>(
				&*items[i + 1]
		) };
		if (n2) {
			
#line 999 "start.x"

	int value { n2->value() };
	items.erase( items.begin() + i,
		items.begin() + i + 2
	);
	items.emplace(items.begin() + i,
		new Machine_Item { value }
	);
	goto restart;

#line 992 "start.x"
;
		}
	}

#line 979 "start.x"
;
	}

#line 1012 "start.x"

	if (ni->name() == "*") {
		
#line 1020 "start.x"

	items.erase(items.begin() + i,
		items.begin() + i + 1);
	int addr = code.size() * 4 +
		0x20010000;
	items.emplace(items.begin() + i,
		new Number_Item { addr }
	);
	goto restart;

#line 1014 "start.x"
;
	}

#line 1461 "start.x"

	if (ni->name() == "(") {
		if (i < items.size() - 4) {
			
#line 1471 "start.x"

	auto n2 {
		dynamic_cast<Number_Item *>(
			&*items[i + 1]
		)
	};
	if (n2) {
		int v1 { n2->value() };
		
#line 1485 "start.x"

	auto n3 {
		dynamic_cast<Named_Item *>(
			&*items[i + 2]
		)
	};
	if (n3 && (
		n3->name() == "+" ||
		n3->name() == "-"
	)) {
		bool neg { n3->name() == "-" };
		
#line 1502 "start.x"

	auto n4 {
		dynamic_cast<Number_Item *>(
			&*items[i + 3]
		)
	};
	if (n4) {
		int v2 { n4->value() };
		if (neg) { v2 = -v2; }
		
#line 1517 "start.x"

	auto n5 {
		dynamic_cast<Named_Item *>(
			&*items[i + 4]
		)
	};
	if (n5 && n5->name() == ")") {
		items.erase(items.begin() + i,
			items.begin() + i + 5
		);
		items.emplace(items.begin() + i,
			new Number_Item { v1 + v2 }
		);
		goto restart;
	}

#line 1511 "start.x"
;
	}

#line 1496 "start.x"
;
	}

#line 1479 "start.x"
;
	}

#line 1464 "start.x"
;
		}
	}

#line 1536 "start.x"

	if (ni->name() == "goto") {
		if (i < items.size() - 1) {
			
#line 1546 "start.x"

	auto n2 {
		dynamic_cast<Number_Item *>(
			&*items[i + 1]
		)
	};
	if (n2) {
		int target {
			n2->value()
		};
		items.erase(items.begin() + i,
			items.begin() + i + 2
		);
		items.emplace(items.begin() + i,
			new Pc_Item { }
		);
		items.emplace(items.begin() + i + 1,
			new Named_Item { "<-" }
		);
		items.emplace(items.begin() + i + 2,
			new Pc_Item { }
		);
		items.emplace(items.begin() + i + 3,
			new Named_Item { "+" }
		);
		items.emplace(items.begin() + i + 4,
			new Named_Item { "(" }
		);
		items.emplace(items.begin() + i + 5,
			new Number_Item { target }
		);
		items.emplace(items.begin() + i + 6,
			new Named_Item { "-" }
		);
		items.emplace(items.begin() + i + 7,
			new Named_Item { "*" }
		);
		items.emplace(items.begin() + i + 8,
			new Named_Item { ")" }
		);
		goto restart;
	}

#line 1539 "start.x"
;
		}
	}

#line 1592 "start.x"

	if (i < items.size() - 1) {
		auto ci {
			dynamic_cast<Named_Item *>(
				&*items[i + 1]
			)
		};
		if (ci && ci->name() == ":") {
			std::string name {
				ni->name()
			};
			items.erase(
				items.begin() + i,
				items.begin() + i + 2
			);
			items.emplace(items.begin() + i,
				new Named_Item { name }
			);
			items.emplace(items.begin() + i + 1,
				new Named_Item { "=" }
			);
			items.emplace(items.begin() + i + 2,
				new Named_Item { "*" }
			);
		}
	}

#line 948 "start.x"
;
	}
} 
#line 1033 "start.x"
 {
	auto *ri {
		dynamic_cast<Register_Item *>(
			&*items[i]
	) };
	if (ri) {
		int rd { ri->nr() };
		
#line 1046 "start.x"

	if (i < items.size() - 2) {
		auto *t2 {
			dynamic_cast<Named_Item *>(
				&*items[i + 1]
		) };
		if (t2 && t2->name() == "<-") {
			
#line 1060 "start.x"
 {
	auto *n3 {
		dynamic_cast<Number_Item *>(
			&*items[i + 2]
	) };
	if (n3) {
		
#line 1073 "start.x"

	int v { n3->value() };
	items.erase(items.begin() + i,
		items.begin() + i + 3
	);

#line 1082 "start.x"
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
#line 1095 "start.x"
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
	goto restart;
} 
#line 1066 "start.x"
;
		i = 0; continue;
	}
} 
#line 1111 "start.x"
 {
	auto c3 {
		dynamic_cast<Csr_Item *>(
			&*items[i + 2]
	) };
	if (c3) {
		
#line 1123 "start.x"

	int cv { c3->nr() };
	items.erase(items.begin() + i,
		items.begin() + i + 3
	);
	items.emplace(items.begin() + i,
		new I_Type_Item {
			cv, 0, 0x2, rd, 0x73
		}
	);
	goto restart;

#line 1117 "start.x"
;
	}
} 
#line 1138 "start.x"
 {
	auto rs1 {
		dynamic_cast<Register_Item *>(
			&*items[i + 2]
		)
	};
	if (rs1) {
		int rs1_nr { rs1->nr() };
		
#line 1152 "start.x"

	if (i < items.size() - 4) {
		auto n3 {
			dynamic_cast<Named_Item *>(
				&*items[i + 3]
			)
		};
		if (n3) {
			
#line 1167 "start.x"

	if (n3->name() == "or") {
		
#line 1175 "start.x"
 {
	auto n4 {
		dynamic_cast<Number_Item *>(
			&*items[i + 4]
		)
	};
	if (n4) {
		
#line 1188 "start.x"

	int imm { n4->value() };
	items.erase(items.begin() + i,
		items.begin() + i + 5
	);
	items.emplace(items.begin() + i,
		new I_Type_Item {
			imm, rs1_nr, 0x6, rd, 0x13
		}
	);
	goto restart;

#line 1182 "start.x"
;
	}
} 
#line 1169 "start.x"
;
	}

#line 1203 "start.x"

	if (n3->name() == "and") {
		
#line 1211 "start.x"
 {
	auto n4 {
		dynamic_cast<Number_Item *>(
			&*items[i + 4]
		)
	};
	if (n4) {
		
#line 1224 "start.x"

	int imm { n4->value() };
	items.erase(items.begin() + i,
		items.begin() + i + 5
	);
	items.emplace(items.begin() + i,
		new I_Type_Item {
			imm, rs1_nr, 0x7, rd, 0x13
		}
	);
	goto restart;

#line 1218 "start.x"
;
	}
} 
#line 1205 "start.x"
;
	}

#line 1160 "start.x"
;
		}
	}

#line 1146 "start.x"
;
	}
} 
#line 1053 "start.x"
;
		}
	}

#line 1040 "start.x"
;
	}
} 
#line 1239 "start.x"
 {
	auto pi {
		dynamic_cast<Pc_Item *>(
			&*items[i]
		)
	};
	if (pi) {
		
#line 1252 "start.x"

	if (i < items.size() - 2) {
		auto n2 {
			dynamic_cast<Named_Item *>(
				&*items[i + 1]
			)
		};
		if (n2 && n2->name() == "<-") {
			
#line 1267 "start.x"

	auto p3 {
		dynamic_cast<Pc_Item *>(
			&*items[i + 2]
		)
	};
	if (p3) {
		
#line 1280 "start.x"

	if (i < items.size() - 4) {
		auto n4 {
			dynamic_cast<Named_Item *>(
				&*items[i + 3]
			)
		};
		if (n4 && (
			n4->name() == "+" ||
			n4->name() == "-"
		)) {
			bool neg {
				n4->name() == "-"
			};
			
#line 1333 "start.x"

	auto n5 {
		dynamic_cast<Number_Item *>(
			&*items[i + 4]
		)
	};
	if (n5) {
		int value { n5->value() };
		if (neg) { value = -value; }
		items.erase(items.begin() + i,
			items.begin() + i + 5
		);
		items.emplace(items.begin() + i,
			new J_Type_Item(
				value, 0, 0x6f
			)
		);
		goto restart;
	}

#line 1294 "start.x"
;
		}
	}

#line 1356 "start.x"

	if (items.begin() + i + 3 == items.end()) {
		items.emplace(items.begin() + i + 3,
			new Named_Item { "+" }
		);
		items.emplace(items.begin() + i + 4,
			new Number_Item { 0 }
		);
		goto restart;
	}

#line 1274 "start.x"
;
	}

#line 1260 "start.x"
;
		}
	}

#line 1246 "start.x"
;
	}
} 
#line 1370 "start.x"
 {
	auto ii {
		dynamic_cast<I_Type_Item *>(
			&*items[i]
		)
	};
	if (ii) {
		
#line 1383 "start.x"

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

#line 1377 "start.x"
;
	}
} 
#line 1401 "start.x"
 {
	auto ui {
		dynamic_cast<U_Type_Item *>(
			&*items[i]
		)
	};
	if (ui) {
		
#line 1415 "start.x"

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

#line 1408 "start.x"
;
		i = 0; continue;
	}
} 
#line 1430 "start.x"
 {
	auto ji {
		dynamic_cast<J_Type_Item *>(
			&*items[i]
		)
	};
	if (ji) {
		
#line 1443 "start.x"

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

#line 1437 "start.x"
;
	}
} 
#line 925 "start.x"
;
				++i;
			}
			++macro;
		}
		
#line 1641 "start.x"

	if (items.size() >= 2) {
		auto ii {
			dynamic_cast<Named_Item *>(
				&*items[0]
			)
		};
		auto ai {
			dynamic_cast<Named_Item *>(
				&*items[1]
			)
		};
		if (ii && ai && ai->name() == "=") {
			
#line 1661 "start.x"

	Items value;
	for (unsigned j = 2;
		j < items.size(); ++j
	) {
		value.push_back(std::move(items[j]));
	}
	Items p;
	p.emplace_back(new Named_Item { ii->name() });
	_macros.emplace_back(std::move(p), std::move(value));
	items.erase(
		items.begin(), items.end()
	);

#line 1654 "start.x"
;
		}
	}

#line 930 "start.x"
;
		
#line 1622 "start.x"

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

#line 931 "start.x"
;
	}
	if (! items.empty()) {
		std::cerr <<
			"cant expand fully [" <<
			line << "]\n";
	}

#line 501 "start.x"
;

#line 82 "start.x"
;
	}

#line 101 "start.x"

	void State::add_machine(int instr) {
		
#line 147 "start.x"

	code.push_back(instr);

#line 103 "start.x"

	}

#line 52 "start.x"
;

#line 154 "start.x"

	void assert_line(const char *line, int expected) {
		std::cerr << "ignoring " << line << "\n";
	}
	void assert_line_2(
		const char *line,
		int expected
	) {
		
#line 281 "start.x"

	clear_symbols();

#line 162 "start.x"
;
		
#line 186 "start.x"

	State s;
	s.add_line(line);
	assert(s.code_size() == 1);
	// std::cerr << "EXP " << std::hex << expected << "\n";
	// std::cerr << "GOT " << s.get_code(0) << std::dec << "\n";
	assert(s.get_code(0) == expected);

#line 163 "start.x"
;
	}

#line 24 "start.x"


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

#line 7 "start.x"
;
	int main(
		int argc, const char *argv[]
	) {
		
#line 171 "start.x"

	
#line 178 "start.x"

	assert_line_2(
		"raw $87654321", 0x87654321
	);

#line 226 "start.x"

	assert_line(
		"%x4 <- %x2 + %x3",
		0x00310233
	);

#line 506 "start.x"

	assert_line_2(
		"%pc <- %pc",
		0x0000006f
	);

#line 517 "start.x"

	assert_line_2(
		"%pc <- %pc - 28",
		0xfe5ff06f
	);

#line 527 "start.x"

	assert_line_2(
		"%pc <- %pc - 32",
		0xfe1ff06f
	);

#line 555 "start.x"

	assert_line_2(
		"%x5 <- %x5 and $ff",
		0x0ff2f293
	);

#line 564 "start.x"

	assert_line_2(
		"%x5 <- %x5 or $1",
		0x0012e293
	);

#line 573 "start.x"

	assert_line_2(
		"%x6 <- %x6 or $1",
		0x00136313
	);

#line 582 "start.x"

	assert_line_2(
		"%x11 <- $0d",
		0x00d00593
	);

#line 591 "start.x"

	assert_line_2(
		"%x12 <- $0a",
		0x00a00613
	);

#line 600 "start.x"

	assert_line_2(
		"%x10 <- $1013000",
		0x1013537
	);

#line 609 "start.x"

	assert_line(
		"%x5 <- %pc",
		0x00000297
	);

#line 618 "start.x"

	assert_line(
		"%mtvec <- %x5",
		0x30529073
	);

#line 627 "start.x"

	assert_line_2(
		"%x5 <- %mhartid",
		0xf14022f3
	);

#line 636 "start.x"

	assert_line(
		"if %x5 < 0: %pc <- %pc + -4",
		0xfe02cee3
	);

#line 645 "start.x"

	assert_line(
		"if %x5 = 0: %pc <- %pc + -12",
		0xfe028ae3
	);

#line 654 "start.x"

	assert_line(
		"if %x5 != %x11: %pc <- %pc + -28",
		0xfeb292e3
	);

#line 663 "start.x"

	assert_line(
		"if %x5 != 0: %pc <- %pc + 0",
		0x00029063
	);

#line 684 "start.x"

	assert_line(
		"%x6 <- [%x10]",
		0x00052303
	);

#line 693 "start.x"

	assert_line(
		"%x5 <- [%x10 + $04]",
		0x00452283
	);

#line 702 "start.x"

	assert_line(
		"[%x10] <- %x12",
		0x00c52023
	);

#line 711 "start.x"

	assert_line(
		"[%x10 + $08] <- %x5",
		0x00552423
	);

#line 172 "start.x"


#line 11 "start.x"

		
#line 537 "start.x"

	
#line 544 "start.x"

	State s;
	std::string l;
	while (std::getline(std::cin, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}

#line 538 "start.x"


#line 16 "hex.x"

	std::cout << s;

#line 12 "start.x"

	}
