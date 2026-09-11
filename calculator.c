#include <stdio.h>
#include <math.h>

#define MAX_HISTORY 20

double memory = 0.0;
double history[MAX_HISTORY];
int historyCount = 0;

void addToHistory(double value) {
    if (historyCount < MAX_HISTORY) {
        history[historyCount++] = value;
    } else {
        for (int i = 0; i < MAX_HISTORY - 1; i++) {
            history[i] = history[i + 1];
        }
        history[MAX_HISTORY - 1] = value;
    }
}

void showHistory() {
    if (historyCount == 0) {
        printf("History is empty.\n");
        return;
    }

    printf("Calculation history:\n");
    for (int i = 0; i < historyCount; i++) {
        printf("%d. %.2f\n", i + 1, history[i]);
    }
}

void clearHistory() {
    historyCount = 0;
    printf("History cleared.\n");
}

int main() {
    int choice;
    char repeat = 'y';
    double a, b, result;

    printf("Advanced Calculator with Memory and History\n");
    printf("All trigonometric functions use degrees.\n\n");

    do {
        printf("Menu:\n");
        printf("1. Addition\n");
        printf("2. Subtraction\n");
        printf("3. Multiplication\n");
        printf("4. Division\n");
        printf("5. Modulo\n");
        printf("6. Power (a^b)\n");
        printf("7. Square root\n");
        printf("8. Sine\n");
        printf("9. Cosine\n");
        printf("10. Tangent\n");
        printf("11. Log base 10\n");
        printf("12. Natural log\n");
        printf("13. Exponential (e^x)\n");
        printf("14. Memory Save (M+)\n");
        printf("15. Memory Recall\n");
        printf("16. Memory Clear\n");
        printf("17. Show History\n");
        printf("18. Clear History\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf(" %d", &choice);

        if (choice == 0) {
            printf("Program ended. Goodbye!\n");
            break;
        }

        switch (choice) {
            case 1:
                printf("Enter two numbers: ");
                scanf(" %lf %lf", &a, &b);
                result = a + b;
                printf("Result: %.2f\n", result);
                addToHistory(result);
                break;

            case 2:
                printf("Enter two numbers: ");
                scanf(" %lf %lf", &a, &b);
                result = a - b;
                printf("Result: %.2f\n", result);
                addToHistory(result);
                break;

            case 3:
                printf("Enter two numbers: ");
                scanf(" %lf %lf", &a, &b);
                result = a * b;
                printf("Result: %.2f\n", result);
                addToHistory(result);
                break;

            case 4:
                printf("Enter two numbers: ");
                scanf(" %lf %lf", &a, &b);
                if (b == 0) {
                    printf("Error: Division by zero is not allowed.\n");
                } else {
                    result = a / b;
                    printf("Result: %.2f\n", result);
                    addToHistory(result);
                }
                break;

            case 5:
                printf("Enter two numbers: ");
                scanf(" %lf %lf", &a, &b);
                if (b == 0) {
                    printf("Error: Modulo by zero is not allowed.\n");
                } else {
                    result = fmod(a, b);
                    printf("Result: %.2f\n", result);
                    addToHistory(result);
                }
                break;

            case 6:
                printf("Enter base and exponent: ");
                scanf(" %lf %lf", &a, &b);
                result = pow(a, b);
                printf("Result: %.2f\n", result);
                addToHistory(result);
                break;

            case 7:
                printf("Enter a number: ");
                scanf(" %lf", &a);
                if (a < 0) {
                    printf("Error: Square root of a negative number is not defined in real numbers.\n");
                } else {
                    result = sqrt(a);
                    printf("Result: %.2f\n", result);
                    addToHistory(result);
                }
                break;

            case 8:
                printf("Enter angle in degrees: ");
                scanf(" %lf", &a);
                result = sin(a * 3.141592653589793 / 180.0);
                printf("Result: %.4f\n", result);
                addToHistory(result);
                break;

            case 9:
                printf("Enter angle in degrees: ");
                scanf(" %lf", &a);
                result = cos(a * 3.141592653589793 / 180.0);
                printf("Result: %.4f\n", result);
                addToHistory(result);
                break;

            case 10:
                printf("Enter angle in degrees: ");
                scanf(" %lf", &a);
                result = tan(a * 3.141592653589793 / 180.0);
                printf("Result: %.4f\n", result);
                addToHistory(result);
                break;

            case 11:
                printf("Enter a positive number: ");
                scanf(" %lf", &a);
                if (a <= 0) {
                    printf("Error: Logarithm is undefined for numbers <= 0.\n");
                } else {
                    result = log10(a);
                    printf("Result: %.4f\n", result);
                    addToHistory(result);
                }
                break;

            case 12:
                printf("Enter a positive number: ");
                scanf(" %lf", &a);
                if (a <= 0) {
                    printf("Error: Natural logarithm is undefined for numbers <= 0.\n");
                } else {
                    result = log(a);
                    printf("Result: %.4f\n", result);
                    addToHistory(result);
                }
                break;

            case 13:
                printf("Enter exponent value: ");
                scanf(" %lf", &a);
                result = exp(a);
                printf("Result: %.4f\n", result);
                addToHistory(result);
                break;

            case 14:
                printf("Enter a value to save in memory: ");
                scanf(" %lf", &a);
                memory = a;
                printf("Memory saved: %.2f\n", memory);
                break;

            case 15:
                printf("Memory value: %.2f\n", memory);
                break;

            case 16:
                memory = 0.0;
                printf("Memory cleared.\n");
                break;

            case 17:
                showHistory();
                break;

            case 18:
                clearHistory();
                break;

            default:
                printf("Invalid choice! Please select a valid option.\n");
                break;
        }

        printf("Do you want to continue? (y/n): ");
        scanf(" %c", &repeat);

    } while (repeat == 'Y' || repeat == 'y');

    printf("Thanks for using Advanced Calculator!\n");
    return 0;
}
