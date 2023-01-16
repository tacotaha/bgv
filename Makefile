ROOT=$(realpath .)
CC=gcc
CFLAGS=-O3 -Wall -Wextra -Werror -I$(ROOT)
LDFLAGS=-lgmp
OBJ=params/params.o rlwe/zq.o
EXEC=bgv

all: $(EXEC)

$(EXEC): $(OBJ) main.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $@

memcheck: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC)

clean:
	rm -rf $(OBJ) $(EXEC) *.o *.a *~

format:
	find . -maxdepth 2 -regextype posix-extended -regex ".*\.(c|h)" | xargs indent -par -br -brf -brs -kr -ci2 -cli2 -i2 -l120 -nut
