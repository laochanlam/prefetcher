#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <assert.h>

#include <xmmintrin.h>
#include <immintrin.h>

#define TEST_W 4096
#define TEST_H 4096
#include "impl.c"

typedef struct object Object;
typedef void (*func_t)(int *src, int *dst, int w, int h);

struct object {
    func_t transpose;
};

int init_naive_transpose(Object **self)
{
    if ((*self = malloc(sizeof(Object))) == NULL) return -1;
    (*self) -> transpose = naive_transpose;
    return 0;
}

int init_sse_transpose(Object **self)
{
    if ((*self = malloc(sizeof(Object))) == NULL) return -1;
    (*self) -> transpose = sse_transpose;
    return 0;
}

int init_sse_prefetch_transpose(Object **self)
{
    if ((*self = malloc(sizeof(Object))) == NULL) return -1;
    (*self) -> transpose = sse_prefetch_transpose;
    return 0;
}

int init_avx_transpose(Object **self)
{
    if ((*self = malloc(sizeof(Object))) == NULL) return -1;
    (*self) -> transpose = avx_transpose;
    return 0;
}

int init_avx_prefetch_transpose(Object **self)
{
    if ((*self = malloc(sizeof(Object))) == NULL) return -1;
    (*self) -> transpose = avx_prefetch_transpose;
    return 0;
}

/* provide the implementations of naive_transpose,
* sse_transpose, sse_prefetch_transpose
 */

static long diff_in_us(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec * 1000000.0 + diff.tv_nsec / 1000.0);
}

int main()
{

    Object *o = NULL;

#ifdef NAIVE_TRANSPOSE
#define OUT_FILE "NAIVE_TRANSPOSE.txt"
    if (init_naive_transpose(&o) == -1)
        printf("error.");
#endif
#ifdef SSE_TRANSPOSE
#define OUT_FILE "SSE_TRANSPOSE.txt"
    if (init_sse_transpose(&o) == -1)
        printf("error.");
#endif
#ifdef SSE_PREFETCH_TRANSPOSE
#define OUT_FILE "SSE_PREFETCH_TRANSPOSE.txt"
    if (init_sse_prefetch_transpose(&o) == -1)
        printf("error.");
#endif
#ifdef AVX_TRANSPOSE
#define OUT_FILE "AVX_TRANSPOSE.txt"
    if (init_avx_transpose(&o) == -1)
        printf("error.");
#define HUGE 1
#endif
#ifdef AVX_PREFETCH_TRANSPOSE
#define OUT_FILE "AVX_PREFETCH_TRANSPOSE.txt"
    if (init_avx_prefetch_transpose(&o) == -1)
        printf("error.");
#define HUGE 1
#endif


#ifndef HUGE
    /* verify the result of 4x4 matrix */
    {
        int testin[16] = { 0, 1,  2,  3,  4,  5,  6,  7,
                           8, 9, 10, 11, 12, 13, 14, 15
                         };
        int testout[16];
        int expected[16] = { 0, 4,  8, 12, 1, 5,  9, 13,
                             2, 6, 10, 14, 3, 7, 11, 15
                           };

        o->transpose(testin, testout, 4, 4);
        assert(0 == memcmp(testout, expected, 16 * sizeof(int)) &&
               "Verification fails");
    }
#endif
#ifdef HUGE
    {
        int testin[64] = {0, 1, 2, 3, 4, 5, 6, 7
                          , 8, 9, 10, 11, 12, 13, 14, 15
                          , 16, 17, 18, 19, 20, 21, 22, 23
                          , 24, 25, 26, 27, 28, 29, 30, 31
                          , 32, 33, 34, 35, 36, 37, 38, 39
                          , 40, 41, 42, 43, 44, 45, 46, 47
                          , 48, 49, 50, 51, 52, 53, 54, 55
                          , 56, 57, 58, 59, 60, 61, 62, 63
                         };
        int testout[64];
        int expected[64] = {0, 8, 16, 24, 32, 40, 48, 56
                            , 1, 9, 17, 25, 33, 41, 49, 57
                            , 2, 10, 18, 26, 34, 42, 50, 58
                            , 3, 11, 19, 27, 35, 43, 51, 59
                            , 4, 12, 20, 28, 36, 44, 52, 60
                            , 5, 13, 21, 29, 37, 45, 53, 61
                            , 6, 14, 22, 30, 38, 46, 54, 62
                            , 7, 15, 23, 31, 39, 47, 55, 63
                           };
        o->transpose(testin, testout, 8, 8);
        assert(0 == memcmp(testout, expected, 16 * sizeof(int)) &&
               "Verification fails");
    }
#endif

    {
        struct timespec start, end;
        int *src  = (int *) malloc(sizeof(int) * TEST_W * TEST_H);
        int *out0 = (int *) malloc(sizeof(int) * TEST_W * TEST_H);
        int *out1 = (int *) malloc(sizeof(int) * TEST_W * TEST_H);
        int *out2 = (int *) malloc(sizeof(int) * TEST_W * TEST_H);

        srand(time(NULL));
        for (int y = 0; y < TEST_H; y++)
            for (int x = 0; x < TEST_W; x++)
                *(src + y * TEST_W + x) = rand();

        clock_gettime(CLOCK_REALTIME, &start);
        o->transpose(src, out0, TEST_W, TEST_H);
        clock_gettime(CLOCK_REALTIME, &end);

        long int cpu_time = diff_in_us(start, end);

        printf("%s\n",OUT_FILE);
        FILE *output = fopen(OUT_FILE, "a");
        fprintf(output, "Execution Time: %ld us\n", cpu_time);
        fclose(output);



#ifdef NAIVE_TRANSPOSE
        printf("NAIVE_TRANSPOSE: \t %ld us\n", cpu_time);
#endif
#ifdef SSE_TRANSPOSE
        printf("SSE_TRANSPOSE: \t %ld us\n", cpu_time);
#endif
#ifdef SSE_PREFETCH_TRANSPOSE
        printf("SSE_PREFETCH_TRANSPOSE: \t %ld us\n", cpu_time);
#endif
#ifdef AVX_TRANSPOSE
        printf("AVX_TRANSPOSE: \t %ld us\n", cpu_time);
#endif
#ifdef AVX_PREFETCH_TRANSPOSE
        printf("AVX_PREFETCH_TRANSPOSE: \t %ld us\n", cpu_time);
#endif
        free(src);
        free(out0);
        free(out1);
        free(out2);
    }

    return 0;
}
