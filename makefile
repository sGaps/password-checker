CC     := gcc
FLEX   := flex
BISON  := bison

DEBUG  := -g -O0
COMMON := $(DEBUG) -Wall -std=c11
LIBFLG := $(COMMON) -c
BINFLG := $(COMMON)
EXTFLG := -lfl -ly

TARGET	   := psw-check
TARGET_DIR := target
OBJS_DIR   := $(TARGET_DIR)/objs
OSRC_DIR   := $(OBJS_DIR)/src
OBIN_DIR   := $(OBJS_DIR)/src-bin

OTST_DIR   := $(OBJS_DIR)/tests
XTST_DIR   := $(TARGET_DIR)/tests

PKG_DIR := package
PACKAGE := package

.PHONY: default

force: | clean default

default: build
	@echo -e "Default Done"


all: build tests
	@echo -e "Everything Done"


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
	mkdir -p $(TARGET_DIR) $(OBJS_DIRS) $(OSRC_DIR) $(OBIN_DIR) $(OSRC_DIR)/types $(OTST_DIR) $(XTST_DIR)

build: prelude | $(OSRC_DIR)/types/password.o \
			 	 $(OSRC_DIR)/interactive.o	  \
				 $(OSRC_DIR)/parser.tab.o 	  \
				 $(OSRC_DIR)/lexer.yy.o 	  \
				 $(OBIN_DIR)/main.o
	@echo -e "Final build"
	$(CC) $(BINFLG) -o $(TARGET_DIR)/$(TARGET) $(BINFL) $| $(EXTFLG)
	@echo -e "Build Done. executable available on $(TARGET_DIR)/"


run: build
	@echo -e ""
	$(TARGET_DIR)/$(TARGET)


clean:
	rm -rf $(TARGET_DIR)
	rm -rf $(wildcard src/*.yy.c) $(wildcard src/*.tab.c) $(wildcard src/*.tab.h)

password-test: prelude | $(OSRC_DIR)/types/password.o
	gcc $(LIBFLG) -o $(OTST_DIR)/password-test.o tests/password-test.c
	gcc $(BINFLG) -o $(XTST_DIR)/$@ $(OTST_DIR)/password-test.o $|


tests: password-test
	@echo -e "Test are availables on $(TARGET_DIR)/tests/"

clean-package:
	rm -rf $(PKG_DIR)

package: clean
	@echo ""
	@echo -e "Prepare to send"
	mkdir -p $(PKG_DIR)
	zip -r $(PKG_DIR)/$(PACKAGE).zip makefile src/ src-bin/ tests/ 
	@echo -e "Remember: you should update all directories that are seen by this tool before send this package!"

