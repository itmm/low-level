	%t0 <- %pc + 104		# 2 words!
	%mtvec <- %t0
	%t0 <- %mhartid
	if %t0 != 0: %pc <- %pc + 0
	%a0 <- $10013000
	%t0 <- [%a0 + $08]
	%a1 <- $0d
	%a2 <- $0a
	%t1 <- [%a0 + $0c]
	%t0 <- %t0 or $01
	%t1 <- %t1 or $01
	[%a0 + $08] <- %t0
	[%a0 + $0c] <- %t1
	%t0 <- [%a0 + $04]
	if %t0 < 0: %pc <- %pc + -4
	%t0 <- %t0 and $ff
	if %t0 = 0: %pc <- %pc + -12
	%t1 <- [%a0]
	if %t1 < 0: %pc <- %pc + -4
	[%a0] <- %t0
	if %t0 != %a1: %pc <- %pc + -28
	%t0 <- [%a0]
	if %t0 < 0: %pc <- %pc + -4
	[%a0] <- %a2
	%pc <- %pc + -44
	%pc <- %pc + 0
