
#line 5 "start.x"

	
#line 28 "start.x"

	#include <cassert>

#line 43 "start.x"

	
#line 58 "start.x"

	#include <string>

#line 113 "start.x"

	#include <vector>

#line 194 "start.x"

	enum class Token_Type {
		unknown,
		becomes,
		plus,
		reg,
		end
	};

#line 206 "start.x"

	#include <cctype>
	class Tokenizer {
			std::string::const_iterator _cur;
			std::string::const_iterator _end;
			Token_Type _type = Token_Type::unknown;
			std::string _value = {};
		public:
			Tokenizer(const std::string &s): _cur { s.begin() }, _end { s.end() } {
				next();
			}
			Token_Type type() const { return _type; }
			const std::string &value() const { return _value; }
			void next() {
				while (_cur != _end && *_cur <= ' ') { ++_cur; }
				if (_cur == _end) {
					_type = Token_Type::end;
					return;
				}
				_type = Token_Type::unknown;
				if (*_cur == '<') {
					if (++_cur != _end) {
						if (*_cur == '-') {
							_type = Token_Type::becomes;
							++_cur;
						}
					}
				} else if (*_cur == '+') {
					_type = Token_Type::plus;
					++_cur;
				} else if (*_cur == '%') {
					++_cur;
					_value = {};
					while (_cur != _end && isalnum(*_cur)) {
						_value += *_cur++;
					}
					if (! _value.empty()) {
						_type = Token_Type::reg;
					}
				}
			}
	};

#line 252 "start.x"

	class Expression {
		public:
			virtual ~Expression() {}
	};

#line 261 "start.x"

	class Register: public Expression {
			const std::string _name;
			const int _nr;
			
			static int nr_from_name(const std::string &name) {
				if (name.empty()) { return -1; }
				if (name == "x") { return -1; }
				if (name[0] != 'x') { return -1; }
				int nr = 0;
				for (int i = 1; i < (int) name.size(); ++i) {
					char ch = name[i];
					if (ch < '0' || ch > '9') { return -1; }
					nr = nr * 10 + (ch - '0');
				}
				if (nr < 0 || nr > 31) { return -1; }
				return nr;
			}

		public:
			Register(const std::string &name):
				_name { name },
				_nr { nr_from_name(name) }
			{ }

			const std::string &name() const { return _name; }
			int nr() const { return _nr; }
			bool is_general() const { return _nr >= 0; }
	};

#line 294 "start.x"

	#include <memory>

#line 300 "start.x"

	class BinaryExpression: public Expression {
			std::unique_ptr<Expression> _frst;
			std::unique_ptr<Expression> _scnd;

		public:
			BinaryExpression(
				std::unique_ptr<Expression> frst,
				std::unique_ptr<Expression> scnd
			): _frst { std::move(frst) }, _scnd { std::move(scnd) }
			{ }

			const std::unique_ptr<Expression> &first() const { return _frst; }
			const std::unique_ptr<Expression> &second() const { return _scnd; }
	};

#line 319 "start.x"

	class Assignment: public BinaryExpression {
		public:
			Assignment(
				std::unique_ptr<Expression> dst,
				std::unique_ptr<Expression> src
			): BinaryExpression(std::move(dst), std::move(src))
			{}
	};

#line 332 "start.x"

	class Addition: public BinaryExpression {
		public:
			Addition(
				std::unique_ptr<Expression> frst,
				std::unique_ptr<Expression> scnd
			): BinaryExpression(std::move(frst), std::move(scnd))
			{}
		
	};

#line 346 "start.x"

	#include <iostream>

