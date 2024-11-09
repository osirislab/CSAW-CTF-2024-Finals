# Makefile for building and cleaning an assembly program

# Phony targets so make won't check for files named 'compile' or 'clean'
.PHONY: compile clean

# Target to compile the program
compile:
	nasm -f elf64 -o app.o app.asm
	ld -o app app.o

# Target to clean up generated files
clean:
	rm -f app app.o a.out

