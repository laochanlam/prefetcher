#include <stdio.h>
#include <stdlib.h>

int main(void)
{

    /* original version */

    FILE *output = fopen("output.txt", "w");

    FILE *fp = fopen("SSE_PREFETCH_TRANSPOSE_2.txt", "r");
    int i = 0;
    char stringA[50], stringB [50];
    long int sum_1 = 0.0, first;
    for (i = 0; i < 50; i++) {
        if (feof(fp)) {
            printf("ERROR: You need 50 datum instead of %d\n", i);
            printf("run 'make run' longer to get enough information\n\n");
            exit(0);
        }
        fscanf(fp, "%s %s %ld \n", stringA, stringB , &first);
        sum_1 += first;

    }
    fclose(fp);

    fp = fopen("SSE_PREFETCH_TRANSPOSE_4.txt", "r");
    long int sum_2 = 0.0, second;
    for (i = 0; i < 50; i++) {
        if (feof(fp)) {
            printf("ERROR: You need 50 datum instead of %d\n", i);
            printf("run 'make run' longer to get enough information\n\n");
            exit(0);
        }
        fscanf(fp, "%s %s %ld \n", stringA, stringB , &second);
        sum_2 += second;

    }
    fclose(fp);

    fp = fopen("SSE_PREFETCH_TRANSPOSE_6.txt", "r");
    long int sum_3 = 0.0, third;
    for (i = 0; i < 50; i++) {
        if (feof(fp)) {
            printf("ERROR: You need 50 datum instead of %d\n", i);
            printf("run 'make run' longer to get enough information\n\n");
            exit(0);
        }
        fscanf(fp, "%s %s %ld \n", stringA, stringB , &third);
        sum_3 += third;

    }
    fclose(fp);

    fp = fopen("SSE_PREFETCH_TRANSPOSE_8.txt", "r");
    long int sum_4 = 0.0, forth;
    for (i = 0; i < 50; i++) {
        if (feof(fp)) {
            printf("ERROR: You need 50 datum instead of %d\n", i);
            printf("run 'make run' longer to get enough information\n\n");
            exit(0);
        }
        fscanf(fp, "%s %s %ld \n", stringA, stringB , &forth);
        sum_4 += forth;

    }
    fclose(fp);

    fp = fopen("SSE_PREFETCH_TRANSPOSE_10.txt", "r");
    long int sum_5 = 0.0, fifth;
    for (i = 0; i < 50; i++) {
        if (feof(fp)) {
            printf("ERROR: You need 50 datum instead of %d\n", i);
            printf("run 'make run' longer to get enough information\n\n");
            exit(0);
        }
        fscanf(fp, "%s %s %ld \n", stringA, stringB , &fifth);
        sum_5 += fifth;

    }
    fclose(fp);

    fp = fopen("SSE_PREFETCH_TRANSPOSE_12.txt", "r");
    long int sum_6 = 0.0, sixth;
    for (i = 0; i < 50; i++) {
        if (feof(fp)) {
            printf("ERROR: You need 50 datum instead of %d\n", i);
            printf("run 'make run' longer to get enough information\n\n");
            exit(0);
        }
        fscanf(fp, "%s %s %ld \n", stringA, stringB , &sixth);
        sum_6 += sixth;

    }
    fclose(fp);

    fp = fopen("SSE_PREFETCH_TRANSPOSE_14.txt", "r");
    long int sum_7 = 0.0, seventh;
    for (i = 0; i < 50; i++) {
        if (feof(fp)) {
            printf("ERROR: You need 50 datum instead of %d\n", i);
            printf("run 'make run' longer to get enough information\n\n");
            exit(0);
        }
        fscanf(fp, "%s %s %ld \n", stringA, stringB , &seventh);
        sum_7 += seventh;

    }
    fclose(fp);

    fprintf(output, "time: %ld %ld %ld %ld %ld %ld %ld\n", sum_1 / 50, sum_2 / 50, sum_3 / 50, sum_4 / 50, sum_5 / 50, sum_6 / 50, sum_7 / 50);
    printf("time: %ld %ld %ld %ld %ld %ld %ld\n", sum_1 / 50, sum_2 / 50, sum_3 / 50, sum_4 / 50, sum_5 / 50, sum_6 / 50, sum_7 / 50);
    fclose(output);
    return 0;
}
