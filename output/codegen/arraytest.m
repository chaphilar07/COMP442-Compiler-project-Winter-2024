align
foo
sw 0(r14),r15
addi r1,r0,3 
sw 12(r14),r1
lw r13,12(r14)
lw r15, 0(r14)
jr r15


align
main
sw 0(r14),r15
addi r1,r0,1 
sw 372(r14),r1
addi r1,r0,12 %s accumulator register is r1 
addi r2,r0,0 
sw 324(r14),r2
lw r2,324(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
lw r1,372(r14)
addi r2,r0,0
lw r2, indexstorage(r2) %s HERE !!
add r2,r14,r2
sw 0(r2),r1
addi r1,r0,7 
sw 188(r14),r1
addi r1,r0,6 
sw 132(r14),r1
addi r1,r0,5 
sw 140(r14),r1
addi r1,r0,4 
sw 8(r14),r1
addi r1,r0,3 
sw 264(r14),r1
addi r1,r0,2 
sw 200(r14),r1
addi r1,r0,1 
sw 208(r14),r1
lw r2,200(r14)
lw r1, 208(r14)
add r3,r1,r2 %s operation add performed 
sw 164(r14),r3
lw r2,264(r14)
lw r1, 164(r14)
add r3,r1,r2 %s operation add performed 
sw 116(r14),r3
lw r2,8(r14)
lw r1, 116(r14)
add r3,r1,r2 %s operation add performed 
sw 292(r14),r3
lw r2,140(r14)
lw r1, 292(r14)
add r3,r1,r2 %s operation add performed 
sw 228(r14),r3
lw r2,132(r14)
lw r1, 228(r14)
add r3,r1,r2 %s operation add performed 
sw 112(r14),r3
lw r2,188(r14)
lw r1, 112(r14)
add r3,r1,r2 %s operation add performed 
sw 348(r14),r3
lw r1,348(r14)
sw 92(r14),r1
addi r1,r0,0 
sw 144(r14),r1
lw r1,144(r14)
sw 300(r14),r1
while100
addi r1,r0,20 
sw 256(r14),r1
lw r2,300(r14) %s relexpr
lw r1, 256(r14)
clt r3,r2,r1
sw 280(r14), r3
lw r1, 280(r14)
bz r1,endwhile100
addi r1,r0,1 
sw 124(r14),r1
lw r2,124(r14)
lw r1, 300(r14)
add r3,r1,r2 %s operation add performed 
sw 216(r14),r3
addi r1,r0,12 %s accumulator register is r1 
lw r2,300(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
lw r1,216(r14)
addi r2,r0,0
lw r2, indexstorage(r2) %s HERE !!
add r2,r14,r2
sw 0(r2),r1
addi r1,r0,12 %s accumulator register is r1 
lw r2,300(r14)
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
addi r1,r0,12 %s accumulator register is r1 
lw r2,300(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 4 
sw indexstorage(r2), r1
addi r4,r0,4
lw r4, indexstorage(r4) %s HERE1!!
add r4,r14,r4
lw r2, 0(r4)
lw r1, 92(r14)
add r3,r1,r2 %s operation add performed 
sw 352(r14),r3
lw r1,352(r14)
sw 92(r14),r1
addi r1,r0,1 
sw 364(r14),r1
lw r2,364(r14)
lw r1, 300(r14)
add r3,r1,r2 %s operation add performed 
sw 296(r14),r3
lw r1,296(r14)
sw 300(r14),r1
j while100
endwhile100
addi r1,r0,12 %s accumulator register is r1 
addi r2,r0,4 
sw 316(r14),r2
lw r2,316(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 4 
sw indexstorage(r2), r1
addi r1,r0,12 %s accumulator register is r1 
addi r2,r0,3 
sw 304(r14),r2
lw r2,304(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 8 
sw indexstorage(r2), r1
addi r1,r0,12 %s accumulator register is r1 
addi r2,r0,2 
sw 356(r14),r2
lw r2,356(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 12 
sw indexstorage(r2), r1
addi r1,r0,12 %s accumulator register is r1 
addi r2,r0,1 
sw 180(r14),r2
lw r2,180(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 16 
sw indexstorage(r2), r1
addi r1,r0,12 %s accumulator register is r1 
addi r2,r0,0 
sw 244(r14),r2
lw r2,244(r14)
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
add r3,r1,r2 %s operation add performed 
sw 160(r14),r3
addi r4,r0,12
lw r4, indexstorage(r4) %s HERE1!!
add r4,r14,r4
lw r2, 0(r4)
lw r1, 160(r14)
add r3,r1,r2 %s operation add performed 
sw 224(r14),r3
addi r4,r0,8
lw r4, indexstorage(r4) %s HERE1!!
add r4,r14,r4
lw r2, 0(r4)
lw r1, 224(r14)
add r3,r1,r2 %s operation add performed 
sw 340(r14),r3
addi r4,r0,4
lw r4, indexstorage(r4) %s HERE1!!
add r4,r14,r4
lw r2, 0(r4)
lw r1, 340(r14)
add r3,r1,r2 %s operation add performed 
sw 108(r14),r3
lw r1,108(r14)
sw 92(r14),r1
addi r1,r0,15 
sw 360(r14),r1
lw r2,92(r14) %s relexpr
lw r1, 360(r14)
ceq r3,r2,r1
sw 332(r14), r3
lw r1,332(r14)
bz r1,else100
addi r1,r0,1 
sw 272(r14),r1
lw r13, 272(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
j endif100
else100
addi r1,r0,0 
sw 204(r14),r1
lw r13, 204(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
endif100
lw r13, 92(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r1,r0,12 %s accumulator register is r1 
addi r2,r0,2 
sw 308(r14),r2
addi r2,r0,12 %s accumulator register is r2 
addi r3,r0,1 
sw 248(r14),r3
lw r3,248(r14)
muli r3,r3,4
add r2,r2,r3
addi r3, r0, 4 
sw indexstorage(r3), r2
lw r3,308(r14)
addi r5,r0,4
lw r5, indexstorage(r5) %s HERE2 !!!
add r5,r14,r5
lw r2, 0(r5)
mul r4,r2,r3 %s operation mul performed 
sw 152(r14),r4
addi r2,r0,12 %s accumulator register is r2 
addi r3,r0,0 
sw 192(r14),r3
lw r3,192(r14)
muli r3,r3,4
add r2,r2,r3
addi r3, r0, 4 
sw indexstorage(r3), r2
lw r3,152(r14)
addi r5,r0,4
lw r5, indexstorage(r5) %s HERE2 !!!
add r5,r14,r5
lw r2, 0(r5)
add r4,r2,r3 %s operation add performed 
sw 288(r14),r4
lw r2,288(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 4 
sw indexstorage(r2), r1
addi r2,r0,4
lw r2, indexstorage(r2) %s here !!!
add r2,r14,r2
lw r1, 0(r2)
sw 92(r14),r1
lw r13, 92(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r14,r14,-16
jl r15,foo
addi r14,r14,16
sw 136(r14),r13
addi r14,r14,-16
jl r15,foo
addi r14,r14,16
sw 376(r14),r13
addi r14,r14,-16
jl r15,foo
addi r14,r14,16
sw 328(r14),r13
lw r2,376(r14)
lw r1, 328(r14)
add r3,r1,r2 %s operation add performed 
sw 232(r14),r3
lw r2,136(r14)
lw r1, 232(r14)
add r3,r1,r2 %s operation add performed 
sw 100(r14),r3
lw r1,100(r14)
sw 92(r14),r1
addi r14,r14,-16
jl r15,foo
addi r14,r14,16
addi r14,r14,-16
jl r15,foo
addi r14,r14,16
addi r14,r14,-16
jl r15,foo
addi r14,r14,16
addi r1,r0,12 %s accumulator register is r1 
addi r2,r0,12 %s accumulator register is r2 
addi r3,r0,1 
sw 268(r14),r3
lw r3,268(r14)
muli r3,r3,4
add r2,r2,r3
addi r3, r0, 4 
sw indexstorage(r3), r2
addi r2,r0,1 
sw 236(r14),r2
addi r2,r0,3 
sw 172(r14),r2
addi r2,r0,2 
sw 320(r14),r2
addi r2,r0,1 
sw 368(r14),r2
lw r3,320(r14)
lw r2, 368(r14)
add r4,r2,r3 %s operation add performed 
sw 156(r14),r4
lw r3,172(r14)
lw r2, 156(r14)
add r4,r2,r3 %s operation add performed 
sw 104(r14),r4
lw r3,236(r14)
lw r2, 104(r14)
add r4,r2,r3 %s operation add performed 
sw 336(r14),r4
addi r5,r0,4
lw r5, indexstorage(r5) %s HERE1!!
add r5,r14,r5
lw r3, 0(r5)
lw r2, 336(r14)
add r4,r2,r3 %s operation add performed 
sw 168(r14),r4
lw r2,168(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 4 
sw indexstorage(r2), r1
addi r2,r0,4
lw r2, indexstorage(r2) %s here !!!
add r2,r14,r2
lw r1, 0(r2)
sw 92(r14),r1
addi r1,r0,12 %s accumulator register is r1 
addi r2,r0,12 %s accumulator register is r2 
addi r3,r0,4 
sw 128(r14),r3
lw r3,128(r14)
muli r3,r3,4
add r2,r2,r3
addi r3, r0, 4 
sw indexstorage(r3), r2
addi r2,r0,12 %s accumulator register is r2 
addi r3,r0,3 
sw 184(r14),r3
lw r3,184(r14)
muli r3,r3,4
add r2,r2,r3
addi r3, r0, 8 
sw indexstorage(r3), r2
addi r2,r0,12 %s accumulator register is r2 
addi r3,r0,2 
sw 4(r14),r3
lw r3,4(r14)
muli r3,r3,4
add r2,r2,r3
addi r3, r0, 12 
sw indexstorage(r3), r2
addi r2,r0,12 %s accumulator register is r2 
addi r3,r0,1 
sw 136(r14),r3
lw r3,136(r14)
muli r3,r3,4
add r2,r2,r3
addi r3, r0, 16 
sw indexstorage(r3), r2
addi r2,r0,12 %s accumulator register is r2 
addi r3,r0,0 
sw 196(r14),r3
lw r3,196(r14)
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
add r4,r2,r3 %s operation add performed 
sw 96(r14),r4
addi r5,r0,12
lw r5, indexstorage(r5) %s HERE1!!
add r5,r14,r5
lw r3, 0(r5)
lw r2, 96(r14)
add r4,r2,r3 %s operation add performed 
sw 328(r14),r4
addi r5,r0,8
lw r5, indexstorage(r5) %s HERE1!!
add r5,r14,r5
lw r3, 0(r5)
lw r2, 328(r14)
add r4,r2,r3 %s operation add performed 
sw 284(r14),r4
addi r5,r0,4
lw r5, indexstorage(r5) %s HERE1!!
add r5,r14,r5
lw r3, 0(r5)
lw r2, 284(r14)
add r4,r2,r3 %s operation add performed 
sw 220(r14),r4
lw r2,220(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 4 
sw indexstorage(r2), r1
addi r2,r0,4
lw r2, indexstorage(r2) %s here !!!
add r2,r14,r2
lw r1, 0(r2)
sw 92(r14),r1
lw r13, 92(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r1,r0,0 
sw 260(r14),r1
lw r1,260(r14)
sw 300(r14),r1
while101
addi r1,r0,10 
sw 240(r14),r1
lw r2,300(r14) %s relexpr
lw r1, 240(r14)
clt r3,r2,r1
sw 212(r14), r3
lw r1, 212(r14)
bz r1,endwhile101
addi r1,r0,12 %s accumulator register is r1 
lw r2,300(r14)
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
sw 176(r14),r2
lw r3,176(r14)
lw r2, 300(r14)
add r4,r2,r3 %s operation add performed 
sw 148(r14),r4
lw r2,148(r14)
sw 300(r14),r2
j while101
endwhile101
addi r2,r0,0 
sw 312(r14),r2
lw r2,312(r14)
sw 300(r14),r2
while102
addi r2,r0,10 
sw 120(r14),r2
lw r3,300(r14) %s relexpr
lw r2, 120(r14)
clt r4,r3,r2
sw 344(r14), r4
lw r2, 344(r14)
bz r2,endwhile102
addi r2,r0,12 %s accumulator register is r2 
lw r3,300(r14)
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
sw 252(r14),r2
lw r3,252(r14)
lw r2, 300(r14)
add r4,r2,r3 %s operation add performed 
sw 276(r14),r4
lw r2,276(r14)
sw 300(r14),r2
j while102
endwhile102
lw r15, 0(r14)
jr r15



entry
addi r14,r0,topaddr
addi r14,r14,-376
jl r15,main
addi r14,r14,376
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
