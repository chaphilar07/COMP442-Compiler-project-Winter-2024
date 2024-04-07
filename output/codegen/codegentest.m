align
main
sw 0(r14),r15
addi r14,r14,-4
jl r15,read
addi r14,r14,4
sw 4(r14),r13
lw r13, 4(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
lw r15, 0(r14)
jr r15



entry
addi r14,r0,topaddr
addi r14,r14,-8
jl r15,main
addi r14,r14,8
hlt




read
sw 0(r14),r15
addi r2,r0,0
getdigit
getc r3
ceqi r4,r3,10
subi r3,r3,48
bnz r4,done
muli r2,r2,10
add r2,r3,r2
j getdigit
done
add r13, r0, r2
lw r15, 0(r14)
jr r15




align
write
sw 0(r14),r15
sw 4(r14), r13
lw r5, 4(r14)
addi r7,r0,1
mag
div r8,r5,r7
cgei r6,r8,10
bz r6,print
muli r7,r7,10
j mag
print
div r8,r5,r7
addi r8,r8,48
putc r8
subi r8,r8,48
mul r6,r8,r7
sub r5, r5, r6
divi r7,r7,10
ceqi r8,r7,0
bz r8, print
lw r15,0(r14)
jr r15



