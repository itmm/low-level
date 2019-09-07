
#line 6 "start.x"

	#include <iostream>
	
#line 24 "start.x"

	
#line 46 "start.x"

	
#line 61 "start.x"

	#include <string>
	#include <vector>
	#include <sstream>

#line 220 "start.x"

	#include <memory>

#line 243 "start.x"

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

#line 304 "start.x"

	int build_r_cmd(
		int funct7, char src2, char src1,
		int funct3, char dst, int opcode
	) {
		return (funct7 << 25) |
			(src2 << 20) | (src1 << 15) |
			(funct3 << 12) | (dst << 7) |
			opcode;
	}

#line 318 "start.x"

	int build_i_cmd(
		int imm, char src1, int funct3, char dst, int opcode
	) {
		return (imm << 20) | (src1 << 15) | (funct3 << 12) | (dst << 7) | opcode;
	}

#line 328 "start.x"

	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}

#line 341 "start.x"

	int build_add(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x0, dst, 0x13
		);
	}

#line 583 "start.x"

	int build_load(
		char dst, char src, int imm
	) {
		return build_i_cmd(
			imm, src, 0x2, dst, 0x03
		);
	}

#line 631 "start.x"

	class Item {
		private:
			const int _escapes;
		protected:
			Item(int escapes): _escapes { escapes } { }
		public:
			virtual ~Item() {};
			virtual Item *clone(int delta_escape) const = 0;
			virtual void write(std::ostream &out) const = 0;
			virtual bool matches(const Item &in) const {
				return false;
			}
			int escapes() const { return _escapes; }
	};
	std::ostream &operator<<(std::ostream &out, const Item &item) {
		item.write(out);
		return out;
	}

