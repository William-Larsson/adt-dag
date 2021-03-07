CC = gcc
CFLAGS = -g -std=gnu11 -Wall -Wextra -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wunreachable-code 
RM = rm -f
INC := -I ./
CFLAGS += $(INC)

all: dag_test dag_mwe

dag_test: dag_test.c dag.o list.o queue.o
	$(CC) $(CFLAGS) $^ -o $@

dag_mwe: dag_mwe.o dag.o list.o queue.o
	$(CC) $(CFLAGS) $^ -o $@

dag_mwe.o: dag_mwe.c
	$(CC) $(CFLAGS) -c $<

dag: dag.o list.o queue.o
	$(CC) $(CFLAGS) $^ -o $@

dag.o: dag.c dag.h list.h queue.h
	$(CC) $(CFLAGS) -c $<

list.o: list.c list.h
	$(CC) $(CFLAGS) -c $<

queue.o: queue.c queue.h list.o
	$(CC) $(CFLAGS) -c $<

valgrind: all
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./dag_test