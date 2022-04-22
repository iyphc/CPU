build_asm: asm.o string.o parser.o
	gcc -g asm.o string.o parser.o -o asm.out

build_cpu: cpu.o stack.o
	gcc -g cpu.o stack.o -o cpu.out

cpu.o: CPU/cpu.c 
	gcc -c -g CPU/cpu.c

asm.o: CPU/asm.c 
	gcc -c -g CPU/asm.c

string.o: parser/string.c
	gcc -c -g parser/string.c 

parser.o: parser/parser.c 
	gcc -c -g parser/parser.c 

stack.o: stack/stack.c 
	gcc -c -g stack/stack.c

clean:
	rm -f *.o 
	rm -f *.out
	rm -f *.myexe