
#line 5 "start.x"

	#include <iostream>
	
#line 23 "start.x"

	
#line 45 "start.x"

	
#line 60 "start.x"

	#include <string>
	#include <vector>
	#include <sstream>

#line 219 "start.x"

	#include <memory>

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

#line 303 "start.x"

	int build_r_cmd(
		int funct7, char src2, char src1,
		int funct3, char dst, int opcode
	) {
		return (funct7 << 25) |
			(src2 << 20) | (src1 << 15) |
			(funct3 << 12) | (dst << 7) |
			opcode;
	}

#line 317 "start.x"

	int build_i_cmd(
		int imm, char src1, int funct3, char dst, int opcode
	) {
		return (imm << 20) | (src1 << 15) | (funct3 << 12) | (dst << 7) | opcode;
	}

#line 327 "start.x"

	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}

#line 340 "start.x"

	int build_add(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x0, dst, 0x13
		);
	}

#line 577 "start.x"

	int build_load(
		char dst, char src, int imm
	) {
		return build_i_cmd(
			imm, src, 0x2, dst, 0x03
		);
	}

#line 625 "start.x"

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

#line 643 "start.x"

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
	class Type_Instance_Item: public Item {
		private:
			std::string _type;
			int _value;
		public:
			Type_Instance_Item(const std::string &type, int value):
				_type { type }, _value { value }
			{ }
			const std::string &type() const { return _type; }
			int value() const { return _value; }
			Item *clone() const override {
				return new Type_Instance_Item { _type, _value };
			}
			void write(std::ostream &out) const override {
				out << '@' << _type << ":$" << std::hex << _value << std::dec;
			}
			bool matches(const Item &in) const override {
				auto o { dynamic_cast<const Type_Instance_Item *>(&in) };
				return o && _type == o->type() && o->value() == _value;
			}
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
				if (o && o->type() == _type) { return true; };
				auto i { dynamic_cast<const Type_Instance_Item *>(&in) };
				if (i && i->type() == _type) { return true; }
				return false;
			};
	};

#line 706 "start.x"

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

#line 746 "start.x"

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

#line 1171 "start.x"

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
			
#line 95 "start.x"

	void add_machine(int instr);

#line 270 "start.x"

	std::vector<Macro> _macros;
	void setup_symbols();

#line 48 "start.x"

		public:
			
#line 69 "start.x"

	void add_line(
		const std::string &line
	);

#line 111 "start.x"

	
#line 118 "start.x"

	private:
		std::vector<int> code;

#line 126 "start.x"

	public:
		int code_size() const {
			return static_cast<int>(
				code.size()
			);
		}

#line 138 "start.x"

	public:
		int get_code(int pos) const {
			return code[pos];
		}

#line 112 "start.x"


#line 285 "start.x"

	State() { setup_symbols(); }

#line 50 "start.x"

	};
	
#line 78 "start.x"

	
#line 30 "start.x"

	#include <cassert>

#line 209 "start.x"

	#include <cctype>

#line 236 "start.x"

	#include <map>

#line 79 "start.x"

	void State::add_line(
		const std::string &line
	) {
		
#line 352 "start.x"

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
			items.emplace_back(new Type_Item { name });
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
			while (cur < end && ispunct(*cur) && *cur != '$' && *cur != '#' && *cur != '@' && *cur != '_' && *cur != '%') {
				++cur;
			}
			items.emplace_back(new Named_Item { { begin, cur } });
		}
	}
	
#line 778 "start.x"

