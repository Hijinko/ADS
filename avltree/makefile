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

all: $(BIN)libavltree.a check

################
# main targets #
################
$(BIN)avltree.o: $(SRC)avltree.c $(INC)avltree.h
	$(CMD) -c $< -o $@

################
# test targets #
################
$(TST)check_check: $(TSTBIN)check_check.o $(TSTBIN)libtestavltree.a
	$(CMD) $^ $(LNK) -o $@
$(TSTBIN)check_check.o: $(TSTSRC)check_check.c
	$(CMD) -c $^ -o $@
$(TSTBIN)test_avltree.o: $(TSTSRC)test_alvtree.c
	$(CMD) -c $^ -o $@ 

####################
# libarary targets #
####################
$(BIN)libavltree.a: $(BIN)libavltree.a($(BIN)avltree.o);
$(TSTBIN)libtestavltree.a: $(TSTBIN)libtestavltree.a($(TSTBIN)test_avltree.o $(BIN)avltree.o);
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
