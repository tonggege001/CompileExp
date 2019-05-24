fb:fb4.l fb4.y
	bison -d fb4.y
	flex fb4.l
	gcc -o $@ fb4.tab.c lex.yy.c
