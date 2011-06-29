## nome do executavel 
PROGRAM=Sorter

## target por omissão do make
all: ${PROGRAM}

## Nome do ficheiro de opcoes do gengetopt (comentar caso não exista)
PROGRAM_OPT=sorter_options

## Directório com o código fonte do programa principal
SRC_DIR=./src
## Directório com o código fonte das bibliotecas auxiliares do programa
SRC_DIR_INCLUDE=${SRC_DIR}/includes
## Directório com o código fonte das bibliotecas desenvolvidas por terceiros
SRC_DIR_3RD=${SRC_DIR}/3rd

## Lista de directórios com ficheiros .c a utilizar como .o para o programa principal
EXTRA_INCLUDE_DIRS=./

${SRC_DIR}/main.o: ${SRC_DIR}/main.c ${SRC_DIR}/main.h ${SRC_DIR_INCLUDE}/definitions.h ${SRC_DIR_3RD}/debug.h ${SRC_DIR_3RD}/${PROGRAM_OPT}.h
${SRC_DIR_3RD}/debug.o: ${SRC_DIR_3RD}/debug.c ${SRC_DIR_3RD}/debug.h

## dependências do ficheiro obj do gengetopt
${SRC_DIR_3RD}/${PROGRAM_OPT}.o: ${SRC_DIR_3RD}/${PROGRAM_OPT}.c ${SRC_DIR_3RD}/${PROGRAM_OPT}.h


## Esta linha força a compilação do programa a 32Bits
#EXTRA_CCFLAGS=-m32

## Bibliotecas a incluir
LIBS=#-pthread 
