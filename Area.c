#include<stdio.h>
#include<math.h>
#define PI 3.1415926 // Define PI
int main()
{
    double radius, area, Circum;
    printf("Enter the radius of the Circle: ");
    scanf("%lf", &radius);
    area = PI * radius * radius;
    printf("The area of the Circle is: %.4lf\n", area);
    Circum = 2 * PI * radius;
    printf("The circumference of the Circle is: %.4lf\n", Circum);  
    return 0;
}