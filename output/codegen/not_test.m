align
main
sw 0(r14),r15
addi r3,r0,3 
sw 32(r14),r3
addi r1,r0,0
lw r2, 32(r14)
sw parameterstorage(r1),r2
addi r14,r14,-8
jl r15,print_not
addi r14,r14,8
addi r3,r0,0 
sw 8(r14),r3
addi r1,r0,0
lw r2, 8(r14)
sw parameterstorage(r1),r2
addi r14,r14,-8
jl r15,print_not
addi r14,r14,8
addi r3,r0,0 
sw 4(r14),r3
addi r1,r0,0
lw r2, 4(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 28(r14),r3
addi r1,r0,4
lw r2, 28(r14)
sw parameterstorage(r1),r2
addi r14,r14,-16
jl r15,x_or_not_y
addi r14,r14,16
sw 12(r14),r13
lw r13, 12(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r3,r0,0 
sw 4(r14),r3
addi r1,r0,0
lw r2, 4(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 28(r14),r3
addi r1,r0,4
lw r2, 28(r14)
sw parameterstorage(r1),r2
addi r14,r14,-16
jl r15,x_or_not_y
addi r14,r14,16
addi r3,r0,1 
sw 24(r14),r3
addi r1,r0,0
lw r2, 24(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 16(r14),r3
addi r1,r0,4
lw r2, 16(r14)
sw parameterstorage(r1),r2
addi r14,r14,-16
jl r15,x_or_not_y
addi r14,r14,16
sw 20(r14),r13
lw r13, 20(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r3,r0,1 
sw 24(r14),r3
addi r1,r0,0
lw r2, 24(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 16(r14),r3
addi r1,r0,4
lw r2, 16(r14)
sw parameterstorage(r1),r2
addi r14,r14,-16
jl r15,x_or_not_y
addi r14,r14,16
lw r15, 0(r14)
jr r15


align
x_or_not_y
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 8(r14),r2
addi r1,r0,4
lw r2, parameterstorage(r1)
sw 4(r14),r2
lw r1, 8(r14)
not r1,r1
sw 8(r14),r1
lw r3,8(r14)
lw r2, 4(r14)
or r4,r2,r3
sw 12(r14),r4
lw r13,12(r14)
lw r15, 0(r14)
jr r15


align
print_not
sw 0(r14),r15
addi r2,r0,0
lw r3, parameterstorage(r2)
sw 4(r14),r3
lw r2, 4(r14)
not r2,r2
sw 4(r14),r2
lw r13, 4(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
lw r15, 0(r14)
jr r15



entry
addi r14,r0,topaddr
addi r14,r14,-36
jl r15,main
addi r14,r14,36
hlt



read
sw 0(r14),r15
addi r3,r0,0
getdigit
getc r4
ceqi r5,r4,10
subi r4,r4,48
bnz r5,done
muli r3,r3,10
add r3,r4,r3
j getdigit
done
add r13, r0, r3
lw r15, 0(r14)
jr r15



align
write
sw 0(r14),r15
sw 4(r14), r13
lw r3, 4(r14)
addi r5,r0,1
mag
div r6,r3,r5
cgei r4,r6,10
bz r4,print
muli r5,r5,10
j mag
print
div r6,r3,r5
addi r6,r6,48
putc r6
subi r6,r6,48
mul r4,r6,r5
sub r3, r3, r4
divi r5,r5,10
ceqi r6,r5,0
bz r6, print
addi r7,r0, 10
putc r7
lw r15,0(r14)
jr r15



parameterstorage res 512
indexstorage res 256 
