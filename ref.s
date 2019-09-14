	.section .init
	.global _enter

_enter:
	sub a0, a1, a2
	and a0, a1, a2
	or a0, a1, a2
	xori a0, a1, 0xff
	xor a0, a1, a2
