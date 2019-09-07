
#line 5 "start.x"

	#include <iostream>
	
#line 23 "start.x"

	
#line 45 "start.x"

	
#line 60 "start.x"

	#include <string>
	#include <vector>

#line 218 "start.x"

	#include <memory>

#line 241 "start.x"

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

#line 336 "start.x"

	int build_r_cmd(
		int funct7, char src2, char src1,
		int funct3, char dst, int opcode
	) {
		return (funct7 << 25) |
			(src2 << 20) | (src1 << 15) |
			(funct3 << 12) | (dst << 7) |
			opcode;
	}

#line 350 "start.x"

	int build_i_cmd(
		int imm, char src1, int funct3, char dst, int opcode
	) {
		return (imm << 20) | (src1 << 15) | (funct3 << 12) | (dst << 7) | opcode;
	}

#line 360 "start.x"

	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}

#line 373 "start.x"

	int build_add(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x0, dst, 0x13
		);
	}

#line 635 "start.x"

	int build_load(
		char dst, char src, int imm
	) {
		return build_i_cmd(
			imm, src, 0x2, dst, 0x03
		);
	}

#line 683 "start.x"

	class Item {
		public:
			virtual ~Item() {};
			virtual Item *clone() const = 0;
			virtual void write(std::ostream &out) const = 0;
			virtual bool matches(const Item &in) const {
				return false;
			}
	};
	std::ostream &operator<<(std::ostream &out, const Item &item) {
		item.write(out);
		return out;
	}

#line 701 "start.x"

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
			bool matches(const Item &in) const override {
				auto o { dynamic_cast<const Named_Item *>(&in) };
				return o && (o->name() == _name || _name.empty());
			};
	};
	class Type_Item: public Item {
		private:
			std::string _type;
		public:
			Type_Item(const std::string &type): _type { type } { }
			const std::string &type() const { return _type; }
			Item *clone() const override {
				return new Type_Item { _type };
			}
			void write(std::ostream &out) const override {
				out << '@' << _type;
			}
			bool matches(const Item &in) const override {
				auto o { dynamic_cast<const Type_Item *>(&in) };
				return o && o->type() == _type;
			};
	};
	class Type_Instance_Item: public Type_Item {
		private:
			int _value;
		public:
			Type_Instance_Item(const std::string &type, int value):
				Type_Item { type }, _value { value }
			{ }
			int value() const { return _value; }
			Item *clone() const override {
				return new Type_Instance_Item { type(), _value };
			}
			void write(std::ostream &out) const override {
				Type_Item::write(out);
				out << ":$" << std::hex << _value << std::dec;
			}
			bool matches(const Item &in) const override {
				auto o { dynamic_cast<const Type_Instance_Item *>(&in) };
				return o && this->Type_Item::matches(*o) && o->value() == _value;
			}
	};

#line 760 "start.x"

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

#line 800 "start.x"

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

#line 1214 "start.x"

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

#line 46 "start.x"
;
	class State {
			
#line 94 "start.x"

	void add_machine(int instr);

#line 269 "start.x"

	std::vector<Macro> _macros;
	void setup_symbols();

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


#line 284 "start.x"

	State() { setup_symbols(); }

#line 50 "start.x"

	};
	
#line 77 "start.x"

	
#line 30 "start.x"

	#include <cassert>

#line 208 "start.x"

	#include <cctype>

#line 235 "start.x"

	#include <map>

#line 78 "start.x"

	void State::add_line(
		const std::string &line
	) {
		
#line 385 "start.x"

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
			items.emplace_back(new Type_Instance_Item { "num", value });
		} else if (*cur == '#') {
			break;
		} else if (*cur == '@') {
			++cur;
			std::string name;
			while (cur < end && isalnum(*cur)) {
				name += *cur++;
			}
			if (*cur == ':') {
				int value { 0 };
				++cur;
				if (*cur == '$') {
					++cur;
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
				} else {
					while (cur < end && isdigit(*cur)) {
						value = value * 10 + (*cur++ - '0');
					}
				}
				items.emplace_back(new Type_Instance_Item { name, value });
			} else {
				items.emplace_back(new Type_Item { name });
			}
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
			items.emplace_back(new Type_Instance_Item { "num", value });
		} else if (ispunct(*cur)) {
			while (cur < end && ispunct(*cur)) {
				++cur;
			}
			items.emplace_back(new Named_Item { { begin, cur } });
		}
	}
	
#line 832 "start.x"

