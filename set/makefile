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

all: $(BIN)libset.a check

################
# main targets #
################
$(BIN)set.o: $(SRC)set.c $(INC)set.h
	$(CMD) -c $< -o $@
$(BIN)list.o: $(SRC)list.c $(INC)list.h
	$(CMD) -c $< -o $@

################
# test targets #
################
$(TST)check_check: $(TSTBIN)check_check.o $(TSTBIN)libtestset.a
	$(CMD) $^ $(LNK) -o $@
$(TSTBIN)check_check.o: $(TSTSRC)check_check.c
	$(CMD) -c $^ -o $@
$(TSTBIN)test_set.o: $(TSTSRC)test_set.c
	$(CMD) -c $^ -o $@ 

####################
# libarary targets #
####################
$(BIN)libset.a: $(BIN)libset.a($(BIN)set.o $(BIN)list.o);
$(TSTBIN)libtestset.a: $(TSTBIN)libtestset.a($(TSTBIN)test_set.o $(BIN)set.o $(BIN)list.o);
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
