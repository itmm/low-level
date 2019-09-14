# init RISC-V

	%t0 <- %pc + 24 # _dummy_irq
	%mtvec <- %t0
	%t0 <- %mhartid
loop_hart:
	if %t0 != 0: goto loop_hart

	%pc <- %pc + 32 # setup

_dummy_irq:
	goto _dummy_irq

hello_msg:
	raw $6c6c6557
	raw $656d6f63
	raw $206f7420
	raw $0a0d6369
	raw 0

prompt_msg:
	raw $203e

# init UART

setup:
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

# startup
startup:
	%t1 <- %pc + (hello_msg - *)
write_startup:
	%t2 <-b [%t1]
	%t1 <- %t1 + 1
	if %t2 = 0: %pc <- %pc + 20 # end_startup
startup_ready:
	%t0 <- [uart_wr]
	if %t0 < 0: goto startup_ready
	[uart_wr] <- %t2
	goto write_startup
end_startup:

# read loop

prompt:
	%t1 <- %pc + (prompt_msg - *)
write_prompt:
	%t2 <-b [%t1]
	%t1 <- %t1 + 1
	if %t2 = 0: %pc <- %pc + 20 # end_prompt
prompt_ready:
	%t0 <- [uart_wr]
	if %t0 < 0: goto prompt_ready
	[uart_wr] <- %t2
	goto write_prompt
end_prompt:

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
	goto prompt

