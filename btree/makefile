CMD = cc -Wall -Wextra
BIN = ./bin/
SRC = ./src/
INC = ./include/
CMD += -I $(INC)

all: $(BIN)libbin.a
$(BIN)btree.o: $(SRC)btree.c $(INC)btree.h
	$(CMD) -c $< -o $@
$(BIN)libbin.a: $(BIN)libbin.a($(BIN)btree.o);
clean:
	find . -type f -iname *.o -exec rm -rf {} \;
	find . -type f -iname *.a -exec rm -rf {} \;
debug: CMD += -g
debug: clean all
