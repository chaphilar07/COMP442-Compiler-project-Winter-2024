align
main
sw 0(r14),r15
addi r3,r0,3 
sw 8(r14),r3
addi r1,r0,0
lw r2, 8(r14)
sw parameterstorage(r1),r2
addi r3,r0,2 
sw 144(r14),r3
addi r1,r0,4
lw r2, 144(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 68(r14),r3
addi r1,r0,8
lw r2, 68(r14)
sw parameterstorage(r1),r2
addi r14,r14,-44
jl r15,complex_expression_with_relops
addi r14,r14,44
addi r3,r0,3 
sw 332(r14),r3
addi r1,r0,0
lw r2, 332(r14)
sw parameterstorage(r1),r2
addi r3,r0,2 
sw 236(r14),r3
addi r1,r0,4
lw r2, 236(r14)
sw parameterstorage(r1),r2
addi r3,r0,2 
sw 160(r14),r3
addi r1,r0,8
lw r2, 160(r14)
sw parameterstorage(r1),r2
addi r14,r14,-44
jl r15,complex_expression_with_relops
addi r14,r14,44
addi r3,r0,0 
sw 4(r14),r3
addi r1,r0,0
lw r2, 4(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 404(r14),r3
addi r1,r0,4
lw r2, 404(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_eq
addi r14,r14,24
addi r3,r0,1 
sw 140(r14),r3
addi r1,r0,0
lw r2, 140(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 224(r14),r3
addi r1,r0,4
lw r2, 224(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_eq
addi r14,r14,24
addi r3,r0,0 
sw 416(r14),r3
addi r1,r0,0
lw r2, 416(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 232(r14),r3
addi r1,r0,4
lw r2, 232(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_eq
addi r14,r14,24
addi r3,r0,1 
sw 424(r14),r3
addi r1,r0,0
lw r2, 424(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 64(r14),r3
addi r1,r0,4
lw r2, 64(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_eq
addi r14,r14,24
addi r3,r0,0 
sw 48(r14),r3
addi r1,r0,0
lw r2, 48(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 72(r14),r3
addi r1,r0,4
lw r2, 72(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_neq
addi r14,r14,24
addi r3,r0,1 
sw 228(r14),r3
addi r1,r0,0
lw r2, 228(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 148(r14),r3
addi r1,r0,4
lw r2, 148(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_neq
addi r14,r14,24
addi r3,r0,0 
sw 324(r14),r3
addi r1,r0,0
lw r2, 324(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 132(r14),r3
addi r1,r0,4
lw r2, 132(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_neq
addi r14,r14,24
addi r3,r0,1 
sw 312(r14),r3
addi r1,r0,0
lw r2, 312(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 212(r14),r3
addi r1,r0,4
lw r2, 212(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_neq
addi r14,r14,24
addi r3,r0,0 
sw 276(r14),r3
addi r1,r0,0
lw r2, 276(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 40(r14),r3
addi r1,r0,4
lw r2, 40(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_gt
addi r14,r14,24
addi r3,r0,1 
sw 288(r14),r3
addi r1,r0,0
lw r2, 288(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 384(r14),r3
addi r1,r0,4
lw r2, 384(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_gt
addi r14,r14,24
addi r3,r0,0 
sw 204(r14),r3
addi r1,r0,0
lw r2, 204(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 388(r14),r3
addi r1,r0,4
lw r2, 388(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_gt
addi r14,r14,24
addi r3,r0,1 
sw 196(r14),r3
addi r1,r0,0
lw r2, 196(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 116(r14),r3
addi r1,r0,4
lw r2, 116(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_gt
addi r14,r14,24
addi r3,r0,0 
sw 380(r14),r3
addi r1,r0,0
lw r2, 380(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 104(r14),r3
addi r1,r0,4
lw r2, 104(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_lt
addi r14,r14,24
addi r3,r0,1 
sw 372(r14),r3
addi r1,r0,0
lw r2, 372(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 268(r14),r3
addi r1,r0,4
lw r2, 268(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_lt
addi r14,r14,24
addi r3,r0,0 
sw 292(r14),r3
addi r1,r0,0
lw r2, 292(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 120(r14),r3
addi r1,r0,4
lw r2, 120(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_lt
addi r14,r14,24
addi r3,r0,1 
sw 280(r14),r3
addi r1,r0,0
lw r2, 280(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 376(r14),r3
addi r1,r0,4
lw r2, 376(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_lt
addi r14,r14,24
addi r3,r0,0 
sw 108(r14),r3
addi r1,r0,0
lw r2, 108(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 188(r14),r3
addi r1,r0,4
lw r2, 188(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_leq
addi r14,r14,24
addi r3,r0,1 
sw 296(r14),r3
addi r1,r0,0
lw r2, 296(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 392(r14),r3
addi r1,r0,4
lw r2, 392(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_leq
addi r14,r14,24
addi r3,r0,0 
sw 36(r14),r3
addi r1,r0,0
lw r2, 36(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 272(r14),r3
addi r1,r0,4
lw r2, 272(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_leq
addi r14,r14,24
addi r3,r0,1 
sw 200(r14),r3
addi r1,r0,0
lw r2, 200(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 124(r14),r3
addi r1,r0,4
lw r2, 124(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_leq
addi r14,r14,24
addi r3,r0,0 
sw 284(r14),r3
addi r1,r0,0
lw r2, 284(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 364(r14),r3
addi r1,r0,4
lw r2, 364(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_geq
addi r14,r14,24
addi r3,r0,1 
sw 112(r14),r3
addi r1,r0,0
lw r2, 112(r14)
sw parameterstorage(r1),r2
addi r3,r0,0 
sw 192(r14),r3
addi r1,r0,4
lw r2, 192(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_geq
addi r14,r14,24
addi r3,r0,0 
sw 32(r14),r3
addi r1,r0,0
lw r2, 32(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 28(r14),r3
addi r1,r0,4
lw r2, 28(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_geq
addi r14,r14,24
addi r3,r0,1 
sw 264(r14),r3
addi r1,r0,0
lw r2, 264(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 368(r14),r3
addi r1,r0,4
lw r2, 368(r14)
sw parameterstorage(r1),r2
addi r14,r14,-24
jl r15,test_geq
addi r14,r14,24
addi r3,r0,3 
sw 24(r14),r3
addi r1,r0,0
lw r2, 24(r14)
sw parameterstorage(r1),r2
addi r3,r0,2 
sw 252(r14),r3
addi r1,r0,4
lw r2, 252(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 348(r14),r3
addi r1,r0,8
lw r2, 348(r14)
sw parameterstorage(r1),r2
addi r14,r14,-44
jl r15,complex_expression_with_relops
addi r14,r14,44
addi r3,r0,3 
sw 88(r14),r3
addi r1,r0,0
lw r2, 88(r14)
sw parameterstorage(r1),r2
addi r3,r0,2 
sw 360(r14),r3
addi r1,r0,4
lw r2, 360(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 20(r14),r3
addi r1,r0,8
lw r2, 20(r14)
sw parameterstorage(r1),r2
addi r14,r14,-44
jl r15,complex_expression_with_relops
addi r14,r14,44
addi r3,r0,3 
sw 176(r14),r3
addi r1,r0,0
lw r2, 176(r14)
sw parameterstorage(r1),r2
addi r3,r0,2 
sw 80(r14),r3
addi r1,r0,4
lw r2, 80(r14)
sw parameterstorage(r1),r2
addi r3,r0,1 
sw 156(r14),r3
addi r1,r0,8
lw r2, 156(r14)
sw parameterstorage(r1),r2
addi r14,r14,-44
jl r15,complex_expression_with_relops
addi r14,r14,44
addi r14,r14,-32
jl r15,another_complex_expression
addi r14,r14,32
sw 340(r14),r13
lw r13, 340(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r14,r14,-32
jl r15,another_complex_expression
addi r14,r14,32
addi r1,r0,3 
sw 244(r14),r1
addi r1,r0,2 
sw 168(r14),r1
addi r1,r0,1 
sw 96(r14),r1
lw r2,168(r14)
lw r1, 96(r14)
add r3,r1,r2
sw 320(r14),r3
lw r2,244(r14)
lw r1, 320(r14)
add r3,r1,r2
sw 220(r14),r3
lw r1,220(r14)
sw 300(r14),r1
lw r13, 300(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r1,r0,10 
sw 16(r14),r1
addi r1,r0,9 
sw 356(r14),r1
addi r1,r0,8 
sw 260(r14),r1
addi r1,r0,7 
sw 180(r14),r1
addi r1,r0,6 
sw 152(r14),r1
addi r1,r0,5 
sw 76(r14),r1
addi r1,r0,4 
sw 344(r14),r1
addi r1,r0,3 
sw 12(r14),r1
addi r1,r0,2 
sw 172(r14),r1
addi r1,r0,1 
sw 248(r14),r1
lw r2,172(r14)
lw r1, 248(r14)
mul r3,r1,r2
sw 420(r14),r3
lw r2,12(r14)
lw r1, 420(r14)
mul r3,r1,r2
sw 60(r14),r3
lw r2,344(r14)
lw r1, 60(r14)
mul r3,r1,r2
sw 136(r14),r3
lw r2,76(r14)
lw r1, 136(r14)
mul r3,r1,r2
sw 400(r14),r3
lw r2,152(r14)
lw r1, 400(r14)
mul r3,r1,r2
sw 308(r14),r3
lw r2,180(r14)
lw r1, 308(r14)
mul r3,r1,r2
sw 208(r14),r3
lw r2,260(r14)
lw r1, 208(r14)
mul r3,r1,r2
sw 128(r14),r3
lw r2,356(r14)
lw r1, 128(r14)
mul r3,r1,r2
sw 56(r14),r3
lw r2,16(r14)
lw r1, 56(r14)
mul r3,r1,r2
sw 412(r14),r3
lw r1,412(r14)
sw 300(r14),r1
lw r13, 300(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r1,r0,4 
sw 240(r14),r1
lw r1,240(r14)
muli r1,r1,-1
sw 240(r14),r1
addi r1,r0,4 
sw 336(r14),r1
addi r1,r0,3 
sw 84(r14),r1
lw r2,336(r14)
lw r1, 84(r14)
mul r3,r1,r2
sw 408(r14),r3
addi r1,r0,2 
sw 164(r14),r1
addi r1,r0,1 
sw 100(r14),r1
lw r2,164(r14)
lw r1, 100(r14)
mul r3,r1,r2
sw 316(r14),r3
lw r2,408(r14)
lw r1, 316(r14)
add r3,r1,r2
sw 52(r14),r3
lw r2,240(r14)
lw r1, 52(r14)
add r3,r1,r2
sw 328(r14),r3
lw r1,328(r14)
sw 300(r14),r1
lw r13, 300(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r1,r0,2 
sw 184(r14),r1
addi r1,r0,6 
sw 352(r14),r1
lw r2,184(r14)
lw r1, 352(r14)
add r3,r1,r2
sw 396(r14),r3
addi r1,r0,4 
sw 256(r14),r1
addi r1,r0,9 
sw 92(r14),r1
lw r2,256(r14)
lw r1, 92(r14)
mul r3,r1,r2
sw 304(r14),r3
lw r2,396(r14)
lw r1, 304(r14)
div r3,r1,r2
sw 216(r14),r3
lw r1,216(r14)
sw 44(r14),r1
lw r13, 44(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
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
lw r1, 8(r14)
lw r2,4(r14) %s relexpr
cge r3,r2,r1
sw 12(r14), r3
lw r1,12(r14)
bz r1,else1000000
addi r1,r0,1 
sw 20(r14),r1
lw r13, 20(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif1000000
else1000000
addi r1,r0,0 
sw 16(r14),r1
lw r13, 16(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif1000000
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
lw r1, 8(r14)
lw r2,4(r14) %s relexpr
cle r3,r2,r1
sw 16(r14), r3
lw r1,16(r14)
bz r1,else1000001
addi r1,r0,1 
sw 20(r14),r1
lw r13, 20(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif1000001
else1000001
addi r1,r0,0 
sw 12(r14),r1
lw r13, 12(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif1000001
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
lw r1, 8(r14)
lw r2,4(r14) %s relexpr
cgt r3,r2,r1
sw 20(r14), r3
lw r1,20(r14)
bz r1,else1000002
addi r1,r0,1 
sw 16(r14),r1
lw r13, 16(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif1000002
else1000002
addi r1,r0,0 
sw 12(r14),r1
lw r13, 12(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif1000002
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
lw r1, 8(r14)
lw r2,4(r14) %s relexpr
clt r3,r2,r1
sw 20(r14), r3
lw r1,20(r14)
bz r1,else1000003
addi r1,r0,1 
sw 16(r14),r1
lw r13, 16(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif1000003
else1000003
addi r1,r0,0 
sw 12(r14),r1
lw r13, 12(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif1000003
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
lw r1, 8(r14)
lw r2,4(r14) %s relexpr
cne r3,r2,r1
sw 12(r14), r3
lw r1,12(r14)
bz r1,else1000004
addi r1,r0,1 
sw 16(r14),r1
lw r13, 16(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif1000004
else1000004
addi r1,r0,0 
sw 20(r14),r1
lw r13, 20(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif1000004
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
lw r1, 8(r14)
lw r2,4(r14) %s relexpr
ceq r3,r2,r1
sw 16(r14), r3
lw r1,16(r14)
bz r1,else1000005
addi r1,r0,1 
sw 20(r14),r1
lw r13, 20(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif1000005
else1000005
addi r1,r0,0 
sw 12(r14),r1
lw r13, 12(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif1000005
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
sw 12(r14),r3
lw r1,12(r14)
sw 16(r14),r1
lw r13, 16(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
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
sw 32(r14),r3
lw r2,8(r14)
lw r1, 32(r14)
add r3,r1,r2
sw 40(r14),r3
lw r2,12(r14)
lw r1, 4(r14)
mul r3,r1,r2
sw 20(r14),r3
lw r2,8(r14)
lw r1, 20(r14)
mul r3,r1,r2
sw 24(r14),r3
lw r1, 24(r14)
lw r2,40(r14) %s relexpr
cge r3,r2,r1
sw 16(r14), r3
lw r1,16(r14)
bz r1,else1000006
addi r1,r0,1 
sw 28(r14),r1
lw r13, 28(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif1000006
else1000006
addi r1,r0,0 
sw 36(r14),r1
lw r13, 36(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif1000006
lw r15, 0(r14)
jr r15


align
another_complex_expression
sw 0(r14),r15
addi r1,r0,3 
sw 16(r14),r1
addi r1,r0,3 
sw 8(r14),r1
addi r1,r0,2 
sw 24(r14),r1
addi r1,r0,1 
sw 20(r14),r1
lw r2,24(r14)
lw r1, 20(r14)
add r3,r1,r2
sw 28(r14),r3
lw r2,8(r14)
lw r1, 28(r14)
mul r3,r1,r2
sw 12(r14),r3
lw r2,16(r14)
lw r1, 12(r14)
div r3,r1,r2
sw 4(r14),r3
lw r13,4(r14)
lw r15, 0(r14)
jr r15
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
sw 16(r14),r3
lw r2,8(r14)
lw r1, 16(r14)
div r3,r1,r2
sw 36(r14),r3
lw r2,8(r14)
lw r1, 12(r14)
mul r3,r1,r2
sw 32(r14),r3
lw r2,32(r14)
lw r1, 4(r14)
add r3,r1,r2
sw 28(r14),r3
lw r2,36(r14)
lw r1, 28(r14)
sub r3,r1,r2
sw 24(r14),r3
lw r2,4(r14)
lw r1, 24(r14)
add r3,r1,r2
sw 20(r14),r3
lw r13,20(r14)
lw r15, 0(r14)
jr r15
lw r15, 0(r14)
jr r15



entry
addi r14,r0,topaddr
addi r14,r14,-428
jl r15,main
addi r14,r14,428
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
