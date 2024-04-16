align
main
sw 0(r14),r15
addi r1,r0,0 
sw 56(r14),r1
lw r1,56(r14)
sw 80(r14),r1
while1000000
addi r1,r0,10 
sw 72(r14),r1
lw r1, 72(r14)
lw r2,80(r14) %s relexpr
clt r3,r2,r1
sw 92(r14), r3
lw r1, 92(r14)
bz r1,endwhile1000000
addi r1,r0,1 
sw 84(r14),r1
lw r2,84(r14)
lw r1, 80(r14)
add r3,r1,r2
sw 64(r14),r3
addi r1,r0,4 %s accumulator register is r1 
lw r2,80(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
lw r1,64(r14)
addi r2,r0,0
lw r2, indexstorage(r2) %s HERE !!
add r2,r14,r2
sw 0(r2),r1
addi r1,r0,1 
sw 48(r14),r1
lw r2,48(r14)
lw r1, 80(r14)
add r3,r1,r2
sw 52(r14),r3
lw r1,52(r14)
sw 80(r14),r1
j while1000000
endwhile1000000
addi r1,r0,0 
sw 76(r14),r1
lw r1,76(r14)
sw 80(r14),r1
while1000001
addi r1,r0,10 
sw 88(r14),r1
lw r1, 88(r14)
lw r2,80(r14) %s relexpr
clt r3,r2,r1
sw 68(r14), r3
lw r1, 68(r14)
bz r1,endwhile1000001
addi r3,r0,4 %s accumulator register is r3 
lw r4,80(r14)
muli r4,r4,4
add r3,r3,r4
addi r4, r0, 0 
sw indexstorage(r4), r3
addi r3,r0,0
add r3,r14,r3
lw r3,0(r3)
addi r1,r0,0
sw parameterstorage(r1), r3
addi r14,r14,-8
jl r15,foo
addi r14,r14,8
addi r1,r0,1 
sw 60(r14),r1
lw r2,60(r14)
lw r1, 80(r14)
add r4,r1,r2
sw 44(r14),r4
lw r1,44(r14)
sw 80(r14),r1
j while1000001
endwhile1000001
lw r15, 0(r14)
jr r15


align
foo
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 4(r14),r2
lw r13, 4(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
lw r15, 0(r14)
jr r15



entry
addi r14,r0,topaddr
addi r14,r14,-96
jl r15,main
addi r14,r14,96
hlt



read
sw 0(r14),r15
addi r1,r0,0
getdigit
getc r2
ceqi r4,r2,10
subi r2,r2,48
bnz r4,done
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
addi r4,r0,1
mag
div r5,r1,r4
cgei r2,r5,10
bz r2,print
muli r4,r4,10
j mag
print
div r5,r1,r4
addi r5,r5,48
putc r5
subi r5,r5,48
mul r2,r5,r4
sub r1, r1, r2
divi r4,r4,10
ceqi r5,r4,0
bz r5, print
addi r6,r0, 10
putc r6
lw r15,0(r14)
jr r15



parameterstorage res 512
indexstorage res 256 
