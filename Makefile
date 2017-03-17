CFLAGS = -msse2 --std gnu99 -O0 -Wall -Wextra

EXEC = Hint_T0 Hint_T1 Hint_T2 Hint_NTA calculate
GIT_HOOKS := .git/hooks/applied
CC ?= gcc
SRCS_common = main.c

Hint_T0: main.c
	$(CC) $(CFLAGS) -DHint_T0 -o $@ $(SRCS_common)

Hint_T1: main.c
	$(CC) $(CFLAGS) -DHint_T1 -o $@ $(SRCS_common)

Hint_T2: main.c
	$(CC) $(CFLAGS) -DHint_T2 -o $@ $(SRCS_common)

Hint_NTA: main.c
	$(CC) $(CFLAGS) -DHint_NTA -o $@ $(SRCS_common)


all: $(GIT_HOOKS) $(EXEC)


$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

perf: $(EXEC)
	perf stat --repeat 50 \
		-e cache-misses,cache-references,instructions,cycles \
		./Hint_T0
	perf stat --repeat 50 \
		-e cache-misses,cache-references,instructions,cycles \
		./Hint_T1
	perf stat --repeat 50 \
		-e cache-misses,cache-references,instructions,cycles \
		./Hint_T2
	perf stat --repeat 50 \
		-e cache-misses,cache-references,instructions,cycles \
		./Hint_NTA


output.txt: calculate perf
	./calculate

plot: output.txt
	gnuplot scripts/runtime.gp

calculate: calculate.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(EXEC) *.o perf.* *.txt
