
	
	* <- $20010000

	tmp = %t0
	tmp <- %pc
	tmp <- tmp + (early_trap - * + 4)
	%mtvec <- tmp

		tmp <- %mhartid
	block_other_harts = *
		if tmp != 0: \
			%pc <- %pc + \
				(block_other_harts - *)

	uart = %a0
	uart <- $1013000

	tmp <- [uart + $08]
	tmp <- tmp | $01
	[uart + $08] <- tmp

	tmp <- [uart + $0c]
	tmp <- tmp | $01
	[uart + $0c] <- tmp

	carriage_return = %a1
	carriage_return <- $0d
	new_line = %a2
	new_line <- $0a

	loop = *
		tmp <- [uart + $04]
		if tmp < 0: \
			%pc <- %pc + (loop - *)
		tmp <- tmp & $ff
		if tmp = 0: \
			%pc <- %pc + (loop - *)

	tmp2 = %t1
	normal = *
		tmp2 <- [uart]
		if tmp2 < 0: \
			%pc <- %pc + (normal - *)
		[uart] <- tmp

	if tmp <> carriage_return: \
		%pc <- %pc + (loop - *)

	do_new_line = *
		tmp <- [uart]
		if tmp < 0: \
			%pc <- \
				%pc + (do_new_line - *)
		[uart] <- new_line
		%pc <- %pc + (loop - *)

	early_trap = *
		%pc <- %pc + (early_trap - *)

