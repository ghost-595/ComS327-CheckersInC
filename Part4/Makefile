edit: edit.o
	gcc -o edit edit.o -ltermbox

edit.o: edit.c changes.o info.o
	gcc -g -c edit.c -ltermbox

changes: changes.c info.o
	gcc -g -c changes.c

info: info.c
	gcc -g -c info.c
