build: asm.o string.o parser.o
	gcc -g asm.o string.o parser.o

asm.o: CPU/asm.c 
	gcc -c -g CPU/asm.c

string.o: parser/string.c
	gcc -c -g parser/string.c 

parser.o: parser/parser.c 
	gcc -c -g parser/parser.c 

clean:
	rm parser.o string.o asm.o