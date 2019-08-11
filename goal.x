# Echo-Client in low-level
* complete source code of an echo client in low-level

This section makes no use of cascaded hex fragments.
It presents a small example of an low-level program as a linear listing
interrupted with some comments.

```
@Def(file: goal.ll)
	@put(src)
@End(file: goal.ll)
```
* the `src` fragment contains all the code

```
@def(src)
	@n(*) <- @n($20010000)
@end(src)
```
* sets start address
* the bootloader jumps to this address to start execution of the
  program
* low-levels allow changing `@n(*)` via an assignment
* hexadecimal numbers are prefixed with `@n($)`

```
@add(src)
	tmp = @k(%t0)
	tmp <- @k(%pc)
	tmp <- tmp + (@t(early_trap) - @n(*) + 4)
	@k(%mtvec) <- tmp
@end(src)
```
* sets the interrupt vector to the `@t(early_trap)` label
* so any interrupt will end in an infinite loop
* and will not produce some weird to debug behavior
* the address is calculated relative to `@k(%pc)`

```
@add(src)
		tmp <- @k(%mhartid)
	@t(block_other_harts) = @n(*)
		if tmp != 0: \
			@k(%pc) <- @k(%pc) + \
				(@t(block_other_harts) - @n(*))
@end(src)
```
* only continue if running on hart `0`
* other harts will stuck in an infinite loop
* so only hart is talking over the UART

```
@add(src)
	uart = @k(%a0)
	uart <- @n($1013000)
@end(src)
```
* store basis to IO block

```
@add(src)
	tmp <- [uart + @n($08)]
	tmp <- tmp | @n($01)
	[uart + @n($08)] <- tmp
@end(src)
```
* enable writing to UART

```
@add(src)
	tmp <- [uart + @n($0c)]
	tmp <- tmp | @n($01)
	[uart + @n($0c)] <- tmp
@end(src)
```
* enable reading from UART

```
@add(src)
	carriage_return = @k(%a1)
	carriage_return <- @n($0d)
	new_line = @k(%a2)
	new_line <- @n($0a)
@end(src)
```
* store ASCII new-line and carriage-return codes in registers for fast
  access

```
@add(src)
	@t(loop) = @n(*)
		tmp <- [uart + @n($04)]
		if tmp < 0: \
			@k(%pc) <- @k(%pc) + (@t(loop) - @n(*))
		tmp <- tmp & @n($ff)
		if tmp = 0: \
			@k(%pc) <- @k(%pc) + (@t(loop) - @n(*))
@end(src)
```
* wait until char is available and read char
* only continue if the char is not `0`

```
@add(src)
	tmp2 = @k(%t1)
	@t(normal) = @n(*)
		tmp2 <- [uart]
		if tmp2 < 0: \
			@k(%pc) <- @k(%pc) + (@t(normal) - *)
		[uart] <- tmp
@end(src)
```
* wait until UART is read for write
* and write char
* then wait for next char

```
@add(src)
	if tmp <> carriage_return: \
		@k(%pc) <- @k(%pc) + (@t(loop) - @n(*))
@end(src)
```
* special processing for new-line characters
* an carriage return is echoed first

```
@add(src)
	@t(do_new_line) = @n(*)
		tmp <- [uart]
		if tmp < 0: \
			@k(%pc) <- \
				@k(%pc) + (@t(do_new_line) - *)
		[uart] <- new_line
		@k(%pc) <- @k(%pc) + (@t(loop) - @n(*))
@end(src)
```
* when new-line char is read first write a carriage return
* then write the char read

```
@add(src)
	@t(early_trap) = @n(*)
		@k(%pc) <- @k(%pc) + (@t(early_trap) - @n(*))
@end(src)
```
* infinite loop to catch interrupts

