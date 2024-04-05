main
sw 0(r14),r15
addi	r1,r0,3
addi	r2,r0,4
addi r14,r14,-12
jl r15,bar
addi r14,r14,12
lw r15, 0(r14)
jr r15


bar
sw 0(r14),r15
addi r14,r14,-12
jl r15,foo
addi r14,r14,12
addi	r3,r0,4
addi	r4,r0,4
lw r15, 0(r14)
jr r15


foo
sw 0(r14),r15
addi	r5,r0,3
addi	r6,r0,4
lw r15, 0(r14)
jr r15



entry
addi r14,r0,topaddr
addi r14,r14,-20
jl r15,main
addi r14,r0,20
hlt
