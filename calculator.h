#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define MAX_HISTORY 20
#define PI 3.141592653589793238462643383279502884

/* Angle Modes */
#define MODE_DEGREE 1
#define MODE_RADIAN 2

/* Status / Error Codes */
#define CALC_SUCCESS 0
#define CALC_ERR_DIV_ZERO 1
#define CALC_ERR_NEG_SQRT 2
#define CALC_ERR_INVALID_LOG 3
#define CALC_ERR_INVALID_TRIG_DOMAIN 4
#define CALC_ERR_FACTORIAL_INVALID 5
#define CALC_ERR_FACTORIAL_OVERFLOW 6
#define CALC_ERR_TAN_UNDEFINED 7
#define CALC_ERR_MOD_ZERO 8
#define CALC_ERR_INVALID_POWER 9
#define CALC_ERR_INVALID_INPUT 10

/**
 * Structure to store current calculation details.
 * Contains the operands, selected operation, result,
 * status flag, and error descriptions.
 */
typedef struct {
    double num1;
    double num2;
    double result;
    char operation[20];
    int is_unary;       /* 1 = single operand operation, 0 = binary operation */
    int status;         /* 0 = success, other = error code */
    char error_msg[64]; /* Descriptive error message if status != 0 */
} Calculator;

/**
 * Structure to store individual calculation history records.
 * Keeps track of previous calculations up to MAX_HISTORY.
 */
typedef struct {
    double num1;
    double num2;
    char operation[20];
    double result;
    int is_unary;
    int status;
    char error_msg[64];
} History;

/* ========================================================================= */
/* FUNCTION PROTOTYPES                                                       */
/* ========================================================================= */

/* --- UI & Helper Functions --- */
void displayMenu(int angleMode);
void printHeader(void);
void clearScreen(void);
const char* getStatusDescription(int status);
double cleanValue(double val);

/* --- Basic Arithmetic Functions --- */
double add(double a, double b);
double subtract(double a, double b);
double multiply(double a, double b);
double divide(double a, double b, int *status);
long long modulus(long long a, long long b, int *status);

/* --- Scientific Functions --- */
double square(double a);
double cube(double a);
double squareRoot(double a, int *status);
double cubeRoot(double a);
double power(double base, double exp, int *status);
double reciprocal(double a, int *status);
double factorial(int n, int *status);
double absoluteVal(double a);

/* --- Angle Conversion & Trigonometric Functions --- */
double degreesToRadians(double deg);
double radiansToDegrees(double rad);
double sine(double angle, int mode);
double cosine(double angle, int mode);
double tangent(double angle, int mode, int *status);
double arcsine(double value, int mode, int *status);
double arccosine(double value, int mode, int *status);
double arctangent(double value, int mode);

/* --- Logarithmic & Exponential Functions --- */
double naturalLog(double a, int *status);
double logBase10(double a, int *status);
double exponential(double a);

/* --- Additional Helper Functions --- */
double percentage(double value, double total, int *status);
long long gcd(long long a, long long b);
long long lcm(long long a, long long b, int *status);

/* --- Structure-based Calculator Engine Functions --- */
Calculator createCalculator(double n1, double n2, const char *op, int is_unary);
Calculator executeCalculation(Calculator calc, int angleMode);
void displayResult(Calculator calc, int angleMode);

/* --- History Management Functions --- */
void initHistory(void);
void addHistory(Calculator calc);
void displayHistory(void);
void clearHistory(void);
int getHistoryCount(void);
History getHistoryAt(int index);

#endif /* CALCULATOR_H */

