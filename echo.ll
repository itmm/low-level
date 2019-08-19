# init RISC-V
	%t0 <- %pc + 104		# 2 words!
	%mtvec <- %t0
	%t0 <- %mhartid
loop_hart = *
	if %t0 != 0: %pc <- %pc + (loop_hart + -*)

# goto LABEL = %pc <- %pc + (LABEL - *)
# R1 <- R2 or R3 = r_inst($2, num(R1), num(R2), $3, num(R3), $33)

# init UART
	uart = %a0
	uart <- $10013000
	%t0 <- [uart + $08]
	ch_cr = %a1
	ch_cr <- $0d
	ch_nl = %a2
	ch_nl <- $0a
	%t1 <- [uart + $0c]
	%t0 <- %t0 or $01
	%t1 <- %t1 or $01
	[uart + $08] <- %t0
	[uart + $0c] <- %t1
	uart_rd = uart + $04
	uart_wr = uart + $00

# read loop
read = *
	%t0 <- [uart_rd]
	if %t0 < 0: %pc <- %pc + (read + -*)
	%t0 <- %t0 and $ff
	if %t0 = 0: %pc <- %pc + (read + -*)
can_write = *
	%t1 <- [uart_wr]
	if %t1 < 0: %pc <- %pc + (can_write + -*)
	[uart + $00] <- %t0
	if %t0 != ch_cr: %pc <- %pc + (read + -*)

#write additional NL after CR
can_write_nl = *
	%t0 <- [uart_wr]
	if %t0 < 0: %pc <- %pc + (can_write_nl + -*)
	[uart + $00] <- ch_nl
	%pc <- %pc + (read + -*)

# dummy interrupt handler
_dummy_irq = *
	%pc <- %pc + (_dummy_irq + -*) 
