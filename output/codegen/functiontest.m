align
main
sw 0(r14),r15
addi r4,r0,2 
sw 8(r14),r4
addi r2,r0,0
lw r3, 8(r14)
sw parameterstorage(r2),r3
addi r4,r0,10 
sw 4(r14),r4
addi r2,r0,4
lw r3, 4(r14)
sw parameterstorage(r2),r3
addi r14,r14,-16
jl r15,foo
addi r14,r14,16
sw 12(r14),r13
lw r13, 12(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r4,r0,2 
sw 8(r14),r4
addi r2,r0,0
lw r3, 8(r14)
sw parameterstorage(r2),r3
addi r4,r0,10 
sw 4(r14),r4
addi r2,r0,4
lw r3, 4(r14)
sw parameterstorage(r2),r3
addi r14,r14,-16
jl r15,foo
addi r14,r14,16
lw r15, 0(r14)
jr r15


align
foo
sw 0(r14),r15
addi r2,r0,0
lw r3, parameterstorage(r2)
sw 8(r14),r3
addi r2,r0,4
lw r3, parameterstorage(r2)
sw 4(r14),r3
lw r3,8(r14)
lw r2, 4(r14)
add r4,r2,r3 %s operation add performed 
sw 12(r14),r4
lw r13,12(r14)
lw r15, 0(r14)
jr r15



entry
addi r14,r0,topaddr
addi r14,r14,-16
jl r15,main
addi r14,r14,16
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
lw r2, 4(r14)
addi r4,r0,1
mag
div r5,r2,r4
cgei r3,r5,10
bz r3,print
muli r4,r4,10
j mag
print
div r5,r2,r4
addi r5,r5,48
putc r5
subi r5,r5,48
mul r3,r5,r4
sub r2, r2, r3
divi r4,r4,10
ceqi r5,r4,0
bz r5, print
addi r6,r0, 10
putc r6
lw r15,0(r14)
jr r15



parameterstorage res 512
indexstorage res 256 
