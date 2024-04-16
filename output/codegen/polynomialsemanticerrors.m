align
main
sw 0(r14),r15
lw r1,180(r14)
sw 180(r14),r1
addi r3,r0,3.5 
sw 164(r14),r3
addi r1,r0,0
lw r2, 164(r14)
sw parameterstorage(r1),r2
addi r3,r0,2 
sw 108(r14),r3
addi r1,r0,4
lw r2, 108(r14)
sw parameterstorage(r1),r2
addi r14,r14,-36
jl r15,LINEAR_build
addi r14,r14,36
lw r1,44(r14)
sw 44(r14),r1
addi r3,r0,0.0 
sw 36(r14),r3
addi r1,r0,0
lw r2, 36(r14)
sw parameterstorage(r1),r2
addi r3,r0,1.0 
sw 260(r14),r3
addi r1,r0,4
lw r2, 260(r14)
sw parameterstorage(r1),r2
addi r3,r0,2.0 
sw 24(r14),r3
lw r3,24(r14)
muli r3,r3,-1
sw 24(r14),r3
addi r1,r0,8
lw r2, 24(r14)
sw parameterstorage(r1),r2
addi r14,r14,-72
jl r15,QUADRATIC_build
addi r14,r14,72
addi r1,r0,1 
sw 160(r14),r1
lw r1,160(r14)
sw 144(r14),r1
lw r1,144(r14)
sw 220(r14),r1
lw r1,24(r14)
sw 220(r14),r1
addi r3,r0,1 
sw 248(r14),r3
addi r1,r0,0
lw r2, 248(r14)
sw parameterstorage(r1),r2
addi r14,r14,-12
jl r15,f
addi r14,r14,12
addi r3,r0,1 
sw 176(r14),r3
addi r1,r0,0
lw r2, 176(r14)
sw parameterstorage(r1),r2
addi r14,r14,-12
jl r15,f
addi r14,r14,12
addi r1,r0,1 
sw 224(r14),r1
addi r1,r0,196 %s accumulator register is r1 
addi r2,r0,2 
sw 104(r14),r2
lw r2, 104(r14)
addi r3,r0,12 %s problem here
mul r2,r2,r3
add r1,r1,r2
addi r2,r0,1 
sw 156(r14),r2
lw r2,156(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
lw r1,224(r14)
addi r2,r0,0
lw r2, indexstorage(r2) %s HERE !!
add r2,r14,r2
sw 0(r2),r1
addi r1,r0,2 
sw 32(r14),r1
addi r1,r0,196 %s accumulator register is r1 
addi r2,r0,2 
sw 236(r14),r2
lw r2, 236(r14)
addi r3,r0,12 %s problem here
mul r2,r2,r3
add r1,r1,r2
addi r2,r0,1.3 
sw 272(r14),r2
lw r2,272(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
lw r1,32(r14)
addi r2,r0,0
lw r2, indexstorage(r2) %s HERE !!
add r2,r14,r2
sw 0(r2),r1
addi r1,r0,0
lw r2, 68(r14)
sw parameterstorage(r1),r2
addi r14,r14,-28
jl r15,f3
addi r14,r14,28
addi r1,r0,1 
sw 148(r14),r1
lw r1,148(r14)
sw -1(r14),r1
while100
addi r1,r0,10 
sw 268(r14),r1
lw r1, 268(r14)
lw r2,220(r14) %s relexpr
cle r3,r2,r1
sw 140(r14), r3
lw r1, 140(r14)
bz r1,endwhile100
lw r13, 220(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
lw r13, 180(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r1,r0,0
lw r2, 220(r14)
sw parameterstorage(r1),r2
addi r14,r14,-36
jl r15,LINEAR_evaluate
addi r14,r14,36
lw r13, 44(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r1,r0,0
lw r2, 220(r14)
sw parameterstorage(r1),r2
addi r14,r14,-36
jl r15,QUADRATIC_evaluate
addi r14,r14,36
j while100
endwhile100
lw r15, 0(r14)
jr r15


align
f3
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 4(r14),r2
align
f
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 4(r14),r2
align
f
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 4(r14),r2
align
f
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 4(r14),r2
align
QUADRATIC_evaluate
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 4(r14),r2
addi r2,r0,0
lw r2, indexstorage(r2) %s here !!!
add r2,r14,r2
lw r1, 0(r2)
sw 28(r14),r1
lw r2,4(r14)
lw r1, 28(r14)
mul r3,r1,r2
sw 24(r14),r3
lw r2,16(r14)
lw r1, 24(r14)
add r3,r1,r2
sw 20(r14),r3
lw r1,20(r14)
sw 28(r14),r1
lw r2,4(r14)
lw r1, 28(r14)
mul r3,r1,r2
sw 16(r14),r3
lw r2,8(r14)
lw r1, 16(r14)
add r3,r1,r2
sw 12(r14),r3
lw r1,12(r14)
sw 28(r14),r1
lw r13,28(r14)
lw r15, 0(r14)
jr r15
lw r15, 0(r14)
jr r15


align
QUADRATIC_build
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 4(r14),r2
addi r1,r0,4
lw r2, parameterstorage(r1)
sw 12(r14),r2
addi r1,r0,8
lw r2, parameterstorage(r1)
sw 20(r14),r2
addi r1,r0,3 
sw 60(r14),r1
lw r2,60(r14)
lw r1, 12(r14)
mul r3,r1,r2
sw 68(r14),r3
lw r2,68(r14)
lw r1, 20(r14)
add r3,r1,r2
sw 64(r14),r3
lw r1,64(r14)
sw 36(r14),r1
lw r1,12(r14)
sw 52(r14),r1
addi r1,r0,1 
sw 28(r14),r1
lw r1,28(r14)
sw 44(r14),r1
addi r1,r0,1 
sw 32(r14),r1
lw r13,32(r14)
lw r15, 0(r14)
jr r15
lw r15, 0(r14)
jr r15


align
LINEAR_build
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 4(r14),r2
addi r1,r0,4
lw r2, parameterstorage(r1)
sw 12(r14),r2
lw r1,12(r14)
sw 20(r14),r1
lw r1,4(r14)
sw 28(r14),r1
lw r13,20(r14)
lw r15, 0(r14)
jr r15
lw r15, 0(r14)
jr r15


align
LINEAR_evaluate
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 4(r14),r2
addi r1,r0,0.0 
sw 24(r14),r1
lw r1,24(r14)
sw 16(r14),r1
lw r2,4(r14)
addi r4,r0,0
lw r4, indexstorage(r4) %s HERE2 !!!
add r4,r14,r4
lw r1, 0(r4)
mul r3,r1,r2
sw 12(r14),r3
lw r2,8(r14)
lw r1, 12(r14)
add r3,r1,r2
sw 32(r14),r3
lw r1,32(r14)
sw 16(r14),r1
lw r13,16(r14)
lw r15, 0(r14)
jr r15
lw r15, 0(r14)
jr r15


align
POLYNOMIAL_evaluate
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 4(r14),r2
addi r1,r0,0 
sw 12(r14),r1
lw r13,12(r14)
lw r15, 0(r14)
jr r15
lw r15, 0(r14)
jr r15



entry
addi r14,r0,topaddr
addi r14,r14,-280
jl r15,main
addi r14,r14,280
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
