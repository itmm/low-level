
# canonical register names

	%x0 <= @reg:0
	%x1 <= @reg:1
	%x2 <= @reg:2
	%x3 <= @reg:3
	%x4 <= @reg:4
	%x5 <= @reg:5
	%x6 <= @reg:6
	%x7 <= @reg:7
	%x8 <= @reg:8
	%x9 <= @reg:9
	%x10 <= @reg:10
	%x11 <= @reg:11
	%x12 <= @reg:12
	%x13 <= @reg:13
	%x14 <= @reg:14
	%x15 <= @reg:15
	%x16 <= @reg:16
	%x17 <= @reg:17
	%x18 <= @reg:18
	%x19 <= @reg:19
	%x20 <= @reg:20
	%x21 <= @reg:21
	%x22 <= @reg:22
	%x23 <= @reg:23
	%x24 <= @reg:24
	%x25 <= @reg:25
	%x26 <= @reg:26
	%x27 <= @reg:27
	%x28 <= @reg:28
	%x29 <= @reg:29
	%x30 <= @reg:30
	%x31 <= @reg:31

# ABI register names

	%zero <= %x0
	%ra <= %x1
	%sp <= %x2
	%gp <= %x3
	%tp <= %x4
	%t0 <= %x5
	%t1 <= %x6
	%t2 <= %x7
	%fp <= %x8
	%s0 <= %x8
	%s1 <= %x9
	%a0 <= %x10
	%a1 <= %x11
	%a2 <= %x12
	%a3 <= %x13
	%a4 <= %x14
	%a5 <= %x15
	%a6 <= %x16
	%a7 <= %x17
	%s2 <= %x18
	%s3 <= %x19
	%s4 <= %x20
	%s5 <= %x21
	%s6 <= %x22
	%s7 <= %x23
	%s8 <= %x24
	%s9 <= %x25
	%s10 <= %x26
	%s11 <= %x27
	%t3 <= %x28
	%t4 <= %x29
	%t5 <= %x30
	%t6 <= %x31

