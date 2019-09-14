std::string setup {
	"%x0 <= @reg:0\n"
	"%x1 <= @reg:1\n"
	"%x2 <= @reg:2\n"
	"%x3 <= @reg:3\n"
	"%x4 <= @reg:4\n"
	"%x5 <= @reg:5\n"
	"%x6 <= @reg:6\n"
	"%x7 <= @reg:7\n"
	"%x8 <= @reg:8\n"
	"%x9 <= @reg:9\n"
	"%x10 <= @reg:10\n"
	"%x11 <= @reg:11\n"
	"%x12 <= @reg:12\n"
	"%x13 <= @reg:13\n"
	"%x14 <= @reg:14\n"
	"%x15 <= @reg:15\n"
	"%x16 <= @reg:16\n"
	"%x17 <= @reg:17\n"
	"%x18 <= @reg:18\n"
	"%x19 <= @reg:19\n"
	"%x20 <= @reg:20\n"
	"%x21 <= @reg:21\n"
	"%x22 <= @reg:22\n"
	"%x23 <= @reg:23\n"
	"%x24 <= @reg:24\n"
	"%x25 <= @reg:25\n"
	"%x26 <= @reg:26\n"
	"%x27 <= @reg:27\n"
	"%x28 <= @reg:28\n"
	"%x29 <= @reg:29\n"
	"%x30 <= @reg:30\n"
	"%x31 <= @reg:31\n"
	"%zero <= %x0\n"
	"%ra <= %x1\n"
	"%sp <= %x2\n"
	"%gp <= %x3\n"
	"%tp <= %x4\n"
	"%t0 <= %x5\n"
	"%t1 <= %x6\n"
	"%t2 <= %x7\n"
	"%fp <= %x8\n"
	"%s0 <= %x8\n"
	"%s1 <= %x9\n"
	"%a0 <= %x10\n"
	"%a1 <= %x11\n"
	"%a2 <= %x12\n"
	"%a3 <= %x13\n"
	"%a4 <= %x14\n"
	"%a5 <= %x15\n"
	"%a6 <= %x16\n"
	"%a7 <= %x17\n"
	"%s2 <= %x18\n"
	"%s3 <= %x19\n"
	"%s4 <= %x20\n"
	"%s5 <= %x21\n"
	"%s6 <= %x22\n"
	"%s7 <= %x23\n"
	"%s8 <= %x24\n"
	"%s9 <= %x25\n"
	"%s10 <= %x26\n"
	"%s11 <= %x27\n"
	"%t3 <= %x28\n"
	"%t4 <= %x29\n"
	"%t5 <= %x30\n"
	"%t6 <= %x31\n"
	"*) <= * )\n"
	":( <= : (\n"
	"(( <= ( (\n"
	")) <= ) )\n"
	"%mtvec <= @csr:$305\n"
	"%mhartid <= @csr:$f14\n"
	"(@num) <= @1\n"
	"@num << @num <= @arithmetic\n"
	"@num >> @num <= @arithmetic\n"
	"@num * @num <= @arithmetic\n"
	"@num / @num <= @arithmetic\n"
	"@num + @num <= @arithmetic\n"
	"@num - @num <= @arithmetic\n"
	"@num and @num <= @arithmetic\n"
	"@num or @num <= @arithmetic\n"
	"@num xor @num <= @arithmetic\n"
	"complement @num <= (@1 xor 0)\n"
	"r_type(@num, @reg, @reg, @num, @reg, @num) <= @raw: (@2 << 25 or @4:value << 20 or @6:value << 15 or @8 << 12 or @10:value << 7 or @12)\n"
	"i_type(@num, @reg, @num, @reg, @num) <= @raw: (@2 << 20 or @4:value << 15 or @6 << 12 or @8:value << 7 or @10)\n"
	"b_type(@num, @reg, @reg, @num, @num) <= @raw: ((@2 and $1000) << (31 - 12) or (@2 and $7e0) << (25 - 5) or @4:value << 20 or @6:value << 15 or @8 << 12 or (@2 and $1e) << (8 - 1) or (@2 and $800) >> (11 - 7) or @10)\n"
	"u_type(@num, @reg, @num) <= @raw: (@2 or @4:value << 7 or @6)\n"
	"j_type(@num, @reg, @num) <= @raw: ((@2 and $100000) << (31 - 20) or (@2 and $7fe) << (21 - 1) or (@2 and $800) << (20 - 11) or (@2 and $ff000) or @4:value << 7 or @6)\n"
	"s_type(@num, @reg, @reg, @num, @num) <= @raw: ((@2 and $fe0) << (25 - 5) or @4:value << 20 or @6:value << 15 or @8 << 12 or (@2 and $1f) << 7 or @10)\n"
	"@reg <- - @num <= @0 <- (0 - @3)\n"
	"@reg <- @num <= big_assign (@0, @2 and $fffff800) small_assign(@0, @2)\n"
	"big_assign(@reg, $00000000) <=\n"
	"big_assign(@reg, $fffff800) <=\n"
	"big_assign(@reg, @num) <= u_type(@4 and $fffff000, @2, $37)\n"
	"small_assign(@reg, 0) <= @2 <- %zero + 0\n"
	"small_assign(@reg, @num) <= small_masked_assign (@2, @4 and $fff)\n"
	"small_masked_assign(@reg, 0) <=\n"
	"small_masked_assign(@reg, @num) <= @2 <- %zero + @4\n"
	"@reg <- @reg + @reg <= r_type($0, @4, @2, $0, @0, $33)\n"
	"@reg <- @reg - @reg <= r_type($20, @4, @2, $0, @0, $33)\n"
	"@reg <- @reg + @num <= i_type(@4, @2, $0, @0, $13)\n"
	"@reg <- @reg - @num <= @0 <- @2 + (0 - @4)\n"
	"@reg <- @reg and @num <= i_type(@4, @2, $7, @0, $13)\n"
	"@reg <- @reg or @num <= i_type(@4, @2, $6, @0, $13)\n"
	"@reg <- @csr <= i_type(@2:value, %zero, $2, @0, $73)\n"
	"@csr <- @reg <= i_type(@0:value, @2, $1, %zero, $73)\n"
	"@reg <- [@reg + @num] <= i_type(@5, @3, $2, @0, $03)\n"
	"@reg <- [@reg] <= @0 <- [@3 + 0]\n"
	"@reg <- [@reg - @num] <= @0 <- [@3 + (0 - @5)]\n"
	"@reg <-b [@reg + @num] <= i_type(@6, @4, $0, @0, $03)\n"
	"@reg <-b [@reg] <= @0 <-b [@4 + 0]\n"
	"@reg <-b [@reg - @num] <= @0 <-b [@4 + (0 - @6)]\n"
	"@reg <-h [@reg + @num] <= i_type(@6, @4, $1, @0, $03)\n"
	"@reg <-h [@reg] <= @0 <-h [@4 + 0]\n"
	"@reg <-h [@reg - @num] <= @0 <-h [@4 + (0 - @6)]\n"
	"@reg <-bu [@reg + @num] <= i_type(@6, @4, $4, @0, $03)\n"
	"@reg <-bu [@reg] <= @0 <-bu [@4 + 0]\n"
	"@reg <-bu [@reg - @num] <= @0 <-bu [@4 + (0 - @6)]\n"
	"@reg <-hu [@reg + @num] <= i_type(@6, @4, $5, @0, $03)\n"
	"@reg <-hu [@reg] <= @0 <-hu [@4 + 0]\n"
	"@reg <-hu [@reg - @num] <= @0 <-hu [@4 + (0 - @6)]\n"
	"[@reg + @num] <- @reg <= s_type(@3, @6, @1, $2, $23)\n"
	"[@reg] <- @reg <= [@1 + 0] <- @4\n"
	"[@reg - @num] <- @reg <= [@1 + (0 - @3) <- @6\n"
	"[@reg + @num] <-b @reg <= s_type(@3, @7, @1, $0, $23)\n"
	"[@reg] <-b @reg <= [@1 + 0] <-b @5\n"
	"[@reg - @num] <-b @reg <= [@1 + (0 - @3) <-b @7\n"
	"[@reg + @num] <-h @reg <= s_type(@3, @7, @1, $1, $23)\n"
	"[@reg] <-h @reg <= [@1 + 0] <-h @5\n"
	"[@reg - @num] <-h @reg <= [@1 + (0 - @3) <-h @7\n"
	"goto @num <= %pc <- %pc + ( @1 - `* )\n"
	"if @reg @str 0: <= if @1 @2 %zero:\n"
	"if @reg < @reg: %pc <- %pc + @num <= b_type(@9, @3, @1, $4, $63)\n"
	"if @reg > @reg: <= if @3 < @1:\n"
	"if @reg >= @reg: %pc <- %pc + @num <= b_type(@9, @3, @1, $5, $63)\n"
	"if @reg `<= @reg: <= if @3 >= @1:\n"
	"if @reg <u @reg: %pc <- %pc + @num <= b_type(@9, @3, @1, $6, $63)\n"
	"if @reg >u @reg: <= if @3 <u @1:\n"
	"if @reg >=u @reg: %pc <- %pc + @num <= b_type(@9, @3, @1, $7, $63)\n"
	"if @reg `<=u @reg: <= if @3 >=u @1:\n"
	"if @reg = @reg: %pc <- %pc + @num <= b_type(@9, @3, @1, $0, $63)\n"
	"if @reg != @reg: %pc <- %pc + @num <= b_type(@9, @3, @1, $1, $63)\n"
	"%pc <- %pc - @num <= %pc <- %pc + (0 - @4)\n"
	"@reg <- %pc, %pc <- %pc + @num <= j_type(@8, @0, $6f)\n"
	"%pc <- %pc + @num <= %zero <- %pc, %pc <- %pc + @4\n"
	"%pc <- %pc <= %pc <- %pc + 0\n"
	"@reg <- %pc, %pc <- @reg + @num <= i_type(@8, @6, $0, @0, $67)\n"
	"@reg <- %pc, %pc <- @reg - @num <= @0 <- %pc, %pc <- @6 + (0 - @8)\n"
	"@reg <- %pc, %pc <- @reg <= @0 <- %pc, %pc <- @6 + 0\n"
	"%pc <- @reg <= %zero <- %pc, %pc <- @2 + 0\n"
	"@reg <- %pc + @num <= u_type(@4 and $fffff000, @0, $17) small_pc(@0, @4 and $fff)\n"
	"small_pc(@reg, 0) <=\n"
	"small_pc(@reg, @num) <= @2 <- @2 + @4\n"
	"@reg <- %pc <= @0 <- %pc + 0\n"
	"@reg <- @reg < @num <= i_type(@4, @2, $2, @0, $13)\n"
	"@reg <- @reg <u @num <= i_type(@5, @2, $3, @0, $13)\n"
	"@reg <- @reg xor @num <= i_type(@4, @2, $4, @0, $13)\n"
	"@reg <- @reg << @num <= i_type(@4 and $1f, @2, $1, @0, $13)\n"
	"@reg <- @reg >> @num <= i_type(@4 and $1f, @2, $5, @0, $13)\n"
	"@reg <- @reg >>> @num <= i_type((@4 and $1f) or $200, @2, $5, @0, $13)\n"
	"nop <= %x0 <- %x0 + 0\n"
	"@str: <= @0 <= `*\n"
	"raw @num <= @raw:@1\n"
	"@reg => @str <= @2 <= @0 `. @0\n"
};
