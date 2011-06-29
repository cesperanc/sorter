##
# Makefile que facilmente se adapta a qualquer tipo de exercicio
# 
# author Vitor Carreira
# date 2009-09-23
# 
# author Cláudio Esperança
# date 2009-10-28
##

## Carrega o ficheiro de configuração
include ./configs/makefile.inc

## Flags para o compilador
CFLAGS=-Wall -W -g -Wmissing-prototypes 

## Flags para a indentacao de codigo 
IFLAGS=-br -brs -npsl -ce -cli4

## Directórios com o código fonte do projecto
INCLUDE_DIRS=${SRC_DIR_3RD} ${SRC_DIR_INCLUDE} ${SRC_DIR} ${EXTRA_INCLUDE_DIRS}

## Cria uma lista de objectos a partir dos ficheiros .c dos directórios especificados na variável INCLUDE_DIRS
PROGRAM_OBJS:=$(patsubst %.c,%.o,$(wildcard $(patsubst %,./%/*.c,${INCLUDE_DIRS})))

## Se a variável com o ficheiro de opções estiver definida, adicionar o seu ficheiro objecto como dependência (caso não esteja já adicionada)
ifdef PROGRAM_OPT
PROGRAM_OPT_o:=${SRC_DIR_3RD}/${PROGRAM_OPT}.o
ifeq (,$(findstring $(PROGRAM_OPT_o),$(PROGRAM_OBJS)))
PROGRAM_OBJS:=$(PROGRAM_OBJS) $(PROGRAM_OPT_o)
endif
endif

## Código da instrução a ser executada na solitação de parametros
ifdef params
PARAMS=zenity --entry --title="Executar programa" --text="Introduza os parametros para o executável"
endif

## Targets abstractos
.PHONY: clean
.PHONY: cleanall
.PHONY: cleandocs
.PHONY: all

## Compilar com depuracao
depuracao: CFLAGS += -D SHOW_DEBUG 
depuracao: ${PROGRAM}

## Constroi o executável
${PROGRAM}: ${PROGRAM_OBJS}
	@echo "A compilar '$@':"
	${CC} ${EXTRA_CCFLAGS} -o $@ ${PROGRAM_OBJS} ${LIBS}

## Compilar .o a partir de .c
.c.o: 
	@echo "A construir o objecto '$@':"
	${CC} ${CFLAGS} ${EXTRA_CCFLAGS} -c -o $@ $<

## Para gerar os ficheiros do gengetopt 
${SRC_DIR_3RD}/${PROGRAM_OPT}.h: configs/${PROGRAM_OPT}.ggo
	gengetopt < configs/${PROGRAM_OPT}.ggo --output-dir=${SRC_DIR_3RD}/ --file-name=${PROGRAM_OPT}

## Para além do target clean, limpa também os ficheiro de opções, bem como a pasta docs. Utilizar com cuidado!
cleanall: clean cleandocs
	@for d in $(INCLUDE_DIRS); do (cd $$d; rm -fv ${PROGRAM_OPT}.h ${PROGRAM_OPT}.c ); done

## Limpeza dos directórios e os respectivos subdirectórios
clean:
	@for d in $(INCLUDE_DIRS); do (cd $$d; echo "A limpar o directório '$$d':"; rm -fv *.o core.* *~ ${PROGRAM} *.bak ); done

## Remove a pasta com a documentação
cleandocs:
	@echo "A eliminar o directório da documentação"; rm -rfv docs

## Para documentação
docs: configs/Doxyfile
	doxygen configs/Doxyfile

configs/Doxyfile:
	doxygen -g configs/Doxyfile

## Para do Windows para o Linux
indent:
	dos2unix *.c *.h
	indent ${IFLAGS} *.c *.h


## Compila e executa o programa numa janela do gnome-terminal; adicionar a definição de uma variável params=true para requisitar parâmetros ao utilizador
run: all
	@gnome-terminal  -t "Execução de ${PROGRAM}" -e "bash -c '\"./${PROGRAM}\" `${PARAMS}`; echo -e  \"\n\n\n----------------\" ; read -n1 -r -p \"Pressione uma tecla para sair...\"'"
