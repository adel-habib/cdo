CC = gcc
CFLAGS = -I./headers

SRCS = src/socket.c src/http.c src/main.c
OBJS = build/socket.o build/http.o build/main.o

all: build_dir $(OBJS)
	$(CC) $(OBJS) -o build/main

build_dir:
	mkdir -p build

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build

run:
	./build/main
