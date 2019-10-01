
#line 5 "start.x"

	#include <iostream>
	
#line 23 "start.x"

	
#line 45 "start.x"

	
#line 60 "start.x"

	#include <string>
	#include <vector>
	#include <sstream>

#line 5 "item.x"

	
#line 19 "item.x"

	enum class Item_Type {
		t_type, t_instance, t_string
	};

#line 6 "item.x"
;
	class Item {
		private:
			
#line 33 "item.x"

	Item_Type _type;

#line 40 "item.x"

	int _escapes;

#line 48 "item.x"

	int _value;
	std::string _str;

#line 9 "item.x"
;
		public:
			
#line 58 "item.x"

	Item(Item_Type type,
		const std::string &str,
		int value, int escapes
	):
		_type { type }, 
		_escapes { escapes },
		_value { value }, _str { str }
	{ }

#line 75 "item.x"

	bool matches(const Item &in) const {
		switch (_type) {
			
#line 87 "item.x"

	case Item_Type::t_type: {
		
#line 97 "item.x"

	if (in._type == Item_Type::t_type &&
		in._str == _str
	) {
		return true;
	}

#line 109 "item.x"

	if (in._type ==
			Item_Type::t_instance &&
		in._str == _str
	) {
		return true;
	}

#line 122 "item.x"

	if (in._type == Item_Type::t_string &&
		_str == "str"
	) {
		return true;
	}

#line 89 "item.x"
;
		return false;
	}

#line 134 "item.x"

	case Item_Type::t_instance: {
		return in._type ==
				Item_Type::t_instance &&
			in._str == _str &&
			in._value == _value;
	}

#line 148 "item.x"

	case Item_Type::t_string: {
		return in._type ==
				Item_Type::t_string &&
			in._str == _str;
	}

#line 78 "item.x"
;
		}
		return false;
	}

#line 160 "item.x"

	int escapes() const {
		return _escapes;
	}

#line 169 "item.x"

	Item_Type type() const {
		return _type;
	}

#line 178 "item.x"

	const std::string &str() const {
		return _str;
	}

#line 187 "item.x"

	int value() const { return _value; }

#line 11 "item.x"
;
	};

#line 197 "item.x"

	std::ostream &operator<<(
		std::ostream &out,
		const Item &item
	) {
		switch (item.type()) {
			
#line 212 "item.x"

	case Item_Type::t_type:
		out << '@' << item.str();
		break;

#line 221 "item.x"

	case Item_Type::t_instance:
		if (item.str() == "num") {
			out << '$' << std::hex <<
				item.value() << std::dec;
		} else if (item.str() == "reg") {
			out << "%x" << item.value();
		} else {
			out << '@' << item.str() <<
				":$" << std::hex <<
				item.value() << std::dec;
		}
		break;

#line 241 "item.x"

	case Item_Type::t_string:
		out << item.str();

#line 203 "item.x"
;
		}
		return out;
	}

#line 10 "macro.x"

	using Items = std::vector<Item>;

#line 17 "macro.x"

	class Macro {
		private:
			
#line 29 "macro.x"

	Items _pattern;
	Items _replacement;

#line 20 "macro.x"
;
		public:
			
#line 37 "macro.x"

	Macro(
		Items &&pattern, Items &&repl
	):
		_pattern { std::move(pattern) },
		_replacement { std::move(repl) }
	{ }

#line 49 "macro.x"

	const Items &pattern() const {
		return _pattern;
	}

#line 58 "macro.x"

	const Items &replacement() const {
		return _replacement;
	}

#line 22 "macro.x"
;
	};

