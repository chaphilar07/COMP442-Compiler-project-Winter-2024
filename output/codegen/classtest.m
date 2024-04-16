align
main
sw 0(r14),r15
addi r1,r0,3 
sw 52(r14),r1
addi r1,r0,8 %s accumulator register is r1 
addi r2,r0,10 
sw 48(r14),r2
lw r2,48(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
