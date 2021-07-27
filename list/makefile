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

all: $(BIN)liblist.a check

# main targets
$(BIN)list.o: $(SRC)list.c $(INC)list.h
	$(CMD) -c $< -o $@

# test targets
$(TST)check_check: $(TSTBIN)check_check.o $(TSTBIN)libtestlist.a
	$(CMD) $^ $(LNK) -o $@
$(TSTBIN)check_check.o: $(TSTSRC)check_check.c
	$(CMD) -c $^ -o $@
$(TSTBIN)test_list.o: $(TSTSRC)test_list.c
	$(CMD) -c $^ -o $@ 

# libarary targets
$(BIN)liblist.a: $(BIN)liblist.a($(BIN)list.o);
$(TSTBIN)libtestlist.a: $(TSTBIN)libtestlist.a($(TSTBIN)test_list.o $(BIN)list.o);
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