#line 10 "macros.x"

	class Macros {
		private:
			
#line 22 "macros.x"

	Macros *_parent;

#line 29 "macros.x"

	using Container = std::vector<Macro>;
	Container _macros;

#line 13 "macros.x"
;
		public:
			
#line 37 "macros.x"

	Macros(Macros *parent):
		_parent { parent }
	{ }

#line 6 "iterator.x"

	class Iterator {
		private:
			
#line 18 "iterator.x"

	const Macros *_macros;
	Container::const_iterator _cur;

#line 27 "iterator.x"

	void fix() {
		
#line 37 "iterator.x"

	while (_macros &&
		_cur == _macros->_macros.end()
	) {
		if (_macros->_parent) {
			
#line 53 "iterator.x"

	_macros = _macros->_parent;
	_cur = _macros->_macros.begin();

#line 42 "iterator.x"
;
		} else {
			_macros = nullptr;
		}
	}

#line 29 "iterator.x"
;
	}

#line 9 "iterator.x"
;
		public:
			
#line 61 "iterator.x"

	Iterator(const Macros *macros,
		Container::const_iterator cur
	):
		_macros { macros },
		_cur { cur }
	{ fix() ;}

#line 74 "iterator.x"

	const Macro &operator*() const {
		return *_cur;
	}

#line 83 "iterator.x"

	const Macro *operator->() const {
		return &*_cur;
	}

#line 92 "iterator.x"

	Iterator &operator++() {
		++_cur;
		fix();
		return *this;
	}

#line 103 "iterator.x"

	bool operator!=(
		const Iterator &o
	) const {
		return o._macros != _macros || (
			_macros != nullptr &&
			o._cur != _cur
		);
	}

#line 11 "iterator.x"
;
	};

#line 55 "macros.x"

	Iterator begin() const {
		return {
			this, _macros.begin()
		};
	}

#line 66 "macros.x"

	Iterator end() const {
		return {
			nullptr,
			Container::iterator { }
		};
	}

#line 78 "macros.x"

	void emplace_back(
		Items &&pattern,
		Items &&replacement
	) {
		_macros.emplace_back(
			std::move(pattern),
			std::move(replacement)
		);
	}

#line 15 "macros.x"
;
	};

#line 4 "forward.x"

	class Forward {
		public:
			enum class Cmd_Style {
				i_type, s_type, b_type,
				u_type, j_type
			};
		private:
			Cmd_Style _style;
			int _position;
			std::string _name;
			bool _relative;
		public:
			Forward(Cmd_Style style, int position, const std::string &name, bool relative):
				_style { style }, _position { position }, _name { name }, _relative { relative }
			{ }
			Cmd_Style style() const { return _style; }
			int position() const { return _position; }
			std::string name() const { return _name; }
			bool relative() const { return _relative; }
	};
	class State;
	class Forwards {
		private:
			using Container = std::vector<Forward>;
			Container _forwards;
		public:
			void emplace_back(
				Forward::Cmd_Style style, int position, const std::string &name, bool relative
			) {
				_forwards.emplace_back(
					style, position, name, relative
				);
			}
			void fill(
				const Macros &macros,
				State &state
			);
	};

#line 46 "start.x"
;
	class State {
			
#line 95 "start.x"

	void add_machine(int instr);

#line 273 "start.x"

	Macros _macros;
	static Macros *setup_symbols();

#line 104 "forward.x"

	Forwards _forwards;

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
		int &mod_code(int pos) {
			return code[pos];
		}

#line 112 "start.x"


#line 294 "start.x"

	State(): _macros { setup_symbols() } { }
	State(Macros *parent): _macros { parent } { }

#line 124 "forward.x"

	void fix_forwards() {
		_forwards.fill(_macros, *this);
	}

#line 50 "start.x"

	};
	
#line 78 "start.x"

	
#line 30 "start.x"

	#include <cassert>

#line 244 "start.x"

	#include <cctype>

#line 263 "start.x"

	#include <map>

#line 79 "start.x"

	void State::add_line(
		const std::string &line
	) {
		
#line 313 "start.x"

	std::vector<Item> items;
	auto end { line.end() };
	auto cur { line.begin() };
	bool log { false };
	if (cur < end && *cur == '!') {
		log = true;
		++cur;
	}
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
	items.emplace_back(Item_Type::t_string, ";", 0, 0);
	
#line 599 "start.x"

restart:
	if (log) {
		std::cerr << "LINE {";
		for (const auto &i : items) {
			std::cerr << i << " ";
		}
		std::cerr << "}\n";
	}
	if (items.size()) {
		auto macro { _macros.begin() };
		while (macro != _macros.end()) {
			unsigned i = 0;
			while (i + macro->pattern().size() <= items.size()) {
				
#line 634 "start.x"

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

#line 659 "start.x"
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
				
#line 110 "forward.x"

	if (e.str() == "fwdgoto") {
		const auto &label { items[k + 1] };
		if (label.type() == Item_Type::t_string) {
			_forwards.emplace_back(
				Forward::Cmd_Style::j_type, code_size(), label.str(), true
			);
			continue;
		}
	}

#line 744 "start.x"
;
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
#line 758 "start.x"
 {
	const auto &ni { items[i] };
	if (ni.type() == Item_Type::t_string) {
		
#line 767 "start.x"

	if (ni.str() == "*" && ni.escapes() <= 0) {
		
#line 775 "start.x"

	items.erase(items.begin() + i,
		items.begin() + i + 1);
	int addr = code.size() * 4 +
		0x20010000;
	items.emplace(items.begin() + i,
		Item_Type::t_instance, "num", addr, 0
	);
	goto restart;

#line 769 "start.x"
;
	}

#line 761 "start.x"
;
	}
} 
#line 613 "start.x"
;
				++i;
			}
			++macro;
		}
		
#line 809 "start.x"

	for (unsigned i = 1; i < items.size(); ++i) {
		const auto &a { items[i] };
		if (a.type() == Item_Type::t_string && a.str() == "<==" && a.escapes() <= 0) {
			
#line 820 "start.x"

	Items value;
	unsigned last { items.size() - 1 }; // skip last ;
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

#line 813 "start.x"
;
		}
	}

#line 618 "start.x"
;
		
#line 788 "start.x"

	while (! items.empty()) {
		const auto &mi { *items.begin() };
		if (mi.type() == Item_Type::t_string && mi.str() == ";") {
			items.erase(
				items.begin(),
				items.begin() + 1
			);
			continue;
		}
		if (mi.type() != Item_Type::t_instance || mi.str() != "raw") { break; }
		add_machine(mi.value());
		items.erase(
			items.begin(),
			items.begin() + 1
		);
	}

#line 619 "start.x"
;
	}
	if (! items.empty()) {
		std::cerr <<
			"cant expand fully [" <<
			line << "]; got [";
		for (const auto &i: items) {
			std::cerr << i << " ";
		}
		std::cerr << "]\n";
	}

#line 378 "start.x"
;

#line 83 "start.x"
;
	}

