.PHONY: default

default: psw
	@echo -e "Default Done"

lexical-syntax-gen:
#bison -d psw.y
	bison --report all -d psw.y
	flex  -o psw.lex.c psw.lex

psw: lexical-syntax-gen
#gcc -o $@ psw.lex.c psw.tab.c -lfl -lm 
	gcc -g -o $@ psw.lex.c psw.tab.c -lfl -lm 

clean:
#rm -rf parser.lex.c parser.tab.c parser.tab.h
	rm -rf parser.lex.c
