all: Latex2Markdown clean
Latex2Markdown: lexer.l parser.y header.h
	bison -d parser.y
	flex -o lexer.c lexer.l
	gcc -o $@.out parser.tab.c lexer.c functions.c -lm
	@echo $@ Parser compilation finished....
clean: 
	rm parser.tab.c parser.tab.h lexer.c