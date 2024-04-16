align
main
sw 0(r14),r15
addi r14,r14,-88
jl r15,read_write_arr
addi r14,r14,88
lw r15, 0(r14)
jr r15


align
read_write_arr
sw 0(r14),r15
addi r1,r0,0 
sw 84(r14),r1
lw r1,84(r14)
sw 72(r14),r1
while100
addi r1,r0,10 
sw 60(r14),r1
lw r2,72(r14) %s relexpr
lw r1, 60(r14)
clt r3,r2,r1
sw 48(r14), r3
lw r1, 48(r14)
bz r1,endwhile100
addi r1,r0,8 %s accumulator register is r1 
lw r2,72(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
addi r1,r0,0 %s the temp register is r1
lw r1,indexstorage(r1)
add r1,r1,r14
addi r14,r14,-4
jl r15,read
addi r14,r14,4
sw 0(r1),r13
addi r2,r0,1 
sw 76(r14),r2
lw r3,76(r14)
lw r2, 72(r14)
add r4,r2,r3
sw 80(r14),r4
lw r2,80(r14)
sw 72(r14),r2
j while100
endwhile100
addi r2,r0,0 
sw 52(r14),r2
lw r2,52(r14)
sw 72(r14),r2
while101
addi r2,r0,10 
sw 4(r14),r2
lw r3,72(r14) %s relexpr
lw r2, 4(r14)
clt r4,r3,r2
sw 68(r14), r4
lw r2, 68(r14)
bz r2,endwhile101
addi r2,r0,8 %s accumulator register is r2 
lw r3,72(r14)
muli r3,r3,4
add r2,r2,r3
addi r3, r0, 0 
sw indexstorage(r3), r2
addi r2,r0,0
lw r2,indexstorage(r2)
add r2,r2,r14
lw r13,0(r2)
addi r14,r14,-8
jl r15, write
addi r14,r14,8
addi r2,r0,1 
sw 56(r14),r2
lw r3,56(r14)
lw r2, 72(r14)
add r4,r2,r3
sw 64(r14),r4
lw r2,64(r14)
sw 72(r14),r2
j while101
endwhile101
lw r15, 0(r14)
jr r15



entry
addi r14,r0,topaddr
addi r14,r14,-4
jl r15,main
addi r14,r14,4
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