restart:
	if (items.size()) {
		auto macro { _macros.begin() };
		while (macro != _macros.end()) {
			unsigned i = 0;
			while (i <= items.size() - macro->pattern().size()) {
				
#line 856 "start.x"
 {
	bool matches { true };
	auto p { macro->pattern().begin() };
	auto e { macro->pattern().end() };
	for (int j = 0; matches && p != e; ++p, ++j) {
		matches = matches && (i + j < items.size());
		matches = matches && (**p).matches(*items[i + j]);
	}
	if (matches) {
		items.erase(items.begin() + i, items.begin() + i + macro->pattern().size());
		for (const auto &e : macro->replacement()) {
			items.emplace(
				items.begin() + i, e->clone()
			);
			++i;
		}
		goto restart;
	}
} 
#line 878 "start.x"
 {
	auto *ni {
		dynamic_cast<Named_Item *>(
			&*items[i]
	) };
	if (ni) {
		
#line 890 "start.x"

	if (ni->name() == "raw") {
		
#line 898 "start.x"

	if (i < items.size() - 1) {
		auto *n2 {
			dynamic_cast<Type_Instance_Item *>(
				&*items[i + 1]
		) };
		if (n2 && n2->type() == "num") {
			
#line 912 "start.x"

	int value { n2->value() };
	items.erase( items.begin() + i,
		items.begin() + i + 2
	);
	items.emplace(items.begin() + i,
		new Type_Instance_Item { "raw", value }
	);
	goto restart;

#line 905 "start.x"
;
		}
	}

#line 892 "start.x"
;
	}

#line 925 "start.x"

	if (ni->name() == "*") {
		
#line 933 "start.x"

	items.erase(items.begin() + i,
		items.begin() + i + 1);
	int addr = code.size() * 4 +
		0x20010000;
	items.emplace(items.begin() + i,
		new Type_Instance_Item { "num", addr }
	);
	goto restart;

#line 927 "start.x"
;
	}

#line 1374 "start.x"

	if (ni->name() == "(") {
		if (i < items.size() - 4) {
			
#line 1384 "start.x"

	auto n2 {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i + 1]
		)
	};
	if (n2 && n2->type() == "num") {
		int v1 { n2->value() };
		
#line 1398 "start.x"

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
		
#line 1415 "start.x"

	auto n4 {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i + 3]
		)
	};
	if (n4 && n4->type() == "num") {
		int v2 { n4->value() };
		if (neg) { v2 = -v2; }
		
#line 1430 "start.x"

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
			new Type_Instance_Item { "num", v1 + v2 }
		);
		goto restart;
	}

#line 1424 "start.x"
;
	}

#line 1409 "start.x"
;
	}

#line 1392 "start.x"
;
	}

#line 1377 "start.x"
;
		}
	}

#line 1449 "start.x"

	if (ni->name() == "goto") {
		if (i < items.size() - 1) {
			
#line 1459 "start.x"

	auto n2 {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i + 1]
		)
	};
	if (n2 && n2->type() == "num") {
		int target {
			n2->value()
		};
		items.erase(items.begin() + i,
			items.begin() + i + 2
		);
		items.emplace(items.begin() + i,
			new Named_Item { "%pc" }
		);
		items.emplace(items.begin() + i + 1,
			new Named_Item { "<-" }
		);
		items.emplace(items.begin() + i + 2,
			new Named_Item { "%pc" }
		);
		items.emplace(items.begin() + i + 3,
			new Named_Item { "+" }
		);
		items.emplace(items.begin() + i + 4,
			new Named_Item { "(" }
		);
		items.emplace(items.begin() + i + 5,
			new Type_Instance_Item { "num", target }
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

#line 1452 "start.x"
;
		}
	}

#line 1505 "start.x"

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

#line 884 "start.x"
;
	}
} 
#line 946 "start.x"
 {
	auto *ri {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i]
	) };
	if (ri && ri->type() == "reg") {
		int rd { ri->value() };
		
#line 959 "start.x"

	if (i < items.size() - 2) {
		auto *t2 {
			dynamic_cast<Named_Item *>(
				&*items[i + 1]
		) };
		if (t2 && t2->name() == "<-") {
			
#line 973 "start.x"
 {
	auto *n3 {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i + 2]
	) };
	if (n3 && n3->type() == "num") {
		
#line 986 "start.x"

	int v { n3->value() };
	items.erase(items.begin() + i,
		items.begin() + i + 3
	);

#line 995 "start.x"
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
#line 1008 "start.x"
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
#line 979 "start.x"
;
		i = 0; continue;
	}
} 
#line 1024 "start.x"
 {
	auto c3 {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i + 2]
	) };
	if (c3 && c3->type() == "csr") {
		
#line 1036 "start.x"

	int cv { c3->value() };
	items.erase(items.begin() + i,
		items.begin() + i + 3
	);
	items.emplace(items.begin() + i,
		new I_Type_Item {
			cv, 0, 0x2, rd, 0x73
		}
	);
	goto restart;

#line 1030 "start.x"
;
	}
} 
#line 1051 "start.x"
 {
	auto rs1 {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i + 2]
		)
	};
	if (rs1 && rs1->type() == "reg") {
		int rs1_nr { rs1->value() };
		
#line 1065 "start.x"

	if (i < items.size() - 4) {
		auto n3 {
			dynamic_cast<Named_Item *>(
				&*items[i + 3]
			)
		};
		if (n3) {
			
#line 1080 "start.x"

	if (n3->name() == "or") {
		
#line 1088 "start.x"
 {
	auto n4 {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i + 4]
		)
	};
	if (n4 && n4->type() == "num") {
		
#line 1101 "start.x"

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

#line 1095 "start.x"
;
	}
} 
#line 1082 "start.x"
;
	}

#line 1116 "start.x"

	if (n3->name() == "and") {
		
#line 1124 "start.x"
 {
	auto n4 {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i + 4]
		)
	};
	if (n4 && n4->type() == "num") {
		
#line 1137 "start.x"

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

#line 1131 "start.x"
;
	}
} 
#line 1118 "start.x"
;
	}

