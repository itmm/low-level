
#line 5 "start.md"

	#include <iostream>
	
#line 23 "start.md"

	
#line 45 "start.md"

	
#line 60 "start.md"

	#include <string>
	#include <vector>
	#include <sstream>

#line 5 "item.md"

	
#line 19 "item.md"

	enum class Item_Type {
		t_type, t_instance, t_string
	};

#line 6 "item.md"
;
	class Item {
		private:
			
#line 33 "item.md"

	Item_Type _type;

#line 40 "item.md"

	int _escapes;

#line 48 "item.md"

	int _value;
	std::string _str;

#line 9 "item.md"
;
		public:
			
#line 58 "item.md"

	Item(Item_Type type,
		const std::string &str,
		int value, int escapes
	):
		_type { type }, 
		_escapes { escapes },
		_value { value }, _str { str }
	{ }

#line 75 "item.md"

	bool matches(const Item &in) const {
		switch (_type) {
			
#line 87 "item.md"

	case Item_Type::t_type: {
		
#line 97 "item.md"

	if (in._type == Item_Type::t_type &&
		in._str == _str
	) {
		return true;
	}

#line 109 "item.md"

	if (in._type ==
			Item_Type::t_instance &&
		in._str == _str
	) {
		return true;
	}

#line 122 "item.md"

	if (in._type == Item_Type::t_string &&
		_str == "str"
	) {
		return true;
	}

#line 89 "item.md"
;
		return false;
	}

#line 134 "item.md"

	case Item_Type::t_instance: {
		return in._type ==
				Item_Type::t_instance &&
			in._str == _str &&
			in._value == _value;
	}

#line 148 "item.md"

	case Item_Type::t_string: {
		return in._type ==
				Item_Type::t_string &&
			in._str == _str;
	}

#line 78 "item.md"
;
		}
		return false;
	}

#line 160 "item.md"

	int escapes() const {
		return _escapes;
	}

#line 169 "item.md"

	Item_Type type() const {
		return _type;
	}

#line 178 "item.md"

	const std::string &str() const {
		return _str;
	}

#line 187 "item.md"

	int value() const { return _value; }

#line 11 "item.md"
;
	};

#line 197 "item.md"

	std::ostream &operator<<(
		std::ostream &out,
		const Item &item
	) {
		switch (item.type()) {
			
#line 212 "item.md"

	case Item_Type::t_type:
		out << '@' << item.str();
		break;

#line 221 "item.md"

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

#line 241 "item.md"

	case Item_Type::t_string:
		out << item.str();

#line 203 "item.md"
;
		}
		return out;
	}

#line 10 "macro.md"

	using Items = std::vector<Item>;

#line 17 "macro.md"

	class Macro {
		private:
			
#line 29 "macro.md"

	Items _pattern;
	Items _replacement;

#line 20 "macro.md"
;
		public:
			
#line 37 "macro.md"

	Macro(
		Items &&pattern, Items &&repl
	):
		_pattern { std::move(pattern) },
		_replacement { std::move(repl) }
	{ }

#line 49 "macro.md"

	const Items &pattern() const {
		return _pattern;
	}

#line 58 "macro.md"

	const Items &replacement() const {
		return _replacement;
	}

#line 22 "macro.md"
;
	};

