.PHONY:all clean
DEMO=demo
CC=gcc
OBJ=obj
LIB=lib
SRC=src
AR=ar
INCLUDE=include
CFLAGS=-g -I$(INCLUDE)
all:$(DEMO)
$(DEMO):$(OBJ)/ul_thread.o $(OBJ)/demo.o
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ)/%.o:$(SRC)/%.c
	$(CC) -c $(CFLAGS) $< -o $@


clean:
	rm -f $(OBJ)/*.o $(LIB)/*.a $(DEMO)