#line 1073 "start.x"
;
		}
	}

#line 1059 "start.x"
;
	}
} 
#line 966 "start.x"
;
		}
	}

#line 953 "start.x"
;
	}
} 
#line 1152 "start.x"
 {
	auto pi {
		dynamic_cast<Named_Item *>(
			&*items[i]
		)
	};
	if (pi && pi->name() == "%pc") {
		
#line 1165 "start.x"

	if (i < items.size() - 2) {
		auto n2 {
			dynamic_cast<Named_Item *>(
				&*items[i + 1]
			)
		};
		if (n2 && n2->name() == "<-") {
			
#line 1180 "start.x"

	auto p3 {
		dynamic_cast<Named_Item *>(
			&*items[i + 2]
		)
	};
	if (p3 && p3->name() == "%pc") {
		
#line 1193 "start.x"

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
			
#line 1246 "start.x"

	auto n5 {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i + 4]
		)
	};
	if (n5 && n5->type() == "num") {
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

#line 1207 "start.x"
;
		}
	}

#line 1269 "start.x"

	if (items.begin() + i + 3 == items.end()) {
		items.emplace(items.begin() + i + 3,
			new Named_Item { "+" }
		);
		items.emplace(items.begin() + i + 4,
			new Type_Instance_Item { "num", 0 }
		);
		goto restart;
	}

#line 1187 "start.x"
;
	}

#line 1173 "start.x"
;
		}
	}

#line 1159 "start.x"
;
	}
} 
#line 1283 "start.x"
 {
	auto ii {
		dynamic_cast<I_Type_Item *>(
			&*items[i]
		)
	};
	if (ii) {
		
#line 1296 "start.x"

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
		new Type_Instance_Item { "raw", result }
	);
	i = 0; continue;

#line 1290 "start.x"
;
	}
} 
#line 1314 "start.x"
 {
	auto ui {
		dynamic_cast<U_Type_Item *>(
			&*items[i]
		)
	};
	if (ui) {
		
#line 1328 "start.x"

	int result {
		ui->immediate() |
		(ui->rd() << 7) | ui->opcode()
	};
	items.erase(items.begin() + i,
		items.begin() + i + 1
	);
	items.emplace(items.begin() + i,
		new Type_Instance_Item { "raw", result }
	);

#line 1321 "start.x"
;
		i = 0; continue;
	}
} 
#line 1343 "start.x"
 {
	auto ji {
		dynamic_cast<J_Type_Item *>(
			&*items[i]
		)
	};
	if (ji) {
		
#line 1356 "start.x"

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
		new Type_Instance_Item { "raw", result }
	);

#line 1350 "start.x"
;
	}
} 
#line 839 "start.x"
;
				++i;
			}
			++macro;
		}
		
#line 1555 "start.x"

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
			
#line 1575 "start.x"

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

#line 1568 "start.x"
;
		}
	}

#line 844 "start.x"
;
		
#line 1535 "start.x"

	while (! items.empty() &&
		dynamic_cast<Type_Instance_Item *>(
			&**items.begin()
	)) {
		auto &mi {
			dynamic_cast<Type_Instance_Item &>(
				**items.begin()
		)};
		if (mi.type() != "raw") { break; }
		add_machine(mi.value());
		items.erase(
			items.begin(),
			items.begin() + 1
		);
	}

#line 845 "start.x"
;
	}
	if (! items.empty()) {
		std::cerr <<
			"cant expand fully [" <<
			line << "]\n";
	}

#line 464 "start.x"
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

