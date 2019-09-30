# Macros

```
@inc(item.x)
```

```
@Add(needed by state)
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
@End(needed by state)
```

```
@Add(needed by state)
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
@End(needed by state)
```