#line 654 "start.x"

	class Named_Item: public Item {
		private:
			std::string _name;
		public:
			Named_Item(const std::string &name, int escapes): Item { escapes }, _name { name } { }
			const std::string &name() const { return _name; }
			Item *clone(int delta_escape) const override {
				return new Named_Item { _name, escapes() + delta_escape };
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
			Type_Instance_Item(const std::string &type, int value, int escapes):
				Item { escapes }, _type { type }, _value { value }
			{ }
			const std::string &type() const { return _type; }
			int value() const { return _value; }
			Item *clone(int delta_escape) const override {
				return new Type_Instance_Item { _type, _value, escapes() + delta_escape };
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
			Type_Item(const std::string &type, int escapes): Item { escapes },  _type { type } { }
			const std::string &type() const { return _type; }
			Item *clone(int delta_escape) const override {
				return new Type_Item { _type, escapes() + delta_escape };
			}
			void write(std::ostream &out) const override {
				out << '@' << _type;
			}
			bool matches(const Item &in) const override {
				auto o { dynamic_cast<const Type_Item *>(&in) };
				if (o && o->type() == _type) { return true; };
				auto i { dynamic_cast<const Type_Instance_Item *>(&in) };
				if (i && i->type() == _type) { return true; }
				if (_type == "str") {
					auto s { dynamic_cast<const Named_Item *>(&in) };
					if (s) { return true; }
				}
				return false;
			};
	};

#line 47 "start.x"
;
	class State {
			
#line 96 "start.x"

	void add_machine(int instr);

#line 271 "start.x"

	std::vector<Macro> _macros;
	void setup_symbols();

#line 49 "start.x"

		public:
			
#line 70 "start.x"

	void add_line(
		const std::string &line
	);

#line 112 "start.x"

	
#line 119 "start.x"

	private:
		std::vector<int> code;

#line 127 "start.x"

	public:
		int code_size() const {
			return static_cast<int>(
				code.size()
			);
		}

#line 139 "start.x"

	public:
		int get_code(int pos) const {
			return code[pos];
		}

#line 113 "start.x"


#line 286 "start.x"

	State() { setup_symbols(); }

#line 51 "start.x"

	};
	
#line 79 "start.x"

	
#line 31 "start.x"

	#include <cassert>

#line 210 "start.x"

	#include <cctype>

#line 237 "start.x"

	#include <map>

#line 80 "start.x"

	void State::add_line(
		const std::string &line
	) {
		
#line 353 "start.x"

	std::vector<std::unique_ptr<Item>> items;
	auto end { line.end() };
	auto cur { line.begin() };
	for (;;) {
		while (cur < end && *cur <= ' ') {
			++cur;
		}
		if (cur == end) { break; }

		int escapes { 0 };
		while (cur < end && *cur == '`') {
			++escapes;
			++cur;
		}
		auto begin { cur };
		if (isalpha(*cur) || *cur == '_' || *cur == '%') {
			while (cur < end && (isalnum(*cur) || *cur == '_' || *cur == '%')) {
				++cur;
			}
			items.emplace_back(new Named_Item { { begin, cur }, escapes } );
		} else if (isdigit(*cur)) {
			int value { 0 };
			while (cur < end && isdigit(*cur)) {
				value = value * 10 + (*cur++ - '0');
			}
			items.emplace_back(new Type_Instance_Item { "num", value, escapes });
		} else if (*cur == '#') {
			break;
		} else if (*cur == '@') {
			++cur;
			std::string name;
			while (cur < end && isalnum(*cur)) {
				name += *cur++;
			}
			items.emplace_back(new Type_Item { name, escapes });
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
			items.emplace_back(new Type_Instance_Item { "num", value, escapes });
		} else if (ispunct(*cur)) {
			while (cur < end && ispunct(*cur) && *cur != '$' && *cur != '#' && *cur != '@' && *cur != '_' && *cur != '%' && *cur != '`') {
				++cur;
			}
			items.emplace_back(new Named_Item { { begin, cur }, escapes });
		}
	}
	
#line 721 "start.x"

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
				
#line 752 "start.x"

	if (i + 2 < items.size()) {
		auto t { dynamic_cast<Type_Item *>(&*items[i]) };
		auto c { dynamic_cast<Named_Item *>(&*items[i + 1]) };
		if (t && c && c->name() == ":") {
			auto v { dynamic_cast<Type_Instance_Item *>(&*items[i + 2]) };
			if (v && v->type() == "num") {
				std::string type { t->type() };
				int value { v->value() };
				items.erase(items.begin() + i, items.begin() + i + 3);
				items.emplace(items.begin() + i, new Type_Instance_Item { type, value, 0 });
				goto restart;
			}
		}
		auto o { dynamic_cast<Type_Instance_Item *>(&*items[i]) };
		if (o && c && c->name() == ":") {
			auto n { dynamic_cast<Named_Item *>(&*items[i + 2]) };
			if (n && n->name() == "value") {
				int value { o->value() };
				items.erase(items.begin() + i, items.begin() + i + 3);
				items.emplace(items.begin() + i, new Type_Instance_Item { "num", value, 0 });
				goto restart;
			}
		}
	}

#line 781 "start.x"
 {
	bool matches { true };
	auto p { macro->pattern().begin() };
	auto e { macro->pattern().end() };
	for (int j = 0; matches && p != e; ++p, ++j) {
		matches = matches && (i + j < items.size());
		matches = matches && (**p).matches(*items[i + j]);
		matches = matches && (*p)->escapes() <= 0;
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
							items.begin() + i, items[k + idx]->clone(0)
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
								items.begin() + i, new Type_Instance_Item { "num", n1->value() + n2->value(), 0 }
							);
							++i;
							continue;
						} else if (op->name() == "-") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() - n2->value(), 0 }
							);
							++i;
							continue;
						} else if (op->name() == "*") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() * n2->value(), 0 }
							);
							++i;
							continue;
						} else if (op->name() == "/" && n2->value() != 0) {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() / n2->value(), 0 }
							);
							++i;
							continue;
						} else if (op->name() == "and") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() & n2->value(), 0 }
							);
							++i;
							continue;
						} else if (op->name() == "or") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() | n2->value(), 0 }
							);
							++i;
							continue;
						} else if (op->name() == "<<") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() << n2->value(), 0 }
							);
							++i;
							continue;
						} else if (op->name() == ">>") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() >> n2->value(), 0 }
							);
							++i;
							continue;
						} else if (op->name() == "xor") {
							items.emplace(
								items.begin() + i, new Type_Instance_Item { "num", n1->value() ^ n2->value(), 0 }
							);
							++i;
							continue;
						}
					}
				}
			}
			items.emplace(
				items.begin() + i, e->clone(-1)
			);
			++i;
		}
		items.erase(items.begin() + k, items.begin() + k + macro->pattern().size());
		goto restart;
	}
} 
#line 880 "start.x"
 {
	auto *ni {
		dynamic_cast<Named_Item *>(
			&*items[i]
	) };
	if (ni) {
		
#line 892 "start.x"

	if (ni->name() == "*" && ni->escapes() <= 0) {
		
#line 900 "start.x"

	items.erase(items.begin() + i,
		items.begin() + i + 1);
	int addr = code.size() * 4 +
		0x20010000;
	items.emplace(items.begin() + i,
		new Type_Instance_Item { "num", addr, 0 }
	);
	goto restart;

#line 894 "start.x"
;
	}

#line 886 "start.x"
;
	}
} 
#line 735 "start.x"
;
				++i;
			}
			++macro;
		}
		