#line 276 "start.x"

	void State::setup_symbols() {
		
#line 290 "start.x"

	static const char* setups[] = {
		"%mtvec = @csr:$305",
		"%mhartid = @csr:$f14",
		"%x0 = @reg:0", "%x1 = @reg:1",
		"%x2 = @reg:2", "%x3 = @reg:3",
		"%x4 = @reg:4", "%x5 = @reg:5",
		"%x6 = @reg:6", "%x7 = @reg:7",
		"%x8 = @reg:8", "%x9 = @reg:9",
		"%x10 = @reg:10", "%x11 = @reg:11",
		"%x12 = @reg:12", "%x13 = @reg:13",
		"%x14 = @reg:14", "%x15 = @reg:15",
		"%x16 = @reg:16", "%x17 = @reg:17",
		"%x18 = @reg:18", "%x19 = @reg:19",
		"%x20 = @reg:20", "%x21 = @reg:21",
		"%x22 = @reg:22", "%x23 = @reg:23",
		"%x24 = @reg:24", "%x25 = @reg:25",
		"%x26 = @reg:26", "%x27 = @reg:27",
		"%x28 = @reg:28", "%x29 = @reg:29",
		"%x30 = @reg:30", "%x31 = @reg:31",
		"%zero = %x0", "%ra = %x1",
		"%sp = %x2", "%gp = %x3",
		"%tp = %x4", "%t0 = %x5",
		"%t1 = %x6", "%t2 = %x7",
		"%s0 = %x8", "%s1 = %x9",
		"%fp = %x8", "%a0 = %x10",
		"%a1 = %x11", "%a2 = %x12",
		"%a3 = %x13", "%a4 = %x14",
		"%a5 = %x15", "%a6 = %x16",
		"%a7 = %x17", "%s2 = %x18",
		"%s3 = %x19", "%s4 = %x20",
		"%s5 = %x21", "%s6 = %x22",
		"%s7 = %x23", "%s8 = %x24",
		"%s9 = %x25", "%s10 = %x26",
		"%s11 = %x27", "%t3 = %x28",
		"%t4 = %x29", "%t5 = %x30",
		"%t6 = %x31", "*) = * )",
		nullptr
	};
	for (auto i = setups; *i; ++i) {
		add_line(*i);
	}

#line 278 "start.x"
;
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
		
#line 170 "start.x"

	
#line 177 "start.x"

	assert_line_2(
		"raw $87654321", 0x87654321
	);

#line 225 "start.x"

	assert_line(
		"%x4 <- %x2 + %x3",
		0x00310233
	);

#line 469 "start.x"

	assert_line_2(
		"%pc <- %pc",
		0x0000006f
	);

#line 480 "start.x"

	assert_line_2(
		"%pc <- %pc - 28",
		0xfe5ff06f
	);

#line 490 "start.x"

	assert_line_2(
		"%pc <- %pc - 32",
		0xfe1ff06f
	);

#line 518 "start.x"

	assert_line_2(
		"%x5 <- %x5 and $ff",
		0x0ff2f293
	);

#line 527 "start.x"

	assert_line_2(
		"%x5 <- %x5 or $1",
		0x0012e293
	);

#line 536 "start.x"

	assert_line_2(
		"%x6 <- %x6 or $1",
		0x00136313
	);

#line 545 "start.x"

	assert_line_2(
		"%x11 <- $0d",
		0x00d00593
	);

#line 554 "start.x"

	assert_line_2(
		"%x12 <- $0a",
		0x00a00613
	);

#line 563 "start.x"

	assert_line_2(
		"%x10 <- $1013000",
		0x1013537
	);

#line 572 "start.x"

	assert_line(
		"%x5 <- %pc",
		0x00000297
	);

#line 581 "start.x"

	assert_line(
		"%mtvec <- %x5",
		0x30529073
	);

#line 590 "start.x"

	assert_line_2(
		"%x5 <- %mhartid",
		0xf14022f3
	);

#line 599 "start.x"

	assert_line(
		"if %x5 < 0: %pc <- %pc + -4",
		0xfe02cee3
	);

#line 608 "start.x"

	assert_line(
		"if %x5 = 0: %pc <- %pc + -12",
		0xfe028ae3
	);

#line 617 "start.x"

	assert_line(
		"if %x5 != %x11: %pc <- %pc + -28",
		0xfeb292e3
	);

#line 626 "start.x"

	assert_line(
		"if %x5 != 0: %pc <- %pc + 0",
		0x00029063
	);

#line 647 "start.x"

	assert_line(
		"%x6 <- [%x10]",
		0x00052303
	);

#line 656 "start.x"

	assert_line(
		"%x5 <- [%x10 + $04]",
		0x00452283
	);

#line 665 "start.x"

	assert_line(
		"[%x10] <- %x12",
		0x00c52023
	);

#line 674 "start.x"

	assert_line(
		"[%x10 + $08] <- %x5",
		0x00552423
	);

#line 171 "start.x"


#line 11 "start.x"

		
#line 500 "start.x"

	
#line 507 "start.x"

	State s;
	std::string l;
	while (std::getline(std::cin, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}

#line 501 "start.x"


#line 16 "hex.x"

	std::cout << s;

#line 12 "start.x"

	}
