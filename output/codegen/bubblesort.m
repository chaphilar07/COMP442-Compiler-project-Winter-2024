align
main
sw 0(r14),r15
addi r1,r0,64 
sw 32(r14),r1
addi r1,r0,4 %s accumulator register is r1 
addi r2,r0,0 
sw 64(r14),r2
lw r2,64(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
lw r1,32(r14)
addi r2,r0,0
lw r2, indexstorage(r2) %s HERE !!
add r2,r14,r2
sw 0(r2),r1
addi r1,r0,34 
sw 80(r14),r1
addi r1,r0,4 %s accumulator register is r1 
addi r2,r0,1 
sw 72(r14),r2
lw r2,72(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
lw r1,80(r14)
addi r2,r0,0
lw r2, indexstorage(r2) %s HERE !!
add r2,r14,r2
sw 0(r2),r1
addi r1,r0,25 
sw 92(r14),r1
addi r1,r0,4 %s accumulator register is r1 
addi r2,r0,2 
sw 40(r14),r2
lw r2,40(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
lw r1,92(r14)
addi r2,r0,0
lw r2, indexstorage(r2) %s HERE !!
add r2,r14,r2
sw 0(r2),r1
addi r1,r0,12 
sw 56(r14),r1
addi r1,r0,4 %s accumulator register is r1 
addi r2,r0,3 
sw 48(r14),r2
lw r2,48(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
lw r1,56(r14)
addi r2,r0,0
lw r2, indexstorage(r2) %s HERE !!
add r2,r14,r2
sw 0(r2),r1
addi r1,r0,22 
sw 68(r14),r1
addi r1,r0,4 %s accumulator register is r1 
addi r2,r0,4 
sw 96(r14),r2
lw r2,96(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
lw r1,68(r14)
addi r2,r0,0
lw r2, indexstorage(r2) %s HERE !!
add r2,r14,r2
sw 0(r2),r1
addi r1,r0,11 
sw 76(r14),r1
addi r1,r0,4 %s accumulator register is r1 
addi r2,r0,5 
sw 60(r14),r2
lw r2,60(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
lw r1,76(r14)
addi r2,r0,0
lw r2, indexstorage(r2) %s HERE !!
add r2,r14,r2
sw 0(r2),r1
addi r1,r0,90 
sw 44(r14),r1
addi r1,r0,4 %s accumulator register is r1 
addi r2,r0,6 
sw 36(r14),r2
lw r2,36(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
lw r1,44(r14)
addi r2,r0,0
lw r2, indexstorage(r2) %s HERE !!
add r2,r14,r2
sw 0(r2),r1
addi r3,r0,7 
sw 88(r14),r3
addi r1,r0,0
lw r2, 88(r14)
sw parameterstorage(r1),r2
addi r1,r0,4
lw r2, 4(r14)
sw parameterstorage(r1),r2
addi r14,r14,-36
jl r15,printArray
addi r14,r14,36
addi r3,r0,7 
sw 52(r14),r3
addi r1,r0,0
lw r2, 52(r14)
sw parameterstorage(r1),r2
addi r1,r0,4
lw r2, 4(r14)
sw parameterstorage(r1),r2
addi r14,r14,-104
jl r15,bubbleSort
addi r14,r14,104
addi r3,r0,7 
sw 84(r14),r3
addi r1,r0,0
lw r2, 84(r14)
sw parameterstorage(r1),r2
addi r1,r0,4
lw r2, 4(r14)
sw parameterstorage(r1),r2
addi r14,r14,-36
jl r15,printArray
addi r14,r14,36
lw r15, 0(r14)
jr r15


align
printArray
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 8(r14),r2
addi r1,r0,4
lw r2, parameterstorage(r1)
sw 4(r14),r2
lw r1,8(r14)
sw 20(r14),r1
addi r1,r0,0 
sw 16(r14),r1
lw r1,16(r14)
sw 24(r14),r1
while100
lw r1, 20(r14)
lw r2,24(r14) %s relexpr
clt r3,r2,r1
sw 28(r14), r3
lw r1, 28(r14)
bz r1,endwhile100
addi r1,r0,4 %s accumulator register is r1 
lw r2,24(r14)
muli r2,r2,0
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
addi r1,r0,0
lw r1,indexstorage(r1)
add r1,r1,r14
lw r13,0(r1)
addi r14,r14,-8
jl r15, write
addi r14,r14,8
addi r1,r0,1 
sw 32(r14),r1
lw r2,32(r14)
lw r1, 24(r14)
add r3,r1,r2
sw 12(r14),r3
lw r1,12(r14)
sw 24(r14),r1
j while100
endwhile100
lw r15, 0(r14)
jr r15


align
bubbleSort
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 8(r14),r2
addi r1,r0,4
lw r2, parameterstorage(r1)
sw 4(r14),r2
lw r1,8(r14)
sw 60(r14),r1
addi r1,r0,0 
sw 100(r14),r1
lw r1,100(r14)
sw 80(r14),r1
addi r1,r0,0 
sw 84(r14),r1
lw r1,84(r14)
sw 36(r14),r1
addi r1,r0,0 
sw 40(r14),r1
lw r1,40(r14)
sw 96(r14),r1
while101
addi r1,r0,1 
sw 48(r14),r1
lw r2,48(r14)
lw r1, 60(r14)
sub r3,r1,r2
sw 68(r14),r3
lw r1, 68(r14)
lw r2,80(r14) %s relexpr
clt r3,r2,r1
sw 52(r14), r3
lw r1, 52(r14)
bz r1,endwhile101
while102
addi r1,r0,1 
sw 44(r14),r1
lw r2,80(r14)
lw r1, 60(r14)
sub r3,r1,r2
sw 92(r14),r3
lw r2,44(r14)
lw r1, 92(r14)
sub r3,r1,r2
sw 76(r14),r3
lw r1, 76(r14)
lw r2,36(r14) %s relexpr
clt r3,r2,r1
sw 32(r14), r3
lw r1, 32(r14)
bz r1,endwhile102
addi r1,r0,4 %s accumulator register is r1 
lw r2,36(r14)
muli r2,r2,0
add r1,r1,r2
addi r2, r0, 4 
sw indexstorage(r2), r1
addi r1,r0,4 %s accumulator register is r1 
addi r2,r0,1 
sw 64(r14),r2
lw r3,64(r14)
lw r2, 36(r14)
add r4,r2,r3
sw 20(r14),r4
lw r2,20(r14)
muli r2,r2,0
add r1,r1,r2
addi r2, r0, 8 
sw indexstorage(r2), r1
addi r4,r0,8
lw r4, indexstorage(r4) %s HERE1!!
add r4,r14,r4
lw r1, 0(r4)
addi r4,r0,4
lw r4, indexstorage(r4) %s HERE2 !!!
add r4,r14,r4
lw r2, 0(r4)
cgt r3,r2,r1
sw 72(r14), r3
lw r1,72(r14)
bz r1,else100
addi r1,r0,4 %s accumulator register is r1 
lw r2,36(r14)
muli r2,r2,0
add r1,r1,r2
addi r2, r0, 4 
sw indexstorage(r2), r1
addi r2,r0,4
lw r2, indexstorage(r2) %s here !!!
add r2,r14,r2
lw r1, 0(r2)
sw 96(r14),r1
addi r1,r0,4 %s accumulator register is r1 
addi r2,r0,1 
sw 12(r14),r2
lw r3,12(r14)
lw r2, 36(r14)
add r4,r2,r3
sw 56(r14),r4
lw r2,56(r14)
muli r2,r2,0
add r1,r1,r2
addi r2, r0, 4 
sw indexstorage(r2), r1
addi r1,r0,4 %s accumulator register is r1 
lw r2,36(r14)
muli r2,r2,0
add r1,r1,r2
addi r2, r0, 8 
sw indexstorage(r2), r1
addi r2,r0,8
lw r2, indexstorage(r2) %s here !!!
add r2,r14,r2
lw r1, 0(r2)
addi r2,r0,4
lw r2, indexstorage(r2) %s HERE !!
add r2,r14,r2
sw 0(r2),r1
addi r1,r0,4 %s accumulator register is r1 
lw r2,36(r14)
muli r2,r2,0
add r1,r1,r2
addi r2, r0, 4 
sw indexstorage(r2), r1
lw r1,96(r14)
addi r2,r0,4
lw r2, indexstorage(r2) %s HERE !!
add r2,r14,r2
sw 0(r2),r1
j endif100
else100
endif100
addi r1,r0,1 
sw 28(r14),r1
lw r2,28(r14)
lw r1, 36(r14)
add r3,r1,r2
sw 16(r14),r3
lw r1,16(r14)
sw 36(r14),r1
j while102
endwhile102
addi r1,r0,1 
sw 24(r14),r1
lw r2,24(r14)
lw r1, 80(r14)
add r3,r1,r2
sw 88(r14),r3
lw r1,88(r14)
sw 80(r14),r1
j while101
endwhile101
lw r15, 0(r14)
jr r15



entry
addi r14,r0,topaddr
addi r14,r14,-100
jl r15,main
addi r14,r14,100
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
