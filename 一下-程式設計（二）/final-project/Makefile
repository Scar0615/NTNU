CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -g

.PHONY: all test bench clean

all: take6 arena

take6: main.c game.c agent.c engine.c game.h agent.h engine.h
	$(CC) $(CFLAGS) -o $@ main.c game.c agent.c engine.c

arena: arena.c contest_agent.c agent.c game.c engine.c contest_agent.h agent.h game.h engine.h
	$(CC) $(CFLAGS) -o $@ arena.c contest_agent.c agent.c game.c engine.c

bench_multi: bench_multi.c game.c agent.c engine.c game.h agent.h engine.h
	$(CC) $(CFLAGS) -O2 -o $@ bench_multi.c game.c agent.c engine.c

test_engine: test_engine.c engine.c engine.h
	$(CC) $(CFLAGS) -o $@ test_engine.c engine.c

test_game: test_game.c game.c agent.c engine.c game.h agent.h engine.h
	$(CC) $(CFLAGS) -o $@ test_game.c game.c agent.c engine.c

test: test_engine test_game
	./test_engine
	./test_game

bench: bench_multi
	./bench_multi

clean:
	rm -f take6 arena bench_multi test_engine test_game