#line 933 "start.x"

	for (unsigned i = 1; i < items.size(); ++i) {
		auto a { dynamic_cast<Named_Item *>(&*items[i]) };
		if (a && a->name() == "=") {
			
#line 944 "start.x"

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

#line 937 "start.x"
;
		}
	}

#line 740 "start.x"
;
		
#line 913 "start.x"

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

#line 741 "start.x"
;
	}
	if (! items.empty()) {
		std::cerr <<
			"cant expand fully [" <<
			line << "]\n";
	}

#line 412 "start.x"
;

#line 84 "start.x"
;
	}

#line 103 "start.x"

	void State::add_machine(int instr) {
		
#line 149 "start.x"

	code.push_back(instr);

#line 105 "start.x"

	}

#line 278 "start.x"

	void State::setup_symbols() {
		
#line 292 "start.x"

	#include "default.h"
	std::istringstream in { setup };
	std::string l;
	while (std::getline(in, l)) {
		if (l.empty()) { continue; }
		add_line(l);
	}

#line 280 "start.x"
;
	}

#line 53 "start.x"
;

#line 156 "start.x"

	void assert_line(const char *line, int expected) {
		std::cerr << "ignoring " << line << "\n";
	}
	void assert_line_2(
		const char *line,
		int expected
	) {
		
#line 187 "start.x"

	State s;
	s.add_line(line);
	assert(s.code_size() == 1);
	// std::cerr << "EXP " << std::hex << expected << "\n";
	// std::cerr << "GOT " << s.get_code(0) << std::dec << "\n";
	assert(s.get_code(0) == expected);

#line 164 "start.x"
;
	}

#line 25 "start.x"


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

#line 8 "start.x"
;
	int main(
		int argc, const char *argv[]
	) {
		
#line 172 "start.x"

	
#line 179 "start.x"

	assert_line_2(
		"raw $87654321", 0x87654321
	);

#line 227 "start.x"

	assert_line_2(
		"%x4 <- %x2 + %x3",
		0x00310233
	);

#line 417 "start.x"

	assert_line_2(
		"%pc <- %pc",
		0x0000006f
	);

#line 428 "start.x"

	assert_line_2(
		"%pc <- %pc - 28",
		0xfe5ff06f
	);

#line 438 "start.x"

	assert_line_2(
		"%pc <- %pc - 32",
		0xfe1ff06f
	);

#line 466 "start.x"

	assert_line_2(
		"%x5 <- %x5 and $ff",
		0x0ff2f293
	);

#line 475 "start.x"

	assert_line_2(
		"%x5 <- %x5 or $1",
		0x0012e293
	);

#line 484 "start.x"

	assert_line_2(
		"%x6 <- %x6 or $1",
		0x00136313
	);

#line 493 "start.x"

	assert_line_2(
		"%x11 <- $0d",
		0x00d00593
	);

#line 502 "start.x"

	assert_line_2(
		"%x12 <- $0a",
		0x00a00613
	);

#line 511 "start.x"

	assert_line_2(
		"%x10 <- $1013000",
		0x1013537
	);

#line 520 "start.x"

	assert_line(
		"%x5 <- %pc",
		0x00000297
	);

#line 529 "start.x"

	assert_line(
		"%mtvec <- %x5",
		0x30529073
	);

#line 538 "start.x"

	assert_line_2(
		"%x5 <- %mhartid",
		0xf14022f3
	);

#line 547 "start.x"

	assert_line_2(
		"if %x5 < 0: %pc <- %pc - 4",
		0xfe02cee3
	);

#line 556 "start.x"

	assert_line_2(
		"if %x5 == 0: %pc <- %pc - 12",
		0xfe028ae3
	);

#line 565 "start.x"

	assert_line_2(
		"if %x5 != %x11: %pc <- %pc - 28",
		0xfeb292e3
	);

#line 574 "start.x"

	assert_line_2(
		"if %x5 != 0: %pc <- %pc + 0",
		0x00029063
	);

#line 595 "start.x"

	assert_line(
		"%x6 <- [%x10]",
		0x00052303
	);

#line 604 "start.x"

	assert_line(
		"%x5 <- [%x10 + $04]",
		0x00452283
	);

#line 613 "start.x"

	assert_line(
		"[%x10] <- %x12",
		0x00c52023
	);

#line 622 "start.x"

	assert_line(
		"[%x10 + $08] <- %x5",
		0x00552423
	);

#line 173 "start.x"


#line 12 "start.x"

		
#line 448 "start.x"

	
#line 455 "start.x"

	State s;
	std::string l;
	while (std::getline(std::cin, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}

#line 449 "start.x"


#line 16 "hex.x"

	std::cout << s;

#line 13 "start.x"

	}
