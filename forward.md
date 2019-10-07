# Forward References

```
@Add(needed by state)
	class Forward {
		public:
			enum class Cmd_Style {
				i_type, s_type, b_type,
				u_type, j_type
			};
		private:
			@put(forward privates);
		public:
			@put(forward publics);
	};
@End(needed by state)
```

```
@def(forward privates)
	Cmd_Style _style;
	int _position;
	std::string _name;
	bool _relative;
@end(forward privates)
```

```
@def(forward publics)
	Forward(Cmd_Style style,int position,
		const std::string &name,
		bool relative
	):
		_style { style },
		_position { position },
		_name { name },
		_relative { relative }
	{ }
@end(forward publics)
```

```
@add(forward publics)
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
@end(forward publics)
```

```
@Add(needed by state)
	class State;
	class Forwards {
		private:
			@put(forwards privates);
		public:
			@put(forwards publics);
	};
@End(needed by state)
```

```
@def(forwards privates)
	using Container =
		std::vector<Forward>;
	Container _forwards;
@end(forwards privates)
```

```
@def(forwards publics)
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
@end(forwards publics)
```

```
@add(forwards publics)
	void fill(
		const Macros &macros,
		State &state
	);
@end(forwards publics)
```

```
@Add(needed by main)
	void Forwards::fill(
		const Macros &macros,
		State &state
	) {
		@put(fill);
	}
@End(needed by main)
```

```
@def(fill)
	for (const auto &f : _forwards) {
		bool found { false };
		int value;
		@put(fill loop);
	}
@end(fill)
```

```
@def(fill loop)
	for (const auto &m : macros) {
		if (m.pattern().size() == 1) {
			const auto &p {
				*m.pattern().begin()
			};
			@put(got single pattern);
		}
	}
@end(fill loop)
```

```
@def(got single pattern)
	if (p.type() == Item_Type::t_string &&
		p.str() == f.name()
	) {
		if (m.replacement().size() == 1) {
			@put(found matching macro);
		}
	}
@end(got single pattern)
```

```
@def(found matching macro)
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
@end(found matching macro)
```

```
@add(fill loop)
	if (! found) {
		std::cerr << "can't expand " <<
			f.name() << '\n';
		continue;
	}
@end(fill loop)
```

```
@add(fill loop)
	if (f.relative()) {
		value = value - 4 * f.position() -
			0x20010000;
	}
@end(fill loop)
```

```
@add(fill loop)
	switch (f.style()) {
		@put(insert in instruction);
		default:
			std::cerr <<
				"unimplemented mode\n";
	}
@end(fill loop)
```

```
@def(insert in instruction)
	case Forward::Cmd_Style::i_type:
		state.mod_code(f.position()) |=
			value << 20;
		break;
@end(insert in instruction)
```

```
@add(insert in instruction)
	case Forward::Cmd_Style::s_type:
		state.mod_code(f.position()) |=
			((value & 0xfe0) <<
				(25 - 5)) |
			((value & 0x1f) << 7);
		break;
@end(insert in instruction)
```

```
@add(insert in instruction)
	case Forward::Cmd_Style::b_type:
		state.mod_code(f.position()) |=
			((value & 0x1000) <<
				(31 - 12)) |
			((value & 0x7e0) <<
				(25 - 5)) |
			((value & 0x1e) << (8 - 1)) |
			((value & 0x800) >> (11 - 7));
		break;
@end(insert in instruction)
```

```
@add(insert in instruction)
	case Forward::Cmd_Style::u_type:
		state.mod_code(f.position()) |=
			value & 0xfffff000;
		break;
@end(insert in instruction)
```

```
@add(insert in instruction)
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
@end(insert in instruction)
```

```
@Add(private state)
	Forwards _forwards;
@End(private state)
```

```
@Def(special macros)
	if (e.str() == "fwdgoto") {
		const auto &label {
			items[k + 1]
		};
		if (label.type() ==
			Item_Type::t_string
		) {
			@put(add j instruction);
			continue;
		}
	}
@End(special macros)
```

```
@def(add j instruction)
	_forwards.emplace_back(
		Forward::Cmd_Style::j_type,
		code_size(), label.str(), true
	);
@end(add j instruction)
```

```
@Add(special macros)
	if (e.str() == "fwdcndgoto") {
		const auto &label {
			items[k + 6]
		};
		if (label.type() ==
			Item_Type::t_string
		) {
			@put(add b instruction);
			continue;
		}
	}
@End(special macros)
```

```
@def(add b instruction)
	_forwards.emplace_back(
		Forward::Cmd_Style::b_type,
		code_size(), label.str(), true
	);
@end(add b instruction)
```

```
@Add(public state)
	void fix_forwards() {
		_forwards.fill(_macros, *this);
	}
@End(public state)
```

```
@Add(main)
	s.fix_forwards();
@End(main)
```

