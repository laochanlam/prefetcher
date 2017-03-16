CFLAGS = -msse2 --std gnu99 -O0 -Wall -Wextra

EXEC = SSE_PREFETCH_TRANSPOSE_2 SSE_PREFETCH_TRANSPOSE_4 SSE_PREFETCH_TRANSPOSE_6 SSE_PREFETCH_TRANSPOSE_8 SSE_PREFETCH_TRANSPOSE_10 SSE_PREFETCH_TRANSPOSE_12 SSE_PREFETCH_TRANSPOSE_14 calculate
GIT_HOOKS := .git/hooks/applied
CC ?= gcc
SRCS_common = main.c

SSE_PREFETCH_TRANSPOSE_2: main.c
	$(CC) $(CFLAGS) -DSSE_PREFETCH_TRANSPOSE_2 -o $@ $(SRCS_common)

SSE_PREFETCH_TRANSPOSE_4: main.c
	$(CC) $(CFLAGS) -DSSE_PREFETCH_TRANSPOSE_4 -o $@ $(SRCS_common)

SSE_PREFETCH_TRANSPOSE_6: main.c
	$(CC) $(CFLAGS) -DSSE_PREFETCH_TRANSPOSE_6 -o $@ $(SRCS_common)

SSE_PREFETCH_TRANSPOSE_8: main.c
	$(CC) $(CFLAGS) -DSSE_PREFETCH_TRANSPOSE_8 -o $@ $(SRCS_common)

SSE_PREFETCH_TRANSPOSE_10: main.c
	$(CC) $(CFLAGS) -DSSE_PREFETCH_TRANSPOSE_10 -o $@ $(SRCS_common)

SSE_PREFETCH_TRANSPOSE_12: main.c
	$(CC) $(CFLAGS) -DSSE_PREFETCH_TRANSPOSE_12 -o $@ $(SRCS_common)

SSE_PREFETCH_TRANSPOSE_14: main.c
	$(CC) $(CFLAGS) -DSSE_PREFETCH_TRANSPOSE_14 -o $@ $(SRCS_common)


all: $(GIT_HOOKS) $(EXEC)


$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

perf: $(EXEC)
	perf stat --repeat 50 \
		-e cache-misses,cache-references,instructions,cycles \
		./SSE_PREFETCH_TRANSPOSE_2
	perf stat --repeat 50 \
		-e cache-misses,cache-references,instructions,cycles \
		./SSE_PREFETCH_TRANSPOSE_4
	perf stat --repeat 50 \
		-e cache-misses,cache-references,instructions,cycles \
		./SSE_PREFETCH_TRANSPOSE_6
	perf stat --repeat 50 \
		-e cache-misses,cache-references,instructions,cycles \
		./SSE_PREFETCH_TRANSPOSE_8
	perf stat --repeat 50 \
		-e cache-misses,cache-references,instructions,cycles \
		./SSE_PREFETCH_TRANSPOSE_10
	perf stat --repeat 50 \
		-e cache-misses,cache-references,instructions,cycles \
		./SSE_PREFETCH_TRANSPOSE_12
	perf stat --repeat 50 \
		-e cache-misses,cache-references,instructions,cycles \
		./SSE_PREFETCH_TRANSPOSE_14

output.txt: calculate perf
	./calculate

plot: output.txt
	gnuplot scripts/runtime.gp

calculate: calculate.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(EXEC) *.o perf.* *.txt
