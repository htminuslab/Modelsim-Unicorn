	.section .text
	.global reset_vec

reset_vec:
	j start

start:
	// init stack pointer
	lui sp, %hi(_stack)
	addi sp, sp, %lo(_stack)

	// jump to C main function
	jal ra, main

	// if it returns issue exception
	ebreak

		