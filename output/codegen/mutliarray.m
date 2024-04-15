align
main
sw 0(r14),r15
addi r1,r0,0 
sw 4180(r14),r1
lw r1,4180(r14)
sw 4132(r14),r1
addi r1,r0,0 
sw 4148(r14),r1
lw r1,4148(r14)
sw 4128(r14),r1
addi r1,r0,0 
sw 4060(r14),r1
lw r1,4060(r14)
sw 4052(r14),r1
addi r1,r0,0 
sw 4028(r14),r1
lw r1,4028(r14)
sw 4020(r14),r1
while100
addi r1,r0,10 
sw 4100(r14),r1
lw r2,4128(r14) %s relexpr
lw r1, 4100(r14)
clt r3,r2,r1
sw 4080(r14), r3
lw r1, 4080(r14)
bz r1,endwhile100
while101
addi r1,r0,10 
sw 4024(r14),r1
lw r2,4052(r14) %s relexpr
lw r1, 4024(r14)
clt r3,r2,r1
sw 4164(r14), r3
lw r1, 4164(r14)
bz r1,endwhile101
while102
addi r1,r0,10 
sw 4176(r14),r1
lw r2,4020(r14) %s relexpr
lw r1, 4176(r14)
clt r3,r2,r1
sw 4124(r14), r3
lw r1, 4124(r14)
bz r1,endwhile102
lw r2,4052(r14)
lw r1, 4128(r14)
add r3,r1,r2 %s operation add performed 
sw 4048(r14),r3
lw r2,4020(r14)
lw r1, 4048(r14)
add r3,r1,r2 %s operation add performed 
sw 4016(r14),r3
addi r1,r0,8 %s accumulator register is r1 
lw r2, 4128(r14)
addi r3,r0,40 %s problem here
mul r2,r2,r3
add r1,r1,r2
lw r2, 4052(r14)
addi r3,r0,40 %s problem here
mul r2,r2,r3
add r1,r1,r2
lw r2,4020(r14)
muli r2,r2,4
add r1,r1,r2
addi r2, r0, 0 
sw indexstorage(r2), r1
lw r1,4016(r14)
addi r2,r0,0
lw r2, indexstorage(r2) %s HERE !!
add r2,r14,r2
sw 0(r2),r1
addi r1,r0,1 
sw 4040(r14),r1
lw r2,4040(r14)
lw r1, 4020(r14)
add r3,r1,r2 %s operation add performed 
sw 4120(r14),r3
lw r1,4120(r14)
sw 4020(r14),r1
addi r1,r0,1 
sw 4076(r14),r1
lw r2,4076(r14)
lw r1, 4132(r14)
add r3,r1,r2 %s operation add performed 
sw 4088(r14),r3
lw r1,4088(r14)
sw 4132(r14),r1
j while102
endwhile102
addi r1,r0,0 
sw 4068(r14),r1
lw r1,4068(r14)
sw 4020(r14),r1
addi r1,r0,1 
sw 4104(r14),r1
lw r2,4104(r14)
lw r1, 4052(r14)
add r3,r1,r2 %s operation add performed 
sw 4012(r14),r3
lw r1,4012(r14)
sw 4052(r14),r1
j while101
endwhile101
addi r1,r0,0 
sw 4(r14),r1
lw r1,4(r14)
sw 4052(r14),r1
addi r1,r0,1 
sw 4036(r14),r1
lw r2,4036(r14)
lw r1, 4128(r14)
add r3,r1,r2 %s operation add performed 
sw 4160(r14),r3
lw r1,4160(r14)
sw 4128(r14),r1
j while100
endwhile100
lw r13, 4132(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
addi r1,r0,0 
sw 4144(r14),r1
lw r1,4144(r14)
sw 4128(r14),r1
addi r1,r0,0 
sw 4136(r14),r1
lw r1,4136(r14)
sw 4052(r14),r1
addi r1,r0,0 
sw 4168(r14),r1
lw r1,4168(r14)
sw 4020(r14),r1
while103
addi r1,r0,10 
sw 4032(r14),r1
lw r2,4128(r14) %s relexpr
lw r1, 4032(r14)
clt r3,r2,r1
sw 4116(r14), r3
lw r1, 4116(r14)
bz r1,endwhile103
while104
addi r1,r0,10 
sw 4064(r14),r1
lw r2,4052(r14) %s relexpr
lw r1, 4064(r14)
clt r3,r2,r1
sw 4152(r14), r3
lw r1, 4152(r14)
bz r1,endwhile104
while105
addi r1,r0,10 
sw 4096(r14),r1
lw r2,4020(r14) %s relexpr
lw r1, 4096(r14)
clt r3,r2,r1
sw 4044(r14), r3
lw r1, 4044(r14)
bz r1,endwhile105
addi r1,r0,8 %s accumulator register is r1 
lw r2, 4128(r14)
addi r3,r0,40 %s problem here
mul r2,r2,r3
add r1,r1,r2
lw r2, 4052(r14)
addi r3,r0,40 %s problem here
mul r2,r2,r3
add r1,r1,r2
lw r2,4020(r14)
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
addi r1,r0,1 
sw 4140(r14),r1
lw r2,4140(r14)
lw r1, 4020(r14)
add r3,r1,r2 %s operation add performed 
sw 4084(r14),r3
lw r1,4084(r14)
sw 4020(r14),r1
addi r1,r0,1 
sw 4172(r14),r1
lw r2,4172(r14)
lw r1, 4132(r14)
sub r3,r1,r2 %s operation sub performed 
sw 4156(r14),r3
lw r1,4156(r14)
sw 4132(r14),r1
j while105
endwhile105
addi r1,r0,0 
sw 4108(r14),r1
lw r1,4108(r14)
sw 4020(r14),r1
addi r1,r0,1 
sw 4072(r14),r1
lw r2,4072(r14)
lw r1, 4052(r14)
add r3,r1,r2 %s operation add performed 
sw 4008(r14),r3
lw r1,4008(r14)
sw 4052(r14),r1
j while104
endwhile104
addi r1,r0,0 
sw 4092(r14),r1
lw r1,4092(r14)
sw 4052(r14),r1
addi r1,r0,1 
sw 4056(r14),r1
lw r2,4056(r14)
lw r1, 4128(r14)
add r3,r1,r2 %s operation add performed 
sw 4112(r14),r3
lw r1,4112(r14)
sw 4128(r14),r1
j while103
endwhile103
lw r13, 4132(r14)
addi r14,r14,-8
jl r15, write
addi r14,r14, 8
lw r15, 0(r14)
jr r15



entry
addi r14,r0,topaddr
addi r14,r14,-4184
jl r15,main
addi r14,r14,4184
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
