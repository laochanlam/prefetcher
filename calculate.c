#include <stdio.h>
#include <stdlib.h>

int main(void)
{

    /* original version */

    FILE *output = fopen("output.txt", "w");

    FILE *fp = fopen("T0.txt", "r");
    int i = 0;
    char stringA[50], stringB [50];
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

    fp = fopen("T1.txt", "r");
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

    fp = fopen("T2.txt", "r");
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

    fp = fopen("NTA.txt", "r");
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


    fprintf(output, "time: %ld %ld %ld %ld\n", sum_2 / 50, sum_2 / 50, sum_3 / 50, sum_4 / 50);
    printf("time: %ld %ld %ld %ld\n", sum_2 / 50, sum_2 / 50, sum_3 / 50, sum_4 / 50);
    fclose(output);
    return 0;
}