restart:
	#if 0
		std::cerr << "LINE {";
		for (const auto &i : items) {
			std::cerr << '[' << *i << "], ";
		}
		std::cerr << "}\n";
	#endif
	if (items.size()) {
		auto macro { _macros.begin() };
		while (macro != _macros.end()) {
			unsigned i = 0;
			while (i + macro->pattern().size() <= items.size()) {
				
#line 809 "start.x"

	if (i + 2 < items.size()) {
		auto t { dynamic_cast<Type_Item *>(&*items[i]) };
		auto c { dynamic_cast<Named_Item *>(&*items[i + 1]) };
		if (t && c && c->name() == ":") {
			auto v { dynamic_cast<Type_Instance_Item *>(&*items[i + 2]) };
			if (v && v->type() == "num") {
				std::string type { t->type() };
				int value { v->value() };
				items.erase(items.begin() + i, items.begin() + i + 3);
				items.emplace(items.begin() + i, new Type_Instance_Item { type, value });
				goto restart;
			}
		}
		auto o { dynamic_cast<Type_Instance_Item *>(&*items[i]) };
		if (o && c && c->name() == ":") {
			auto n { dynamic_cast<Named_Item *>(&*items[i + 2]) };
			if (n && n->name() == "value") {
				int value { o->value() };
				items.erase(items.begin() + i, items.begin() + i + 3);
				items.emplace(items.begin() + i, new Type_Instance_Item { "num", value });
				goto restart;
			}
		}
	}

#line 838 "start.x"
 {
	bool matches { true };
	auto p { macro->pattern().begin() };
	auto e { macro->pattern().end() };
	for (int j = 0; matches && p != e; ++p, ++j) {
		matches = matches && (i + j < items.size());
		matches = matches && (**p).matches(*items[i + j]);
	}
	if (matches) {
		auto k { i };
		i += macro->pattern().size();
		for (const auto &e : macro->replacement()) {
			auto para { dynamic_cast<Type_Item *>(&*e) };
			if (para && ! para->type().empty()) {
				if (isdigit(para->type()[0])) {
					int idx { std::stoi(para->type()) };
					if (idx >= 0 && idx < (int) macro->pattern().size()) {
						items.emplace(
							items.begin() + i, items[k + idx]->clone()
						);
						++i;
						continue;
					}
				} else if (para->type() == "arithmetic") {
					auto n1 { dynamic_cast<Type_Instance_Item *>(&*items[k + 0]) };
					auto op { dynamic_cast<Named_Item *>(&*items[k + 1]) };
					auto n2 { dynamic_cast<Type_Instance_Item *>(&*items[k + 2]) };
					if (n1 && n1->type() == "num" && op && n2 && n2->type() == "num") {
						if (op->name() == "+") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() + n2->value() }
							);
							++i;
							continue;
						} else if (op->name() == "-") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() - n2->value() }
							);
							++i;
							continue;
						} else if (op->name() == "*") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() * n2->value() }
							);
							++i;
							continue;
						} else if (op->name() == "/" && n2->value() != 0) {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() / n2->value() }
							);
							++i;
							continue;
						} else if (op->name() == "and") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() & n2->value() }
							);
							++i;
							continue;
						} else if (op->name() == "or") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() | n2->value() }
							);
							++i;
							continue;
						} else if (op->name() == "<<") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() << n2->value() }
							);
							++i;
							continue;
						} else if (op->name() == ">>") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() >> n2->value() }
							);
							++i;
							continue;
						} else if (op->name() == "xor") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() ^ n2->value() }
							);
							++i;
							continue;
						}
					}
				}
			}
			items.emplace(
				items.begin() + i, e->clone()
			);
			++i;
		}
		items.erase(items.begin() + k, items.begin() + k + macro->pattern().size());
		goto restart;
	}
} 
#line 936 "start.x"
 {
	auto *ni {
		dynamic_cast<Named_Item *>(
			&*items[i]
	) };
	if (ni) {
		
#line 948 "start.x"

	if (ni->name() == "raw") {
		
#line 956 "start.x"

	if (i + 1 < items.size()) {
		auto *n2 {
			dynamic_cast<Type_Instance_Item *>(
				&*items[i + 1]
		) };
		if (n2 && n2->type() == "num") {
			
#line 970 "start.x"

	int value { n2->value() };
	items.erase( items.begin() + i,
		items.begin() + i + 2
	);
	items.emplace(items.begin() + i,
		new Type_Instance_Item { "raw", value }
	);
	goto restart;

#line 963 "start.x"
;
		}
	}

#line 950 "start.x"
;
	}

#line 983 "start.x"

	if (ni->name() == "*") {
		
#line 991 "start.x"

	items.erase(items.begin() + i,
		items.begin() + i + 1);
	int addr = code.size() * 4 +
		0x20010000;
	items.emplace(items.begin() + i,
		new Type_Instance_Item { "num", addr }
	);
	goto restart;

#line 985 "start.x"
;
	}

#line 1331 "start.x"

	if (ni->name() == "goto") {
		if (i + 1 < items.size()) {
			
#line 1341 "start.x"

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

#line 1334 "start.x"
;
		}
	}

#line 1387 "start.x"

	if (i + 1 < items.size()) {
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

#line 942 "start.x"
;
	}
} 
#line 1004 "start.x"
 {
	auto *ri {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i]
	) };
	if (ri && ri->type() == "reg") {
		int rd { ri->value() };
		
#line 1017 "start.x"

	if (i + 2 < items.size()) {
		auto *t2 {
			dynamic_cast<Named_Item *>(
				&*items[i + 1]
		) };
		if (t2 && t2->name() == "<-") {
			
#line 1031 "start.x"
 {
	auto *n3 {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i + 2]
	) };
	if (n3 && n3->type() == "num") {
		
#line 1044 "start.x"

	int v { n3->value() };
	items.erase(items.begin() + i,
		items.begin() + i + 3
	);

#line 1053 "start.x"
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
#line 1066 "start.x"
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
#line 1037 "start.x"
;
		i = 0; continue;
	}
} 
#line 1082 "start.x"
 {
	auto c3 {
		dynamic_cast<Type_Instance_Item *>(
			&*items[i + 2]
	) };
	if (c3 && c3->type() == "csr") {
		
#line 1094 "start.x"

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

#line 1088 "start.x"
;
	}
} 
#line 1024 "start.x"
;
		}
	}

