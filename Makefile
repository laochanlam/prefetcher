CFLAGS = -msse2 --std gnu99 -O0 -Wall -Wextra
DFLAGS = -DMETHOD="\"$@.c\""
EXEC = naive_transpose sse_transpose sse_prefetch_transpose
GIT_HOOKS := .git/hooks/applied
CC ?= gcc
SRCS_common = main.c

naive_transpose: main.c naive_transpose.c
	$(CC) $(CFLAGS) $(DFLAGS) -o $@ $(SRCS_common)

sse_transpose: main.c sse_transpose.c
	$(CC) $(CFLAGS) $(DFLAGS) -o $@ $(SRCS_common)

sse_prefetch_transpose: main.c sse_prefetch_transpose.c
	$(CC) $(CFLAGS) $(DFLAGS) -o $@ $(SRCS_common)


all: $(GIT_HOOKS) $(EXEC)


$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

perf: $(EXEC)
	perf stat --repeat 20 \
		-e cache-misses,cache-references,instructions,cycles \
		./naive_transpose
	perf stat --repeat 20 \
		-e cache-misses,cache-references,instructions,cycles \
		./sse_transpose
	perf stat --repeat 20 \
		-e cache-misses,cache-references,instructions,cycles \
		./sse_prefetch_transpose

clean:
	$(RM) $(EXEC) *.o perf.*