#line 10 "macros.md"

	class Macros {
		private:
			
#line 22 "macros.md"

	Macros *_parent;

#line 29 "macros.md"

	using Container = std::vector<Macro>;
	Container _macros;

#line 13 "macros.md"
;
		public:
			
#line 37 "macros.md"

	Macros(Macros *parent):
		_parent { parent }
	{ }

#line 6 "iterator.md"

	class Iterator {
		private:
			
#line 18 "iterator.md"

	const Macros *_macros;
	Container::const_iterator _cur;

#line 27 "iterator.md"

	void fix() {
		
#line 37 "iterator.md"

	while (_macros &&
		_cur == _macros->_macros.end()
	) {
		if (_macros->_parent) {
			
#line 53 "iterator.md"

	_macros = _macros->_parent;
	_cur = _macros->_macros.begin();

#line 42 "iterator.md"
;
		} else {
			_macros = nullptr;
		}
	}

#line 29 "iterator.md"
;
	}

#line 9 "iterator.md"
;
		public:
			
#line 61 "iterator.md"

	Iterator(const Macros *macros,
		Container::const_iterator cur
	):
		_macros { macros },
		_cur { cur }
	{ fix() ;}

#line 74 "iterator.md"

	const Macro &operator*() const {
		return *_cur;
	}

#line 83 "iterator.md"

	const Macro *operator->() const {
		return &*_cur;
	}

#line 92 "iterator.md"

	Iterator &operator++() {
		++_cur;
		fix();
		return *this;
	}

#line 103 "iterator.md"

	bool operator!=(
		const Iterator &o
	) const {
		return o._macros != _macros || (
			_macros != nullptr &&
			o._cur != _cur
		);
	}

#line 11 "iterator.md"
;
	};

#line 55 "macros.md"

	Iterator begin() const {
		return {
			this, _macros.begin()
		};
	}

#line 66 "macros.md"

	Iterator end() const {
		return {
			nullptr,
			Container::iterator { }
		};
	}

#line 78 "macros.md"

	void emplace_back(
		Items &&pattern,
		Items &&replacement
	) {
		_macros.emplace_back(
			std::move(pattern),
			std::move(replacement)
		);
	}

#line 15 "macros.md"
;
	};

#line 4 "forward.md"

	class Forward {
		public:
			enum class Cmd_Style {
				i_type, s_type, b_type,
				u_type, j_type
			};
		private:
			
#line 20 "forward.md"

	Cmd_Style _style;
	int _position;
	std::string _name;
	bool _relative;

#line 12 "forward.md"
;
		public:
			
#line 29 "forward.md"

	Forward(Cmd_Style style,int position,
		const std::string &name,
		bool relative
	):
		_style { style },
		_position { position },
		_name { name },
		_relative { relative }
	{ }

#line 43 "forward.md"

	Cmd_Style style() const {
		return _style;
	}
	int position() const {
		return _position;
	}
	std::string name() const {
		return _name;
	}
	bool relative() const {
		return _relative;
	}

#line 14 "forward.md"
;
	};

#line 60 "forward.md"

	class State;
	class Forwards {
		private:
			
#line 72 "forward.md"

	using Container =
		std::vector<Forward>;
	Container _forwards;

#line 64 "forward.md"
;
		public:
			
#line 80 "forward.md"

	void emplace_back(
		Forward::Cmd_Style style,
		int position,
		const std::string &name,
		bool relative
	) {
		_forwards.emplace_back(
			style, position,
			name, relative
		);
	}

#line 96 "forward.md"

	void fill(
		const Macros &macros,
		State &state
	);

#line 66 "forward.md"
;
	};

#line 46 "start.md"
;
	class State {
			
#line 95 "start.md"

	void add_machine(int instr);

#line 343 "start.md"

	Macros _macros;

#line 350 "start.md"

	static Macros *setup_symbols();

#line 255 "forward.md"

	Forwards _forwards;

#line 48 "start.md"

		public:
			
#line 69 "start.md"

	void add_line(
		const std::string &line
	);

#line 111 "start.md"

	
#line 118 "start.md"

	private:
		std::vector<int> code;

#line 126 "start.md"

	public:
		int code_size() const {
			return static_cast<int>(
				code.size()
			);
		}

#line 138 "start.md"

	public:
		int get_code(int pos) const {
			return code[pos];
		}
		int &mod_code(int pos) {
			return code[pos];
		}

#line 112 "start.md"


#line 373 "start.md"

	State():
		_macros { setup_symbols() }
	{ }
	State(Macros *parent):
		_macros { parent }
	{ }

#line 311 "forward.md"

	void fix_forwards() {
		_forwards.fill(_macros, *this);
	}

#line 50 "start.md"

	};
	
#line 78 "start.md"

	
#line 30 "start.md"

	#include <cassert>

