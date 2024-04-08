align
main
sw 0(r14),r15
addi r2,r0,5
sw 40(r14),r2
lw r4,40(r14)
add r3,r0,r4
sw 8(r14),r3
addi r5,r0,3
sw 4(r14),r5
lw r7,4(r14)
add r6,r0,r7
sw 176(r14),r6
addi r8,r0,1
sw 208(r14),r8
addi r9,r0,2
sw 36(r14),r9
lw r11,208(r14)
lw r10, 36(r14)
clt r12,r11,r10
sw 140(r14), r12
lw r1,140(r14)
bz r1,else100
addi r2,r0,1
sw 72(r14),r2
lw r13, 72(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r3,r0,1
sw 116(r14),r3
addi r4,r0,2
sw 160(r14),r4
lw r5,116(r14)
lw r6,160(r14)
mul r7,r5,r6
sw 180(r14),r7
addi r8,r0,3
sw 200(r14),r8
lw r9,180(r14)
lw r10,200(r14)
mul r11,r9,r10
sw 52(r14),r11
addi r12,r0,4
sw 132(r14),r12
lw r1,52(r14)
lw r2,132(r14)
mul r3,r1,r2
sw 96(r14),r3
addi r4,r0,5
sw 80(r14),r4
lw r5,96(r14)
lw r6,80(r14)
mul r7,r5,r6
sw 184(r14),r7
addi r8,r0,1
sw 112(r14),r8
lw r9,176(r14)
lw r10,112(r14)
add r11,r9,r10
sw 16(r14),r11
addi r12,r0,2
sw 68(r14),r12
lw r1,16(r14)
lw r2,68(r14)
add r3,r1,r2
sw 136(r14),r3
addi r4,r0,3
sw 196(r14),r4
lw r5,136(r14)
lw r6,196(r14)
add r7,r5,r6
sw 92(r14),r7
addi r8,r0,4
sw 156(r14),r8
lw r9,92(r14)
lw r10,156(r14)
add r11,r9,r10
sw 192(r14),r11
addi r12,r0,5
sw 164(r14),r12
lw r1,192(r14)
lw r2,164(r14)
add r3,r1,r2
sw 148(r14),r3
lw r5,184(r14)
lw r4, 148(r14)
cgt r6,r5,r4
sw 56(r14), r6
lw r7,56(r14)
bz r7,else101
addi r8,r0,2
sw 120(r14),r8
lw r13, 120(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif101
else101
addi r9,r0,3
sw 32(r14),r9
lw r13, 32(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif101
j endif100
else100
addi r10,r0,2
sw 204(r14),r10
lw r13, 204(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
lw r11,8(r14)
lw r12,176(r14)
mul r1,r11,r12
sw 24(r14),r1
addi r14,r14,-8
jl r15,return1
addi r14,r14,8
sw 44(r14),r13
lw r2,24(r14)
lw r3,44(r14)
add r4,r2,r3
sw 144(r14),r4
addi r14,r14,-8
jl r15,return2
addi r14,r14,8
sw 84(r14),r13
lw r5,144(r14)
lw r6,84(r14)
add r7,r5,r6
sw 100(r14),r7
lw r8,8(r14)
lw r9,176(r14)
mul r10,r8,r9
sw 20(r14),r10
lw r12,100(r14)
lw r11, 20(r14)
cge r1,r12,r11
sw 188(r14), r1
lw r2,188(r14)
bz r2,else102
addi r3,r0,4
sw 76(r14),r3
lw r13, 76(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif102
else102
addi r4,r0,5
sw 124(r14),r4
lw r13, 124(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif102
endif100
addi r5,r0,1
sw 172(r14),r5
lw r7,172(r14)
add r6,r0,r7
sw 28(r14),r6
while100
addi r8,r0,10
sw 168(r14),r8
lw r10,28(r14)
lw r9, 168(r14)
cle r11,r10,r9
sw 60(r14), r11
lw r12, 60(r14)
bz r12,endwhile100
lw r13, 28(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
while101
addi r1,r0,10
sw 64(r14),r1
lw r3,152(r14)
lw r2, 64(r14)
cle r4,r3,r2
sw 104(r14), r4
lw r5, 104(r14)
bz r5,endwhile101
lw r13, 152(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r6,r0,1
sw 108(r14),r6
lw r7,152(r14)
lw r8,108(r14)
add r9,r7,r8
sw 12(r14),r9
lw r11,12(r14)
add r10,r0,r11
sw 152(r14),r10
j while101
endwhile101
addi r12,r0,1
sw 128(r14),r12
lw r1,28(r14)
lw r2,128(r14)
add r3,r1,r2
sw 48(r14),r3
lw r5,48(r14)
add r4,r0,r5
sw 28(r14),r4
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
addi r6,r0,10101
sw 4(r14),r6
lw r13, 4(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
lw r15, 0(r14)
jr r15


align
return2
sw 0(r14),r15
addi r7,r0,2
sw 4(r14),r7
lw r13,4(r14)
lw r15, 0(r14)
jr r15


align
return1
sw 0(r14),r15
addi r8,r0,1
sw 4(r14),r8
lw r13,4(r14)
lw r15, 0(r14)
jr r15



entry
addi r14,r0,topaddr
addi r14,r14,-212
jl r15,main
addi r14,r14,212
hlt



read
sw 0(r14),r15
addi r9,r0,0
getdigit
getc r10
ceqi r11,r10,10
subi r10,r10,48
bnz r11,done
muli r9,r9,10
add r9,r10,r9
j getdigit
done
add r13, r0, r9
lw r15, 0(r14)
jr r15



align
write
sw 0(r14),r15
sw 4(r14), r13
lw r12, 4(r14)
addi r2,r0,1
mag
div r3,r12,r2
cgei r1,r3,10
bz r1,print
muli r2,r2,10
j mag
print
div r3,r12,r2
addi r3,r3,48
putc r3
subi r3,r3,48
mul r1,r3,r2
sub r12, r12, r1
divi r2,r2,10
ceqi r3,r2,0
bz r3, print
addi r4,r0, 10
putc r4
lw r15,0(r14)
jr r15



