#include <stdio.h>

int main() {
    int choice, rows, i, j;

    printf("Choose a pattern:\n");
    printf("1. Star pattern\n");
    printf("2. Alphabet pattern\n");
    printf("3. Number pyramid\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    printf("Enter number of rows: ");
    scanf("%d", &rows);

    switch (choice) {
        case 1:
            printf("\nStar pattern:\n");
            for (i = 1; i <= rows; i++) {
                for (j = 1; j <= i; j++) {
                    printf("*");
                }
                printf("\n");
            }
            break;

        case 2:
            printf("\nAlphabet pattern:\n");
            for (i = 1; i <= rows; i++) {
                for (j = 0; j < i; j++) {
                    printf("%c ", 'A' + j);
                }
                printf("\n");
            }
            break;

        case 3:
            printf("\nNumber pyramid:\n");
            for (i = 1; i <= rows; i++) {
                for (j = 1; j <= i; j++) {
                    printf("%d ", j);
                }
                printf("\n");
            }
            break;

        default:
            printf("\nInvalid choice!\n");
            break;
    }

    return 0;
}
