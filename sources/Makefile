CC=-pedantic -ansi

Propa:main.c import.c parserASPARTIX.c propagation1.c propagation2.c propagation3.c BH.c Dbs.c Bbs.c fuzzyLabel.c alphaBBS.c cs.c propa.h
	gcc $(CC) -o propa *.c -lm -g

clean:
	rm propa
	rm combine/all
	rm *~

#valgrind --leak-check=full --read-var-info=yes
