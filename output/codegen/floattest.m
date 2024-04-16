align
main
sw 0(r14),r15
lw r2,24(r14)
lw r1, 12(r14)
mul r3,r1,r2
sw 16(r14),r3
lw r1,16(r14)
sw 4(r14),r1
lw r13, 4(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
lw r15, 0(r14)
jr r15



entry
addi r14,r0,topaddr
addi r14,r14,-32
jl r15,main
addi r14,r14,32
hlt



read
sw 0(r14),r15
addi r1,r0,0
getdigit
getc r2
ceqi r3,r2,10
subi r2,r2,48
bnz r3,done
muli r1,r1,10
add r1,r2,r1
j getdigit
done
add r13, r0, r1
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



parameterstorage res 512
indexstorage res 256 
