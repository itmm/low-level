# Item Type

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

```
@def(needed by item)
	enum class Item_Type {
		t_type, t_instance, t_string
	};
@end(needed by item)
```

```
@def(privates)
	Item_Type _type;
	int _escapes;
	int _value;
	std::string _str;
@end(privates)
```

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

```
@add(publics)
	void write(std::ostream &out) const {
		switch (_type) {
			@put(write cases);
		}
	}
@end(publics)
```

```
@def(write cases)
	case Item_Type::t_type:
		out << '@' << _str;
		break;
@end(write cases)
```

```
@add(write cases)
	case Item_Type::t_instance:
		if (_str == "num") {
			out << '$' << std::hex <<
				_value << std::dec;
		} else if (_str == "reg") {
			out << "%x" << _value;
		} else {
			out << '@' << _str << ":$" <<
				std::hex << _value <<
				std::dec;
		}
		break;
@end(write cases)
```

```
@add(write cases)
	case Item_Type::t_string:
		out << _str;
@end(write cases)
```

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

```
@def(matches cases)
	case Item_Type::t_type: {
		@put(matches type);
		return false;
	}
@end(matches cases)
```

```
@def(matches type)
	if (in._type == Item_Type::t_type &&
		in._str == _str
	) {
		return true;
	}
@end(matches type)
```

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

```
@add(matches type)
	if (in._type == Item_Type::t_string &&
		_str == "str"
	) {
		return true;
	}
@end(matches type)
```

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

```
@add(matches cases)
	case Item_Type::t_string: {
		return in._type ==
				Item_Type::t_string &&
			in._str == _str;
	}
@end(matches cases)
```

```
@add(publics)
	int escapes() const {
		return _escapes;
	}
@end(publics)
```

```
@add(publics)
	Item_Type type() const {
		return _type;
	}
@end(publics)
```

```
@add(publics)
	const std::string &str() const {
		return _str;
	}
@end(publics)
```

```
@add(publics)
	int value() const { return _value; }
@end(publics)
```

```
@Add(needed by state)
	std::ostream &operator<<(
		std::ostream &out,
		const Item &item
	) {
		item.write(out);
		return out;
	}
@End(needed by state)
```

