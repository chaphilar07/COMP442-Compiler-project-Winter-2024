align
main
sw 0(r14),r15
addi r14,r14,-8
jl r15,return3
addi r14,r14,8
sw 64(r14),r13
addi r2,r0,1
sw 84(r14),r2
lw r3,64(r14)
lw r4,84(r14)
add r5,r4,r3
sw 36(r14),r5
addi r6,r0,2
sw 108(r14),r6
lw r7,36(r14)
lw r8,108(r14)
add r9,r8,r7
sw 16(r14),r9
addi r10,r0,3
sw 24(r14),r10
lw r11,16(r14)
lw r12,24(r14)
add r1,r12,r11
sw 92(r14),r1
addi r2,r0,4
sw 100(r14),r2
lw r3,92(r14)
lw r4,100(r14)
add r5,r4,r3
sw 60(r14),r5
addi r6,r0,5
sw 40(r14),r6
addi r7,r0,7
sw 68(r14),r7
lw r8,40(r14)
lw r9,68(r14)
mul r10,r9,r8
sw 12(r14),r10
lw r11,60(r14)
lw r12,12(r14)
add r1,r12,r11
sw 116(r14),r1
lw r3,116(r14)
add r2,r0,r3
sw 112(r14),r2
addi r14,r14,-8
jl r15,return3
addi r14,r14,8
addi r14,r14,-12
jl r15,foo
addi r14,r14,12
addi r14,r14,-12
jl r15,bar
addi r14,r14,12
lw r15, 0(r14)
jr r15


align
return3
sw 0(r14),r15
addi r4,r0,3
sw 4(r14),r4
lw r13,4(r14)
lw r15, 0(r14)
jr r15


align
bar
sw 0(r14),r15
addi r14,r14,-12
jl r15,foo
addi r14,r14,12
addi r5,r0,4
sw 8(r14),r5
lw r7,8(r14)
add r6,r0,r7
sw 4(r14),r6
lw r15, 0(r14)
jr r15


align
foo
sw 0(r14),r15
addi r8,r0,3
sw 8(r14),r8
lw r10,8(r14)
add r9,r0,r10
sw 4(r14),r9
lw r15, 0(r14)
jr r15



entry
addi r14,r0,topaddr
addi r14,r14,-136
jl r15,main
addi r14,r14,136
hlt
