# @file makefile
# @brief archivo empleado para la compilación
# @author Gabriel Peraza. CI: 26929687
# @version 0.1.0
# @date 2021-08-17

# Compilador, lex y yacc:
CC     := gcc
FLEX   := flex
BISON  := bison

# Banderas de compilación:
DEBUG  := -g -O0
COMMON := $(DEBUG) -Wall -std=c11
LIBFLG := $(COMMON) -c
BINFLG := $(COMMON)
# Banderas especiales de las librerías de flex y bison
EXTFLG := -lfl -ly

# Nombre del ejecutable y carpeta destino
TARGET	   := password-checker.out
TARGET_DIR := .

# Nombres de la carpetas donde se encontrarán los archivos objetos
OBJS_DIR   := $(TARGET_DIR)/objs
OSRC_DIR   := $(OBJS_DIR)/src
OBIN_DIR   := $(OBJS_DIR)/src-bin

# Nombre de las carpetas donde se encontrarán los programas de prueba
OTST_DIR   := $(OBJS_DIR)/tests
XTST_DIR   := $(TARGET_DIR)/xtests

# Variables para empaquetar el proyecto:
PKG_DIR := package
ZIPNAME := project-password-checker
COMMENT := 'Tarea 01. Lenguajes de Programacion'

.PHONY: default

# Compilación predeterminada
default: build
	@echo -e "Default Done"

# Forza la compilación del proyecto completo
force: | clean default


# Compila el programa principal y los programas de prueba:
all: build tests
	@echo -e "Everything Done"


# Compila las contraseñas (Tipo de dato)
$(OSRC_DIR)/types/password.o:
	$(CC) $(LIBFLG) src/types/password.c -o $@

# Compila las funciones utilitarias para la sesión interactiva.
$(OSRC_DIR)/interactive.o:
	$(CC) $(LIBFLG) src/interactive.c -o $@

# Compila los archivos del parser (bison)
$(OSRC_DIR)/parser.tab.o:
	@echo -e "Bison translation:"
	$(BISON) -d src/parser.y --output=src/parser.tab.c
	@echo -e "compilation (Bison-C source):"
	$(CC) $(LIBFLG) src/parser.tab.c -o $@ $(EXTFLG)

# Compila los archivos del analizador léxico (flex)
$(OSRC_DIR)/lexer.yy.o: src/lexer.lex
	@echo -e "Bison translation:"
	$(FLEX) -o src/lexer.yy.c src/lexer.lex
	@echo -e "compilation (Flex-C source):"
	$(CC) $(LIBFLG) -o $(OSRC_DIR)/lexer.yy.o src/lexer.yy.c $(EXTFLG)

# Compila el programa principal:
$(OBIN_DIR)/main.o:
	$(CC) $(LIBFLG) src-bin/main.c -o $@ $(EXTFLG)

# Crea el entorno de compilación:
prelude:
	mkdir -p $(TARGET_DIR) $(OBJS_DIRS) $(OSRC_DIR) $(OBIN_DIR) $(OSRC_DIR)/types $(OTST_DIR) $(XTST_DIR)

# Compila todos los archivos objetos en un ejecutable
build: prelude | $(OSRC_DIR)/types/password.o \
			 	 $(OSRC_DIR)/interactive.o	  \
				 $(OSRC_DIR)/parser.tab.o 	  \
				 $(OSRC_DIR)/lexer.yy.o 	  \
				 $(OBIN_DIR)/main.o
	@echo -e "Final build"
	$(CC) $(BINFLG) -o $(TARGET_DIR)/$(TARGET) $(BINFL) $| $(EXTFLG)
	@echo -e "Build Done. executable available on $(TARGET_DIR)/"

# Compila y ejecuta el programa generado
run: build
	@echo -e ""
	$(TARGET_DIR)/$(TARGET)


# Remueve los archivos generados por bison y flex, además de eliminar los archivos objetos.
clean:
	rm -rf $(TARGET_DIR)/$(TARGET) $(XTST_DIR) $(OBJS_DIR) 
	rm -rf $(wildcard src/*.yy.c) $(wildcard src/*.tab.c) $(wildcard src/*.tab.h)

# Construye el caso de preuba para las contraseñas (Password)
password-test: prelude | $(OSRC_DIR)/types/password.o
	gcc $(LIBFLG) -o $(OTST_DIR)/password-test.o tests/password-test.c
	gcc $(BINFLG) -o $(XTST_DIR)/$@ $(OTST_DIR)/password-test.o $|

# Construye todos los casos de pruebas
tests: password-test
	@echo -e "Test are availables on $(TARGET_DIR)/tests/"

# Remueve los archivos generados para distrubución
clean-package:
	rm -rf $(PKG_DIR)

# Genera los archivos para su distribución. Puede emplear
# ```
# make package ZIPNAME='nombre-del-archivo-comprimido'
# ```
# para generar un archivo zip del proyecto con el nombre `nombre-del-archivo-comprimido.zip`
package: clean
	@echo ""
	@echo -e "Prepare to send"
	mkdir -p $(ZIPNAME) $(PKG_DIR)
	cp  -r makefile src/ src-bin/ tests/ $(ZIPNAME)
	zip -r $(ZIPNAME)/$(ZIPNAME).zip $(ZIPNAME)/* 
	mv 	   $(ZIPNAME)/$(ZIPNAME).zip $(PKG_DIR)/
	rm  -r $(ZIPNAME)
	@echo -e "Remember: you should update all directories that are seen by this tool before sending this package!"