#line 312 "start.md"

	#include <cctype>

#line 331 "start.md"

	#include <map>

#line 79 "start.md"

	void State::add_line(
		const std::string &line
	) {
		
#line 401 "start.md"

	std::vector<Item> items;

#line 408 "start.md"

	auto end { line.end() };
	auto cur { line.begin() };

#line 416 "start.md"

	bool log { false };
	if (cur < end && *cur == '!') {
		log = true;
		++cur;
	}

#line 427 "start.md"

	for (;;) {
		
#line 436 "start.md"

	while (cur < end && *cur <= ' ') {
		++cur;
	}
	if (cur == end) { break; }

#line 447 "start.md"

	int escapes { 0 };
	while (cur < end && *cur == '`') {
		++escapes;
		++cur;
	}

#line 458 "start.md"

	auto begin { cur };
	if (isalpha(*cur) || *cur == '_' ||
		*cur == '%'
	) {
		
#line 470 "start.md"

	while (cur < end && (
		isalnum(*cur) || *cur == '_' ||
			*cur == '%')
	) {
		++cur;
	}
	items.emplace_back(
		Item_Type::t_string,
		std::string { begin, cur },
		0, escapes
	);

#line 463 "start.md"
;
	}

#line 488 "start.md"

	else if (isdigit(*cur)) {
		
#line 497 "start.md"

	int value { 0 };
	while (cur < end && isdigit(*cur)) {
		value =
			value * 10 + (*cur++ - '0');
	}
	items.emplace_back(
		Item_Type::t_instance, "num",
		value, escapes
	);

#line 490 "start.md"
;
	}

#line 513 "start.md"

	else if (*cur == '#') {
		break;
	}

#line 522 "start.md"

	else if (*cur == '@') {
		++cur; std::string name;
		while (
			cur < end && isalnum(*cur)
		) {
			name += *cur++;
		}
		items.emplace_back(
			Item_Type::t_type, name,
			0,escapes
		);
	}

#line 541 "start.md"

	else if (*cur == '$') {
		
#line 550 "start.md"

	++cur;
	int value { 0 };
	while (cur < end && isxdigit(*cur)) {
		
#line 567 "start.md"

	int digit;
	if (isdigit(*cur)) {
		digit = *cur - '0';
	} else if (*cur <= 'F') {
		digit = *cur - 'A' + 10;
	} else {
		digit = *cur - 'a' + 10;
	}
	value = value * 16 + digit;

#line 554 "start.md"
;
		++cur;
	}
	items.emplace_back(
		Item_Type::t_instance, "num",
		value, escapes
	);

#line 543 "start.md"
;
	}

#line 583 "start.md"

	else if (ispunct(*cur)) {
		
#line 592 "start.md"

	while (cur < end && ispunct(*cur) &&
		*cur != '$' && *cur != '#' &&
		*cur != '@' && *cur != '_' &&
		*cur != '%' && *cur != '`'
	) {
		++cur;
	}
	items.emplace_back(
		Item_Type::t_string,
		std::string { begin, cur },
		0, escapes
	);

#line 585 "start.md"
;
	}

#line 429 "start.md"
;
	}

#line 611 "start.md"

	items.emplace_back(
		Item_Type::t_string, ";", 0, 0
	);
	
#line 867 "start.md"

restart:
	if (log) {
		std::cerr << "LINE {";
		for (const auto &i : items) {
			std::cerr << i << " ";
		}
		std::cerr << "}\n";
	}

#line 881 "start.md"

	if (items.size()) {
		auto macro { _macros.begin() };
		while (macro != _macros.end()) {
			
#line 898 "start.md"

	unsigned i = 0;
	while (i + macro->pattern().size() <=
		items.size()
	) {
		
#line 926 "start.md"

	if (i + 2 < items.size()) {
		const auto &t { items[i] };
		const auto &c { items[i + 1] };
		const auto &v { items[i + 2] };
		if (c.type() ==
				Item_Type::t_string &&
			c.str() == ":"
		) {
			
#line 943 "start.md"

	if (t.type() == Item_Type::t_type &&
		v.type() == Item_Type::t_instance
			&& v.str() == "num"
	) {
		
#line 956 "start.md"

	std::string type { t.str() };
	int value { v.value() };
	items.erase(items.begin() + i,
		items.begin() + i + 3
	);
	items.emplace(items.begin() + i,
		Item_Type::t_instance, type,
		value, 0
	);

#line 948 "start.md"
;
		goto restart;
	}

#line 971 "start.md"

	if (t.type() == Item_Type::t_instance
			&& v.type() ==
				Item_Type::t_string &&
		v.str() == "value"
	) {
		
#line 986 "start.md"

	int value { t.value() };
	items.erase(items.begin() + i,
		items.begin() + i + 3
	);
	items.emplace(items.begin() + i,
		Item_Type::t_instance, "num",
		value, 0
	);

#line 977 "start.md"
;
		goto restart;
	}

#line 935 "start.md"
;
		}
	}

#line 1001 "start.md"
 {
	bool matches { true };
	auto p { macro->pattern().begin() };
	auto e { macro->pattern().end() };
	for (int j = 0; matches && p != e;
		++p, ++j
	) {
		
#line 1015 "start.md"

	matches = matches &&
		(i + j < items.size());
	matches = matches &&
		p->matches(items[i + j]);
	matches = matches &&
		p->escapes() <= 0;

#line 1008 "start.md"
;
	}

#line 1027 "start.md"

	if (matches) {
		auto k { i };
		i += macro->pattern().size();
		
#line 1043 "start.md"

	for (const auto &e :
		macro->replacement()
	) {
		
#line 1054 "start.md"

	if (e.type() == Item_Type::t_type &&
		! e.str().empty()
	) {
		
#line 1072 "start.md"
;
	if (isdigit(e.str()[0])) {
		
#line 1088 "start.md"

	int idx { std::stoi(e.str()) };
	if (idx >= 0 &&idx <
		(int) macro->pattern().size()
	) {
		items.insert(items.begin() + i,
			items[k + idx]
		);
		++i;
		continue;
	}

#line 1074 "start.md"
;
	} else if (e.str() == "arithmetic") {
		
#line 1105 "start.md"

	const auto &n1 { items[k + 0] };
	const auto &op { items[k + 1] };
	const auto &n2 { items[k + 2] };
	if (n1.type() == Item_Type::t_instance
			&& n1.str() == "num" &&
		op.type() == Item_Type::t_string
			&& n2.type() ==
				Item_Type::t_instance &&
		n2.str() == "num"
	) {
		
#line 1123 "start.md"

	if (op.str() == "+") {
		items.emplace(
			items.begin() + i,
			Item_Type::t_instance, "num",
			n1.value() + n2.value(), 0
		);
		++i;
		continue;
	}

#line 1138 "start.md"

	else if (op.str() == "-") {
		items.emplace(
			items.begin() + i,
			Item_Type::t_instance, "num",
			n1.value() - n2.value(), 0
		);
		++i;
		continue;
	}

#line 1153 "start.md"

	else if (op.str() == "*") {
		items.emplace(
			items.begin() + i,
			Item_Type::t_instance, "num",
			n1.value() * n2.value(), 0
		);
		++i;
		continue;
	}

#line 1168 "start.md"

	else if (op.str() == "/" &&
		n2.value() != 0
	) {
		items.emplace(
			items.begin() + i,
			Item_Type::t_instance, "num",
			n1.value() / n2.value(), 0
		);
		++i;
		continue;
	}

#line 1186 "start.md"

	else if (op.str() == "and") {
		items.emplace(
			items.begin() + i,
			Item_Type::t_instance, "num",
			n1.value() & n2.value(), 0
		);
		++i;
		continue;
	}

#line 1201 "start.md"

	else if (op.str() == "or") {
		items.emplace(
			items.begin() + i,
			Item_Type::t_instance, "num",
			n1.value() | n2.value(), 0
		);
		++i;
		continue;
	}

#line 1216 "start.md"

	else if (op.str() == "<<") {
		items.emplace(
			items.begin() + i,
			Item_Type::t_instance, "num",
			n1.value() << n2.value(), 0
		);
		++i;
		continue;
	}

#line 1231 "start.md"

	else if (op.str() == ">>") {
		items.emplace(
			items.begin() + i,
			Item_Type::t_instance, "num",
			n1.value() >> n2.value(), 0
		);
		++i;
		continue;
	}

#line 1246 "start.md"

	else if (op.str() == "xor") {
		items.emplace(
			items.begin() + i,
			Item_Type::t_instance, "num",
			n1.value() ^ n2.value(), 0
		);
		++i;
		continue;
	}

#line 1116 "start.md"
;
	}

#line 1076 "start.md"
;
	}
	
#line 261 "forward.md"

	if (e.str() == "fwdgoto") {
		const auto &label {
			items[k + 1]
		};
		if (label.type() ==
			Item_Type::t_string
		) {
			
#line 277 "forward.md"

	_forwards.emplace_back(
		Forward::Cmd_Style::j_type,
		code_size(), label.str(), true
	);

#line 269 "forward.md"
;
			continue;
		}
	}

#line 286 "forward.md"

	if (e.str() == "fwdcndgoto") {
		const auto &label {
			items[k + 6]
		};
		if (label.type() ==
			Item_Type::t_string
		) {
			
#line 302 "forward.md"

	_forwards.emplace_back(
		Forward::Cmd_Style::b_type,
		code_size(), label.str(), true
	);

#line 294 "forward.md"
;
			continue;
		}
	}

#line 1078 "start.md"
;

#line 1058 "start.md"
;
	}
	items.emplace(
		items.begin() + i, e.type(),
		e.str(), e.value(),
		e.escapes() - 1
	);
	++i;

#line 1047 "start.md"
;
	}

#line 1031 "start.md"
;
		items.erase(items.begin() + k,
			items.begin() + k +
				macro->pattern().size()
		);
		goto restart;
	}
} 
#line 1261 "start.md"
 {
	const auto &ni { items[i] };
	if (
		ni.type() == Item_Type::t_string
	) {
		
#line 1273 "start.md"

	if (
		ni.str() == "*" &&
		ni.escapes() <= 0
	) {
		
#line 1285 "start.md"

	items.erase(items.begin() + i,
		items.begin() + i + 1);
	int addr = code.size() * 4 +
		0x20010000;
	items.emplace(items.begin() + i,
		Item_Type::t_instance,
		"num", addr, 0
	);
	goto restart;

#line 1278 "start.md"
;
	}

#line 1266 "start.md"
;
	}
} 
#line 903 "start.md"
;
		++i;
	}

#line 885 "start.md"
;
			++macro;
		}
		
#line 1343 "start.md"

	for (unsigned i = 1;
		i < items.size(); ++i
	) {
		const auto &a { items[i] };
		if (a.type() ==
				Item_Type::t_string &&
			a.str() == "<==" &&
			a.escapes() <= 0
		) {
			
#line 1361 "start.md"

	Items value;
	unsigned last { items.size() - 1 };

#line 1369 "start.md"

	for (
		unsigned j = i + 1; j < last; ++j
	) {
		const auto &cur { items[j] };
		
#line 1382 "start.md"

	if (cur.type() ==
			Item_Type::t_string &&
		cur.str() == "." &&
		cur.escapes() <= 0
	) {
		last = j; break;
	}

#line 1374 "start.md"
;
		value.push_back(items[j]);
	}

#line 1395 "start.md"

	Items p;
	for (unsigned j = 0; j < i; ++j) {
		p.push_back(items[j]);
	}
	_macros.emplace_back(std::move(p),
		std::move(value)
	);

#line 1409 "start.md"

	if (last < items.size()) { ++last; }
	items.erase(
		items.begin(),
		items.begin() + last
	);

#line 1353 "start.md"
;
		}
	}

#line 888 "start.md"
;
		
#line 1300 "start.md"

	while (! items.empty()) {
		const auto &mi { *items.begin() };
		
#line 1312 "start.md"

	if (mi.type() ==
			Item_Type::t_string &&
		mi.str() == ";"
	) {
		items.erase(
			items.begin(),
			items.begin() + 1
		);
		continue;
	}

#line 1303 "start.md"
;
		
#line 1328 "start.md"

	if (mi.type() !=
			Item_Type::t_instance ||
		mi.str() != "raw"
	) { break; }
	add_machine(mi.value());
	items.erase(
		items.begin(),
		items.begin() + 1
	);

#line 1304 "start.md"
;
	}

#line 889 "start.md"
;
	}

#line 911 "start.md"

	if (! items.empty()) {
		std::cerr <<
			"cant expand fully [" <<
			line << "]; got [";
		for (const auto &i: items) {
			std::cerr << i << " ";
		}
		std::cerr << "]\n";
	}

#line 615 "start.md"
;

#line 83 "start.md"
;
	}

