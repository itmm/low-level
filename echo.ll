# init RISC-V

	raw $00000297
	raw $06828293		#		%t0 <- %pc + 104
	raw $30529073		#		%mtvec <- %t0
	%t0 <- %mhartid
loop_hart:
	if %t0 != 0: goto loop_hart

# init UART

	uart = %a0
	uart <- $10013000
	raw $00852283		#		%t0 <- [uart + $08]
	ch_cr = %a1
	ch_cr <- $0d
	ch_nl = %a2
	ch_nl <- $0a
	raw $00c52303		#		%t1 <- [uart + $0c]
	%t0 <- %t0 or $01
	%t1 <- %t1 or $01
	raw $00552423		#		[uart + $08] <- %t0
	raw $00652623		#		[uart + $0c] <- %t1
	uart_rd = uart + $04
	uart_wr = uart + $00

# read loop

read:
	raw $00452283		#		%t0 <- [uart_rd]
	if %t0 < 0: goto read
	%t0 <- %t0 and $ff
	if %t0 == 0: goto read
can_write:
	raw $00052303		#		%t1 <- [uart_wr]
	if %t1 < 0: goto can_write
	raw $00552023		#		[uart_wr] <- %t0
	if %t0 != ch_cr: goto read

# write additional NL after CR

can_write_nl:
	raw $00052283		#		%t0 <- [uart_wr]
	if %t0 < 0: goto can_write_nl
	raw $00c52023		#		[uart_wr] <- ch_nl
	goto read

# dummy interrupt handler

_dummy_irq:
	goto _dummy_irq
