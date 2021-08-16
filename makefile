.PHONY: default

default: psw
	@echo -e "Default Done"

lexical-syntax-gen:
#bison -d psw.y
	bison --report all -d psw.y
	flex  -o psw.lex.c psw.lex

password-test: password.o password.test.c
	gcc -g -o $@ password.test.c password.o

password.o: password.c password.h
	gcc -g -c password.c -o $@

psw: lexical-syntax-gen password.o
#gcc -o $@ psw.lex.c psw.tab.c -lfl -lm 
	gcc -g -o $@ psw.lex.c psw.tab.c password.o -lfl -lm 

clean:
#rm -rf parser.lex.c parser.tab.c parser.tab.h
	rm -rf parser.lex.c
