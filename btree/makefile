CMD = cc -Wall -Wextra
SRC = ./src/
BIN = ./bin/
INC = ./include/
CMD += -I $(INC)
TST = ./test/
TSTSRC = ./test/src/
TSTBIN = ./test/bin/
TSTINC = ./test/include
LNK = -lcheck -lm -lpthread -lrt -lsubunit

all: $(BIN)libadt.a check

# main targets
$(BIN)btree.o: $(SRC)btree.c $(INC)btree.h
	$(CMD) -c $< -o $@

# test targets
$(TST)check_check: $(TSTBIN)check_check.o $(TSTBIN)libtestadt.a
	$(CMD) $^ $(LNK) -o $@
$(TSTBIN)check_check.o: $(TSTSRC)check_check.c
	$(CMD) -c $^ -o $@
$(TSTBIN)test_btree.o: $(TSTSRC)test_btree.c
	$(CMD) -c $^ -o $@ 

# libarary targets
$(BIN)libadt.a: $(BIN)libadt.a($(BIN)btree.o);
$(TSTBIN)libtestadt.a: $(TSTBIN)libtestadt.a($(TSTBIN)test_btree.o $(BIN)btree.o);
clean:
	find . -type f -iname *.o -exec rm -rf {} \;
	find . -type f -iname *.a -exec rm -rf {} \;
	find . -type f -iname check_check -exec rm -rf {} \;
debug: CMD += -g
debug: clean all
check: CMD += -I $(TSTINC)
check: debug $(TST)check_check
valgrind: debug
	valgrind --leak-check=full ./test/check_check
