
	
	OUTPUT_ARCH("riscv")

	ENTRY(_enter)

	MEMORY {
		ram (wxa!ri) : ORIGIN = 0x80000000, LENGTH = 0x4000
		flash (rxai!w) : ORIGIN = 0x20010000, LENGTH = 0x6a120
	}

	PHDRS {
		flash PT_LOAD;
		ram PT_NULL;
	}

	SECTIONS {
		
	.init : {
		*(.init)
	} >flash AT>flash :flash

	.init_array : {
		PROVIDE(_init_array_begin = .);
		KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)));
		PROVIDE(_init_array_end = .);
	} >flash AT>flash :flash

	. = ALIGN(8);
	.text : {
		*(.text .text*)
	} >flash AT>flash :flash

	.rodata : {
		*(.rodata)
		*(.sdata)
	} >flash AT>flash :flash

	.bss : {
		*(.data)
		PROVIDE(__global_pointer$ = . + 0x800);
		*(.bss)
	} >ram AT>ram :ram

	.stack : {
		. = . + 0x400;
		PROVIDE(_sp = .);
	} >ram AT>ram :ram

	}

