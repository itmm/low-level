	.section .init
	.global _enter

_enter:
	la t0, dummy_irq
	csrw mtvec, t0

	csrr t0, mhartid
hart_loop:
	bnez t0, hart_loop

	lui a0, 0x10013

	lw t0, 0x08(a0)
	addi a1, x0, 0x0d
	addi a2, x0, 0x0a
	lw t1, 0x0c(a0)
	ori t0, t0, 0x01
	ori t1, t1, 0x01
	sw t0, 0x08(a0)
	sw t1, 0x0c(a0)

loop:
	lw t0, 0x04(a0)
	bltz t0, loop
	andi t0, t0, 0xff
	beqz t0, loop

write_out:
	lw t1, 0x00(a0)
	bltz t1, write_out
	sw t0, 0x00(a0)
	bne t0, a1, loop

newline:
	lw t0, 0x00(a0)
	bltz t0, newline
	sw a2, 0x00(a0)
	j loop

dummy_irq:
	j dummy_irq
