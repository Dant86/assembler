assembler: linkedlist.o hashtable.o assembler.o parser.o
	gcc -o assembler linkedlist.o hashtable.o assembler.o parser.o

assembler.o: assembler.c linkedlist.h hashtable.h parser.h
	gcc -c assembler.c

parser.o: parser.c parser.h hashtable.h
	gcc -c parser.c

hashtable.o: hashtable.c linkedlist.h hashtable.h
	gcc -c hashtable.c

linkedlist.o: linkedlist.c linkedlist.h
	gcc -c linkedlist.c