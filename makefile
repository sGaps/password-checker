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

COLOR := false
ifeq ($(strip $(COLOR)),false)
BL  :=
RL	:=
RS  :=
else
BL  :=\u001b[38;5;32m
RL  := \u001b[38;5;9m
RS  :=\u001b[0m
endif

.PHONY: default

# Compilación predeterminada
default: build
	@echo -e "$(BL)Compilación predeterminada completada.$(RS)"

# Forza la compilación del proyecto completo
force: | clean default


# Compila el programa principal y los programas de prueba:
all: build tests
	@echo -e "$(BL)Todo listo$(RS)"


# Compila las contraseñas (Tipo de dato)
$(OSRC_DIR)/types/password.o:
	@echo -e "$(BL)Construyendo tipo de datos Password en:$(RS) $(OTST_DIR)/tests/"
	$(CC) $(LIBFLG) src/types/password.c -o $@
	@echo -e ""

# Compila las funciones utilitarias para la sesión interactiva.
$(OSRC_DIR)/interactive.o:
	@echo -e "$(BL)Construyendo funciones para la sesión interactiva en:$(RS) $(OTST_DIR)/tests/"
	$(CC) $(LIBFLG) src/interactive.c -o $@
	@echo -e ""

# Compila los archivos del parser (bison)
$(OSRC_DIR)/parser.tab.o:
	@echo -e "$(BL)Traducción (Bison):$(RS)"
	$(BISON) -d src/parser.y --output=src/parser.tab.c
	@echo -e "$(BL)Compilacion del Parser (Bison/C):$(RS)"
	$(CC) $(LIBFLG) src/parser.tab.c -o $@ $(EXTFLG)
	@echo -e ""

# Compila los archivos del analizador léxico (flex)
$(OSRC_DIR)/lexer.yy.o: src/lexer.lex
	@echo -e "$(BL)Traducción (Flex):$(RS)"
	$(FLEX) -o src/lexer.yy.c src/lexer.lex
	@echo -e "$(BL)Compilacion del Analizador léxico (Flex/C):$(RS)"
	$(CC) $(LIBFLG) -o $(OSRC_DIR)/lexer.yy.o src/lexer.yy.c $(EXTFLG)
	@echo -e ""

# Compila el programa principal:
$(OBIN_DIR)/main.o:
	@echo -e "$(BL)Compilación del programa principal:$(RS)"
	$(CC) $(LIBFLG) src-bin/main.c -o $@ $(EXTFLG)
	@echo -e ""

# Crea el entorno de compilación:
prelude:
	@echo -e "$(BL)Ajustando el entorno para la compilación...$(RS)"
	mkdir -p $(TARGET_DIR) $(OBJS_DIRS) $(OSRC_DIR) $(OBIN_DIR) $(OSRC_DIR)/types $(OTST_DIR) $(XTST_DIR)
	@echo -e ""

# Compila todos los archivos objetos en un ejecutable
build: prelude | $(OSRC_DIR)/types/password.o \
			 	 $(OSRC_DIR)/interactive.o	  \
				 $(OSRC_DIR)/parser.tab.o 	  \
				 $(OSRC_DIR)/lexer.yy.o 	  \
				 $(OBIN_DIR)/main.o
	@echo -e "$(BL)Compilando Ejecutable (.out):$(RS)"
	$(CC) $(BINFLG) -o $(TARGET_DIR)/$(TARGET) $(BINFL) $| $(EXTFLG)
	@echo -e "$(BL)Hecho.$(RS)"
	@echo -e "$(BL)El archivo ejectuable está disponible en el directorio:$(RS) $(TARGET_DIR)/"
	@echo -e "$(BL)puede ejecutarlo con la instrucción $(RS)"
	@echo -e "    $(TARGET_DIR)/$(TARGET)"
	@echo -e "$(BL)y también, con el comando:$(RS)"
	@echo -e "    make run"
	@echo -e ""

# Compila y ejecuta el programa generado
run: build
	@echo -e ""
	$(TARGET_DIR)/$(TARGET)


# Remueve los archivos generados por bison y flex, además de eliminar los archivos objetos.
clean:
	@echo -e "$(RL)Eliminando archivos de compilación...$(RS)"
	rm -rf $(TARGET_DIR)/$(TARGET) $(XTST_DIR) $(OBJS_DIR) 
	rm -rf $(wildcard src/*.yy.c) $(wildcard src/*.tab.c) $(wildcard src/*.tab.h)
	@echo -e "$(BL)Hecho.$(RS)\n"

# Construye el caso de preuba para las contraseñas (Password)
password-test: prelude | $(OSRC_DIR)/types/password.o
	@echo -e "$(BL)Construyendo los casos de Prueba para el Password en:$(RS) $(OTST_DIR)/tests/"
	gcc $(LIBFLG) -o $(OTST_DIR)/password-test.o tests/password-test.c
	gcc $(BINFLG) -o $(XTST_DIR)/$@ $(OTST_DIR)/password-test.o $|
	@echo -e ""

# Construye todos los casos de pruebas
tests: password-test
	@echo -e "$(BL)Ejecutables de pruebas disponibles en el directorio:$(RS) $(XTST_DIR)/"
	@echo -e ""

# Remueve los archivos generados para distrubución
clean-package:
	rm -rf $(PKG_DIR)
	@echo -e "$(BL)Hecho.$(RS)\n"

# Genera los archivos para su distribución. Puede emplear
# ```
# make package ZIPNAME='nombre-del-archivo-comprimido'
# ```
# para generar un archivo zip del proyecto con el nombre `nombre-del-archivo-comprimido.zip`
package: clean
	@echo ""
	@echo -e "$(BL)Preparando para enviar...$(RS)"
	mkdir -p $(ZIPNAME) $(PKG_DIR)
	cp  -r makefile src/ src-bin/ tests/ $(ZIPNAME)
	@echo -e "$(BL)Comprimiendo archivos...$(RS)"
	zip -r $(ZIPNAME)/$(ZIPNAME).zip $(ZIPNAME)/* 
	mv 	   $(ZIPNAME)/$(ZIPNAME).zip $(PKG_DIR)/
	rm  -r $(ZIPNAME)
	@echo -e "$(BL)Hecho.$(RS)\n"
	@echo -e "$(RL)Recuerda: ¡Deberías actualizar el directorio antes de enviar este paquete!$(RS)"

