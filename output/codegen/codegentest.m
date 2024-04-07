align
main
sw 0(r14),r15
addi r2,r0,5
sw 20(r14),r2
lw r4,20(r14)
add r3,r0,r4
sw 8(r14),r3
addi r5,r0,3
sw 4(r14),r5
lw r7,4(r14)
add r6,r0,r7
sw 64(r14),r6
addi r14,r14,-8
jl r15,return1
addi r14,r14,8
sw 24(r14),r13
addi r14,r14,-8
jl r15,return2
addi r14,r14,8
sw 32(r14),r13
lw r8,24(r14)
lw r9,32(r14)
add r10,r8,r9
sw 12(r14),r10
addi r11,r0,3
sw 28(r14),r11
lw r12,12(r14)
lw r1,28(r14)
add r2,r12,r1
sw 68(r14),r2
lw r4,68(r14)
add r3,r0,r4
sw 36(r14),r3
addi r14,r14,-8
jl r15,return1
addi r14,r14,8
addi r14,r14,-8
jl r15,return2
addi r14,r14,8
lw r13, 36(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r14,r14,-4
jl r15,foo
addi r14,r14,4
addi r5,r0,1
sw 44(r14),r5
lw r13, 44(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r6,r0,2
sw 16(r14),r6
lw r13, 16(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
lw r15, 0(r14)
jr r15


align
foo
sw 0(r14),r15
addi r14,r14,-8
jl r15,bar
addi r14,r14,8
lw r15, 0(r14)
jr r15


align
bar
sw 0(r14),r15
addi r7,r0,10101
sw 4(r14),r7
lw r13, 4(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
lw r15, 0(r14)
jr r15


align
return2
sw 0(r14),r15
addi r8,r0,2
sw 4(r14),r8
lw r13,4(r14)
lw r15, 0(r14)
jr r15


align
return1
sw 0(r14),r15
addi r9,r0,1
sw 4(r14),r9
lw r13,4(r14)
lw r15, 0(r14)
jr r15



entry
addi r14,r0,topaddr
addi r14,r14,-72
jl r15,main
addi r14,r14,72
hlt



read
sw 0(r14),r15
addi r10,r0,0
getdigit
getc r11
ceqi r12,r11,10
subi r11,r11,48
bnz r12,done
muli r10,r10,10
add r10,r11,r10
j getdigit
done
add r13, r0, r10
lw r15, 0(r14)
jr r15



align
write
sw 0(r14),r15
sw 4(r14), r13
lw r1, 4(r14)
addi r3,r0,1
mag
div r4,r1,r3
cgei r2,r4,10
bz r2,print
muli r3,r3,10
j mag
print
div r4,r1,r3
addi r4,r4,48
putc r4
subi r4,r4,48
mul r2,r4,r3
sub r1, r1, r2
divi r3,r3,10
ceqi r4,r3,0
bz r4, print
addi r5,r0, 10
putc r5
lw r15,0(r14)
jr r15



