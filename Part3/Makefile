rankmoves: rankmoves.o
	gcc -o rankmoves rankmoves.o

rankmoves.o: rankmoves.c changes.o info.o
	gcc -g -c rankmoves.c

changes: changes.c info.o
	gcc -g -c changes.c

info: info.c
	gcc -g -c info.c
