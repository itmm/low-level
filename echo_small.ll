				%x5 <- %pc + 104 # 2 words!
				%mtvec <- %x5
$f14022f3	#	%x5 <- %mhartid
$00029063	#	if %x5 != 0: %pc <- %pc + 0
				%x10 <- $10013000
$00852283	#	%x5 <- [%x10 + $08]
				%x11 <- $0d
				%x12 <- $0a
$00c52303	#	%x6 <- [%x10 + $0c]
				%x5 <- %x5 | $01
				%x6 <- %x6 | $01
$00552423	#	[%x10 + $08] <- %x5
$00652623	#	[%x10 + $0c] <- %x6
$00452283	#	%x5 <- [%x10 + $04]
$fe02cee3	#	if %x5 < 0: %pc <- %pc - 4
				%x5 <- %x5 & $ff
$fe028ae3	#	if %t0 = 0: %pc <- %pc - 12
$00052303	#	%x6 <- [%x10]
$fe034ee3	#	if %x6 < 0: %pc <- %pc - 4
$00552023	#	[%x10] <- %x5
$feb292e3	#	if %x5 != %x11: %pc <- %pc - 28
$00052283	#	%x5 <- [%x10]
$fe02cee3	#	if %x5 < 0: %pc <- %pc - 4
$00c52023	#	[%x10] <- %x12
				%pc <- %pc + -44
				%pc <- %pc + 0
