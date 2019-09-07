# canonical register names

	%x0 = @reg:0
	%x1 = @reg:1
	%x2 = @reg:2
	%x3 = @reg:3
	%x4 = @reg:4
	%x5 = @reg:5
	%x6 = @reg:6
	%x7 = @reg:7
	%x8 = @reg:8
	%x9 = @reg:9
	%x10 = @reg:10
	%x11 = @reg:11
	%x12 = @reg:12
	%x13 = @reg:13
	%x14 = @reg:14
	%x15 = @reg:15
	%x16 = @reg:16
	%x17 = @reg:17
	%x18 = @reg:18
	%x19 = @reg:19
	%x20 = @reg:20
	%x21 = @reg:21
	%x22 = @reg:22
	%x23 = @reg:23
	%x24 = @reg:24
	%x25 = @reg:25
	%x26 = @reg:26
	%x27 = @reg:27
	%x28 = @reg:28
	%x29 = @reg:29
	%x30 = @reg:30
	%x31 = @reg:31

# ABI register names

	%zero = %x0
	%ra = %x1
	%sp = %x2
	%gp = %x3
	%tp = %x4
	%t0 = %x5
	%t1 = %x6
	%t2 = %x7
	%fp = %x8
	%s0 = %x8
	%s1 = %x9
	%a0 = %x10
	%a1 = %x11
	%a2 = %x12
	%a3 = %x13
	%a4 = %x14
	%a5 = %x15
	%a6 = %x16
	%a7 = %x17
	%s2 = %x18
	%s3 = %x19
	%s4 = %x20
	%s5 = %x21
	%s6 = %x22
	%s7 = %x23
	%s8 = %x24
	%s9 = %x25
	%s10 = %x26
	%s11 = %x27
	%t3 = %x28
	%t4 = %x29
	%t5 = %x30
	%t6 = %x31

# CSR registers

	%mtvec = @csr:$305
	%mhartid = @csr:$f14

# arithemitc operations

	(@num) = @1
	@num << @num = @arithmetic
	@num >> @num = @arithmetic
	@num * @num = @arithmetic
	@num / @num = @arithmetic
	@num + @num = @arithmetic
	@num - @num = @arithmetic
	@num and @num = @arithmetic
	@num or @num = @arithmetic
	@num xor @num = @arithmetic
	complement @num = (@1 xor 0)

# splitting special chars sequences

	*) = * )
	:( = : (
	(( = ( (

# instruction types

	r_type(@num, @reg, @reg, @num, @reg, @num) = @raw: (@2 << 25 or @4:value << 20 or @6:value << 15 or @8 << 12 or @10:value << 7 or @12)
	i_type(@num, @reg, @num, @reg, @num) = @raw: (@2 << 20 or @4:value << 15 or @6 << 12 or @8:value << 7 or @10)
	b_type(@num, @reg, @reg, @num, @num) = @raw: ((@2 and $1000) << (31 - 12) or (@2 and $7e0) << (25 - 5) or @4:value << 20 or @6:value << 15 or @8 << 12 or (@2 and $1e) << (8 - 1) or (@2 and $800) >> (11 - 7) or @10)
	u_type(@num, @reg, @num) = @raw: (@2 or @4:value << 7 or @6)
	j_type(@num, @reg, @num) = @raw: ((@2 and $100000) << (31 - 20) or (@2 and $7fe) << (21 - 1) or (@2 and $800) << (20 - 11) or (@2 and $ff000) or @4:value << 7 or @6)

# instructions

	@reg <- @num = @0 big_add (@2 and $fffff800) @0 small_add @2
	@reg big_add $00000000 =
	@reg big_add $fffff800 =
	@reg big_add @num = u_type(@2 and $fffff000, @0, $37)
	@reg small_add 0 = @0 <- %zero + 0
	@reg small_add @num = @0 small_add_add (@2 and $fff)
	@reg small_add_add $0 =
	@reg small_add_add @num = @0 <- %zero + @2

	@reg <- @reg + @reg = r_type($0, @4, @2, $0, @0, $33)
	@reg <- @reg + @num = i_type(@4, @2, $0, @0, $13)
	@reg <- @reg - @num = @0 <- @2 + (0 - @4)
	@reg <- @reg and @num = i_type(@4, @2, $7, @0, $13)
	@reg <- @reg or @num = i_type(@4, @2, $6, @0, $13)
	@reg <- @csr = i_type(@2:value, %zero, $2, @0, $73)

	goto @num = %pc <- %pc + ( @1 - `* )

	if @reg @str 0: = if @1 @2 %zero:
	%pc <- %pc - @num = %pc <- %pc + (0 - @4)
	if @reg < @reg: %pc <- %pc + @num = b_type(@9, @3, @1, $4, $63)
	if @reg >= @reg: = if @3 < @1:
	if @reg == @reg: %pc <- %pc + @num = b_type(@9, @3, @1, $0, $63)
	if @reg != @reg: %pc <- %pc + @num = b_type(@9, @3, @1, $1, $63)

	%pc <- %pc + @num = j_type(@4, %zero, $6f)
	%pc <- %pc = %pc <- %pc + 0

# pseudo-instructions

	nop = %x0 <- %x0 + 0

# basic extensions
	
	@str: = @0 = `*
	raw @num = @raw:@1
