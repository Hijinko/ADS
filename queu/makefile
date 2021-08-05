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

all: $(BIN)libqueu.a check

################
# main targets #
################
$(BIN)queu.o: $(SRC)queu.c $(INC)queu.h
	$(CMD) -c $< -o $@

################
# test targets #
################
$(TST)check_check: $(TSTBIN)check_check.o $(TSTBIN)libtestqueu.a
	$(CMD) $^ $(LNK) -o $@
$(TSTBIN)check_check.o: $(TSTSRC)check_check.c
	$(CMD) -c $^ -o $@
$(TSTBIN)test_queu.o: $(TSTSRC)test_queu.c
	$(CMD) -c $^ -o $@ 

####################
# libarary targets #
####################
$(BIN)libqueu.a: $(BIN)libqueu.a($(BIN)queu.o);
$(TSTBIN)libtestqueu.a: $(TSTBIN)libtestqueu.a($(TSTBIN)test_queu.o $(BIN)queu.o);
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
