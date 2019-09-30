# Macros Iterator
* iterates over all `Macro`s in a `Macros` instance
* and over the elements of the parent instances

```
@Add(publics)
	class Iterator {
		private:
			@put(privates);
		public:
			@put(publics);
	};
@End(publics)
```
* `class` has `public` and `private` elements

```
@def(privates)
	const Macros *_macros;
	Container::const_iterator _cur;
@end(privates)
```
* contains a pointer to the `Macros` instance
* and an iterator in the current `Macros` instance

```
@add(privates)
	void fix() {
		@put(fix);
	}
@end(privates)
```
* ensures that the iterator points to the correct entries after an
  increment

```
@def(fix)
	while (_macros &&
		_cur == _macros->_macros.end()
	) {
		if (_macros->_parent) {
			@put(jump to next);
		} else {
			_macros = nullptr;
		}
	}
@end(fix)
```
* if the end of a collection is reached and there is a parent instance,
  the iterator moves to the next instance

```
@def(jump to next)
	_macros = _macros->_parent;
	_cur = _macros->_macros.begin();
@end(jump to next)
```
* sets the iterator to the first element in the next instance

```
@def(publics)
	Iterator(const Macros *macros,
		Container::const_iterator cur
	):
		_macros { macros },
		_cur { cur }
	{ fix() ;}
@end(publics)
```
* the constructor initializes to the first element in the passed
  instance

```
@add(publics)
	const Macro &operator*() const {
		return *_cur;
	}
@end(publics)
```
* get current `const Macro`

```
@add(publics)
	const Macro *operator->() const {
		return &*_cur;
	}
@end(publics)
```
* get current `const Macro`

```
@add(publics)
	Iterator &operator++() {
		++_cur;
		fix();
		return *this;
	}
@end(publics)
```
* move to next `Macro`

```
@add(publics)
	bool operator!=(
		const Iterator &o
	) const {
		return o._macros != _macros || (
			_macros != nullptr &&
			o._cur != _cur
		);
	}
@end(publics)
```
* compare two iterators

