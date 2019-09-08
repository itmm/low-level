	.section .init
	.global _enter

# init RISC-V

_enter:
	la t0, dummy_irq
	csrw mtvec, t0

	csrr t0, mhartid
loop_hart:
	bnez t0, loop_hart

# init UART

	li a0, 0x10013000
	lw t0, 0x08(a0)
	addi a1, x0, 0x0d
	addi a2, x0, 0x0a
	lw t1, 0x0c(a0)
	ori t0, t0, 0x01
	ori t1, t1, 0x01
	sw t0, 0x08(a0)
	sw t1, 0x0c(a0)

# read loop

read:
	lw t0, 0x04(a0)
	bltz t0, read
	andi t0, t0, 0xff
	beqz t0, read

can_write:
	lw t1, 0x00(a0)
	bltz t1, can_write
	sw t0, 0x00(a0)
	bne t0, a1, read

# write additional NL after CR

can_write_nl:
	lw t0, 0x00(a0)
	bltz t0, can_write_nl
	sw a2, 0x00(a0)
	j read

# dummy interrupt handler

dummy_irq:
	j dummy_irq
