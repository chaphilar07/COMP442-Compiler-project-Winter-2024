entry
%s This subroutine can read a value from the standard input.
read 
getc r1
ceqi r2,r1,10
subi r1,r1,48
bnz r2,done
add r3,r3,r1
muli r3,r3,10
j read

done
hlt

