# Macro
* `class` for handling macros

```
@inc(item.x)
```
* needs `Item`

```
@Add(needed by state)
	using Items = std::vector<Item>;
@End(needed by state)
```
* macros use sequences of `Item`s

```
@Add(needed by state)
	class Macro {
		private:
			@put(privates);
		public:
			@put(publics);
	};
@End(needed by state)
```
* a `Macro` has `public` and `private` entries

```
@def(privates)
	Items _pattern;
	Items _replacement;
@end(privates)
```
* the `private` entries are the pattern and its replacement

```
@def(publics)
	Macro(
		Items &&pattern, Items &&repl
	):
		_pattern { std::move(pattern) },
		_replacement { std::move(repl) }
	{ }
@end(publics)
```
* constructor saves pattern and replacement

```
@add(publics)
	const Items &pattern() const {
		return _pattern;
	}
@end(publics)
```
* getter for pattern

```
@add(publics)
	const Items &replacement() const {
		return _replacement;
	}
@end(publics)
```
* getter for replacement