#line 102 "start.x"

	void State::add_machine(int instr) {
		
#line 151 "start.x"

	code.push_back(instr);

#line 104 "start.x"

	}

#line 280 "start.x"

	Macros *State::setup_symbols() {
		static State s { nullptr };
		static bool initialized { false };
		if (! initialized) {
			
#line 301 "start.x"

	#include "default.h"
	std::istringstream in { setup };
	std::string l;
	while (std::getline(in, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}

#line 285 "start.x"
;
			initialized = true;
		}
		return &s._macros;
	}

#line 52 "start.x"
;

#line 158 "start.x"

	void assert_line(
		const char *line,
		int expected
	) {
		
#line 198 "start.x"

	State s;
	s.add_line(line);
	if (s.code_size() != 1) {
		std::cerr << "assert failed in [" << line << "]\n";
		std::cerr << "got " << s.code_size() << " elements\n";
	}
	assert(s.code_size() == 1);
	if (s.get_code(0) != expected) {
		std::cerr << "assert failed in [" << line << "]\n";
		std::cerr << "EXP " << std::hex << expected << "\n";
		std::cerr << "GOT " << s.get_code(0) << std::dec << "\n";
	}
	assert(s.get_code(0) == expected);

#line 163 "start.x"
;
	}

#line 218 "start.x"

	void assert_line_2(const char *line, int exp1, int exp2) {
		State s;
		s.add_line(line);
		assert(s.code_size() == 2);
		if (s.get_code(0) != exp1 || s.get_code(1) != exp2) {
	 		std::cerr << "EXP " << std::hex << exp1 << ", " << exp2 << "\n";
			std::cerr << "GOT " << s.get_code(0) << ", " << s.get_code(1) << std::dec << "\n";
	 	}
		assert(s.get_code(0) == exp1);
		assert(s.get_code(1) == exp2);
	}

#line 47 "forward.x"

	void Forwards::fill(
		const Macros &macros,
		State &state
	) {
		for (const auto &f : _forwards) {
			bool found { false };
			int value;
			for (const auto &m : macros) {
				if (m.pattern().size() == 1) {
					const auto &p { *m.pattern().begin() };
					if (p.type() == Item_Type::t_string && p.str() == f.name()) {
						if (m.replacement().size() == 1) {
							const auto &r { *m.replacement().begin() };
							if (r.type() == Item_Type::t_instance && r.str() == "num") {
								found = true;
								value = r.value();
								break;
							}
						}
					}
				}
			}
			if (! found) {
				std::cerr << "can't expand " << f.name() << '\n';
				continue;
			}

			if (f.relative()) {
				value = value - 4 * f.position() - 0x20010000;
			}

			switch (f.style()) {
				case Forward::Cmd_Style::i_type:
					state.mod_code(f.position()) |= value << 20;
					break;
				case Forward::Cmd_Style::s_type:
					state.mod_code(f.position()) |= ((value & 0xfe0) << (25 - 5)) | ((value & 0x1f) << 7);
					break;
				case Forward::Cmd_Style::b_type:
					state.mod_code(f.position()) |= ((value & 0x1000) << (31 - 12)) | ((value & 0x7e0) << (25 - 5)) | ((value & 0x1e) << (8 - 1)) | ((value & 0x800) >> (11 - 7));
					break;
				case Forward::Cmd_Style::u_type:
					state.mod_code(f.position()) |= value & 0xfffff000;
					break;
				case Forward::Cmd_Style::j_type:
					state.mod_code(f.position()) |= ((value & 0x100000) << (31 - 20)) | ((value & 0x7fe) << (21 - 1)) | ((value & 0x800) << (20 - 11)) | (value & 0xff000);
					break;
				default:
					std::cerr << "unimplemented mode\n";
			}
		}
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

	assert_line(
		"raw $87654321", 0x87654321
	);
	assert_line(
		"raw (1000 - 200)", 800
	);
	assert_line(
		"raw (200 - 1000)", -800
	);
	assert_line(
		"raw ($20010020 - $20010010)", 0x10
	);
	assert_line(
		"raw ($20010010 - $20010020)", -0x10
	);

#line 251 "start.x"

	assert_line(
		"%x4 <- %x2 + %x3", 0x00310233
	);
	assert_line(
		"%a0 <- %a1 - %a2", 0x40c58533
	);

#line 383 "start.x"

	assert_line(
		"%pc <- %pc", 0x0000006f
	);

#line 393 "start.x"

	assert_line(
		"%pc <- %pc - 28", 0xfe5ff06f
	);

#line 402 "start.x"

	assert_line(
		"%pc <- %pc - 32", 0xfe1ff06f
	);

#line 429 "start.x"

	assert_line(
		"%x5 <- %x5 and $ff", 0x0ff2f293
	);
	assert_line(
		"%a0 <- %a1 and %a2", 0x00c5f533
	);

#line 440 "start.x"

	assert_line(
		"%x5 <- %x5 or $1", 0x0012e293
	);
	assert_line(
		"%a0 <- %a1 or %a2", 0x00c5e533
	);

#line 451 "start.x"

	assert_line(
		"%x6 <- %x6 or $1", 0x00136313
	);

#line 459 "start.x"

	assert_line(
		"%a0 <- %a1 xor $ff", 0x0ff5c513
	);
	assert_line(
		"%a0 <- %a1 xor %a2", 0x00c5c533
	);
	assert_line(
		"%a0 <- complement %a1", 0x0005c513
	);

#line 473 "start.x"

	assert_line(
		"%x11 <- $0d", 0x00d00593
	);

#line 481 "start.x"

	assert_line(
		"%x12 <- $0a", 0x00a00613
	);

#line 489 "start.x"

	assert_line(
		"%x10 <- $1013000", 0x1013537
	);

#line 497 "start.x"

	assert_line(
		"%x5 <- %pc", 0x00000297
	);
	assert_line_2(
		"%x5 <- %pc + $20", 0x00000297, 0x02028293
	);
	assert_line_2(
		"%x5 <- %pc - $20", 0x00000297, 0xfe028293
	);

#line 511 "start.x"

	assert_line(
		"%mtvec <- %x5", 0x30529073
	);

#line 519 "start.x"

	assert_line(
		"%x5 <- %mhartid", 0xf14022f3
	);

#line 527 "start.x"

	assert_line(
		"if %x5 < 0: %pc <- %pc - 4",
		0xfe02cee3
	);

#line 536 "start.x"

	assert_line(
		"if %x5 = 0: %pc <- %pc - 12",
		0xfe028ae3
	);

#line 545 "start.x"

	assert_line(
		"if %x5 != %x11: %pc <- %pc - 28",
		0xfeb292e3
	);

#line 554 "start.x"

	assert_line(
		"if %x5 != 0: %pc <- %pc + 0",
		0x00029063
	);

#line 563 "start.x"

	assert_line(
		"%x6 <- [%x10]",
		0x00052303
	);

#line 572 "start.x"

	assert_line(
		"%x5 <- [%x10 + $04]",
		0x00452283
	);

#line 581 "start.x"

	assert_line(
		"[%x10] <- %x12",
		0x00c52023
	);

#line 590 "start.x"

	assert_line(
		"[%x10 + $08] <- %x5",
		0x00552423
	);

#line 172 "start.x"


#line 11 "start.x"

		
#line 411 "start.x"

	
#line 418 "start.x"

	State s;
	std::string l;
	while (std::getline(std::cin, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}

#line 412 "start.x"


#line 132 "forward.x"

	s.fix_forwards();

#line 16 "hex.x"

	std::cout << s;

#line 12 "start.x"

	}
