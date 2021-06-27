.extern W
MAIN:	mov @r3, LENGTH
LOOP:	jmp L1
	prn -5
	bne W
	sub @r1, @r4
	bne L3
L1:	inc K
	jmp W
;END:	stop
STR:	.string "lala"
LENGTH:	.data 6,-9,15, +55, 89, -48
K:	.data 22
.extern L3

mov K, @r1
KOKOJA: cmp K, @r1
add K, @r1
sub 3, @r1
lea K, @r1
not @r2
clr @r2
		  
inc @r2
dec K
jmp LOOP

bne LOOP
red @r1
prn @r1
jsr LOOP
rts
stop
