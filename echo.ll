	%t0 <- %pc + 104		# 2 words!
	%mtvec <- %t0
	%t0 <- %mhartid
loop_hart = *
	if %t0 != 0: %pc <- %pc + (loop_hart + -*)
	%a0 <- $10013000
	%t0 <- [%a0 + $08]
	%a1 <- $0d
	%a2 <- $0a
	%t1 <- [%a0 + $0c]
	%t0 <- %t0 or $01
	%t1 <- %t1 or $01
	[%a0 + $08] <- %t0
	[%a0 + $0c] <- %t1
read = *
	%t0 <- [%a0 + $04]
	if %t0 < 0: %pc <- %pc + (read + -*)
	%t0 <- %t0 and $ff
	if %t0 = 0: %pc <- %pc + (read + -*)
can_write = *
	%t1 <- [%a0]
	if %t1 < 0: %pc <- %pc + (can_write + -*)
	[%a0] <- %t0
	if %t0 != %a1: %pc <- %pc + (read + -*)
can_write_cr = *
	%t0 <- [%a0]
	if %t0 < 0: %pc <- %pc + (can_write_cr + -*)
	[%a0] <- %a2
	%pc <- %pc + (read + -*)
_dummy_irq = *
	%pc <- %pc + (_dummy_irq + -*) 
