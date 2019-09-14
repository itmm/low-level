
#line 6 "start.x"

	#include <iostream>
	
#line 24 "start.x"

	
#line 46 "start.x"

	
#line 61 "start.x"

	#include <string>
	#include <vector>
	#include <sstream>

#line 217 "start.x"

	#include <memory>

#line 242 "start.x"

	enum class Item_Type {
		t_type, t_instance, t_string
	};
	class Item {
		private:
			Item_Type _type;
			int _escapes;
			int _value;
			std::string _str;
		public:
			Item(Item_Type type, const std::string &str, int value, int escapes):
				_type { type }, 
				_escapes { escapes },
				_value { value },
				_str { str }
			{ }
			void write(std::ostream &out) const {
				switch (_type) {
					case Item_Type::t_type:
						out << '@' << _str;
						break;
					case Item_Type::t_instance:
						out << '@' << _str << ":$" << std::hex << _value << std::dec;
						break;
					case Item_Type::t_string:
						out << "@str:" << _str;
				}
			}
			virtual bool matches(const Item &in) const {
				switch (_type) {
					case Item_Type::t_type: {
						if (in._type == Item_Type::t_type && in._str == _str) {
							return true;
						}
						if (in._type == Item_Type::t_instance && in._str == _str) {
							return true;
						}
						if (in._type == Item_Type::t_string && _str == "str") {
							return true;
						}
						return false;
					}
					case Item_Type::t_instance: {
						return in._type == Item_Type::t_instance && in._str == _str && in._value == _value;
					}
					case Item_Type::t_string: {
						return in._type == Item_Type::t_string && in._str == _str;
					}
				}
				return false;
			}
			int escapes() const { return _escapes; }
			Item_Type type() const { return _type; }
			const std::string &str() const { return _str; }
			int value() const { return _value; }
	};
	std::ostream &operator<<(std::ostream &out, const Item &item) {
		item.write(out);
		return out;
	}

#line 307 "start.x"

	using Items = std::vector<Item>;
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

#line 332 "start.x"

	class Macros {
		private:
			Macros *_parent;
			using Container = std::vector<Macro>;
			Container _macros;
		public:
			Macros(Macros *parent): _parent { parent } {}
			
			class Iterator {
				private:
					Macros *_macros;
					Container::iterator _cur;
					void fix() {
						if (_macros && _cur == _macros->_macros.end()) {
							if (_macros->_parent) {
								_macros = _macros->_parent;
								_cur = _macros->_macros.begin();
							} else {
								_macros = nullptr;
							}
						}
					}
				public:
					Iterator(Macros *macros, Container::iterator cur):
						_macros { macros },
						_cur { cur }
					{ fix() ;}
					Macro &operator*() { return *_cur; }
					Macro *operator->() { return &*_cur; }
					const Macro &operator*() const { return *_cur; }
					const Macro *operator->() const { return &*_cur; }
					Iterator &operator++() {
						++_cur;
						fix();
						return *this;
					}
					bool operator!=(const Iterator &o) const {
						return o._macros != _macros || (_macros != nullptr && o._cur != _cur);
					}
			};

			Iterator begin() {
				return { this , _macros.begin() };
			}

			Iterator end() {
				return { nullptr, Container::iterator { } };
			}

			void emplace_back(Items &&pattern, Items &&replacement) {
				_macros.emplace_back(std::move(pattern), std::move(replacement));
			}
	};

#line 430 "start.x"

	int build_r_cmd(
		int funct7, char src2, char src1,
		int funct3, char dst, int opcode
	) {
		return (funct7 << 25) |
			(src2 << 20) | (src1 << 15) |
			(funct3 << 12) | (dst << 7) |
			opcode;
	}

#line 444 "start.x"

	int build_i_cmd(
		int imm, char src1, int funct3, char dst, int opcode
	) {
		return (imm << 20) | (src1 << 15) | (funct3 << 12) | (dst << 7) | opcode;
	}

#line 454 "start.x"

	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}

#line 467 "start.x"

	int build_add(
		char dst, char src1, int imm
	) {
		return build_i_cmd(
			imm, src1, 0x0, dst, 0x13
		);
	}

#line 717 "start.x"

	int build_load(
		char dst, char src, int imm
	) {
		return build_i_cmd(
			imm, src, 0x2, dst, 0x03
		);
	}

