## Common
#CC     := gcc
#DEBUG  := -g -O0
#COMMON := $(DEBUG) -Wall -std=c11
#LIBFL  := $(COMMON) -c 
#BINFL  := $(COMMON)
#
## Directories
#SRC_DIRS  := src
#BIN_DIRS  := src-bin
#
## use:
## bison -d file.y --output=dir/file.tab.c
## first bisons, then flexes, compile and generate binaries.
#MAIN   := parser.c
#BISONS := $(foreach sdir, $(SRC_DIRS) $(BIN_DIRS) , $(wildcard $(sdir)/*.y))
#FLEXES := $(foreach sdir, $(SRC_DIRS) $(BIN_DIRS) , $(wildcard $(srdir)*.lex))
#
#BINS   = $(MAIN)
#SRCS   = $(foreach sdir, $(SRC_DIRS)  ,$(wildcard $(sdir)/*.c))
#TESTS := $(foreach tdir, $(TEST_DIRS) ,$(wildcard $(tdir)/*.c))
#
#TARGET_DIR  := target
#OBJS_DIR    := $(TARGET_DIR)/objs
#TARGET_TREE := $(addprefix $(OBJS_DIR)/,$(SRC_DIRS) $(BIN_DIRS))
#OBJS         = $(foreach src, $(SRCS) $(BINS),$(OBJS_DIR)/$(basename $(src)).o)
#
## Files & Sources:
#OSRC_DIR := $(OBJS_DIR)/$(SRC_DIR)
#OBIN_DIR := $(OBJS_DIR)/$(BIN_DIR)

CC	   := gcc
FLEX   := flex
BISON  := bison

DEBUG  := -g -O0
COMMON := $(DEBUG) -Wall -std=c11
LIBFLG := $(COMMON) -c
BINFLG := $(COMMON)
EXTFLG := -lfl -ly

TARGET	   := psw-check
TARGET_DIR := target
OBJS_DIR   := target/objs
OSRC_DIR   := target/objs/src
OBIN_DIR   := target/objs/src-bin

.PHONY: default

default: build
	@echo -e "Default Done"

# Data-Type:
$(OSRC_DIR)/types/password.o:
	$(CC) $(LIBFLG) src/types/password.c -o $@

# Data-Type:
$(OSRC_DIR)/interactive.o:
	$(CC) $(LIBFLG) src/interactive.c -o $@

# Bison:
$(OSRC_DIR)/parser.tab.o:
	@echo -e "Bison translation:"
	$(BISON) -d src/parser.y --output=src/parser.tab.c
	@echo -e "compilation (Bison-C source):"
	$(CC) $(LIBFLG) src/parser.tab.c -o $@ $(EXTFLG)

# Flex:
$(OSRC_DIR)/lexer.yy.o: src/lexer.lex
	@echo -e "Bison translation:"
	$(FLEX) -o src/lexer.yy.c src/lexer.lex
	@echo -e "compilation (Flex-C source):"
	$(CC) $(LIBFLG) -o $(OSRC_DIR)/lexer.yy.o src/lexer.yy.c $(EXTFLG)

# Main Binary:
$(OBIN_DIR)/main.o:
	$(CC) $(LIBFLG) src-bin/main.c -o $@ $(EXTFLG)

prelude:
	mkdir -p $(TARGET_DIR) $(OBJS_DIRS) $(OSRC_DIR) $(OBIN_DIR) $(OSRC_DIR)/types

build: prelude | $(OSRC_DIR)/types/password.o \
			 	 $(OSRC_DIR)/interactive.o	  \
				 $(OSRC_DIR)/parser.tab.o 	  \
				 $(OSRC_DIR)/lexer.yy.o 	  \
				 $(OBIN_DIR)/main.o
	@echo -e "Final build"
	$(CC) $(BINFLG) -o $(TARGET_DIR)/$(TARGET) $(BINFL) $| $(EXTFLG)

clean:
	rm -rf $(TARGET_DIR)


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

old-clean:
#rm -rf parser.lex.c parser.tab.c parser.tab.h
	rm -rf parser.lex.c
