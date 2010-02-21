main:
	flex CJuicer.lex
	gcc lex.yy.c -lfl -o CJuicer.x -Wall
test:
	./CJuicer.x < SomeTestFile.c
	more test.dat