# Item
* class for elements read by the tokenizer

```
@Add(needed by state)
	@put(needed by item);
	class Item {
		private:
			@put(privates);
		public:
			@put(publics);
	};
@End(needed by state)
```
* `class` has `private` and `public` elements
* also other types are needed by `Item`

```
@def(needed by item)
	enum class Item_Type {
		t_type, t_instance, t_string
	};
@end(needed by item)
```
* each item is of one of three types
* a `t_type` item has a type name,
* a `t_instance` item has a type name and an integer value,
* a `t_string` has the type name `@s(str)` and a string value
* string is a special type, so that only one string needs to be stored
  in each item

```
@def(privates)
	Item_Type _type;
@end(privates)
```
* each `Item` has a type

```
@add(privates)
	int _escapes;
@end(privates)
```
* this attributes signals how often the item was escaped with a backtick
* on expansion escaped items are only unescaped, but not expanded

```
@add(privates)
	int _value;
	std::string _str;
@end(privates)
```
* each `Item` can store an integer value and a string
* the string is either the type name or the string value, if the type
  is `t_string`

```
@def(publics)
	Item(Item_Type type,
		const std::string &str,
		int value, int escapes
	):
		_type { type }, 
		_escapes { escapes },
		_value { value }, _str { str }
	{ }
@end(publics)
```
* the constructor sets all attributes

## Matching
* can an `Item` match another `Item`?

```
@add(publics)
	bool matches(const Item &in) const {
		switch (_type) {
			@put(matches cases);
		}
		return false;
	}
@end(publics)
```
* checks if the current `Item` matches the argument

```
@def(matches cases)
	case Item_Type::t_type: {
		@put(matches type);
		return false;
	}
@end(matches cases)
```
* if the current `Item` is a type item, some cases must be distinguished

```
@def(matches type)
	if (in._type == Item_Type::t_type &&
		in._str == _str
	) {
		return true;
	}
@end(matches type)
```
* if the other `Item` is also a type item with the same type name, it
  matches

```
@add(matches type)
	if (in._type ==
			Item_Type::t_instance &&
		in._str == _str
	) {
		return true;
	}
@end(matches type)
```
* if the other `Item` is an instance item with the same type name, it
  matches

```
@add(matches type)
	if (in._type == Item_Type::t_string &&
		_str == "str"
	) {
		return true;
	}
@end(matches type)
```
* if the other `Item` is a string item and the current type name is
  `@s(str)`, it matches

```
@add(matches cases)
	case Item_Type::t_instance: {
		return in._type ==
				Item_Type::t_instance &&
			in._str == _str &&
			in._value == _value;
	}
@end(matches cases)
```
* when the current `Item` is an instance item it only matches, if the
  other `Item` is also an instance item and has the same type name and
  value

```
@add(matches cases)
	case Item_Type::t_string: {
		return in._type ==
				Item_Type::t_string &&
			in._str == _str;
	}
@end(matches cases)
```
* if the current `Item` is a string item it only matches, if the other
  `Item` is also a string item and has the same string value

```
@add(publics)
	int escapes() const {
		return _escapes;
	}
@end(publics)
```
* getter for escape count

```
@add(publics)
	Item_Type type() const {
		return _type;
	}
@end(publics)
```
* getter for item type

```
@add(publics)
	const std::string &str() const {
		return _str;
	}
@end(publics)
```
* getter for string value

```
@add(publics)
	int value() const { return _value; }
@end(publics)
```
* getter for integer value

## Serialization
* write an `Item` into a `std::ostream`

```
@Add(needed by state)
	std::ostream &operator<<(
		std::ostream &out,
		const Item &item
	) {
		switch (item.type()) {
			@put(write cases);
		}
		return out;
	}
@End(needed by state)
```
* serialize items in streams

```
@def(write cases)
	case Item_Type::t_type:
		out << '@' << item.str();
		break;
@end(write cases)
```
* types are written as the type name prefixed by an `@`

```
@add(write cases)
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
@end(write cases)
```
* numeric instances are written as hexadecimal literals
* register instances are prefixed by `%x`
* other instances are written by their type followed with their value

```
@add(write cases)
	case Item_Type::t_string:
		out << item.str();
@end(write cases)
```
* string `Item`s are serialized as their string value

