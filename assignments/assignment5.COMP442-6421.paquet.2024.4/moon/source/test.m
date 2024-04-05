topofstackptr res 4

entry

addi r14, r0, topaddr
sw topofstackptr(r0), r14 

%s we want to push a frame onto the stack in  this case we SUBTRACT from the stack pointer.

addi r14, r14, -96
sw topofstackptr(r0), r14


hlt
