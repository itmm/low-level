# Write binary in Intel HEX format

```
@Add(needed by main)
	@put(needed by write hex);
	std::ostream &operator<<(
		std::ostream &out, const State &s
	) {
		@put(write hex);
		return out;
	}
@End(needed by main)
```

```
@Add(main)
	std::cout << s;
@End(main)
```

```
@def(write hex)
	@put(write startup);
	@put(write data);
	@put(write finish);
@end(write hex)
```

```
@def(needed by write hex)
	void write_byte(
		std::ostream &out, int b
	) {
		@put(write byte);
	}
@end(needed by write hex)
```

```
@add(needed by write hex)
	void write_word(
		std::ostream &out, int w
	) {
		write_byte(out, w >> 8);
		write_byte(out, w);
	}
@end(needed by write hex)
```

```
@def(write startup)
	int base = 0x20010000;
	out << ":02000004";
	int sum = 0x06;
	sum += (base >> 24) + (base >> 16);
	write_word(out, base >> 16);
	write_byte(out, -sum);
	out << '\n';
	
@end(write startup)
```

```
@def(write data)
	int len = s.code_size();
	int c = 0;
	for (; c < len; c += 4) {
		out << ':';
		int l = ((c + 4 > len) ? (len - c) : 4);
		write_byte(out, l * 4);
		sum = l * 4;
		int addr = base + c * 4;
		sum += (addr >> 8) + addr;
		write_word(out, addr);
		out << "00";
		for (int i = 0; i < l; ++i) {
			int m = s.get_code(c + i);
			write_byte(out, m); sum += m;
			m = m >> 8;
			write_byte(out, m); sum += m;
			m = m >> 8;
			write_byte(out, m); sum += m;
			m = m >> 8;
			write_byte(out, m); sum += m;
		}
		write_byte(out, -sum);
		out << '\n';
	}
@end(write data)
```

```
@def(write finish)
	sum =  9;
	out << ":04000005";
	write_word(out, base >> 16);
	write_word(out, base);
	sum += (base >> 24) + (base >> 16) +
		(base >> 8) + base;
	write_byte(out, -sum);
	out << '\n';
	out << ":00000001FF\n";
@end(write finish)
```

```
@def(write byte)
	static const char map[] {
		"0123456789ABCDEF"
	};
	out << map[(b >> 4) & 0xf];
	out << map[b & 0xf];
@end(write byte)
```
