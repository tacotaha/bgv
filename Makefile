ROOT=$(realpath .)
CC=gcc
CFLAGS=-O3 -Wall -Wextra -Werror -fopenmp -I$(ROOT)
LDFLAGS=-lm -lgmp -lgomp
OBJ=utils/utils.o bgv/bgv.o ring/ntt.o ring/ring.o ring/poly.o
EXEC=test

all: $(EXEC)

$(EXEC): $(OBJ) main.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $@

memcheck: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC)

clean:
	rm -f $(OBJ) $(EXEC) *.o *.a *~
