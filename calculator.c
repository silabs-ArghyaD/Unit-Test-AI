#include <stdio.h>

/**
 * @brief Add two integers
 * @param a First integer
 * @param b Second integer
 * @return Sum of a and b
 */
int add(int a, int b) {
    return a + b;
}

/**
 * @brief Subtract two integers
 * @param a First integer
 * @param b Second integer
 * @return Difference of a and b
 */
int subtract(int a, int b) {
    return a - b;
}

/**
 * @brief Multiply two integers
 * @param a First integer
 * @param b Second integer
 * @return Product of a and b
 */
int multiply(int a, int b) {
    return a * b;
}

/**
 * @brief Divide two integers
 * @param a Dividend
 * @param b Divisor
 * @return Quotient of a divided by b, or 0 if b is 0
 */
int divide(int a, int b) {
    if (b == 0) {
        return 0; // Simple error handling
    }
    return a / b;
}

/**
 * @brief Check if a number is even
 * @param n Number to check
 * @return 1 if even, 0 if odd
 */
int is_even(int n) {
    return (n % 2) == 0;
}

/**
 * @brief Find maximum of two integers
 * @param a First integer
 * @param b Second integer
 * @return Maximum of a and b
 */
int max(int a, int b) {
    return (a > b) ? a : b;
}

/**
 * @brief Main function to demonstrate the calculator
 */
int main() {
    printf("Calculator Demo\n");
    printf("===============\n");
    
    int x = 10, y = 3;
    
    printf("Numbers: %d and %d\n", x, y);
    printf("Add: %d + %d = %d\n", x, y, add(x, y));
    printf("Subtract: %d - %d = %d\n", x, y, subtract(x, y));
    printf("Multiply: %d * %d = %d\n", x, y, multiply(x, y));
    printf("Divide: %d / %d = %d\n", x, y, divide(x, y));
    printf("Max: max(%d, %d) = %d\n", x, y, max(x, y));
    printf("Is %d even? %s\n", x, is_even(x) ? "Yes" : "No");
    printf("Is %d even? %s\n", y, is_even(y) ? "Yes" : "No");
    
    return 0;
}