#line 47 "start.x"
;
	class State {
			
#line 96 "start.x"

	void add_machine(int instr);

#line 390 "start.x"

	Macros _macros;
	static Macros *setup_symbols();

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


#line 411 "start.x"

	State(): _macros { setup_symbols() } { }
	State(Macros *parent): _macros { parent } { }

#line 51 "start.x"

	};
	
#line 79 "start.x"

	
#line 31 "start.x"

	#include <cassert>

#line 207 "start.x"

	#include <cctype>

#line 236 "start.x"

	#include <map>

#line 80 "start.x"

	void State::add_line(
		const std::string &line
	) {
		
#line 479 "start.x"

	std::vector<Item> items;
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
			items.emplace_back(Item_Type::t_string, std::string { begin, cur }, 0, escapes);
		} else if (isdigit(*cur)) {
			int value { 0 };
			while (cur < end && isdigit(*cur)) {
				value = value * 10 + (*cur++ - '0');
			}
			items.emplace_back(Item_Type::t_instance, "num", value, escapes);
		} else if (*cur == '#') {
			break;
		} else if (*cur == '@') {
			++cur;
			std::string name;
			while (cur < end && isalnum(*cur)) {
				name += *cur++;
			}
			items.emplace_back(Item_Type::t_type, name, 0, escapes);
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
			items.emplace_back(Item_Type::t_instance, "num", value, escapes);
		} else if (ispunct(*cur)) {
			while (cur < end && ispunct(*cur) && *cur != '$' && *cur != '#' && *cur != '@' && *cur != '_' && *cur != '%' && *cur != '`') {
				++cur;
			}
			items.emplace_back(Item_Type::t_string, std::string { begin, cur }, 0, escapes);
		}
	}
	
#line 765 "start.x"

