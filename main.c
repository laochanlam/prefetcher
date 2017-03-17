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

typedef struct object Object;
typedef void (*func_t)(int *src, int *dst, int w, int h, int Locality_Hint);

struct object {
    func_t transpose;
};

int init_sse_prefetch_transpose(Object **self)
{
    if ((*self = malloc(sizeof(Object))) == NULL) return -1;
    (*self) -> transpose = sse_prefetch_transpose;
    return 0;
}


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

#ifdef Hint_T0
#define Locality_Hint 0
#define OUT_FILE "T0.txt"
    if (init_sse_prefetch_transpose(&o) == -1)
        printf("error.");
#endif
#ifdef Hint_T1
#define Locality_Hint 1
#define OUT_FILE "T1.txt"
    if (init_sse_prefetch_transpose(&o) == -1)
        printf("error.");
#endif
#ifdef Hint_T2
#define Locality_Hint 2
#define OUT_FILE "T2.txt"
    if (init_sse_prefetch_transpose(&o) == -1)
        printf("error.");
#endif
#ifdef Hint_NTA
#define Locality_Hint 3
#define OUT_FILE "NTA.txt"
    if (init_sse_prefetch_transpose(&o) == -1)
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

        o->transpose(testin, testout, 4, 4, Locality_Hint);
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
        o->transpose(src, out0, TEST_W, TEST_H, PFDIST);
        clock_gettime(CLOCK_REALTIME, &end);

        long int cpu_time = diff_in_us(start, end);

        printf("%s\n",OUT_FILE);
        FILE *output = fopen(OUT_FILE, "a");
        fprintf(output, "Execution Time: %ld us\n", cpu_time);
        fclose(output);



#ifdef Hint_T0
        printf("T0: \t %ld us\n", cpu_time);
#endif
#ifdef Hint_T1
        printf("T1: \t %ld us\n", cpu_time);
#endif
#ifdef Hint_T2
        printf("T2: \t %ld us\n", cpu_time);
#endif
#ifdef Hint_NTA
        printf("NTA: \t %ld us\n", cpu_time);
#endif


        free(src);
        free(out0);
        free(out1);
        free(out2);
    }

    return 0;
}
