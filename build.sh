gcc -shared -O2 charset.c -o libcharset.so
gcc -Wall -pedantic -ansi -O2 -L./ -lm -lcharset chartests.c -o chartest
