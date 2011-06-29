##
# UdpTime Makefile
# 
# author Vitor Carreira
# date 2009-09-23
# 
# author Cláudio Esperança
# date 2009-10-28
##

## Loads the configuration file
include ./configs/makefile.inc

## Flags to the compiler
CFLAGS=-Wall -W -g -Wmissing-prototypes 

## Flags to code indentation
IFLAGS=-br -brs -npsl -ce -cli4

## Directories with the project source code
INCLUDE_DIRS=${SRC_DIR_3RD} ${SRC_DIR_INCLUDE} ${SRC_DIR} ${EXTRA_INCLUDE_DIRS}

## Generates a list of objects from the .c files on the directories specified on the variable INCLUDE_DIRS
PROGRAM_OBJS:=$(patsubst %.c,%.o,$(wildcard $(patsubst %,./%/*.c,${INCLUDE_DIRS})))

## If the variable with the options file is set, add the object file as a dependency
ifdef PROGRAM_OPT
PROGRAM_OPT_o:=${SRC_DIR_3RD}/${PROGRAM_OPT}.o
ifeq (,$(findstring $(PROGRAM_OPT_o),$(PROGRAM_OBJS)))
PROGRAM_OBJS:=$(PROGRAM_OBJS) $(PROGRAM_OPT_o)
endif
endif

## Code to be executed to obtain the execution parameters
ifdef params
PARAMS=zenity --entry --title="Execute program" --text="Enter the parameters to the executable"
endif

## Abstract Targets
.PHONY: clean
.PHONY: cleanall
.PHONY: cleandocs
.PHONY: all

## Compile with depuration
depuracao: CFLAGS += -D SHOW_DEBUG 
depuracao: ${PROGRAM}

## Constructs the executable
${PROGRAM}: ${PROGRAM_OBJS}
	@echo "Compiling '$@':"
	${CC} ${EXTRA_CCFLAGS} -o $@ ${PROGRAM_OBJS} ${LIBS}

## Compile .o from .c
.c.o: 
	@echo "Construction the object '$@':"
	${CC} ${CFLAGS} ${EXTRA_CCFLAGS} -c -o $@ $<

## To generate the files with gengetopt 
${SRC_DIR_3RD}/${PROGRAM_OPT}.h: configs/${PROGRAM_OPT}.ggo
	gengetopt < configs/${PROGRAM_OPT}.ggo --output-dir=${SRC_DIR_3RD}/ --file-name=${PROGRAM_OPT}

## Besides the clean target, also cleans the options files and the docs folder. Use with care!
cleanall: clean cleandocs
	@for d in $(INCLUDE_DIRS); do (cd $$d; rm -fv ${PROGRAM_OPT}.h ${PROGRAM_OPT}.c ); done

## Cleaning of the directories and subdirectories
clean:
	@for d in $(INCLUDE_DIRS); do (cd $$d; echo "Cleaning the directory '$$d':"; rm -fv *.o core.* *~ ${PROGRAM} *.bak ); done

## Remove the documentação folder
cleandocs:
	@echo "Removing the documentation folder"; rm -rfv docs

## To documentation
docs: configs/Doxyfile
	doxygen configs/Doxyfile

configs/Doxyfile:
	doxygen -g configs/Doxyfile

## From Windows do Linux
indent:
	dos2unix *.c *.h
	indent ${IFLAGS} *.c *.h


## Compiles and executes the program on a gnome-terminal window; add the variable params=true to request parameters from the user
run: all
	@gnome-terminal  -t "Execution of ${PROGRAM}" -e "bash -c '\"./${PROGRAM}\" `${PARAMS}`; echo -e  \"\n\n\n----------------\" ; read -n1 -r -p \"Press a key to exit...\"'"
