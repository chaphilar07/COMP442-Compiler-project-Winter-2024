align
main
sw 0(r14),r15
addi r14,r14,-8
jl r15,return1
addi r14,r14,8
sw 20(r14),r13
addi r14,r14,-8
jl r15,return2
addi r14,r14,8
sw 8(r14),r13
addi r14,r14,-8
jl r15,return3
addi r14,r14,8
sw 12(r14),r13
lw r2,8(r14)
lw r1, 12(r14)
add r3,r1,r2 %s operation add performed 
sw 24(r14),r3
lw r2,20(r14)
lw r1, 24(r14)
add r3,r1,r2 %s operation add performed 
sw 16(r14),r3
lw r1,16(r14)
sw 4(r14),r1
lw r13, 4(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
lw r15, 0(r14)
jr r15


align
rec
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 4(r14),r2
addi r1,r0,1 
sw 8(r14),r1
lw r2,4(r14) %s relexpr
lw r1, 8(r14)
cge r3,r2,r1
sw 12(r14), r3
lw r1,12(r14)
bz r1,else100
lw r13, 4(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif100
else100
endif100
lw r15, 0(r14)
jr r15


align
return3
sw 0(r14),r15
addi r1,r0,3 
sw 4(r14),r1
lw r13,4(r14)
lw r15, 0(r14)
jr r15


align
return2
sw 0(r14),r15
addi r1,r0,2 
sw 4(r14),r1
lw r13,4(r14)
lw r15, 0(r14)
jr r15


align
return1
sw 0(r14),r15
addi r1,r0,1 
sw 4(r14),r1
lw r13,4(r14)
lw r15, 0(r14)
jr r15



entry
addi r14,r0,topaddr
addi r14,r14,-28
jl r15,main
addi r14,r14,28
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
