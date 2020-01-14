include make.conf

####################################################################################
#                               COMPILER FLAGS                                     #
CFLAGS+=-g -Wall -O3
CC=g++
####################################################################################

all: main rcpsp

CTAGS:
	ctags -R .

#getcplexcuts.o : getcplexcuts.c getcplexcuts.h
#	${CC} -c ${CFLAGS} -I$(CPLEX_INC_DIR) getcplexcuts.c
 
rcpsp: rcpsp.cpp rcpsp.hpp
	${CC} -I$(CP_INC_DIR) -I$(CONCERT_INC_DIR) -I$(CPLEX_INC_DIR) -c rcpsp.cpp  -o rcpsp.o -Wl,-no_compact_unwind -DIL_STD -g #-stdlib=libc++

main: main.cpp rcpsp
	${CC} -I$(CP_INC_DIR) -I$(CONCERT_INC_DIR) -I$(CPLEX_INC_DIR) main.cpp $(CP_LIB_DIR)/libcp.a $(CONCERT_LIB_DIR)/libconcert.a $(CPLEX_LIB_DIR)/libcplex.a rcpsp.o -o solve -Wl,-no_compact_unwind -DIL_STD -g #-stdlib=libc++

#main: rcpsp.cpp 
#	${CC} -I$(CP_INC_DIR) -I$(CONCERT_INC_DIR) -I$(CPLEX_INC_DIR) rcpsp.cpp $(CP_LIB_DIR)/libcp.a $(CONCERT_LIB_DIR)/libconcert.a $(CPLEX_LIB_DIR)/libcplex.a -o solve -Wl,-no_compact_unwind -DIL_STD -g #-stdlib=libc++




clean:
	rm -rf *.o cuts *.dSYM
