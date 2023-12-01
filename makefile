CFLAGS= -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2
VFLAGS= --leak-check=full --track-origins=yes --show-reachable=yes --show-leak-kinds=all
EXEC= pokemon
CFILES= main.c src/pokemon.c src/cajas.c

run: build
	./$(EXEC)

build: $(CFILES)
	gcc $(CFLAGS) $(CFILES) -o $(EXEC)

valgrind: build
	valgrind $(VFLAGS) ./$(EXEC)

clean:
	rm -f *.o $(EXEC)