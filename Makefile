dreg: dreg.c dreg.h charset.h charset.c
	cc -Os -Wall -ansi -pedantic -o libcharset.so -shared charset.c
	cc -Os -Wall -ansi -pedantic -o libdreg.so -shared dreg.c

install:
	cp libcharset.so /usr/lib
	cp libdreg.so /usr/lib

clean:
	rm *.o *.so

test:
	cc -Os -Wall -ansi -pedantic -o chartest.o -lm -lcharset chartests.c

push:
	git add *.c *.h Makefile README
	git commit 
	git push -u origin master
