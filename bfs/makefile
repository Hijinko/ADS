CMD = cc -Wall -Wextra -Wall -Wextra -Wpedantic -Waggregate-return -Wwrite-strings -Wvla -Wfloat-equal
SRC = ./src/
BIN = ./bin/
INC = ./include/
CMD += -I $(INC)
TST = ./test/
TSTSRC = ./test/src/
TSTBIN = ./test/bin/
TSTINC = ./test/include
LNK = -lcheck -lm -lpthread -lrt -lsubunit

all: $(BIN)libgraph.a

################
# main targets #
################
$(BIN)graph.o: $(SRC)graph.c $(INC)graph.h
	$(CMD) -c $< -o $@
$(BIN)list.o: $(SRC)list.c $(INC)list.h
	$(CMD) -c $< -o $@
$(BIN)bfs.o: $(SRC)bfs.c $(INC)bfs.h
	$(CMD) -c $< -o $@
$(BIN)set.o: $(SRC)set.c $(INC)set.h
	$(CMD) -c $< -o $@

################
# test targets #
################
$(TST)check_check: $(TSTBIN)check_check.o $(TSTBIN)libtestgraph.a
	$(CMD) $^ $(LNK) -o $@
$(TSTBIN)check_check.o: $(TSTSRC)check_check.c
	$(CMD) -c $^ -o $@
$(TSTBIN)test_graph.o: $(TSTSRC)test_graph.c
	$(CMD) -c $^ -o $@ 

####################
# libarary targets #
####################
$(BIN)libgraph.a: $(BIN)libgraph.a($(BIN)graph.o $(BIN)list.o $(BIN)set.o $(BIN)bfs.o);
$(TSTBIN)libtestgraph.a: $(TSTBIN)libtestgraph.a($(TSTBIN)test_graph.o $(BIN)graph.o $(BIN)list.o $(BIN)set.o $(BIN)bfs.o);
clean:
	find . -type f -iname *.o -exec rm -rf {} \;
	find . -type f -iname *.a -exec rm -rf {} \;
	find . -type f -iname check_check -exec rm -rf {} \;
debug: CMD += -g
debug: clean all
profile: CMD += -pg
profile: debug
check: CMD += -I $(TSTINC)
check: debug
check: $(TST)check_check
valgrind: check 
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./test/check_check