restart:
	#if 0
		std::cerr << "LINE {";
		for (const auto &i : items) {
			std::cerr << '[' << i << "], ";
		}
		std::cerr << "}\n";
	#endif
	if (items.size()) {
		auto macro { _macros.begin() };
		while (macro != _macros.end()) {
			unsigned i = 0;
			while (i + macro->pattern().size() <= items.size()) {
				
#line 800 "start.x"

	if (i + 2 < items.size()) {
		const auto &t { items[i] };
		const auto &c { items[i + 1] };
		const auto &v { items[i + 2] };
		if (c.type() == Item_Type::t_string && c.str() == ":") {
			if (t.type() == Item_Type::t_type && v.type() == Item_Type::t_instance && v.str() == "num") {
				std::string type { t.str() };
				int value { v.value() };
				items.erase(items.begin() + i, items.begin() + i + 3);
				items.emplace(items.begin() + i, Item_Type::t_instance, type, value, 0);
				goto restart;
			}
			if (t.type() == Item_Type::t_instance && v.type() == Item_Type::t_string && v.str() == "value") {
				int value { t.value() };
				items.erase(items.begin() + i, items.begin() + i + 3);
				items.emplace(items.begin() + i, Item_Type::t_instance, "num", value, 0);
				goto restart;
			}
		}
	}

#line 825 "start.x"
 {
	bool matches { true };
	auto p { macro->pattern().begin() };
	auto e { macro->pattern().end() };
	for (int j = 0; matches && p != e; ++p, ++j) {
		matches = matches && (i + j < items.size());
		matches = matches && p->matches(items[i + j]);
		matches = matches && p->escapes() <= 0;
	}
	if (matches) {
		auto k { i };
		i += macro->pattern().size();
		for (const auto &e : macro->replacement()) {
			if (e.type() == Item_Type::t_type && ! e.str().empty()) {
				if (isdigit(e.str()[0])) {
					int idx { std::stoi(e.str()) };
					if (idx >= 0 && idx < (int) macro->pattern().size()) {
						items.insert(
							items.begin() + i, items[k + idx]
						);
						++i;
						continue;
					}
				} else if (e.str() == "arithmetic") {
					const auto &n1 { items[k + 0] };
					const auto &op { items[k + 1] };
					const auto &n2 { items[k + 2] };
					if (n1.type() == Item_Type::t_instance && n1.str() == "num" && op.type() == Item_Type::t_string && n2.type() == Item_Type::t_instance && n2.str() == "num") {
						if (op.str() == "+") {
							items.emplace(
								items.begin() + i, Item_Type::t_instance, "num", n1.value() + n2.value(), 0
							);
							++i;
							continue;
						} else if (op.str() == "-") {
							items.emplace(
								items.begin() + i, Item_Type::t_instance, "num", n1.value() - n2.value(), 0
							);
							++i;
							continue;
						} else if (op.str() == "*") {
							items.emplace(
								items.begin() + i, Item_Type::t_instance, "num", n1.value() * n2.value(), 0
							);
							++i;
							continue;
						} else if (op.str() == "/" && n2.value() != 0) {
							items.emplace(
								items.begin() + i, Item_Type::t_instance, "num", n1.value() / n2.value(), 0
							);
							++i;
							continue;
						} else if (op.str() == "and") {
							items.emplace(
								items.begin() + i, Item_Type::t_instance, "num", n1.value() & n2.value(), 0
							);
							++i;
							continue;
						} else if (op.str() == "or") {
							items.emplace(
								items.begin() + i, Item_Type::t_instance, "num", n1.value() | n2.value(), 0
							);
							++i;
							continue;
						} else if (op.str() == "<<") {
							items.emplace(
								items.begin() + i, Item_Type::t_instance, "num", n1.value() << n2.value(), 0
							);
							++i;
							continue;
						} else if (op.str() == ">>") {
							items.emplace(
								items.begin() + i, Item_Type::t_instance, "num", n1.value() >> n2.value(), 0
							);
							++i;
							continue;
						} else if (op.str() == "xor") {
							items.emplace(
								items.begin() + i, Item_Type::t_instance, "num", n1.value() ^ n2.value(), 0
							);
							++i;
							continue;
						}
					}
				}
			}
			items.emplace(
				items.begin() + i, e.type(), e.str(), e.value(), e.escapes() - 1
			);
			++i;
		}
		items.erase(items.begin() + k, items.begin() + k + macro->pattern().size());
		goto restart;
	}
} 
#line 923 "start.x"
 {
	const auto &ni { items[i] };
	if (ni.type() == Item_Type::t_string) {
		
#line 932 "start.x"

	if (ni.str() == "*" && ni.escapes() <= 0) {
		
#line 940 "start.x"

	items.erase(items.begin() + i,
		items.begin() + i + 1);
	int addr = code.size() * 4 +
		0x20010000;
	items.emplace(items.begin() + i,
		Item_Type::t_instance, "num", addr, 0
	);
	goto restart;

#line 934 "start.x"
;
	}

#line 926 "start.x"
;
	}
} 
#line 779 "start.x"
;
				++i;
			}
			++macro;
		}
		
#line 967 "start.x"

	for (unsigned i = 1; i < items.size(); ++i) {
		const auto &a { items[i] };
		if (a.type() == Item_Type::t_string && a.str() == "<=" && a.escapes() <= 0) {
			
#line 978 "start.x"

	Items value;
	unsigned last { items.size() };
	for (unsigned j = i + 1;
		j < last; ++j
	) {
		const auto &cur { items[j] };
		if (cur.type() == Item_Type::t_string && cur.str() == "." && cur.escapes() <= 0) {
			last = j; break;
		}
		value.push_back(items[j]);
	}
	Items p;
	for (unsigned j = 0; j < i; ++j) {
		p.push_back(items[j]);
	}
	_macros.emplace_back(std::move(p), std::move(value));
	if (last < items.size()) { ++last; }
	items.erase(
		items.begin(), items.begin() + last
	);

#line 971 "start.x"
;
		}
	}

#line 784 "start.x"
;
		
#line 953 "start.x"

	while (! items.empty()) {
		const auto &mi { *items.begin() };
		if (mi.type() != Item_Type::t_instance || mi.str() != "raw") { break; }
		add_machine(mi.value());
		items.erase(
			items.begin(),
			items.begin() + 1
		);
	}

#line 785 "start.x"
;
	}
	if (! items.empty()) {
		std::cerr <<
			"cant expand fully [" <<
			line << "]; got [";
		for (const auto &i: items) {
			std::cerr << i << ", ";
		}
		std::cerr << "\n";
	}

#line 538 "start.x"
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

