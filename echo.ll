# init RISC-V

	%t0 <- %pc + 104
	%mtvec <- %t0
	%t0 <- %mhartid
loop_hart:
	if %t0 != 0: goto loop_hart

# init UART

	%a0 => uart <- $10013000
	%t0 <- [uart + $08]
	%a1 => ch_cr <- $0d
	%a2 => ch_nl <- $0a
	%t1 <- [uart + $0c]
	%t0 <- %t0 or $01
	%t1 <- %t1 or $01
	[uart + $08] <- %t0
	[uart + $0c] <- %t1
	uart_rd <== uart + $04
	uart_wr <== uart + $00

# read loop

read:
	%t0 <- [uart_rd]
	if %t0 < 0: goto read
	%t0 <- %t0 and $ff
	if %t0 = 0: goto read
can_write:
	%t1 <- [uart_wr]
	if %t1 < 0: goto can_write
	[uart_wr] <- %t0
	if %t0 != ch_cr: goto read

# write additional NL after CR

can_write_nl:
	%t0 <- [uart_wr]
	if %t0 < 0: goto can_write_nl
	[uart_wr] <- ch_nl
	goto read

# dummy interrupt handler

_dummy_irq:
	goto _dummy_irq
