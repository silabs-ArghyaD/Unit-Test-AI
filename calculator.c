#include "calculator.h"
#include <stdio.h>

/**
 * @brief Add two integers
 */
int add(int a, int b) {
    return a + b;
}

/**
 * @brief Subtract two integers
 */
int subtract(int a, int b) {
    return a - b;
}

/**
 * @brief Multiply two integers
 */
int multiply(int a, int b) {
    return a * b;
}

/**
 * @brief Divide two integers
 */
int divide(int a, int b) {
    if (b == 0) {
        return 0; // Simple error handling
    }
    return a / b;
}

/**
 * @brief Check if a number is even
 */
int is_even(int n) {
    return (n % 2) == 0;
}

/**
 * @brief Find maximum of two integers
 */
int max(int a, int b) {
    return (a > b) ? a : b;
}
