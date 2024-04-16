align
main
sw 0(r14),r15
addi r3,r0,3 
sw 160(r14),r3
addi r1,r0,0
lw r2, 160(r14)
sw parameterstorage(r1),r2
addi r3,r0,2 
sw 240(r14),r3
addi r1,r0,4
lw r2, 240(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 200(r14),r3
addi r1,r0,8
lw r2, 200(r14)
sw parameterstorage(r1),r2
addi r14,r14,-44
jl r15,complex_expression_with_relops
addi r14,r14,44
addi r3,r0,3 
sw 100(r14),r3
addi r1,r0,0
lw r2, 100(r14)
sw parameterstorage(r1),r2
addi r3,r0,2 
sw 44(r14),r3
addi r1,r0,4
lw r2, 44(r14)
sw parameterstorage(r1),r2
addi r3,r0,2 
sw 256(r14),r3
addi r1,r0,8
lw r2, 256(r14)
sw parameterstorage(r1),r2
addi r14,r14,-44
jl r15,complex_expression_with_relops
addi r14,r14,44
addi r3,r0,0 
sw 156(r14),r3
addi r1,r0,0
lw r2, 156(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 144(r14),r3
addi r1,r0,4
lw r2, 144(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_eq
addi r14,r14,24
addi r3,r0,1 
sw 208(r14),r3
addi r1,r0,0
lw r2, 208(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 248(r14),r3
addi r1,r0,4
lw r2, 248(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_eq
addi r14,r14,24
addi r3,r0,0 
sw 96(r14),r3
addi r1,r0,0
lw r2, 96(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 4(r14),r3
addi r1,r0,4
lw r2, 4(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_eq
addi r14,r14,24
addi r3,r0,1 
sw 104(r14),r3
addi r1,r0,0
lw r2, 104(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 152(r14),r3
addi r1,r0,4
lw r2, 152(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_eq
addi r14,r14,24
addi r3,r0,0 
sw 148(r14),r3
addi r1,r0,0
lw r2, 148(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 204(r14),r3
addi r1,r0,4
lw r2, 204(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_neq
addi r14,r14,24
addi r3,r0,1 
sw 36(r14),r3
addi r1,r0,0
lw r2, 36(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 244(r14),r3
addi r1,r0,4
lw r2, 244(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_neq
addi r14,r14,24
addi r3,r0,0 
sw 92(r14),r3
addi r1,r0,0
lw r2, 92(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 236(r14),r3
addi r1,r0,4
lw r2, 236(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_neq
addi r14,r14,24
addi r3,r0,1 
sw 88(r14),r3
addi r1,r0,0
lw r2, 88(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 32(r14),r3
addi r1,r0,4
lw r2, 32(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_neq
addi r14,r14,24
addi r3,r0,0 
sw 60(r14),r3
addi r1,r0,0
lw r2, 60(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 192(r14),r3
addi r1,r0,4
lw r2, 192(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_gt
addi r14,r14,24
addi r3,r0,1 
sw 76(r14),r3
addi r1,r0,0
lw r2, 76(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 132(r14),r3
addi r1,r0,4
lw r2, 132(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_gt
addi r14,r14,24
addi r3,r0,0 
sw 24(r14),r3
addi r1,r0,0
lw r2, 24(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 136(r14),r3
addi r1,r0,4
lw r2, 136(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_gt
addi r14,r14,24
addi r3,r0,1 
sw 228(r14),r3
addi r1,r0,0
lw r2, 228(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 196(r14),r3
addi r1,r0,4
lw r2, 196(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_gt
addi r14,r14,24
addi r3,r0,0 
sw 80(r14),r3
addi r1,r0,0
lw r2, 80(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 180(r14),r3
addi r1,r0,4
lw r2, 180(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_lt
addi r14,r14,24
addi r3,r0,1 
sw 68(r14),r3
addi r1,r0,0
lw r2, 68(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 16(r14),r3
addi r1,r0,4
lw r2, 16(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_lt
addi r14,r14,24
addi r3,r0,0 
sw 28(r14),r3
addi r1,r0,0
lw r2, 28(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 224(r14),r3
addi r1,r0,4
lw r2, 224(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_lt
addi r14,r14,24
addi r3,r0,1 
sw 64(r14),r3
addi r1,r0,0
lw r2, 64(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 128(r14),r3
addi r1,r0,4
lw r2, 128(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_lt
addi r14,r14,24
addi r3,r0,0 
sw 216(r14),r3
addi r1,r0,0
lw r2, 216(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 8(r14),r3
addi r1,r0,4
lw r2, 8(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_leq
addi r14,r14,24
addi r3,r0,1 
sw 84(r14),r3
addi r1,r0,0
lw r2, 84(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 140(r14),r3
addi r1,r0,4
lw r2, 140(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_leq
addi r14,r14,24
addi r3,r0,0 
sw 188(r14),r3
addi r1,r0,0
lw r2, 188(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 56(r14),r3
addi r1,r0,4
lw r2, 56(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_leq
addi r14,r14,24
addi r3,r0,1 
sw 20(r14),r3
addi r1,r0,0
lw r2, 20(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 232(r14),r3
addi r1,r0,4
lw r2, 232(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_leq
addi r14,r14,24
addi r3,r0,0 
sw 72(r14),r3
addi r1,r0,0
lw r2, 72(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 120(r14),r3
addi r1,r0,4
lw r2, 120(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_geq
addi r14,r14,24
addi r3,r0,1 
sw 220(r14),r3
addi r1,r0,0
lw r2, 220(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 12(r14),r3
addi r1,r0,4
lw r2, 12(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_geq
addi r14,r14,24
addi r3,r0,0 
sw 184(r14),r3
addi r1,r0,0
lw r2, 184(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 176(r14),r3
addi r1,r0,4
lw r2, 176(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_geq
addi r14,r14,24
addi r3,r0,1 
sw 52(r14),r3
addi r1,r0,0
lw r2, 52(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 124(r14),r3
addi r1,r0,4
lw r2, 124(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_geq
addi r14,r14,24
addi r3,r0,3 
sw 172(r14),r3
addi r1,r0,0
lw r2, 172(r14)
sw parameterstorage(r1),r2
addi r3,r0,2 
sw 108(r14),r3
addi r1,r0,4
lw r2, 108(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 164(r14),r3
addi r1,r0,8
lw r2, 164(r14)
sw parameterstorage(r1),r2
addi r14,r14,-44
jl r15,complex_expression_with_relops
addi r14,r14,44
addi r3,r0,3 
sw 48(r14),r3
addi r1,r0,0
lw r2, 48(r14)
sw parameterstorage(r1),r2
addi r3,r0,2 
sw 168(r14),r3
addi r1,r0,4
lw r2, 168(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 212(r14),r3
addi r1,r0,8
lw r2, 212(r14)
sw parameterstorage(r1),r2
addi r14,r14,-44
jl r15,complex_expression_with_relops
addi r14,r14,44
addi r3,r0,3 
sw 116(r14),r3
addi r1,r0,0
lw r2, 116(r14)
sw parameterstorage(r1),r2
addi r3,r0,2 
sw 252(r14),r3
addi r1,r0,4
lw r2, 252(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 40(r14),r3
addi r1,r0,8
lw r2, 40(r14)
sw parameterstorage(r1),r2
addi r14,r14,-44
jl r15,complex_expression_with_relops
addi r14,r14,44
addi r14,r14,-32
jl r15,another_complex_expression
addi r14,r14,32
sw 112(r14),r13
lw r13, 112(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r14,r14,-32
jl r15,another_complex_expression
addi r14,r14,32
lw r15, 0(r14)
jr r15


align
test_geq
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 8(r14),r2
addi r1,r0,4
lw r2, parameterstorage(r1)
sw 4(r14),r2
lw r2,4(r14) %s relexpr
lw r1, 8(r14)
cge r3,r2,r1
sw 12(r14), r3
lw r1,12(r14)
bz r1,else100
addi r1,r0,1 
sw 20(r14),r1
lw r13, 20(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif100
else100
addi r1,r0,0 
sw 16(r14),r1
lw r13, 16(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif100
lw r15, 0(r14)
jr r15


align
test_leq
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 8(r14),r2
addi r1,r0,4
lw r2, parameterstorage(r1)
sw 4(r14),r2
lw r2,4(r14) %s relexpr
lw r1, 8(r14)
cle r3,r2,r1
sw 16(r14), r3
lw r1,16(r14)
bz r1,else101
addi r1,r0,1 
sw 20(r14),r1
lw r13, 20(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif101
else101
addi r1,r0,0 
sw 12(r14),r1
lw r13, 12(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif101
lw r15, 0(r14)
jr r15


align
test_gt
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 8(r14),r2
addi r1,r0,4
lw r2, parameterstorage(r1)
sw 4(r14),r2
lw r2,4(r14) %s relexpr
lw r1, 8(r14)
cgt r3,r2,r1
sw 12(r14), r3
lw r1,12(r14)
bz r1,else102
addi r1,r0,1 
sw 20(r14),r1
lw r13, 20(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif102
else102
addi r1,r0,0 
sw 16(r14),r1
lw r13, 16(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif102
lw r15, 0(r14)
jr r15


align
test_lt
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 8(r14),r2
addi r1,r0,4
lw r2, parameterstorage(r1)
sw 4(r14),r2
lw r2,4(r14) %s relexpr
lw r1, 8(r14)
clt r3,r2,r1
sw 16(r14), r3
lw r1,16(r14)
bz r1,else103
addi r1,r0,1 
sw 12(r14),r1
lw r13, 12(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif103
else103
addi r1,r0,0 
sw 20(r14),r1
lw r13, 20(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif103
lw r15, 0(r14)
jr r15


align
test_neq
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 8(r14),r2
addi r1,r0,4
lw r2, parameterstorage(r1)
sw 4(r14),r2
lw r2,4(r14) %s relexpr
lw r1, 8(r14)
cne r3,r2,r1
sw 12(r14), r3
lw r1,12(r14)
bz r1,else104
addi r1,r0,1 
sw 16(r14),r1
lw r13, 16(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif104
else104
addi r1,r0,0 
sw 20(r14),r1
lw r13, 20(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif104
lw r15, 0(r14)
jr r15


align
test_eq
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 8(r14),r2
addi r1,r0,4
lw r2, parameterstorage(r1)
sw 4(r14),r2
lw r2,4(r14) %s relexpr
lw r1, 8(r14)
ceq r3,r2,r1
sw 12(r14), r3
lw r1,12(r14)
bz r1,else105
addi r1,r0,1 
sw 16(r14),r1
lw r13, 16(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif105
else105
addi r1,r0,0 
sw 20(r14),r1
lw r13, 20(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif105
lw r15, 0(r14)
jr r15


align
test_and_or
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 8(r14),r2
addi r1,r0,4
lw r2, parameterstorage(r1)
sw 4(r14),r2
lw r2,8(r14)
lw r1, 4(r14)
or r3,r1,r2
sw 20(r14),r3
lw r1,20(r14)
sw 16(r14),r1
lw r13, 16(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
lw r2,8(r14)
lw r1, 4(r14)
or r3,r1,r2
sw 12(r14),r3
lw r1,12(r14)
sw 16(r14),r1
lw r13, 16(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
lw r15, 0(r14)
jr r15


align
complex_expression_with_relops
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 8(r14),r2
addi r1,r0,4
lw r2, parameterstorage(r1)
sw 12(r14),r2
addi r1,r0,8
lw r2, parameterstorage(r1)
sw 4(r14),r2
lw r2,12(r14)
lw r1, 4(r14)
add r3,r1,r2
sw 28(r14),r3
lw r2,8(r14)
lw r1, 28(r14)
add r3,r1,r2
sw 32(r14),r3
lw r2,12(r14)
lw r1, 4(r14)
mul r3,r1,r2
sw 40(r14),r3
lw r2,8(r14)
lw r1, 40(r14)
mul r3,r1,r2
sw 20(r14),r3
lw r2,32(r14) %s relexpr
lw r1, 20(r14)
cge r3,r2,r1
sw 36(r14), r3
lw r1,36(r14)
bz r1,else106
addi r1,r0,1 
sw 16(r14),r1
lw r13, 16(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif106
else106
addi r1,r0,0 
sw 24(r14),r1
lw r13, 24(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif106
lw r15, 0(r14)
jr r15


align
another_complex_expression
sw 0(r14),r15
addi r1,r0,3 
sw 28(r14),r1
addi r1,r0,3 
sw 20(r14),r1
addi r1,r0,2 
sw 8(r14),r1
addi r1,r0,1 
sw 4(r14),r1
lw r2,8(r14)
lw r1, 4(r14)
add r3,r1,r2
sw 12(r14),r3
lw r2,20(r14)
lw r1, 12(r14)
mul r3,r1,r2
sw 24(r14),r3
lw r2,28(r14)
lw r1, 24(r14)
div r3,r1,r2
sw 16(r14),r3
lw r13,16(r14)
lw r15, 0(r14)
jr r15


align
complex_expression
sw 0(r14),r15
addi r1,r0,0
lw r2, parameterstorage(r1)
sw 8(r14),r2
addi r1,r0,4
lw r2, parameterstorage(r1)
sw 12(r14),r2
addi r1,r0,8
lw r2, parameterstorage(r1)
sw 4(r14),r2
lw r2,12(r14)
lw r1, 4(r14)
mul r3,r1,r2
sw 32(r14),r3
lw r2,8(r14)
lw r1, 32(r14)
div r3,r1,r2
sw 28(r14),r3
lw r2,8(r14)
lw r1, 12(r14)
mul r3,r1,r2
sw 24(r14),r3
lw r2,24(r14)
lw r1, 4(r14)
add r3,r1,r2
sw 20(r14),r3
lw r2,28(r14)
lw r1, 20(r14)
sub r3,r1,r2
sw 16(r14),r3
lw r2,4(r14)
lw r1, 16(r14)
add r3,r1,r2
sw 36(r14),r3
lw r13,36(r14)
lw r15, 0(r14)
jr r15



entry
addi r14,r0,topaddr
addi r14,r14,-260
jl r15,main
addi r14,r14,260
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
