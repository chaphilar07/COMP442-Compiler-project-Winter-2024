align
main
sw 0(r14),r15
addi r1,r0,1 
sw 276(r14),r1
addi r1,r0,16 %s accumulator register is r1 
addi r2,r0,0 
sw 228(r14),r2
lw r2,228(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
lw r1,276(r14)
addi r2,r0,0
lw r2, indexstorage(r2) %s HERE !!
add r2,r14,r2
sw 0(r2),r1
addi r1,r0,7 
sw 12(r14),r1
addi r1,r0,6 
sw 336(r14),r1
addi r1,r0,5 
sw 348(r14),r1
addi r1,r0,4 
sw 284(r14),r1
addi r1,r0,3 
sw 216(r14),r1
addi r1,r0,2 
sw 260(r14),r1
addi r1,r0,1 
sw 8(r14),r1
lw r2,260(r14)
lw r1, 8(r14)
add r3,r1,r2
sw 324(r14),r3
lw r2,216(r14)
lw r1, 324(r14)
add r3,r1,r2
sw 264(r14),r3
lw r2,284(r14)
lw r1, 264(r14)
add r3,r1,r2
sw 136(r14),r3
lw r2,348(r14)
lw r1, 136(r14)
add r3,r1,r2
sw 392(r14),r3
lw r2,336(r14)
lw r1, 392(r14)
add r3,r1,r2
sw 240(r14),r3
lw r2,12(r14)
lw r1, 240(r14)
add r3,r1,r2
sw 192(r14),r3
lw r1,192(r14)
sw 104(r14),r1
addi r1,r0,0 
sw 160(r14),r1
lw r1,160(r14)
sw 288(r14),r1
while1000000
addi r1,r0,20 
sw 320(r14),r1
lw r1, 320(r14)
lw r2,288(r14) %s relexpr
clt r3,r2,r1
sw 108(r14), r3
lw r1, 108(r14)
bz r1,endwhile1000000
addi r1,r0,1 
sw 188(r14),r1
lw r2,188(r14)
lw r1, 288(r14)
add r3,r1,r2
sw 352(r14),r3
addi r1,r0,16 %s accumulator register is r1 
lw r2,288(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
lw r1,352(r14)
addi r2,r0,0
lw r2, indexstorage(r2) %s HERE !!
add r2,r14,r2
sw 0(r2),r1
addi r1,r0,16 %s accumulator register is r1 
lw r2,288(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
addi r1,r0,0
lw r1,indexstorage(r1)
add r1,r1,r14
lw r13,0(r1)
addi r14,r14,-8
jl r15, write
addi r14,r14,8
addi r1,r0,16 %s accumulator register is r1 
lw r2,288(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 4 
sw indexstorage(r2), r1
addi r4,r0,4
lw r4, indexstorage(r4) %s HERE1!!
add r4,r14,r4
lw r2, 0(r4)
lw r1, 104(r14)
add r3,r1,r2
sw 220(r14),r3
lw r1,220(r14)
sw 104(r14),r1
addi r1,r0,1 
sw 132(r14),r1
lw r2,132(r14)
lw r1, 288(r14)
add r3,r1,r2
sw 164(r14),r3
lw r1,164(r14)
sw 288(r14),r1
j while1000000
endwhile1000000
addi r1,r0,16 %s accumulator register is r1 
addi r2,r0,4 
sw 388(r14),r2
lw r2,388(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 4 
sw indexstorage(r2), r1
addi r1,r0,16 %s accumulator register is r1 
addi r2,r0,3 
sw 344(r14),r2
lw r2,344(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 8 
sw indexstorage(r2), r1
addi r1,r0,16 %s accumulator register is r1 
addi r2,r0,2 
sw 100(r14),r2
lw r2,100(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 12 
sw indexstorage(r2), r1
addi r1,r0,16 %s accumulator register is r1 
addi r2,r0,1 
sw 208(r14),r2
lw r2,208(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 16 
sw indexstorage(r2), r1
addi r1,r0,16 %s accumulator register is r1 
addi r2,r0,0 
sw 152(r14),r2
lw r2,152(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 20 
sw indexstorage(r2), r1
addi r4,r0,20
lw r4, indexstorage(r4) %s HERE1!!
add r4,r14,r4
lw r2, 0(r4)
addi r4,r0,16
lw r4, indexstorage(r4) %s HERE2 !!!
add r4,r14,r4
lw r1, 0(r4)
add r3,r1,r2
sw 248(r14),r3
addi r4,r0,12
lw r4, indexstorage(r4) %s HERE1!!
add r4,r14,r4
lw r2, 0(r4)
lw r1, 248(r14)
add r3,r1,r2
sw 308(r14),r3
addi r4,r0,8
lw r4, indexstorage(r4) %s HERE1!!
add r4,r14,r4
lw r2, 0(r4)
lw r1, 308(r14)
add r3,r1,r2
sw 144(r14),r3
addi r4,r0,4
lw r4, indexstorage(r4) %s HERE1!!
add r4,r14,r4
lw r2, 0(r4)
lw r1, 144(r14)
add r3,r1,r2
sw 200(r14),r3
lw r1,200(r14)
sw 104(r14),r1
addi r1,r0,15 
sw 272(r14),r1
lw r1, 272(r14)
lw r2,104(r14) %s relexpr
ceq r3,r2,r1
sw 120(r14), r3
lw r1,120(r14)
bz r1,else1000000
addi r1,r0,1 
sw 332(r14),r1
lw r13, 332(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif1000000
else1000000
addi r1,r0,0 
sw 400(r14),r1
lw r13, 400(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif1000000
lw r13, 104(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r14,r14,-8
jl r15,return2
addi r14,r14,8
sw 224(r14),r13
addi r1,r0,16 %s accumulator register is r1 
addi r2,r0,16 %s accumulator register is r2 
addi r3,r0,0 
sw 168(r14),r3
lw r3,168(r14)
muli r3,r3,4
add r2,r2,r3
addi r3, r0, 4 
sw indexstorage(r3), r2
addi r2,r0,16 %s accumulator register is r2 
addi r3,r0,1 
sw 96(r14),r3
lw r3,96(r14)
muli r3,r3,4
add r2,r2,r3
addi r3, r0, 8 
sw indexstorage(r3), r2
addi r5,r0,8
lw r5, indexstorage(r5) %s HERE1!!
add r5,r14,r5
lw r3, 0(r5)
addi r5,r0,4
lw r5, indexstorage(r5) %s HERE2 !!!
add r5,r14,r5
lw r2, 0(r5)
mul r4,r2,r3
sw 176(r14),r4
addi r2,r0,16 %s accumulator register is r2 
addi r3,r0,0 
sw 364(r14),r3
lw r3,364(r14)
muli r3,r3,4
add r2,r2,r3
addi r3, r0, 4 
sw indexstorage(r3), r2
lw r3,176(r14)
addi r5,r0,4
lw r5, indexstorage(r5) %s HERE2 !!!
add r5,r14,r5
lw r2, 0(r5)
add r4,r2,r3
sw 236(r14),r4
lw r2,236(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 4 
sw indexstorage(r2), r1
lw r2,224(r14)
addi r4,r0,4
lw r4, indexstorage(r4) %s HERE2 !!!
add r4,r14,r4
lw r1, 0(r4)
mul r3,r1,r2
sw 372(r14),r3
lw r1,372(r14)
sw 104(r14),r1
addi r14,r14,-8
jl r15,return2
addi r14,r14,8
lw r13, 104(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r14,r14,-8
jl r15,return4
addi r14,r14,8
sw 296(r14),r13
addi r14,r14,-8
jl r15,return3
addi r14,r14,8
sw 204(r14),r13
addi r14,r14,-8
jl r15,return2
addi r14,r14,8
sw 252(r14),r13
addi r14,r14,-8
jl r15,return1
addi r14,r14,8
sw 312(r14),r13
lw r2,252(r14)
lw r1, 312(r14)
add r3,r1,r2
sw 140(r14),r3
lw r2,204(r14)
lw r1, 140(r14)
add r3,r1,r2
sw 280(r14),r3
lw r2,296(r14)
lw r1, 280(r14)
add r3,r1,r2
sw 340(r14),r3
lw r1,340(r14)
sw 104(r14),r1
addi r14,r14,-8
jl r15,return1
addi r14,r14,8
addi r14,r14,-8
jl r15,return2
addi r14,r14,8
addi r14,r14,-8
jl r15,return3
addi r14,r14,8
addi r14,r14,-8
jl r15,return4
addi r14,r14,8
lw r13, 104(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r1,r0,16 %s accumulator register is r1 
addi r2,r0,16 %s accumulator register is r2 
addi r3,r0,1 
sw 376(r14),r3
lw r3,376(r14)
muli r3,r3,4
add r2,r2,r3
addi r3, r0, 4 
sw indexstorage(r3), r2
addi r2,r0,1 
sw 268(r14),r2
addi r2,r0,3 
sw 328(r14),r2
addi r2,r0,2 
sw 396(r14),r2
addi r2,r0,1 
sw 148(r14),r2
lw r3,396(r14)
lw r2, 148(r14)
add r4,r2,r3
sw 368(r14),r4
lw r3,328(r14)
lw r2, 368(r14)
add r4,r2,r3
sw 304(r14),r4
lw r3,268(r14)
lw r2, 304(r14)
add r4,r2,r3
sw 244(r14),r4
addi r5,r0,4
lw r5, indexstorage(r5) %s HERE1!!
add r5,r14,r5
lw r3, 0(r5)
lw r2, 244(r14)
add r4,r2,r3
sw 196(r14),r4
lw r2,196(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 4 
sw indexstorage(r2), r1
addi r2,r0,4
lw r2, indexstorage(r2) %s here !!!
add r2,r14,r2
lw r1, 0(r2)
sw 104(r14),r1
lw r13, 104(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r1,r0,16 %s accumulator register is r1 
addi r2,r0,16 %s accumulator register is r2 
addi r3,r0,4 
sw 360(r14),r3
lw r3,360(r14)
muli r3,r3,4
add r2,r2,r3
addi r3, r0, 4 
sw indexstorage(r3), r2
addi r2,r0,16 %s accumulator register is r2 
addi r3,r0,3 
sw 112(r14),r3
lw r3,112(r14)
muli r3,r3,4
add r2,r2,r3
addi r3, r0, 8 
sw indexstorage(r3), r2
addi r2,r0,16 %s accumulator register is r2 
addi r3,r0,2 
sw 156(r14),r3
lw r3,156(r14)
muli r3,r3,4
add r2,r2,r3
addi r3, r0, 12 
sw indexstorage(r3), r2
addi r2,r0,16 %s accumulator register is r2 
addi r3,r0,1 
sw 4(r14),r3
lw r3,4(r14)
muli r3,r3,4
add r2,r2,r3
addi r3, r0, 16 
sw indexstorage(r3), r2
addi r2,r0,16 %s accumulator register is r2 
addi r3,r0,0 
sw 256(r14),r3
lw r3,256(r14)
muli r3,r3,4
add r2,r2,r3
addi r3, r0, 20 
sw indexstorage(r3), r2
addi r5,r0,20
lw r5, indexstorage(r5) %s HERE1!!
add r5,r14,r5
lw r3, 0(r5)
addi r5,r0,16
lw r5, indexstorage(r5) %s HERE2 !!!
add r5,r14,r5
lw r2, 0(r5)
add r4,r2,r3
sw 292(r14),r4
addi r5,r0,12
lw r5, indexstorage(r5) %s HERE1!!
add r5,r14,r5
lw r3, 0(r5)
lw r2, 292(r14)
add r4,r2,r3
sw 232(r14),r4
addi r5,r0,8
lw r5, indexstorage(r5) %s HERE1!!
add r5,r14,r5
lw r3, 0(r5)
lw r2, 232(r14)
add r4,r2,r3
sw 172(r14),r4
addi r5,r0,4
lw r5, indexstorage(r5) %s HERE1!!
add r5,r14,r5
lw r3, 0(r5)
lw r2, 172(r14)
add r4,r2,r3
sw 116(r14),r4
lw r2,116(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 4 
sw indexstorage(r2), r1
addi r2,r0,4
lw r2, indexstorage(r2) %s here !!!
add r2,r14,r2
lw r1, 0(r2)
sw 104(r14),r1
lw r13, 104(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r1,r0,0 
sw 212(r14),r1
lw r1,212(r14)
sw 288(r14),r1
while1000001
addi r1,r0,10 
sw 316(r14),r1
lw r1, 316(r14)
lw r2,288(r14) %s relexpr
clt r3,r2,r1
sw 184(r14), r3
lw r1, 184(r14)
bz r1,endwhile1000001
addi r1,r0,16 %s accumulator register is r1 
lw r2,288(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 4 
sw indexstorage(r2), r1
addi r1,r0,4 %s the temp register is r1
lw r1,indexstorage(r1)
add r1,r1,r14
addi r14,r14,-4
jl r15,read
addi r14,r14,4
sw 0(r1),r13
addi r2,r0,1 
sw 380(r14),r2
lw r3,380(r14)
lw r2, 288(r14)
add r4,r2,r3
sw 356(r14),r4
lw r2,356(r14)
sw 288(r14),r2
j while1000001
endwhile1000001
addi r2,r0,0 
sw 180(r14),r2
lw r2,180(r14)
sw 288(r14),r2
while1000002
addi r2,r0,10 
sw 300(r14),r2
lw r2, 300(r14)
lw r3,288(r14) %s relexpr
clt r4,r3,r2
sw 384(r14), r4
lw r2, 384(r14)
bz r2,endwhile1000002
addi r2,r0,16 %s accumulator register is r2 
lw r3,288(r14)
muli r3,r3,4
add r2,r2,r3
addi r3, r0, 4 
sw indexstorage(r3), r2
addi r2,r0,4
lw r2,indexstorage(r2)
add r2,r2,r14
lw r13,0(r2)
addi r14,r14,-8
jl r15, write
addi r14,r14,8
addi r2,r0,1 
sw 124(r14),r2
lw r3,124(r14)
lw r2, 288(r14)
add r4,r2,r3
sw 128(r14),r4
lw r2,128(r14)
sw 288(r14),r2
j while1000002
endwhile1000002
lw r15, 0(r14)
jr r15


align
return4
sw 0(r14),r15
addi r2,r0,4 
sw 4(r14),r2
lw r13,4(r14)
lw r15, 0(r14)
jr r15
lw r15, 0(r14)
jr r15


align
return3
sw 0(r14),r15
addi r2,r0,3 
sw 4(r14),r2
lw r13,4(r14)
lw r15, 0(r14)
jr r15
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
lw r15, 0(r14)
jr r15



entry
addi r14,r0,topaddr
addi r14,r14,-404
jl r15,main
addi r14,r14,404
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
