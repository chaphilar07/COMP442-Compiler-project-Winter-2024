align
main
sw 0(r14),r15
addi r1,r0,3 
sw 108(r14),r1
lw r1,108(r14)
sw 80(r14),r1
addi r1,r0,4 
sw 76(r14),r1
lw r1,76(r14)
sw 84(r14),r1
lw r2,80(r14)
lw r1, 84(r14)
add r3,r1,r2
sw 92(r14),r3
lw r13, 92(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r1,r0,0 
sw 148(r14),r1
lw r1,148(r14)
sw 140(r14),r1
while1000000
addi r1,r0,10 
sw 112(r14),r1
lw r1, 112(r14)
lw r2,140(r14) %s relexpr
clt r3,r2,r1
sw 64(r14), r3
lw r1, 64(r14)
bz r1,endwhile1000000
addi r1,r0,1 
sw 132(r14),r1
lw r2,132(r14)
lw r1, 140(r14)
add r3,r1,r2
sw 164(r14),r3
addi r1,r0,12 %s accumulator register is r1 
lw r2,140(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
lw r1,164(r14)
addi r2,r0,0
lw r2, indexstorage(r2) %s HERE !!
add r2,r14,r2
sw 0(r2),r1
addi r1,r0,1 
sw 128(r14),r1
lw r2,128(r14)
lw r1, 140(r14)
add r3,r1,r2
sw 116(r14),r3
lw r1,116(r14)
sw 140(r14),r1
j while1000000
endwhile1000000
addi r1,r0,0 
sw 8(r14),r1
lw r1,8(r14)
sw 140(r14),r1
while1000001
addi r1,r0,10 
sw 120(r14),r1
lw r1, 120(r14)
lw r2,140(r14) %s relexpr
clt r3,r2,r1
sw 104(r14), r3
lw r1, 104(r14)
bz r1,endwhile1000001
addi r1,r0,12 %s accumulator register is r1 
lw r2,140(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
addi r1,r0,1 
sw 100(r14),r1
lw r2,100(r14)
lw r1, 140(r14)
add r3,r1,r2
sw 72(r14),r3
lw r1, 72(r14)
addi r4,r0,0
lw r4, indexstorage(r4) %s HERE2 !!!
add r4,r14,r4
lw r2, 0(r4)
ceq r3,r2,r1
sw 144(r14), r3
lw r1,144(r14)
bz r1,else1000000
addi r1,r0,1 
sw 136(r14),r1
lw r13, 136(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif1000000
else1000000
addi r1,r0,0 
sw 156(r14),r1
lw r13, 156(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif1000000
addi r1,r0,1 
sw 152(r14),r1
lw r2,152(r14)
lw r1, 140(r14)
add r3,r1,r2
sw 124(r14),r3
lw r1,124(r14)
sw 140(r14),r1
j while1000001
endwhile1000001
addi r1,r0,12 %s accumulator register is r1 
addi r2,r0,2 
sw 4(r14),r2
addi r2,r0,12 %s accumulator register is r2 
addi r3,r0,1 
sw 68(r14),r3
lw r3,68(r14)
muli r3,r3,4
add r2,r2,r3
addi r3, r0, 0 
sw indexstorage(r3), r2
lw r3,4(r14)
addi r5,r0,0
lw r5, indexstorage(r5) %s HERE2 !!!
add r5,r14,r5
lw r2, 0(r5)
mul r4,r2,r3
sw 56(r14),r4
addi r2,r0,2 
sw 60(r14),r2
addi r2,r0,12 %s accumulator register is r2 
addi r3,r0,0 
sw 88(r14),r3
lw r3,88(r14)
muli r3,r3,4
add r2,r2,r3
addi r3, r0, 0 
sw indexstorage(r3), r2
lw r3,60(r14)
addi r5,r0,0
lw r5, indexstorage(r5) %s HERE2 !!!
add r5,r14,r5
lw r2, 0(r5)
mul r4,r2,r3
sw 96(r14),r4
lw r3,56(r14)
lw r2, 96(r14)
add r4,r2,r3
sw 160(r14),r4
lw r2,160(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
addi r2,r0,0
lw r2, indexstorage(r2) %s here !!!
add r2,r14,r2
lw r1, 0(r2)
sw 52(r14),r1
lw r13, 52(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
lw r15, 0(r14)
jr r15



entry
addi r14,r0,topaddr
addi r14,r14,-168
jl r15,main
addi r14,r14,168
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
