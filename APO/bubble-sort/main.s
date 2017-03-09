.globl    array
.data
.align    2

array:
.word    5,3,8,9,4,1,2,7,6

.text
.globl start
.ent start

start:
la $16, array  // save array addr
addi $30, $0, 1  // create const. 1
addi $17, $0, 0  // i = 0
addi $18, $0, 9  // i < 9
for1:
	beq $17, $18, done1  // if i == 9 jump
	addi $19, $0, 0  // j = 0
	addi $20, $18, 0  // j < 9
	sub $20, $20, 1  // j < 9 - 1
	sub $20, $20, $17  // j < 9 - 1 - i
	for2:
		beq $19, $20, done2  // j == 9 - 1 - i
		lw $8, 0x0($16)  // load array[N]
                addi $16, $16, 0x4  // N+1
                lw $9, 0x0($16)  // load array[N+1]
                slt $10, $8, $9  // array[N+1] < array[N]
                beq $10, $30, ifend  // if upper if is ok
			add $11, $0, $9  // switch
                        add $9, $0, $8  // switch
                        add $8, $0, $11  // switch
		ifend:
                sw $9, 0x0($16)
		sub $16, $16, 0x4
		sw $8, 0x0($16)
                addi $16, $16, 0x4
		addi $19, $19, 1
                j for2
	done2:
	addi $17, $17, 1
        j for1
done1:
nop
.end start
