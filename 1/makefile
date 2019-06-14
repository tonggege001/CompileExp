parser.exe:parser.l parser.y
	bison -d -v parser.y
	flex parser.l
	gcc -o $@ def.c parser.tab.c lex.yy.c 