#define t0 $8
#define t1 $9
#define t2 $10
#define t3 $11
#define t4 $12

#define s0 $16
#define s1 $17
#define s2 $18
#define s3 $19
#define s4 $20
#define s5 $21
#define s6 $22

#define z $30


.globl    pole
.data
.align    2

pole:
.word    5,3,4,1,2

.text
.globl start
.ent start

start:
la s0, pole  
addi z, $0, 1
addi s1, $0, 0
addi s2, $0, 5
for1:
	beq s1, s2, done1
	addi s3, $0, 0
	addi s4, $0, 0
	addi s4, s2, 0
	sub s4, s4, 1
	sub s4, s4, s1
	for2:
		beq s3, s4, done2
		lw t0, 0x0(s0)
                addi s0, s0, 0x4
                lw t1, 0x0(s0)
                slt t2, t1, t0
                beq t2, z, ifend
			add t3, $0, t1
                        add t1, $0, t0
                        add t0, $0, t3
		ifend:
                sw t1, 0x0(s0)
		sub s0, s0, 0x4
		sw t0, 0x0(s0)
                addi s0, s0, 0x4
		addi s3, s3, 1
                j for2
	done2:
	addi s1, s1, 1
        j for1
done1:
nop
.end start
