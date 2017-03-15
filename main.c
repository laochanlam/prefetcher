#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <assert.h>

#include <xmmintrin.h>

#define TEST_W 4096
#define TEST_H 4096
#include "impl.c"

typedef struct object Interface;
typedef void (*func_t)(int *src, int *dst, int w, int h);

struct object {
    func_t transpose;
};

int init_naive_transpose(Interface **self)
{
    if ((*self = malloc(sizeof(Interface))) == NULL) return -1;
    (*self) -> transpose = naive_transpose;
    return 0;
}

int init_sse_transpose(Interface **self)
{
    if ((*self = malloc(sizeof(Interface))) == NULL) return -1;
    (*self) -> transpose = sse_transpose;
    return 0;
}

int init_sse_prefetch_transpose(Interface **self)
{
    if ((*self = malloc(sizeof(Interface))) == NULL) return -1;
    (*self) -> transpose = sse_prefetch_transpose;
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

    Interface *interface = NULL;

#ifdef NAIVE_TRANSPOSE
    if (init_naive_transpose(&interface) == -1)
        printf("error.");
#endif
#ifdef SSE_TRANSPOSE
    if (init_sse_transpose(&interface) == -1)
        printf("error.");
#endif
#ifdef SSE_PREFETCH_TRANSPOSE
    if (init_sse_prefetch_transpose(&interface) == -1)
        printf("error.");
#endif

    /* verify the result of 4x4 matrix */
    {
        int testin[16] = { 0, 1,  2,  3,  4,  5,  6,  7,
                           8, 9, 10, 11, 12, 13, 14, 15
                         };
        int testout[16];
        int expected[16] = { 0, 4,  8, 12, 1, 5,  9, 13,
                             2, 6, 10, 14, 3, 7, 11, 15
                           };

        interface->transpose(testin, testout, 4, 4);
        assert(0 == memcmp(testout, expected, 16 * sizeof(int)) &&
               "Verification fails");
    }

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
        interface->transpose(src, out0, TEST_W, TEST_H);
        clock_gettime(CLOCK_REALTIME, &end);

#ifdef NAIVE_TRANSPOSE
        printf("NAIVE_TRANSPOSE: \t %ld us\n", diff_in_us(start, end));
#endif
#ifdef SSE_TRANSPOSE
        printf("SSE_TRANSPOSE: \t %ld us\n", diff_in_us(start, end));
#endif
#ifdef SSE_PREFETCH_TRANSPOSE
        printf("SSE_PREFETCH_TRANSPOSE: \t %ld us\n", diff_in_us(start, end));
#endif

        free(src);
        free(out0);
        free(out1);
        free(out2);
    }

    return 0;
}
