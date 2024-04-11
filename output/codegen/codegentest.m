align
main
sw 0(r14),r15
addi r2,r0,5 
sw 108(r14),r2
lw r2,108(r14)
sw 12(r14),r2
addi r2,r0,3 
sw 52(r14),r2
lw r2,52(r14)
sw 236(r14),r2
addi r2,r0,6 
sw 20(r14),r2
addi r2,r0,5 
sw 76(r14),r2
addi r2,r0,4 
sw 192(r14),r2
addi r2,r0,3 
sw 244(r14),r2
addi r2,r0,2 
sw 88(r14),r2
addi r2,r0,1 
sw 140(r14),r2
lw r3,88(r14)
lw r2, 140(r14)
add r4,r2,r3 %s operation add performed 
sw 144(r14),r4
lw r3,244(r14)
lw r2, 144(r14)
add r4,r2,r3 %s operation add performed 
sw 196(r14),r4
lw r3,192(r14)
lw r2, 196(r14)
add r4,r2,r3 %s operation add performed 
sw 100(r14),r4
lw r3,76(r14)
lw r2, 100(r14)
add r4,r2,r3 %s operation add performed 
sw 156(r14),r4
lw r3,20(r14)
lw r2, 156(r14)
add r4,r2,r3 %s operation add performed 
sw 208(r14),r4
lw r2,208(r14)
sw 12(r14),r2
lw r13, 12(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r2,r0,3 
sw 256(r14),r2
lw r3,256(r14)
lw r2, 12(r14)
mul r4,r2,r3 %s operation mul performed 
sw 92(r14),r4
lw r2,92(r14)
sw 12(r14),r2
addi r2,r0,4 
sw 36(r14),r2
addi r2,r0,2 
sw 152(r14),r2
addi r2,r0,1 
sw 204(r14),r2
lw r3,152(r14)
lw r2, 204(r14)
add r4,r2,r3 %s operation add performed 
sw 80(r14),r4
lw r3,80(r14)
lw r2, 12(r14)
mul r4,r2,r3 %s operation mul performed 
sw 132(r14),r4
lw r3,36(r14)
lw r2, 132(r14)
div r4,r2,r3 %s operation div performed 
sw 40(r14),r4
lw r13, 40(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r2,r0,1 
sw 0(r14),r2
addi r2,r0,2 
sw 0(r14),r2
lw r3,0(r14) %s relexpr
lw r2, 0(r14)
clt r4,r3,r2
sw 4(r14), r4
lw r2,4(r14)
bz r2,else101
addi r2,r0,1 
sw 68(r14),r2
lw r13, 68(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r2,r0,5 
sw 0(r14),r2
addi r2,r0,4 
sw 0(r14),r2
addi r2,r0,3 
sw 180(r14),r2
addi r2,r0,2 
sw 232(r14),r2
addi r2,r0,1 
sw 8(r14),r2
lw r3,232(r14)
lw r2, 8(r14)
mul r4,r2,r3 %s operation mul performed 
sw 64(r14),r4
lw r3,180(r14)
lw r2, 64(r14)
mul r4,r2,r3 %s operation mul performed 
sw 60(r14),r4
addi r5,r0,4
lw r5, indexstorage(r5) %s HERE1!!
add r5,r14,r5
lw r3, 0(r5)
lw r2, 60(r14)
mul r4,r2,r3 %s operation mul performed 
sw 120(r14),r4
addi r5,r0,0
lw r5, indexstorage(r5) %s HERE1!!
add r5,r14,r5
lw r3, 0(r5)
lw r2, 120(r14)
mul r4,r2,r3 %s operation mul performed 
sw 228(r14),r4
addi r2,r0,5 
sw 28(r14),r2
addi r2,r0,4 
sw 104(r14),r2
addi r2,r0,3 
sw 44(r14),r2
addi r2,r0,2 
sw 248(r14),r2
addi r2,r0,1 
sw 24(r14),r2
lw r3,24(r14)
lw r2, 236(r14)
add r4,r2,r3 %s operation add performed 
sw 268(r14),r4
lw r3,248(r14)
lw r2, 268(r14)
add r4,r2,r3 %s operation add performed 
sw 216(r14),r4
lw r3,44(r14)
lw r2, 216(r14)
add r4,r2,r3 %s operation add performed 
sw 160(r14),r4
lw r3,104(r14)
lw r2, 160(r14)
add r4,r2,r3 %s operation add performed 
sw 168(r14),r4
lw r3,28(r14)
lw r2, 168(r14)
add r4,r2,r3 %s operation add performed 
sw 116(r14),r4
lw r3,228(r14) %s relexpr
lw r2, 116(r14)
cgt r4,r3,r2
sw 260(r14), r4
lw r2,260(r14)
bz r2,else102
addi r2,r0,2 
sw 84(r14),r2
lw r13, 84(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif102
else102
addi r2,r0,3 
sw 136(r14),r2
lw r13, 136(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif102
j endif101
else101
addi r2,r0,2 
sw 184(r14),r2
lw r13, 184(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r14,r14,-8
jl r15,return2
addi r14,r14,8
sw 148(r14),r13
addi r14,r14,-8
jl r15,return1
addi r14,r14,8
sw 96(r14),r13
lw r3,236(r14)
lw r2, 12(r14)
mul r4,r2,r3 %s operation mul performed 
sw 220(r14),r4
lw r3,96(r14)
lw r2, 220(r14)
add r4,r2,r3 %s operation add performed 
sw 224(r14),r4
lw r3,148(r14)
lw r2, 224(r14)
add r4,r2,r3 %s operation add performed 
sw 172(r14),r4
lw r3,236(r14)
lw r2, 12(r14)
mul r4,r2,r3 %s operation mul performed 
sw 56(r14),r4
lw r3,172(r14) %s relexpr
lw r2, 56(r14)
cge r4,r3,r2
sw 264(r14), r4
lw r2,264(r14)
bz r2,else103
addi r2,r0,4 
sw 200(r14),r2
lw r13, 200(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif103
else103
addi r2,r0,5 
sw 252(r14),r2
lw r13, 252(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif103
endif101
addi r2,r0,1 
sw 240(r14),r2
lw r2,240(r14)
sw 48(r14),r2
while103
addi r2,r0,10 
sw 32(r14),r2
lw r3,48(r14) %s relexpr
lw r2, 32(r14)
cle r4,r3,r2
sw 112(r14), r4
lw r2, 112(r14)
bz r2,endwhile103
lw r13, 48(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
while104
addi r2,r0,10 
sw 16(r14),r2
lw r3,212(r14) %s relexpr
lw r2, 16(r14)
cle r4,r3,r2
sw 164(r14), r4
lw r2, 164(r14)
bz r2,endwhile104
lw r13, 212(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r2,r0,1 
sw 72(r14),r2
lw r3,72(r14)
lw r2, 212(r14)
add r4,r2,r3 %s operation add performed 
sw 124(r14),r4
lw r2,124(r14)
sw 212(r14),r2
j while104
endwhile104
addi r2,r0,1 
sw 188(r14),r2
lw r3,188(r14)
lw r2, 48(r14)
add r4,r2,r3 %s operation add performed 
sw 176(r14),r4
lw r2,176(r14)
sw 48(r14),r2
j while103
endwhile103
lw r15, 0(r14)
jr r15


align
foo
sw 0(r14),r15
addi r14,r14,-8
jl r15,bar
addi r14,r14,8
lw r15, 0(r14)
jr r15


align
bar
sw 0(r14),r15
addi r2,r0,10101 
sw 4(r14),r2
lw r13, 4(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
lw r15, 0(r14)
jr r15


align
return2
sw 0(r14),r15
addi r2,r0,2 
sw 4(r14),r2
lw r13,4(r14)
lw r15, 0(r14)
jr r15


align
return1
sw 0(r14),r15
addi r2,r0,1 
sw 4(r14),r2
lw r13,4(r14)
lw r15, 0(r14)
jr r15



entry
addi r14,r0,topaddr
addi r14,r14,-272
jl r15,main
addi r14,r14,272
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