#line 352 "start.x"

	std::unique_ptr<Expression> parse(Tokenizer &t) {
		std::unique_ptr<Expression> result;
		if (t.type() != Token_Type::reg) {
			std::cerr << "expected state\n";
			return result;
		}
		auto dst = std::make_unique<Register>(t.value());
		if (! dst->is_general()) {
			std::cerr << "general purpose register expected\n";
			return result;
		}
		t.next();
		if (t.type() == Token_Type::becomes) {
			t.next();
			auto src = parse(t);
			if (! src) {
				std::cerr << "no expression after <-\n";
				return result;
			}
			result = std::make_unique<Assignment>(std::move(dst), std::move(src));
		} else if (t.type() == Token_Type::plus) {
			t.next();
			auto src = parse(t);
			if (! src) {
				std::cerr << "no expression after +\n";
				return result;
			}
			result = std::make_unique<Addition>(std::move(dst), std::move(src));
		} else {
			result = std::move(dst);
		}
		return result;
	}

#line 390 "start.x"

	int build_r_cmd(
		int funct7, char src2, char src1,
		int funct3, char dst, int opcode
	) {
		return (funct7 << 25) |
			(src2 << 20) | (src1 << 15) |
			(funct3 << 12) | (dst << 7) |
			opcode;
	}

#line 404 "start.x"

	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}

#line 44 "start.x"
;
	class State {
			
#line 90 "start.x"

	void add_machine(int instr);

#line 46 "start.x"

		public:
			
#line 65 "start.x"

	void add_line(
		const std::string &line
	);

#line 106 "start.x"

	
#line 120 "start.x"

	private:
		std::vector<int> code;

#line 128 "start.x"

	public:
		int code_size() const {
			return static_cast<int>(
				code.size()
			);
		}

#line 140 "start.x"

	public:
		int get_code(int pos) const {
			return code[pos];
		}

#line 107 "start.x"


#line 48 "start.x"

	};
	
#line 74 "start.x"

	void State::add_line(
		const std::string &line
	) {
		
#line 417 "start.x"

	Tokenizer t { line };
	auto e = parse(t);
	if (! e) {
		std::cerr << "no statement on line " << line << "\n";
		return;
	}
	Assignment *a = dynamic_cast<Assignment *>(&*e);
	if (! a) {
		std::cerr << "assignment expected on line " << line << "\n";
		return;
	}
	const Register *dst = dynamic_cast<const Register *>(&*a->first());
	if (! dst) {
		std::cerr << "no assignment to register\n";
		return;
	}
	if (! dst->is_general()) {
		std::cerr << "assigned register is not general\n";
		return;
	}
	const Addition *o = dynamic_cast<const Addition *>(&*a->second());
	if (! o) {
		std::cerr << "only addition supported now\n";
		return;
	}
	const Register *src1 = dynamic_cast<const Register *>(&*o->first());
	if (! src1) {
		std::cerr << "first op of addition no register\n";
		return;
	}
	if (! src1->is_general()) {
		std::cerr << "first of of addition no general register\n";
	}
	const Register *src2 = dynamic_cast<const Register *>(&*o->second());
	if (! src2) {
		std::cerr << "second op of addition no register\n";
		return;
	}
	if (! src2->is_general()) {
		std::cerr << "second of of addition no general register\n";
	}
	add_machine(build_add(
		(char) dst->nr(), (char) src1->nr(), (char) src2->nr()
	));

#line 78 "start.x"
;
	}

#line 97 "start.x"

	void State::add_machine(int instr) {
		
#line 150 "start.x"

	code.push_back(instr);

#line 99 "start.x"

	}

#line 50 "start.x"
;

#line 157 "start.x"

	void assert_line(
		const char *line,
		int expected
	) {
		
#line 181 "start.x"

	State s;
	s.add_line(line);
	assert(s.code_size() == 1);
	assert(s.get_code(0) == expected);

#line 162 "start.x"
;
	}

#line 6 "start.x"
;
	int main(
		int argc, const char *argv[]
	) {
		
#line 21 "start.x"

	
#line 171 "start.x"

	assert_line(
		"%x4 <- %x2 + %x3",
		0x00310233
	);

#line 22 "start.x"


#line 10 "start.x"

	}
