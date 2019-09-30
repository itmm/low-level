# Macros
* collection of `Macro`s

```
@inc(macro.x)
```
* needs `Macro`

```
@Add(needed by state)
	class Macros {
		private:
			@put(privates);
		public:
			@put(publics);
	};
@End(needed by state)
```
* `class` has `private` and `public` elements

```
@def(privates)
	Macros *_parent;
@end(privates)
```
* `Macros` can cascade

```
@add(privates)
	using Container = std::vector<Macro>;
	Container _macros;
@end(privates)
```
* `Macros` contain a collection of `Macro`s

```
@def(publics)
	Macros(Macros *parent):
		_parent { parent }
	{ }
@end(publics)
```
* constructor needs cascaded container
* but the parent container will not be destroyed when this instance is
  `delete`d


```
@inc(iterator.x)
```
* needs `Iterator` class for `Macros`
* this iterator also iterates over the parent elements

```
@add(publics)
	Iterator begin() const {
		return {
			this, _macros.begin()
		};
	}
@end(publics)
```
* `Iterator` to first element

```
@add(publics)
	Iterator end() const {
		return {
			nullptr,
			Container::iterator { }
		};
	}
@end(publics)
```
* `Iterator` after the last element

```
@add(publics)
	void emplace_back(
		Items &&pattern,
		Items &&replacement
	) {
		_macros.emplace_back(
			std::move(pattern),
			std::move(replacement)
		);
	}
@end(publics)
```
* add a new `Macro` in the collection