# splitting special chars sequences

	*) <= * )
	:( <= : (
	(( <= ( (
	)) <= ) )

# CSR registers

	%mtvec <= @csr:$305
	%mhartid <= @csr:$f14

# arithemitc operations

	(@num) <= @1
	@num << @num <= @arithmetic
	@num >> @num <= @arithmetic
	@num * @num <= @arithmetic
	@num / @num <= @arithmetic
	@num + @num <= @arithmetic
	@num - @num <= @arithmetic
	@num and @num <= @arithmetic
	@num or @num <= @arithmetic
	@num xor @num <= @arithmetic
	complement @num <= (@1 xor 0)

# instruction types

	r_type(@num, @reg, @reg, @num, @reg, @num) <= @raw: (@2 << 25 or @4:value << 20 or @6:value << 15 or @8 << 12 or @10:value << 7 or @12)
	i_type(@num, @reg, @num, @reg, @num) <= @raw: (@2 << 20 or @4:value << 15 or @6 << 12 or @8:value << 7 or @10)
	b_type(@num, @reg, @reg, @num, @num) <= @raw: ((@2 and $1000) << (31 - 12) or (@2 and $7e0) << (25 - 5) or @4:value << 20 or @6:value << 15 or @8 << 12 or (@2 and $1e) << (8 - 1) or (@2 and $800) >> (11 - 7) or @10)
	u_type(@num, @reg, @num) <= @raw: (@2 or @4:value << 7 or @6)
	j_type(@num, @reg, @num) <= @raw: ((@2 and $100000) << (31 - 20) or (@2 and $7fe) << (21 - 1) or (@2 and $800) << (20 - 11) or (@2 and $ff000) or @4:value << 7 or @6)
	s_type(@num, @reg, @reg, @num, @num) <= @raw: ((@2 and $fe0) << (25 - 5) or @4:value << 20 or @6:value << 15 or @8 << 12 or (@2 and $1f) << 7 or @10)

# instructions

	@reg <- - @num <= @0 <- (0 - @3)
	@reg <- @num <= big_assign (@0, @2 and $fffff800) small_assign(@0, @2)
	big_assign(@reg, $00000000) <=
	big_assign(@reg, $fffff800) <=
	big_assign(@reg, @num) <= u_type(@4 and $fffff000, @2, $37)
	small_assign(@reg, 0) <= @2 <- %zero + 0
	small_assign(@reg, @num) <= small_masked_assign (@2, @4 and $fff)
	small_masked_assign(@reg, 0) <=
	small_masked_assign(@reg, @num) <= @2 <- %zero + @4

	@reg <- @reg + @reg <= r_type($0, @4, @2, $0, @0, $33)
	@reg <- @reg - @reg <= r_type($20, @4, @2, $0, @0, $33)
	@reg <- @reg + @num <= i_type(@4, @2, $0, @0, $13)
	@reg <- @reg - @num <= @0 <- @2 + (0 - @4)
	@reg <- @reg and @num <= i_type(@4, @2, $7, @0, $13)
	@reg <- @reg or @num <= i_type(@4, @2, $6, @0, $13)
	@reg <- @csr <= i_type(@2:value, %zero, $2, @0, $73)
	@csr <- @reg <= i_type(@0:value, @2, $1, %zero, $73)

	@reg <- [@reg + @num] <= i_type(@5, @3, $2, @0, $03)
	@reg <- [@reg] <= @0 <- [@3 + 0]
	@reg <- [@reg - @num] <= @0 <- [@3 + (0 - @5)]
	@reg <-b [@reg + @num] <= i_type(@6, @4, $0, @0, $03)
	@reg <-b [@reg] <= @0 <-b [@4 + 0]
	@reg <-b [@reg - @num] <= @0 <-b [@4 + (0 - @6)]
	@reg <-h [@reg + @num] <= i_type(@6, @4, $1, @0, $03)
	@reg <-h [@reg] <= @0 <-h [@4 + 0]
	@reg <-h [@reg - @num] <= @0 <-h [@4 + (0 - @6)]
	@reg <-bu [@reg + @num] <= i_type(@6, @4, $4, @0, $03)
	@reg <-bu [@reg] <= @0 <-bu [@4 + 0]
	@reg <-bu [@reg - @num] <= @0 <-bu [@4 + (0 - @6)]
	@reg <-hu [@reg + @num] <= i_type(@6, @4, $5, @0, $03)
	@reg <-hu [@reg] <= @0 <-hu [@4 + 0]
	@reg <-hu [@reg - @num] <= @0 <-hu [@4 + (0 - @6)]

	[@reg + @num] <- @reg <= s_type(@3, @6, @1, $2, $23)
	[@reg] <- @reg <= [@1 + 0] <- @4
	[@reg - @num] <- @reg <= [@1 + (0 - @3) <- @6
	[@reg + @num] <-b @reg <= s_type(@3, @7, @1, $0, $23)
	[@reg] <-b @reg <= [@1 + 0] <-b @5
	[@reg - @num] <-b @reg <= [@1 + (0 - @3) <-b @7
	[@reg + @num] <-h @reg <= s_type(@3, @7, @1, $1, $23)
	[@reg] <-h @reg <= [@1 + 0] <-h @5
	[@reg - @num] <-h @reg <= [@1 + (0 - @3) <-h @7

	goto @num <= %pc <- %pc + ( @1 - `* )
	if @reg @str 0: <= if @1 @2 %zero:
	if @reg < @reg: %pc <- %pc + @num <= b_type(@9, @3, @1, $4, $63)
	if @reg > @reg: <= if @3 < @1:
	if @reg >= @reg: %pc <- %pc + @num <= b_type(@9, @3, @1, $5, $63)
	if @reg `<= @reg: <= if @3 >= @1:
	if @reg <u @reg: %pc <- %pc + @num <= b_type(@9, @3, @1, $6, $63)
	if @reg >u @reg: <= if @3 <u @1:
	if @reg >=u @reg: %pc <- %pc + @num <= b_type(@9, @3, @1, $7, $63)
	if @reg `<=u @reg: <= if @3 >=u @1:
	if @reg = @reg: %pc <- %pc + @num <= b_type(@9, @3, @1, $0, $63)
	if @reg != @reg: %pc <- %pc + @num <= b_type(@9, @3, @1, $1, $63)

	%pc <- %pc - @num <= %pc <- %pc + (0 - @4)
	@reg <- %pc, %pc <- %pc + @num <= j_type(@8, @0, $6f)
	%pc <- %pc + @num <= %zero <- %pc, %pc <- %pc + @4
	%pc <- %pc <= %pc <- %pc + 0
	@reg <- %pc, %pc <- @reg + @num <= i_type(@8, @6, $0, @0, $67)
	@reg <- %pc, %pc <- @reg - @num <= @0 <- %pc, %pc <- @6 + (0 - @8)
	@reg <- %pc, %pc <- @reg <= @0 <- %pc, %pc <- @6 + 0
	%pc <- @reg <= %zero <- %pc, %pc <- @2 + 0

	@reg <- %pc + @num <= u_type(@4 and $fffff000, @0, $17) small_pc(@0, @4 and $fff)
	small_pc(@reg, 0) <=
	small_pc(@reg, @num) <= @2 <- @2 + @4
	@reg <- %pc <= @0 <- %pc + 0

	@reg <- @reg < @num <= i_type(@4, @2, $2, @0, $13)
	@reg <- @reg <u @num <= i_type(@5, @2, $3, @0, $13)
	@reg <- @reg xor @num <= i_type(@4, @2, $4, @0, $13)
	@reg <- @reg < @reg <= r_type($0, @4, @2, $2, @0, $33)
	@reg <- @reg <u @reg <= r_type($0, @5, @2, $3, @0, $33)
	@reg <- @reg xor @reg <= r_type($0, @4, @2, $4, @0, $33)
	@reg <- complement @reg <= @0 <- @3 xor 0
	@reg <- @reg != 0 <= @0 <- %zero <u @2
	@reg <- @reg > @reg <= @0 <- @4 < @2
	@reg <- @reg >u @reg <= @0 <- @5 <u @2
	@reg <- @reg and @reg <= r_type($0, @4, @2, $f, @0, $33)
	@reg <- @reg or @reg <= r_type($0, @4, @2, $e, @0, $33)
	@reg <- @reg << @num <= i_type(@4 and $1f, @2, $1, @0, $13)
	@reg <- @reg >> @num <= i_type(@4 and $1f, @2, $5, @0, $13)
	@reg <- @reg >>> @num <= i_type((@4 and $1f) or $200, @2, $5, @0, $13)
	@reg <- @reg << @reg <= r_type($0, @4, @2, $1, @0, $33)
	@reg <- @reg >> @reg <= r_type($0, @4, @2, $5, @0, $33)
	@reg <- @reg >>> @reg <= r_type($20, @4, @2, $5, @0, $33)

	fence(@num, @num) <= i_type(@2 << 4 or @4, %zero, $0, %zero, $0f)
	fence.i <= i_type(0, %zero, $1, %zero, $0f)

	trap <= i_type($0, %zero, 0, %zero, $73)
	break <= i_type($1, %zero, 0, %zero, $73)

# pseudo-instructions

	nop <= %x0 <- %x0 + 0

# basic extensions
	
	@str: <= @0 <= `*
	raw @num <= @raw:@1
	@reg => @str <= @2 <= @0 `. @0