#line 102 "start.md"

	void State::add_machine(int instr) {
		
#line 151 "start.md"

	code.push_back(instr);

#line 104 "start.md"

	}

#line 358 "start.md"

	Macros *State::setup_symbols() {
		static State s { nullptr };
		static bool initialized { false };
		if (! initialized) {
			
#line 386 "start.md"

	#include "default.h"
	std::istringstream in { setup };
	std::string l;
	while (std::getline(in, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}

#line 363 "start.md"
;
			initialized = true;
		}
		return &s._macros;
	}

#line 52 "start.md"
;

#line 158 "start.md"

	void assert_line(
		const char *line,
		int expected
	) {
		
#line 225 "start.md"

	State s;
	s.add_line(line);
	if (s.code_size() != 1) {
		std::cerr <<
			"assert failed in [" <<
			line << "]\n";
		std::cerr << "got " <<
			s.code_size() <<
			" elements\n";
	}

#line 242 "start.md"

	assert(s.code_size() == 1);
	if (s.get_code(0) != expected) {
		std::cerr <<
			"assert failed in [" <<
			line << "]\n";
		std::cerr << "EXP " <<
			std::hex << expected << "\n";
		std::cerr << "GOT " <<
			s.get_code(0) << std::dec <<
			"\n";
	}
	assert(s.get_code(0) == expected);

#line 163 "start.md"
;
	}

#line 261 "start.md"

	void assert_line_2(
		const char *line, int exp1,
		int exp2
	) {
		
#line 273 "start.md"

	State s;
	s.add_line(line);
	assert(s.code_size() == 2);
	if (s.get_code(0) != exp1 ||
		s.get_code(1) != exp2
	) {
		
#line 291 "start.md"

	std::cerr << "EXP " << std::hex <<
		exp1 << ", " << exp2 << "\n";
	std::cerr << "GOT " <<
		s.get_code(0) << ", " <<
		s.get_code(1) << std::dec << "\n";

#line 280 "start.md"
;
	}
	assert(s.get_code(0) == exp1);
	assert(s.get_code(1) == exp2);

#line 266 "start.md"
;
	}

#line 105 "forward.md"

	void Forwards::fill(
		const Macros &macros,
		State &state
	) {
		
#line 116 "forward.md"

	for (const auto &f : _forwards) {
		bool found { false };
		int value;
		
#line 126 "forward.md"

	for (const auto &m : macros) {
		if (m.pattern().size() == 1) {
			const auto &p {
				*m.pattern().begin()
			};
			
#line 139 "forward.md"

	if (p.type() == Item_Type::t_string &&
		p.str() == f.name()
	) {
		if (m.replacement().size() == 1) {
			
#line 151 "forward.md"

	const auto &r {
		*m.replacement().begin()
	};
	if (r.type() ==
			Item_Type::t_instance &&
		r.str() == "num"
	) {
		found = true;
		value = r.value();
		break;
	}

#line 144 "forward.md"
;
		}
	}

#line 132 "forward.md"
;
		}
	}

#line 167 "forward.md"

	if (! found) {
		std::cerr << "can't expand " <<
			f.name() << '\n';
		continue;
	}

#line 177 "forward.md"

	if (f.relative()) {
		value = value - 4 * f.position() -
			0x20010000;
	}

#line 186 "forward.md"

	switch (f.style()) {
		
#line 197 "forward.md"

	case Forward::Cmd_Style::i_type:
		state.mod_code(f.position()) |=
			value << 20;
		break;

#line 206 "forward.md"

	case Forward::Cmd_Style::s_type:
		state.mod_code(f.position()) |=
			((value & 0xfe0) <<
				(25 - 5)) |
			((value & 0x1f) << 7);
		break;

#line 217 "forward.md"

	case Forward::Cmd_Style::b_type:
		state.mod_code(f.position()) |=
			((value & 0x1000) <<
				(31 - 12)) |
			((value & 0x7e0) <<
				(25 - 5)) |
			((value & 0x1e) << (8 - 1)) |
			((value & 0x800) >> (11 - 7));
		break;

#line 231 "forward.md"

	case Forward::Cmd_Style::u_type:
		state.mod_code(f.position()) |=
			value & 0xfffff000;
		break;

#line 240 "forward.md"

	case Forward::Cmd_Style::j_type:
		state.mod_code(f.position()) |=
			((value & 0x100000) <<
				(31 - 20)) |
			((value & 0x7fe) <<
				(21 - 1)) |
			((value & 0x800) <<
				(20 - 11)) |
			(value & 0xff000);
		break;

#line 188 "forward.md"
;
		default:
			std::cerr <<
				"unimplemented mode\n";
	}

#line 120 "forward.md"
;
	}

#line 110 "forward.md"
;
	}

#line 24 "start.md"


#line 6 "hex.md"

	
#line 35 "hex.md"

	void write_byte(
		std::ostream &out, int b
	) {
		
#line 135 "hex.md"

	static const char map[] {
		"0123456789ABCDEF"
	};
	out << map[(b >> 4) & 0xf];
	out << map[b & 0xf];

#line 39 "hex.md"
;
	}

#line 46 "hex.md"

	void write_word(
		std::ostream &out, int w
	) {
		write_byte(out, w >> 8);
		write_byte(out, w);
	}

#line 7 "hex.md"
;
	std::ostream &operator<<(
		std::ostream &out, const State &s
	) {
		
#line 26 "hex.md"

	
#line 58 "hex.md"

	int base = 0x20010000;
	out << ":02000004";
	int sum = 0x06;
	sum += (base >> 24) + (base >> 16);
	write_word(out, base >> 16);
	write_byte(out, -sum);
	out << "\r\n";

#line 27 "hex.md"
;
	
#line 73 "hex.md"

	int len = s.code_size();
	int c = 0;
	for (; c < len; c += 4) {
		
#line 85 "hex.md"

	out << ':';
	int l =
		((c + 4 > len) ? (len - c) : 4);
	write_byte(out, l * 4);
	sum = l * 4;
	int addr = base + c * 4;
	sum += (addr >> 8) + addr;
	write_word(out, addr);
	out << "00";

#line 101 "hex.md"

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

#line 77 "hex.md"
;
		out << "\r\n";
	}

#line 28 "hex.md"
;
	
#line 120 "hex.md"

	sum =  9;
	out << ":04000005";
	write_word(out, base >> 16);
	write_word(out, base);
	sum += (base >> 24) + (base >> 16) +
		(base >> 8) + base;
	write_byte(out, -sum);
	out << "\r\n";
	out << ":00000001FF\r\n";

#line 29 "hex.md"
;

#line 11 "hex.md"
;
		return out;
	}

#line 7 "start.md"
;
	int main(
		int argc, const char *argv[]
	) {
		
#line 171 "start.md"

	
#line 178 "start.md"

	assert_line(
		"raw $87654321", 0x87654321
	);

#line 187 "start.md"

	assert_line(
		"raw (1000 - 200)", 800
	);

#line 196 "start.md"

	assert_line(
		"raw (200 - 1000)", -800
	);

#line 205 "start.md"

	assert_line(
		"raw ($20010020 - $20010010)",
		0x10
	);

#line 215 "start.md"

	assert_line(
		"raw ($20010010 - $20010020)",
		-0x10
	);

#line 319 "start.md"

	assert_line(
		"%x4 <- %x2 + %x3", 0x00310233
	);
	assert_line(
		"%a0 <- %a1 - %a2", 0x40c58533
	);

#line 622 "start.md"

	assert_line(
		"%pc <- %pc", 0x0000006f
	);

#line 632 "start.md"

	assert_line(
		"%pc <- %pc - 28", 0xfe5ff06f
	);

#line 641 "start.md"

	assert_line(
		"%pc <- %pc - 32", 0xfe1ff06f
	);

#line 670 "start.md"

	assert_line(
		"%x5 <- %x5 and $ff", 0x0ff2f293
	);
	assert_line(
		"%a0 <- %a1 and %a2", 0x00c5f533
	);

#line 682 "start.md"

	assert_line(
		"%x5 <- %x5 or $1", 0x0012e293
	);
	assert_line(
		"%a0 <- %a1 or %a2", 0x00c5e533
	);
	assert_line(
		"%x6 <- %x6 or $1", 0x00136313
	);

#line 697 "start.md"

	assert_line(
		"%a0 <- %a1 xor $ff", 0x0ff5c513
	);
	assert_line(
		"%a0 <- %a1 xor %a2", 0x00c5c533
	);

#line 709 "start.md"

	assert_line(
		"%a0 <- complement %a1",
		0x0005c513
	);

#line 719 "start.md"

	assert_line(
		"%x11 <- $0d", 0x00d00593
	);
	assert_line(
		"%x12 <- $0a", 0x00a00613
	);

#line 731 "start.md"

	assert_line(
		"%x10 <- $1013000", 0x1013537
	);

#line 740 "start.md"

	assert_line(
		"%x5 <- %pc", 0x00000297
	);

#line 749 "start.md"

	assert_line_2(
		"%x5 <- %pc + $20",
		0x00000297, 0x02028293
	);

#line 759 "start.md"

	assert_line_2(
		"%x5 <- %pc - $20",
		0x00000297, 0xfe028293
	);

#line 769 "start.md"

	assert_line(
		"%mtvec <- %x5", 0x30529073
	);

#line 778 "start.md"

	assert_line(
		"%x5 <- %mhartid", 0xf14022f3
	);

#line 787 "start.md"

	assert_line(
		"if %x5 < 0: %pc <- %pc - 4",
		0xfe02cee3
	);

#line 797 "start.md"

	assert_line(
		"if %x5 = 0: %pc <- %pc - 12",
		0xfe028ae3
	);

#line 807 "start.md"

	assert_line(
		"if %x5 != %x11: %pc <- %pc - 28",
		0xfeb292e3
	);

#line 817 "start.md"

	assert_line(
		"if %x5 != 0: %pc <- %pc + 0",
		0x00029063
	);

#line 827 "start.md"

	assert_line(
		"%x6 <- [%x10]",
		0x00052303
	);

#line 837 "start.md"

	assert_line(
		"%x5 <- [%x10 + $04]",
		0x00452283
	);

#line 847 "start.md"

	assert_line(
		"[%x10] <- %x12",
		0x00c52023
	);

#line 857 "start.md"

	assert_line(
		"[%x10 + $08] <- %x5",
		0x00552423
	);

#line 172 "start.md"


#line 11 "start.md"

		
#line 650 "start.md"

	
#line 657 "start.md"

	State s;
	std::string l;
	while (std::getline(std::cin, l)) {
		if (l.empty()) { continue; }
		s.add_line(l);
	}

#line 651 "start.md"


#line 319 "forward.md"

	s.fix_forwards();

#line 19 "hex.md"

	std::cout << s;

#line 12 "start.md"

	}
