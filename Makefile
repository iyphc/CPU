build_asm: asm.o string.o parser.o
	clang -g asm.o string.o parser.o -o asm.out

build_cpu: cpu.o stack.o
	clang -g cpu.o stack.o -o cpu.out

cpu.o: CPU/cpu.c 
	clang -c -g CPU/cpu.c

asm.o: CPU/asm.c 
	clang -c -g CPU/asm.c

string.o: parser/string.c
	clang -c -g parser/string.c 

parser.o: parser/parser.c 
	clang -c -g parser/parser.c 

stack.o: stack/stack.c 
	clang -c -g stack/stack.c

clean:
	rm -f *.o 
	rm -f *.out
	rm -f *.myexe