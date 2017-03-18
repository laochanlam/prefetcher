CFLAGS = -msse2 --std gnu99 -O0 -Wall -Wextra -mavx2

EXEC = local/naive_transpose local/sse_transpose local/sse_prefetch_transpose local/avx_transpose local/avx_prefetch_transpose local/calculate
GIT_HOOKS := .git/hooks/applied
CC ?= gcc
SRCS_common = main.c

local/naive_transpose: main.c
	$(CC) $(CFLAGS) -DNAIVE_TRANSPOSE -o $@ $(SRCS_common)

local/sse_transpose: main.c
	$(CC) $(CFLAGS) -DSSE_TRANSPOSE -o $@ $(SRCS_common)

local/sse_prefetch_transpose: main.c
	$(CC) $(CFLAGS) -DSSE_PREFETCH_TRANSPOSE -o $@ $(SRCS_common)

local/avx_transpose: main.c
	$(CC) $(CFLAGS) -DAVX_TRANSPOSE -o $@ $(SRCS_common)

local/avx_prefetch_transpose: main.c
	$(CC) $(CFLAGS) -DAVX_PREFETCH_TRANSPOSE -o $@ $(SRCS_common)


all: $(GIT_HOOKS) $(EXEC)


$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

perf: $(EXEC)
	perf stat --repeat 20 \
		-e cache-misses,cache-references,instructions,cycles \
		./local/naive_transpose
	perf stat --repeat 20 \
		-e cache-misses,cache-references,instructions,cycles \
		./local/sse_transpose
	perf stat --repeat 20 \
		-e cache-misses,cache-references,instructions,cycles \
		./local/sse_prefetch_transpose
	perf stat --repeat 20 \
		-e cache-misses,cache-references,instructions,cycles \
		./local/avx_transpose
	perf stat --repeat 20 \
		-e cache-misses,cache-references,instructions,cycles \
		./local/avx_prefetch_transpose

output.txt: local/calculate perf
	./local/calculate

plot: output.txt
	gnuplot scripts/runtime.gp

local/calculate: calculate.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(EXEC) *.o perf.* *.txt
