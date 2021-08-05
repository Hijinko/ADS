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

all: $(BIN)libqueue.a check

################
# main targets #
################
$(BIN)queue.o: $(SRC)queue.c $(INC)queue.h
	$(CMD) -c $< -o $@
$(BIN)list.o: $(SRC)list.c $(INC)list.h
	$(CMD) -c $< -o $@

################
# test targets #
################
$(TST)check_check: $(TSTBIN)check_check.o $(TSTBIN)libtestqueue.a
	$(CMD) $^ $(LNK) -o $@
$(TSTBIN)check_check.o: $(TSTSRC)check_check.c
	$(CMD) -c $^ -o $@
$(TSTBIN)test_queue.o: $(TSTSRC)test_queue.c
	$(CMD) -c $^ -o $@ 

####################
# libarary targets #
####################
$(BIN)libqueue.a: $(BIN)libqueue.a($(BIN)queue.o $(BIN)list.o);
$(TSTBIN)libtestqueue.a: $(TSTBIN)libtestqueue.a($(TSTBIN)test_queue.o $(BIN)queue.o $(BIN)list.o);
clean:
	find . -type f -iname *.o -exec rm -rf {} \;
	find . -type f -iname *.a -exec rm -rf {} \;
	find . -type f -iname check_check -exec rm -rf {} \;
debug: CMD += -g
debug: clean all
profile: CMD += -pg
profile: debug
check: CMD += -I $(TSTINC)
check: $(TST)check_check
valgrind: debug check
	valgrind --leak-check=full --show-leak-kinds=all ./test/check_check
