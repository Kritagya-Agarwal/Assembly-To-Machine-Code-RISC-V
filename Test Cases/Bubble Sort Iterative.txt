.data
var1: .word 11 23 45 5 61 5 66
var2: .word 7
.text
la x4 var1
lw x5 var2
addi x6 x6 0
addi x7 x5 -1

loop1: 
beq x6 x7 exit
sub x8 x7 x6
addi x9 x0 0
loop2:
beq x9 x8 next
slli x10 x9 2
add x10 x10 x4
lw x11 0(x10)
addi x12 x10 4
lw x13 0(x12)
blt x13 x11 swap
addi x9 x9 1
jal x0 loop2
swap:
sw x11 0(x12)
sw x13 0(x10)
addi x9 x9 1
jal x0 loop2
next:
addi x6 x6 1
jal x0 loop1

exit:#FALL_THRU