#ifndef CALCULATOR_H
#define CALCULATOR_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Add two integers
 * @param a First integer
 * @param b Second integer
 * @return Sum of a and b
 */
int add(int a, int b);

/**
 * @brief Subtract two integers
 * @param a First integer
 * @param b Second integer
 * @return Difference of a and b
 */
int subtract(int a, int b);

/**
 * @brief Multiply two integers
 * @param a First integer
 * @param b Second integer
 * @return Product of a and b
 */
int multiply(int a, int b);

/**
 * @brief Divide two integers
 * @param a Dividend
 * @param b Divisor
 * @return Quotient of a divided by b, or 0 if b is 0
 */
int divide(int a, int b);

/**
 * @brief Check if a number is even
 * @param n Number to check
 * @return 1 if even, 0 if odd
 */
int is_even(int n);

/**
 * @brief Find maximum of two integers
 * @param a First integer
 * @param b Second integer
 * @return Maximum of a and b
 */
int max(int a, int b);

/**
 * @brief Find minimum of two integers
 * @param a First integer
 * @param b Second integer
 * @return Minimum of a and b
 */
int min(int a, int b);

#ifdef __cplusplus
}
#endif

#endif // CALCULATOR_H
