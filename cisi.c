#include<stdio.h>
#include<math.h>
int main()
{
    float p,r,t,si,ci,amount;
    printf("Enter principal amount\n");
    scanf("%f",&p);
    printf("Enter rate of interest\n");
    scanf("%f",&r);
    printf("Enter time in months\n");
    scanf("%f",&t); 
    /* Calculate simple interest */
    si=(p*r*t)/100;
    /* Calculate compound interest */
    amount=p*pow((1+r/100),t);
    ci=amount-p;
    printf("Simple Interest = %.2f\n",si);
    printf("Compound Interest = %.2f\n",ci);
    printf("Total Amount = %.2f\n",amount);
    return 0;
}