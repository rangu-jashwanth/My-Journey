#include <stdio.h>
int main()
{
    int a, b, c, min, max;
    printf("enter 3 numbers\n");
    scanf("%d %d %d", &a, &b, &c);
    
    max = a;
    min = a;
    
    if (b > max)
        max = b;
    if (b < min)
        min = b;
    if (c > max)
        max = c;
    if (c < min)
        min = c;
    
    printf("Maximum is %d\n", max);
    printf("Minimum is %d\n", min);
    return 0;
}
 