#line 397 "start.x"

	Macros *State::setup_symbols() {
		static State s { nullptr };
		static bool initialized { false };
		if (! initialized) {
			
#line 418 "start.x"

	#include "default.h"
	std::istringstream in { setup };
	std::string l;
	while (std::getline(in, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}

#line 402 "start.x"
;
			initialized = true;
		}
		return &s._macros;
	}

#line 53 "start.x"
;

#line 156 "start.x"

	void assert_line(
		const char *line,
		int expected
	) {
		
#line 184 "start.x"

	State s;
	s.add_line(line);
	assert(s.code_size() == 1);
	// std::cerr << "EXP " << std::hex << expected << "\n";
	// std::cerr << "GOT " << s.get_code(0) << std::dec << "\n";
	assert(s.get_code(0) == expected);

#line 161 "start.x"
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
		
#line 169 "start.x"

	
#line 176 "start.x"

	assert_line(
		"raw $87654321", 0x87654321
	);

#line 224 "start.x"

	assert_line(
		"%x4 <- %x2 + %x3", 0x00310233
	);
	assert_line(
		"%a0 <- %a1 - %a2", 0x40c58533
	);

#line 543 "start.x"

	assert_line(
		"%pc <- %pc", 0x0000006f
	);

#line 553 "start.x"

	assert_line(
		"%pc <- %pc - 28", 0xfe5ff06f
	);

#line 562 "start.x"

	assert_line(
		"%pc <- %pc - 32", 0xfe1ff06f
	);

#line 589 "start.x"

	assert_line(
		"%x5 <- %x5 and $ff", 0x0ff2f293
	);
	assert_line(
		"%a0 <- %a1 and %a2", 0x00c5f533
	);

#line 600 "start.x"

	assert_line(
		"%x5 <- %x5 or $1", 0x0012e293
	);
	assert_line(
		"%a0 <- %a1 or %a2", 0x00c5e533
	);

#line 611 "start.x"

	assert_line(
		"%x6 <- %x6 or $1", 0x00136313
	);

#line 619 "start.x"

	assert_line(
		"%a0 <- %a1 xor $ff", 0x0ff5c513
	);
	assert_line(
		"%a0 <- %a1 xor %a2", 0x00c5c533
	);
	assert_line(
		"%a0 <- complement %a1", 0x0005c513
	);

#line 633 "start.x"

	assert_line(
		"%x11 <- $0d", 0x00d00593
	);

#line 641 "start.x"

	assert_line(
		"%x12 <- $0a", 0x00a00613
	);

#line 649 "start.x"

	assert_line(
		"%x10 <- $1013000", 0x1013537
	);

#line 657 "start.x"

	assert_line(
		"%x5 <- %pc", 0x00000297
	);

#line 665 "start.x"

	assert_line(
		"%mtvec <- %x5", 0x30529073
	);

#line 673 "start.x"

	assert_line(
		"%x5 <- %mhartid", 0xf14022f3
	);

#line 681 "start.x"

	assert_line(
		"if %x5 < 0: %pc <- %pc - 4",
		0xfe02cee3
	);

#line 690 "start.x"

	assert_line(
		"if %x5 = 0: %pc <- %pc - 12",
		0xfe028ae3
	);

#line 699 "start.x"

	assert_line(
		"if %x5 != %x11: %pc <- %pc - 28",
		0xfeb292e3
	);

#line 708 "start.x"

	assert_line(
		"if %x5 != 0: %pc <- %pc + 0",
		0x00029063
	);

#line 729 "start.x"

	assert_line(
		"%x6 <- [%x10]",
		0x00052303
	);

#line 738 "start.x"

	assert_line(
		"%x5 <- [%x10 + $04]",
		0x00452283
	);

#line 747 "start.x"

	assert_line(
		"[%x10] <- %x12",
		0x00c52023
	);

#line 756 "start.x"

	assert_line(
		"[%x10 + $08] <- %x5",
		0x00552423
	);

#line 170 "start.x"


#line 12 "start.x"

		
#line 571 "start.x"

	
#line 578 "start.x"

	State s;
	std::string l;
	while (std::getline(std::cin, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}

#line 572 "start.x"


#line 16 "hex.x"

	std::cout << s;

#line 13 "start.x"

	}
