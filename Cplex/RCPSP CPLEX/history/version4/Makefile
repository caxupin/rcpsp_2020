include make.conf

####################################################################################
#                               COMPILER FLAGS                                     #
CFLAGS+=-g -Wall -O3
CC=g++
CPP=g++
####################################################################################

all: main rcpsp

CTAGS:
	ctags -R .

rcpsp: cp_rcpsp.cpp cp_rcpsp.hpp
	${CPP} -I$(CP_INC_DIR) -I$(CONCERT_INC_DIR) -I$(CPLEX_INC_DIR) -c cp_rcpsp.cpp  -o cp_rcpsp.o -Wl,-no_compact_unwind -DIL_STD -g #-stdlib=libc++

main: main.c rcpsp
	${CPP} -I$(CP_INC_DIR) -I$(CONCERT_INC_DIR) -I$(CPLEX_INC_DIR) main.c $(CP_LIB_DIR)/libcp.a $(CONCERT_LIB_DIR)/libconcert.a $(CPLEX_LIB_DIR)/libcplex.a cp_rcpsp.o -o solve -Wl,-no_compact_unwind -DIL_STD -g #-stdlib=libc++

clean:
	rm -rf *.o cuts *.dSYM
