# Write binary in Intel HEX format
* Low-Level directly generates an Intel HEX file
* that can be uploaded to a RISC-V board

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
* use shift-operator to serialize a `State` as an Intel HEX file

```
@Add(main)
	std::cout << s;
@End(main)
```
* the state is serialized to `std::out` as an Intel HEX file

```
@def(write hex)
	@put(write startup);
	@put(write data);
	@put(write finish);
@end(write hex)
```
* an Intel HEX file consists of three parts

```
@def(needed by write hex)
	void write_byte(
		std::ostream &out, int b
	) {
		@put(write byte);
	}
@end(needed by write hex)
```
* writes a single byte as a two-digit hexadecimal value

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
* 16 bit words are written in big endian format

```
@def(write startup)
	int base = 0x20010000;
	out << ":02000004";
	int sum = 0x06;
	sum += (base >> 24) + (base >> 16);
	write_word(out, base >> 16);
	write_byte(out, -sum);
	out << "\r\n";
@end(write startup)
```
* most significant 16 bits of the start address
* `sum` is a checksum
* only the last byte of it will be used

```
@def(write data)
	int len = s.code_size();
	int c = 0;
	for (; c < len; c += 4) {
		@put(write line);
		out << "\r\n";
	}
@end(write data)
```
* write lines of data

```
@def(write line)
	out << ':';
	int l =
		((c + 4 > len) ? (len - c) : 4);
	write_byte(out, l * 4);
	sum = l * 4;
	int addr = base + c * 4;
	sum += (addr >> 8) + addr;
	write_word(out, addr);
	out << "00";
@end(write line)
```
* each line starts with an address
* `sum` is again a byte checksum

```
@add(write line)
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
@end(write line)
```
* write four words per line
* for each word four bytes are written
* each line ends with a checksum

```
@def(write finish)
	sum =  9;
	out << ":04000005";
	write_word(out, base >> 16);
	write_word(out, base);
	sum += (base >> 24) + (base >> 16) +
		(base >> 8) + base;
	write_byte(out, -sum);
	out << "\r\n";
	out << ":00000001FF\r\n";
@end(write finish)
```
* signals end of Intel HEX file

```
@def(write byte)
	static const char map[] {
		"0123456789ABCDEF"
	};
	out << map[(b >> 4) & 0xf];
	out << map[b & 0xf];
@end(write byte)
```
* write a byte

