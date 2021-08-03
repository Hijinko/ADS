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

all: $(BIN)libheap.a check

################
# main targets #
################
$(BIN)heap.o: $(SRC)heap.c $(INC)heap.h
	$(CMD) -c $< -o $@

################
# test targets #
################
$(TST)check_check: $(TSTBIN)check_check.o $(TSTBIN)libtestheap.a
	$(CMD) $^ $(LNK) -o $@
$(TSTBIN)check_check.o: $(TSTSRC)check_check.c
	$(CMD) -c $^ -o $@
$(TSTBIN)test_heap.o: $(TSTSRC)test_heap.c
	$(CMD) -c $^ -o $@ 

####################
# libarary targets #
####################
$(BIN)libheap.a: $(BIN)libheap.a($(BIN)heap.o);
$(TSTBIN)libtestheap.a: $(TSTBIN)libtestheap.a($(TSTBIN)test_heap.o $(BIN)heap.o);
clean:
	find . -type f -iname *.o -exec rm -rf {} \;
	find . -type f -iname *.a -exec rm -rf {} \;
	find . -type f -iname check_check -exec rm -rf {} \;
debug: CMD += -g
debug: clean all
check: CMD += -I $(TSTINC)
check: $(TST)check_check
valgrind: debug check
	valgrind --leak-check=full --show-leak-kinds=all ./test/check_check
