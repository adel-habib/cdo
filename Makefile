CC=gcc
CFLAGS=-I.
DEPS = headers/http.h headers/rest.h headers/socket.h
OBJ = src/http.o src/socket.o src/main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

run: main
	./main

.PHONY: clean

clean:
	rm -f src/*.o main