#line 1011 "start.x"
;
	}
} 
#line 1109 "start.x"
 {
	auto pi {
		dynamic_cast<Named_Item *>(
			&*items[i]
		)
	};
	if (pi && pi->name() == "%pc") {
		
#line 1122 "start.x"

	if (i + 2 < items.size()) {
		auto n2 {
			dynamic_cast<Named_Item *>(
				&*items[i + 1]
			)
		};
		if (n2 && n2->name() == "<-") {
			
#line 1137 "start.x"

	auto p3 {
		dynamic_cast<Named_Item *>(
			&*items[i + 2]
		)
	};
	if (p3 && p3->name() == "%pc") {
		
#line 1150 "start.x"

	if (i + 4 < items.size()) {
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
			
#line 1203 "start.x"

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

#line 1164 "start.x"
;
		}
	}

#line 1226 "start.x"

	if (items.begin() + i + 3 == items.end()) {
		items.emplace(items.begin() + i + 3,
			new Named_Item { "+" }
		);
		items.emplace(items.begin() + i + 4,
			new Type_Instance_Item { "num", 0 }
		);
		goto restart;
	}

#line 1144 "start.x"
;
	}

#line 1130 "start.x"
;
		}
	}

#line 1116 "start.x"
;
	}
} 
#line 1240 "start.x"
 {
	auto ii {
		dynamic_cast<I_Type_Item *>(
			&*items[i]
		)
	};
	if (ii) {
		
#line 1253 "start.x"

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

#line 1247 "start.x"
;
	}
} 
#line 1271 "start.x"
 {
	auto ui {
		dynamic_cast<U_Type_Item *>(
			&*items[i]
		)
	};
	if (ui) {
		
#line 1285 "start.x"

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

#line 1278 "start.x"
;
		i = 0; continue;
	}
} 
#line 1300 "start.x"
 {
	auto ji {
		dynamic_cast<J_Type_Item *>(
			&*items[i]
		)
	};
	if (ji) {
		
#line 1313 "start.x"

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

#line 1307 "start.x"
;
	}
} 
#line 792 "start.x"
;
				++i;
			}
			++macro;
		}
		
#line 1437 "start.x"

	for (unsigned i = 1; i < items.size(); ++i) {
		auto a { dynamic_cast<Named_Item *>(&*items[i]) };
		if (a && a->name() == "=") {
			
#line 1448 "start.x"

	Items value;
	for (unsigned j = i + 1;
		j < items.size(); ++j
	) {
		value.push_back(std::move(items[j]));
	}
	Items p;
	for (unsigned j = 0; j < i; ++j) {
		p.push_back(std::move(items[j]));
	}
	_macros.emplace_back(std::move(p), std::move(value));
	items.erase(
		items.begin(), items.end()
	);

#line 1441 "start.x"
;
		}
	}

