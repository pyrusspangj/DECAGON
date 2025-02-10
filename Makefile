CC:=gcc
CFLAGS=-I
DEPS = interpreter.h Helper/filehelp.h Helper/stringhelp.h
OBJ = main.o interpreter.o Helper/filehelp.o Helper/stringhelp.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $<

decagon: $(OBJ)
	$(CC) -o $@ $^
