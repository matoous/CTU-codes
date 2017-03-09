#define t0 $8
#define t1 $9
#define t2 $10
#define t3 $11
#define t4 $12

#define k1 $31
#define k2 $32
#define k3 $33
#define k4 $34
#define k5 $35

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
// init loop 1
addi z, $0, 1 // pernamentni 1
addi s1, $0, 0  // i = 0
addi s2, $0, 5  // delka pole
for1:
	beq s1, s2, done1  // if loop 1 end
	addi s3, $0, 0  // j = 0
	addi s4, $0, 0  // horni mez loop 2
	addi s4, s2, 0  // horni mez loop 2 = N
	sub s4, s4, 1  // horni mez loop 2 -= 1
	sub s4, s4, s1	 // horni mez loop 2 -= i
	for2:
		beq s3, s4, done2  // if loop2 end
		lw t0, 0x0(s0)  // nacte z pole prvek N
                addi s0, s0, 0x4  // posun v poli o 1
                lw t1, 0x0(s0)  // nacte z pole prvek N+1
                slt t2, t1, t0  // porovno jestli je t1 mensi t2
                beq t2, z, ifend
			add t3, $0, t1  // temp = pole n+1
                        add t1, $0, t0  // pole n+1 = pole n
                        add t0, $0, t3  // pole n  = temp
		ifend:
                sw t1, 0x0(s0) // uloz pole[n+1]
		sub s0, s0, 0x4  // prejdi na pole [n]
		sw t0, 0x0(s0)  // uloz pole[n]
                addi s0, s0, 0x4 // prejdi na pole[n]
		addi s3, s3, 1  // j++
                j for2  // repeat loop 2
	done2:
	addi s1, s1, 1  // i++
        j for1  // repeat loop 1
done1:
nop
.end start
