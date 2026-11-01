mypreprocessor: pre.o comment_removal.o header_inclusion.o macro_sub.o
	gcc pre.o comment_removal.o header_inclusion.o macro_sub.o -o mypreprocessor

pre.o: pre.c
	gcc -c pre.c -o pre.o

comment_removal.o: comment_removal.c
	gcc -c comment_removal.c -o comment_removal.o

header_inclusion.o: header_inclusion.c
	gcc -c header_inclusion.c -o header_inclusion.o

macro_sub.o: macro_sub.c
	gcc -c macro_sub.c -o macro_sub.o

clean:
	rm -f pre.o comment_removal.o header_inclusion.o macro_sub.o mypreprocessor
