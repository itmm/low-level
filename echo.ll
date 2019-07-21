
	
	* <- $20010000

	tmp = %t0
	tmp <- %pc + (early_trap - *)
	%mtvec <- tmp

		tmp <- %mhartid
	block_other_harts = *
		if (tmp != 0) { \
			%pc <- %pc + \
				(block_other_harts - *) \
		}

	uart = %a4
	uart <- $1013000

	tmp <- [uart + $08]
	tmp <- tmp | $01
	[uart + $08] <- tmp

	tmp <- [uart + $0c]
	tmp <- tmp | $01
	[uart + $0c] <- tmp

	new_line = %a0
	new_line <- $0a
	carriage_return = %a1
	carriage_return <- $0c

		chr = %a2
	loop = *
		tmp <- [uart + $04]
		if (tmp < 0 { \
			%pc <- %pc + (loop - *) \
		}
		chr <- tmp & $ff
		if (chr = 0) { \
			%pc <- %pc + (loop - *) \
		}

	if (chr = new_line) { \
		%pc <- %pc + (do_new_line - *) \
	}

	normal = *
		tmp <- [uart]
		if (tmp < 0) { jump normal }
		[uart] <- chr
		%pc <- %pc + (loop - *)

	do_new_line = *
		tmp <- [uart]
		if (tmp < 0) { \
			jump do_new_line \
		}
		[uart] <- carriage_return
		%pc <- %pc + (normal - *)

	early_trap = *
		%pc <- %pc + (early_trap - *)

