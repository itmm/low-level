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
@End(needed by state)
```

```
@Add(needed by main)
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
@End(needed by main)
```

```
@Add(private state)
	Forwards _forwards;
@End(private state)
```

```
@Def(special macros)
	if (e.str() == "fwdgoto") {
		const auto &label { items[k + 1] };
		if (label.type() == Item_Type::t_string) {
			_forwards.emplace_back(
				Forward::Cmd_Style::j_type, code_size(), label.str(), true
			);
			continue;
		}
	}
@End(special macros)
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

