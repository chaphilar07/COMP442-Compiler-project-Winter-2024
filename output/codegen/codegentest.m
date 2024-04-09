align
main
sw 0(r14),r15
addi r2,r0,5
sw 60(r14),r2
lw r4,60(r14)
add r3,r0,r4
sw 12(r14),r3
addi r5,r0,3
sw 8(r14),r5
lw r7,8(r14)
add r6,r0,r7
sw 240(r14),r6
addi r8,r0,1
sw 44(r14),r8
addi r9,r0,2
sw 276(r14),r9
lw r10,44(r14)
lw r11,276(r14)
add r12,r10,r11
sw 36(r14),r12
addi r1,r0,3
sw 64(r14),r1
lw r2,36(r14)
lw r3,64(r14)
add r4,r2,r3
sw 204(r14),r4
addi r5,r0,4
sw 124(r14),r5
lw r6,204(r14)
lw r7,124(r14)
add r8,r6,r7
sw 148(r14),r8
addi r9,r0,5
sw 116(r14),r9
lw r10,148(r14)
lw r11,116(r14)
add r12,r10,r11
sw 32(r14),r12
addi r1,r0,6
sw 180(r14),r1
lw r2,32(r14)
lw r3,180(r14)
add r4,r2,r3
sw 260(r14),r4
lw r6,260(r14)
add r5,r0,r6
sw 12(r14),r5
lw r13, 12(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r7,r0,3
sw 172(r14),r7
lw r8,12(r14)
lw r9,172(r14)
mul r10,r8,r9
sw 84(r14),r10
lw r12,84(r14)
add r11,r0,r12
sw 12(r14),r11
addi r1,r0,1
sw 268(r14),r1
addi r2,r0,2
sw 216(r14),r2
lw r3,268(r14)
lw r4,216(r14)
add r5,r3,r4
sw 136(r14),r5
lw r6,12(r14)
lw r7,136(r14)
mul r8,r6,r7
sw 264(r14),r8
addi r9,r0,4
sw 228(r14),r9
lw r10,264(r14)
lw r11,228(r14)
div r12,r10,r11
sw 208(r14),r12
lw r13, 208(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r1,r0,1
sw 56(r14),r1
addi r2,r0,2
sw 4(r14),r2
lw r4,56(r14)
lw r3, 4(r14)
clt r5,r4,r3
sw 196(r14), r5
lw r6,196(r14)
bz r6,else100
addi r7,r0,1
sw 104(r14),r7
lw r13, 104(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r8,r0,1
sw 48(r14),r8
addi r9,r0,2
sw 280(r14),r9
lw r10,48(r14)
lw r11,280(r14)
mul r12,r10,r11
sw 140(r14),r12
addi r1,r0,3
sw 232(r14),r1
lw r2,140(r14)
lw r3,232(r14)
mul r4,r2,r3
sw 76(r14),r4
addi r5,r0,4
sw 92(r14),r5
lw r6,76(r14)
lw r7,92(r14)
mul r8,r6,r7
sw 24(r14),r8
addi r9,r0,5
sw 156(r14),r9
lw r10,24(r14)
lw r11,156(r14)
mul r12,r10,r11
sw 252(r14),r12
addi r1,r0,1
sw 184(r14),r1
lw r2,240(r14)
lw r3,184(r14)
add r4,r2,r3
sw 88(r14),r4
addi r5,r0,2
sw 236(r14),r5
lw r6,88(r14)
lw r7,236(r14)
add r8,r6,r7
sw 152(r14),r8
addi r9,r0,3
sw 284(r14),r9
lw r10,152(r14)
lw r11,284(r14)
add r12,r10,r11
sw 20(r14),r12
addi r1,r0,4
sw 52(r14),r1
lw r2,20(r14)
lw r3,52(r14)
add r4,r2,r3
sw 72(r14),r4
addi r5,r0,5
sw 108(r14),r5
lw r6,72(r14)
lw r7,108(r14)
add r8,r6,r7
sw 200(r14),r8
lw r10,252(r14)
lw r9, 200(r14)
cgt r11,r10,r9
sw 248(r14), r11
lw r12,248(r14)
bz r12,else101
addi r1,r0,2
sw 168(r14),r1
lw r13, 168(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif101
else101
addi r2,r0,3
sw 220(r14),r2
lw r13, 220(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif101
j endif100
else100
addi r3,r0,2
sw 272(r14),r3
lw r13, 272(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
lw r4,12(r14)
lw r5,240(r14)
mul r6,r4,r5
sw 80(r14),r6
addi r14,r14,-8
jl r15,return1
addi r14,r14,8
sw 188(r14),r13
lw r7,80(r14)
lw r8,188(r14)
add r9,r7,r8
sw 144(r14),r9
addi r14,r14,-8
jl r15,return2
addi r14,r14,8
sw 120(r14),r13
lw r10,144(r14)
lw r11,120(r14)
add r12,r10,r11
sw 256(r14),r12
lw r1,12(r14)
lw r2,240(r14)
mul r3,r1,r2
sw 28(r14),r3
lw r5,256(r14)
lw r4, 28(r14)
cge r6,r5,r4
sw 192(r14), r6
lw r7,192(r14)
bz r7,else102
addi r8,r0,4
sw 164(r14),r8
lw r13, 164(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif102
else102
addi r9,r0,5
sw 100(r14),r9
lw r13, 100(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif102
endif100
addi r10,r0,1
sw 112(r14),r10
lw r12,112(r14)
add r11,r0,r12
sw 40(r14),r11
while100
addi r1,r0,10
sw 224(r14),r1
lw r3,40(r14)
lw r2, 224(r14)
cle r4,r3,r2
sw 132(r14), r4
lw r5, 132(r14)
bz r5,endwhile100
lw r13, 40(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
while101
addi r6,r0,10
sw 160(r14),r6
lw r8,212(r14)
lw r7, 160(r14)
cle r9,r8,r7
sw 68(r14), r9
lw r10, 68(r14)
bz r10,endwhile101
lw r13, 212(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r11,r0,1
sw 96(r14),r11
lw r12,212(r14)
lw r1,96(r14)
add r2,r12,r1
sw 16(r14),r2
lw r4,16(r14)
add r3,r0,r4
sw 212(r14),r3
j while101
endwhile101
addi r5,r0,1
sw 176(r14),r5
lw r6,40(r14)
lw r7,176(r14)
add r8,r6,r7
sw 244(r14),r8
lw r10,244(r14)
add r9,r0,r10
sw 40(r14),r9
j while100
endwhile100
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
addi r11,r0,10101
sw 4(r14),r11
lw r13, 4(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
lw r15, 0(r14)
jr r15


align
return2
sw 0(r14),r15
addi r12,r0,2
sw 4(r14),r12
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
addi r14,r14,-288
jl r15,main
addi r14,r14,288
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
lw r5, 4(r14)
addi r7,r0,1
mag
div r8,r5,r7
cgei r6,r8,10
bz r6,print
muli r7,r7,10
j mag
print
div r8,r5,r7
addi r8,r8,48
putc r8
subi r8,r8,48
mul r6,r8,r7
sub r5, r5, r6
divi r7,r7,10
ceqi r8,r7,0
bz r8, print
addi r9,r0, 10
putc r9
lw r15,0(r14)
jr r15



