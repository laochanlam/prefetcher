#include <stdio.h>
#include <stdlib.h>

int main(void)
{

    /* original version */

    FILE *output = fopen("local/output.txt", "w");

    FILE *fp = fopen("local/NAIVE_TRANSPOSE.txt", "r");
    int i = 0;
    char stringA[20], stringB [20];
    long int naive_transpose_sum_a = 0.0, naive_transpose_a;
    for (i = 0; i < 20; i++) {
        if (feof(fp)) {
            printf("ERROR: You need 20 datum instead of %d\n", i);
            printf("run 'make run' longer to get enough information\n\n");
            exit(0);
        }
        fscanf(fp, "%s %s %ld \n", stringA, stringB , &naive_transpose_a);
        naive_transpose_sum_a += naive_transpose_a;

    }
    fclose(fp);

    fp = fopen("local/SSE_TRANSPOSE.txt", "r");
    long int sse_transpose_sum_a = 0.0, sse_transpose_a;
    for (i = 0; i < 20; i++) {
        if (feof(fp)) {
            printf("ERROR: You need 20 datum instead of %d\n", i);
            printf("run 'make run' longer to get enough information\n\n");
            exit(0);
        }
        fscanf(fp, "%s %s %ld \n", stringA, stringB , &sse_transpose_a);
        sse_transpose_sum_a += sse_transpose_a;

    }
    fclose(fp);

    fp = fopen("local/SSE_PREFETCH_TRANSPOSE.txt", "r");
    long int sse_prefetch_transpose_sum_a = 0.0, sse_prefetch_transpose_a;
    for (i = 0; i < 20; i++) {
        if (feof(fp)) {
            printf("ERROR: You need 20 datum instead of %d\n", i);
            printf("run 'make run' longer to get enough information\n\n");
            exit(0);
        }
        fscanf(fp, "%s %s %ld \n", stringA, stringB , &sse_prefetch_transpose_a);
        sse_prefetch_transpose_sum_a += sse_prefetch_transpose_a;

    }
    fclose(fp);

    fp = fopen("local/AVX_TRANSPOSE.txt", "r");
    long int avx_transpose_sum_a = 0.0, avx_transpose_a;
    for (i = 0; i < 20; i++) {
        if (feof(fp)) {
            printf("ERROR: You need 20 datum instead of %d\n", i);
            printf("run 'make run' longer to get enough information\n\n");
            exit(0);
        }
        fscanf(fp, "%s %s %ld \n", stringA, stringB , &avx_transpose_a);
        avx_transpose_sum_a += avx_transpose_a;

    }
    fclose(fp);

    fp = fopen("local/AVX_PREFETCH_TRANSPOSE.txt", "r");
    long int avx_prefetch_transpose_sum_a = 0.0, avx_prefetch_transpose_a;
    for (i = 0; i < 20; i++) {
        if (feof(fp)) {
            printf("ERROR: You need 20 datum instead of %d\n", i);
            printf("run 'make run' longer to get enough information\n\n");
            exit(0);
        }
        fscanf(fp, "%s %s %ld \n", stringA, stringB , &avx_prefetch_transpose_a);
        avx_prefetch_transpose_sum_a += avx_prefetch_transpose_a;

    }
    fclose(fp);


    fprintf(output, "time: %ld %ld %ld %ld %ld\n", naive_transpose_sum_a / 20, sse_transpose_sum_a / 20, sse_prefetch_transpose_sum_a / 20, avx_transpose_sum_a / 20, avx_prefetch_transpose_sum_a / 20);
    printf("time: %ld %ld %ld %ld %ld\n", naive_transpose_sum_a / 20, sse_transpose_sum_a / 20, sse_prefetch_transpose_sum_a / 20,avx_transpose_sum_a / 20, avx_prefetch_transpose_sum_a / 20);
    fclose(output);
    return 0;
}
