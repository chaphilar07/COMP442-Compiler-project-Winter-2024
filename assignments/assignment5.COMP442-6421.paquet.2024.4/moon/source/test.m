entry

addi r1,r0,12345 % a 10 digit number.
addi r2,r0,1

mag

div r3,r1,r2
cgei r4,r3,10 
bz r4,print 
muli r2,r2,10 
j mag


print
div r3, r1,r2 
addi r3,r3,48
putc r3
subi r3,r3,48
mul r4,r3,r2 
sub r1, r1, r4
divi r2,r2,10 
ceqi r3,r2,0 
bnz r3, done
j print


done hlt
