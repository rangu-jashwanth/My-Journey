#include <stdio.h>

int main() {
    int num, i;
    printf("Enter a number (0 to 255): ");
    scanf("%d", &num);
    if(num < 0 || num > 255)
    {
        printf("Invalid input! Please enter a number between 0 and 255.\n");
        return 0;
    }
    printf("Binary equivalent of %d is: ", num);
    for(i = 7; i >= 0; i--)
    {
        int bit = (num >> i) & 1;
        printf("%d", bit);
    }
    printf("\n");
    return 0;
} 