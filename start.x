# First Code fragments
* start low-level with the first unit-test

```
@Def(file: ll.cpp)
	@put(needed by main);
	int main(
		int argc, const char *argv[]
	) {
		@put(main)
	}
@End(file: ll.cpp)
```
* the start is easy in C++
* a naked `@f(main)` function with arguments array
* it will call some other things that must be declared before the
  `@f(main)` function
* so there is a fragment for prerequisites and the content

```
@def(main)
	@put(unit-tests)
@end(main)
```
* first some unit-tests are performed at every start of low-level

```
@def(needed by main)
	#include <cassert>
@end(needed by main)
```
* the unit-tests need the `@f(assert)`-macro

## Parsing a single line
* the first test focuses on parsing a single line
* other code is responsible for reading files and extract the low-level
  lines from it

The parsing is part of a `State` object that contains all generated
machine instructions.

```
@add(needed by main)
	@put(needed by state);
	class State {
			@put(private state)
		public:
			@put(public state)
	};
	@put(state impl);
@end(needed by main)
```
* the `State` class can contain private and public members
* also a fragment is reserved for containing the implementation of
  non-inlined methods

```
@def(needed by state)
	#include <string>
@end(needed by state)
```
* `@f(add_line)` needs `std::string`

```
@def(public state)
	void add_line(
		const std::string &line
	);
@end(public state)
```
* method to consume one line with one instruction

```
@def(state impl)
	void State::add_line(
		const std::string &line
	) {
		@put(add line);
	}
@end(state impl)
```
* populate the empty implementation when dependencies are present


Where will the machine instructions be written?
To abstract the storage away from the code generation a new method stores
the machine code.

```
@def(private state)
	void add_machine(int instr);
@end(private state)
```
* adding one machine instruction to the current output

```
@add(state impl)
	void State::add_machine(int instr) {
		@put(add machine)
	}
@end(state impl)
```
* populate empty implementation when dependencies are present

```
@add(public state)
	@put(code storage impl)
@end(public state)
```
* first a simple code storage mechanism will be used

```
@add(needed by state)
	#include <vector>
@end(needed by state)
```
* simple code storage needs `std::vector`

```
@def(code storage impl)
	private:
		std::vector<int> code;
@end(code storage impl)
```
* code is stored in one big array

```
@add(code storage impl)
	public:
		int code_size() const {
			return static_cast<int>(
				code.size()
			);
		}
@end(code storage impl)
```
* code size is the size of the array

```
@add(code storage impl)
	public:
		int get_code(int pos) const {
			return code[pos];
		}
@end(code storage impl)
```
* machine instructions can be accessed directly from the array

```
@def(add machine)
	code.push_back(instr);
@end(add machine)
```
* simple implementation just saves the data

```
@add(needed by main)
	void assert_line(
		const char *line,
		int expected
	) {
		@put(assert line);
	}
@end(needed by main)
```
* simple helper function to check that some source code generates the
  expected instruction


```
@def(unit-tests)
	assert_line(
		"%x4 <- %x2 + %x3",
		0x00310233
	);
@end(unit-tests)
```
* the first test checks that a simple register add can be parsed

```
@def(assert line)
	State s;
	s.add_line(line);
	assert(s.code_size() == 1);
	assert(s.get_code(0) == expected);
@end(assert line)
```
* instantiate a state and let it parse the line

## Tokenizing
* split the input line into token

```
@add(needed by state)
	enum class Token_Type {
		unknown,
		becomes,
		plus,
		reg,
		end
	};
@end(needed by state)
```

```
@add(needed by state)
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
@end(needed by state)
```

```
@add(needed by state)
	class Expression {
		public:
			virtual ~Expression() {}
	};
@end(needed by state)
```

```
@add(needed by state)
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
@end(needed by state)
```

```
@add(needed by state)
	#include <memory>
@end(needed by state)
```

```
@add(needed by state)
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
@end(needed by state)
```

```
@add(needed by state)
	class Assignment: public BinaryExpression {
		public:
			Assignment(
				std::unique_ptr<Expression> dst,
				std::unique_ptr<Expression> src
			): BinaryExpression(std::move(dst), std::move(src))
			{}
	};
@end(needed by state)
```

```
@add(needed by state)
	class Addition: public BinaryExpression {
		public:
			Addition(
				std::unique_ptr<Expression> frst,
				std::unique_ptr<Expression> scnd
			): BinaryExpression(std::move(frst), std::move(scnd))
			{}
		
	};
@end(needed by state)
```

```
@add(needed by state)
	#include <iostream>
@end(needed by state)
```

```
@add(needed by state)
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
@end(needed by state)
```

```
@add(needed by state)
	int build_r_cmd(
		int funct7, char src2, char src1,
		int funct3, char dst, int opcode
	) {
		return (funct7 << 25) |
			(src2 << 20) | (src1 << 15) |
			(funct3 << 12) | (dst << 7) |
			opcode;
	}
@end(needed by state)
```

```
@add(needed by state)
	int build_add(
		char dst, char src1, char src2
	) {
		return build_r_cmd(
			0x00, src2, src1,
			0x0, dst, 0x33
		);
	}
@end(needed by state)
```

```
@def(add line)
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
@end(add line)
```

```
add(needed by main)
	put(needed by do riscv)
	int do_riscv(
		const char *src
	) {
		int got = 0x00000000;
		put(do riscv);
		return got;
	}
end(needed by main)
```