#line 797 "start.x"
;
		
#line 1417 "start.x"

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

#line 798 "start.x"
;
	}
	if (! items.empty()) {
		std::cerr <<
			"cant expand fully [" <<
			line << "]\n";
	}

#line 406 "start.x"
;

#line 83 "start.x"
;
	}

#line 102 "start.x"

	void State::add_machine(int instr) {
		
#line 148 "start.x"

	code.push_back(instr);

#line 104 "start.x"

	}

#line 277 "start.x"

	void State::setup_symbols() {
		
#line 291 "start.x"

	#include "default.h"
	std::istringstream in { setup };
	std::string l;
	while (std::getline(in, l)) {
		if (l.empty()) { continue; }
		add_line(l);
	}

#line 279 "start.x"
;
	}

#line 52 "start.x"
;

#line 155 "start.x"

	void assert_line(const char *line, int expected) {
		std::cerr << "ignoring " << line << "\n";
	}
	void assert_line_2(
		const char *line,
		int expected
	) {
		
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

	assert_line_2(
		"%x4 <- %x2 + %x3",
		0x00310233
	);

#line 411 "start.x"

	assert_line_2(
		"%pc <- %pc",
		0x0000006f
	);

#line 422 "start.x"

	assert_line_2(
		"%pc <- %pc - 28",
		0xfe5ff06f
	);

#line 432 "start.x"

	assert_line_2(
		"%pc <- %pc - 32",
		0xfe1ff06f
	);

#line 460 "start.x"

	assert_line_2(
		"%x5 <- %x5 and $ff",
		0x0ff2f293
	);

#line 469 "start.x"

	assert_line_2(
		"%x5 <- %x5 or $1",
		0x0012e293
	);

#line 478 "start.x"

	assert_line_2(
		"%x6 <- %x6 or $1",
		0x00136313
	);

#line 487 "start.x"

	assert_line_2(
		"%x11 <- $0d",
		0x00d00593
	);

#line 496 "start.x"

	assert_line_2(
		"%x12 <- $0a",
		0x00a00613
	);

#line 505 "start.x"

	assert_line_2(
		"%x10 <- $1013000",
		0x1013537
	);

#line 514 "start.x"

	assert_line(
		"%x5 <- %pc",
		0x00000297
	);

#line 523 "start.x"

	assert_line(
		"%mtvec <- %x5",
		0x30529073
	);

#line 532 "start.x"

	assert_line_2(
		"%x5 <- %mhartid",
		0xf14022f3
	);

#line 541 "start.x"

	assert_line(
		"if %x5 < 0: %pc <- %pc + -4",
		0xfe02cee3
	);

#line 550 "start.x"

	assert_line(
		"if %x5 = 0: %pc <- %pc + -12",
		0xfe028ae3
	);

#line 559 "start.x"

	assert_line(
		"if %x5 != %x11: %pc <- %pc + -28",
		0xfeb292e3
	);

#line 568 "start.x"

	assert_line(
		"if %x5 != 0: %pc <- %pc + 0",
		0x00029063
	);

#line 589 "start.x"

	assert_line(
		"%x6 <- [%x10]",
		0x00052303
	);

#line 598 "start.x"

	assert_line(
		"%x5 <- [%x10 + $04]",
		0x00452283
	);

#line 607 "start.x"

	assert_line(
		"[%x10] <- %x12",
		0x00c52023
	);

#line 616 "start.x"

	assert_line(
		"[%x10 + $08] <- %x5",
		0x00552423
	);

#line 172 "start.x"


#line 11 "start.x"

		
#line 442 "start.x"

	
#line 449 "start.x"

	State s;
	std::string l;
	while (std::getline(std::cin, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}

#line 443 "start.x"


#line 16 "hex.x"

	std::cout << s;

#line 12 "start.x"

	}
