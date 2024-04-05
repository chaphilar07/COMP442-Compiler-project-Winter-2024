topofstackpointer res 4
stackregion res 2048
main
sw topofstackpointer(r0),r15
addi	r1,r0,3
addi	r2,r0,16
sw topofstackpointer(r2),r1
addi	r3,r0,3
addi	r4,r0,4
sw topofstackpointer(r4),r3
lw r15, topofstackpointer(r0)
jr r15

entry
addi r14,r0,-24
sw topofstackpointer(r0),r14
jl r15,main
hlt
