#include "calculator.h"

/**
 * Clean small floating point inaccuracies (e.g. 1.22e-16 -> 0.0, 0.49999999999999994 -> 0.5)
 */
double cleanValue(double val) {
    if (fabs(val) < 1e-12) {
        return 0.0;
    }
    double rounded = round(val);
    if (fabs(val - rounded) < 1e-12) {
        return rounded;
    }
    /* Check common clean halves like 0.5, 1.5, 2.5 */
    double halfCheck = val * 2.0;
    double roundHalf = round(halfCheck);
    if (fabs(halfCheck - roundHalf) < 1e-12) {
        return roundHalf / 2.0;
    }
    return val;
}

const char* getStatusDescription(int status) {
    switch (status) {
        case CALC_SUCCESS:
            return "Calculation successful.";
        case CALC_ERR_DIV_ZERO:
            return "Error: Division by zero is undefined.";
        case CALC_ERR_NEG_SQRT:
            return "Error: Square root of a negative number is undefined in real numbers.";
        case CALC_ERR_INVALID_LOG:
            return "Error: Logarithm is only defined for strictly positive numbers (> 0).";
        case CALC_ERR_INVALID_TRIG_DOMAIN:
            return "Error: Input value for inverse trigonometric function must be between -1 and 1.";
        case CALC_ERR_FACTORIAL_INVALID:
            return "Error: Factorial is only defined for non-negative integers (n >= 0).";
        case CALC_ERR_FACTORIAL_OVERFLOW:
            return "Error: Factorial input exceeds maximum limit (n <= 20) to prevent 64-bit overflow.";
        case CALC_ERR_TAN_UNDEFINED:
            return "Error: Tangent is undefined at odd multiples of 90 degrees (cos = 0).";
        case CALC_ERR_MOD_ZERO:
            return "Error: Modulo by zero is undefined.";
        case CALC_ERR_INVALID_POWER:
            return "Error: Invalid power operation (e.g. 0^0 or negative base with non-integer exponent).";
        case CALC_ERR_INVALID_INPUT:
            return "Error: Invalid numeric input provided.";
        default:
            return "Error: Unknown calculation error.";
    }
}

/* ========================================================================= */
/* BASIC ARITHMETIC OPERATIONS                                               */
/* ========================================================================= */

double add(double a, double b) {
    return a + b;
}

double subtract(double a, double b) {
    return a - b;
}

double multiply(double a, double b) {
    return a * b;
}

double divide(double a, double b, int *status) {
    if (fabs(b) < 1e-15) {
        *status = CALC_ERR_DIV_ZERO;
        return 0.0;
    }
    *status = CALC_SUCCESS;
    return a / b;
}

long long modulus(long long a, long long b, int *status) {
    if (b == 0) {
        *status = CALC_ERR_MOD_ZERO;
        return 0;
    }
    *status = CALC_SUCCESS;
    return a % b;
}

/* ========================================================================= */
/* SCIENTIFIC OPERATIONS                                                     */
/* ========================================================================= */

double square(double a) {
    return a * a;
}

double cube(double a) {
    return a * a * a;
}

double squareRoot(double a, int *status) {
    if (a < 0.0) {
        *status = CALC_ERR_NEG_SQRT;
        return 0.0;
    }
    *status = CALC_SUCCESS;
    return cleanValue(sqrt(a));
}

double cubeRoot(double a) {
    return cleanValue(cbrt(a));
}

double power(double base, double exp, int *status) {
    if (fabs(base) < 1e-15 && exp <= 0.0) {
        *status = CALC_ERR_INVALID_POWER;
        return 0.0;
    }
    if (base < 0.0 && floor(exp) != exp) {
        *status = CALC_ERR_INVALID_POWER;
        return 0.0;
    }
    *status = CALC_SUCCESS;
    return cleanValue(pow(base, exp));
}

double reciprocal(double a, int *status) {
    if (fabs(a) < 1e-15) {
        *status = CALC_ERR_DIV_ZERO;
        return 0.0;
    }
    *status = CALC_SUCCESS;
    return 1.0 / a;
}

double factorial(int n, int *status) {
    if (n < 0) {
        *status = CALC_ERR_FACTORIAL_INVALID;
        return 0.0;
    }
    if (n > 20) {
        *status = CALC_ERR_FACTORIAL_OVERFLOW;
        return 0.0;
    }
    *status = CALC_SUCCESS;
    if (n == 0 || n == 1) {
        return 1.0;
    }
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= (double)i;
    }
    return result;
}

double absoluteVal(double a) {
    return fabs(a);
}

/* ========================================================================= */
/* ANGLE CONVERSION AND TRIGONOMETRIC OPERATIONS                            */
/* ========================================================================= */

double degreesToRadians(double deg) {
    return deg * (PI / 180.0);
}

double radiansToDegrees(double rad) {
    return rad * (180.0 / PI);
}

double sine(double angle, int mode) {
    double rad = (mode == MODE_DEGREE) ? degreesToRadians(angle) : angle;
    return cleanValue(sin(rad));
}

double cosine(double angle, int mode) {
    double rad = (mode == MODE_DEGREE) ? degreesToRadians(angle) : angle;
    return cleanValue(cos(rad));
}

double tangent(double angle, int mode, int *status) {
    if (mode == MODE_DEGREE) {
        /* Check for 90 + 180k */
        double normalized = fmod(fabs(angle), 180.0);
        if (fabs(normalized - 90.0) < 1e-6) {
            *status = CALC_ERR_TAN_UNDEFINED;
            return 0.0;
        }
    }
    double rad = (mode == MODE_DEGREE) ? degreesToRadians(angle) : angle;
    double cosVal = cos(rad);
    if (fabs(cosVal) < 1e-12) {
        *status = CALC_ERR_TAN_UNDEFINED;
        return 0.0;
    }
    *status = CALC_SUCCESS;
    return cleanValue(tan(rad));
}

double arcsine(double value, int mode, int *status) {
    if (value < -1.0 || value > 1.0) {
        *status = CALC_ERR_INVALID_TRIG_DOMAIN;
        return 0.0;
    }
    *status = CALC_SUCCESS;
    double rad = asin(value);
    if (mode == MODE_DEGREE) {
        return cleanValue(radiansToDegrees(rad));
    }
    return cleanValue(rad);
}

double arccosine(double value, int mode, int *status) {
    if (value < -1.0 || value > 1.0) {
        *status = CALC_ERR_INVALID_TRIG_DOMAIN;
        return 0.0;
    }
    *status = CALC_SUCCESS;
    double rad = acos(value);
    if (mode == MODE_DEGREE) {
        return cleanValue(radiansToDegrees(rad));
    }
    return cleanValue(rad);
}

double arctangent(double value, int mode) {
    double rad = atan(value);
    if (mode == MODE_DEGREE) {
        return cleanValue(radiansToDegrees(rad));
    }
    return cleanValue(rad);
}

/* ========================================================================= */
/* LOGARITHMIC AND EXPONENTIAL OPERATIONS                                    */
/* ========================================================================= */

double naturalLog(double a, int *status) {
    if (a <= 0.0) {
        *status = CALC_ERR_INVALID_LOG;
        return 0.0;
    }
    *status = CALC_SUCCESS;
    return cleanValue(log(a));
}

double logBase10(double a, int *status) {
    if (a <= 0.0) {
        *status = CALC_ERR_INVALID_LOG;
        return 0.0;
    }
    *status = CALC_SUCCESS;
    return cleanValue(log10(a));
}

double exponential(double a) {
    return cleanValue(exp(a));
}

/* ========================================================================= */
/* ADDITIONAL MATHEMATICAL FUNCTIONS                                         */
/* ========================================================================= */

double percentage(double value, double total, int *status) {
    *status = CALC_SUCCESS;
    /* Computes: (value * total) / 100.0 (value percent of total) */
    return (value * total) / 100.0;
}

long long gcd(long long a, long long b) {
    a = llabs(a);
    b = llabs(b);
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

long long lcm(long long a, long long b, int *status) {
    if (a == 0 || b == 0) {
        *status = CALC_SUCCESS;
        return 0;
    }
    *status = CALC_SUCCESS;
    long long g = gcd(a, b);
    return (llabs(a) / g) * llabs(b);
}

/* ========================================================================= */
/* STRUCTURE-BASED CALCULATOR ENGINE                                         */
/* ========================================================================= */

/**
 * Creates and initializes a Calculator structure instance.
 * Demonstrates structure variable initialization.
 */
Calculator createCalculator(double n1, double n2, const char *op, int is_unary) {
    Calculator calc;
    calc.num1 = n1;
    calc.num2 = n2;
    calc.result = 0.0;
    calc.is_unary = is_unary;
    calc.status = CALC_SUCCESS;
    strncpy(calc.operation, op, sizeof(calc.operation) - 1);
    calc.operation[sizeof(calc.operation) - 1] = '\0';
    calc.error_msg[0] = '\0';
    return calc;
}

/**
 * Executes calculation using the passed Calculator structure.
 * Demonstrates:
 * - Passing structures to functions
 * - Returning modified structures from functions
 * - Switch-case statements mapped to operation identifiers
 */
Calculator executeCalculation(Calculator calc, int angleMode) {
    int status = CALC_SUCCESS;

    if (strcmp(calc.operation, "ADD") == 0) {
        calc.result = add(calc.num1, calc.num2);
    } else if (strcmp(calc.operation, "SUB") == 0) {
        calc.result = subtract(calc.num1, calc.num2);
    } else if (strcmp(calc.operation, "MUL") == 0) {
        calc.result = multiply(calc.num1, calc.num2);
    } else if (strcmp(calc.operation, "DIV") == 0) {
        calc.result = divide(calc.num1, calc.num2, &status);
    } else if (strcmp(calc.operation, "MOD") == 0) {
        calc.result = (double)modulus((long long)calc.num1, (long long)calc.num2, &status);
    } else if (strcmp(calc.operation, "SQUARE") == 0) {
        calc.result = square(calc.num1);
    } else if (strcmp(calc.operation, "CUBE") == 0) {
        calc.result = cube(calc.num1);
    } else if (strcmp(calc.operation, "SQRT") == 0) {
        calc.result = squareRoot(calc.num1, &status);
    } else if (strcmp(calc.operation, "CBRT") == 0) {
        calc.result = cubeRoot(calc.num1);
    } else if (strcmp(calc.operation, "POW") == 0) {
        calc.result = power(calc.num1, calc.num2, &status);
    } else if (strcmp(calc.operation, "RECIPROCAL") == 0) {
        calc.result = reciprocal(calc.num1, &status);
    } else if (strcmp(calc.operation, "FACTORIAL") == 0) {
        calc.result = factorial((int)calc.num1, &status);
    } else if (strcmp(calc.operation, "ABS") == 0) {
        calc.result = absoluteVal(calc.num1);
    } else if (strcmp(calc.operation, "SIN") == 0) {
        calc.result = sine(calc.num1, angleMode);
    } else if (strcmp(calc.operation, "COS") == 0) {
        calc.result = cosine(calc.num1, angleMode);
    } else if (strcmp(calc.operation, "TAN") == 0) {
        calc.result = tangent(calc.num1, angleMode, &status);
    } else if (strcmp(calc.operation, "ASIN") == 0) {
        calc.result = arcsine(calc.num1, angleMode, &status);
    } else if (strcmp(calc.operation, "ACOS") == 0) {
        calc.result = arccosine(calc.num1, angleMode, &status);
    } else if (strcmp(calc.operation, "ATAN") == 0) {
        calc.result = arctangent(calc.num1, angleMode);
    } else if (strcmp(calc.operation, "LN") == 0) {
        calc.result = naturalLog(calc.num1, &status);
    } else if (strcmp(calc.operation, "LOG10") == 0) {
        calc.result = logBase10(calc.num1, &status);
    } else if (strcmp(calc.operation, "EXP") == 0) {
        calc.result = exponential(calc.num1);
    } else if (strcmp(calc.operation, "PERCENT") == 0) {
        calc.result = percentage(calc.num1, calc.num2, &status);
    } else if (strcmp(calc.operation, "GCD") == 0) {
        calc.result = (double)gcd((long long)calc.num1, (long long)calc.num2);
    } else if (strcmp(calc.operation, "LCM") == 0) {
        calc.result = (double)lcm((long long)calc.num1, (long long)calc.num2, &status);
    } else {
        status = CALC_ERR_INVALID_INPUT;
    }

    calc.status = status;
    if (status != CALC_SUCCESS) {
        strncpy(calc.error_msg, getStatusDescription(status), sizeof(calc.error_msg) - 1);
        calc.error_msg[sizeof(calc.error_msg) - 1] = '\0';
    } else {
        calc.error_msg[0] = '\0';
    }

    return calc;
}

/**
 * Displays calculation result or error message using structure fields.
 */
void displayResult(Calculator calc, int angleMode) {
    const char *unit = (angleMode == MODE_DEGREE) ? "deg" : "rad";

    printf("\n+-----------------------------------------------------------+\n");
    printf("|                    CALCULATION RESULT                     |\n");
    printf("+-----------------------------------------------------------+\n");

    if (calc.status != CALC_SUCCESS) {
        printf("| Status : \033[1;31mFAILED\033[0m\n");
        printf("| Error  : \033[1;31m%s\033[0m\n", calc.error_msg);
    } else {
        printf("| Status : \033[1;32mSUCCESS\033[0m\n");

        if (calc.is_unary) {
            if (strcmp(calc.operation, "SIN") == 0 ||
                strcmp(calc.operation, "COS") == 0 ||
                strcmp(calc.operation, "TAN") == 0) {
                printf("| Expression: %s(%.4f %s)\n", calc.operation, calc.num1, unit);
            } else if (strcmp(calc.operation, "ASIN") == 0 ||
                       strcmp(calc.operation, "ACOS") == 0 ||
                       strcmp(calc.operation, "ATAN") == 0) {
                printf("| Expression: %s(%.4f) -> Result in %s\n", calc.operation, calc.num1, unit);
            } else if (strcmp(calc.operation, "SQRT") == 0) {
                printf("| Expression: sqrt(%.4f)\n", calc.num1);
            } else if (strcmp(calc.operation, "CBRT") == 0) {
                printf("| Expression: cbrt(%.4f)\n", calc.num1);
            } else if (strcmp(calc.operation, "SQUARE") == 0) {
                printf("| Expression: (%.4f)^2\n", calc.num1);
            } else if (strcmp(calc.operation, "CUBE") == 0) {
                printf("| Expression: (%.4f)^3\n", calc.num1);
            } else if (strcmp(calc.operation, "FACTORIAL") == 0) {
                printf("| Expression: (%.0f)!\n", calc.num1);
            } else if (strcmp(calc.operation, "RECIPROCAL") == 0) {
                printf("| Expression: 1 / (%.4f)\n", calc.num1);
            } else if (strcmp(calc.operation, "ABS") == 0) {
                printf("| Expression: |%.4f|\n", calc.num1);
            } else if (strcmp(calc.operation, "LN") == 0) {
                printf("| Expression: ln(%.4f)\n", calc.num1);
            } else if (strcmp(calc.operation, "LOG10") == 0) {
                printf("| Expression: log10(%.4f)\n", calc.num1);
            } else if (strcmp(calc.operation, "EXP") == 0) {
                printf("| Expression: e^(%.4f)\n", calc.num1);
            } else {
                printf("| Expression: %s(%.4f)\n", calc.operation, calc.num1);
            }
        } else {
            if (strcmp(calc.operation, "ADD") == 0) {
                printf("| Expression: %.4f + %.4f\n", calc.num1, calc.num2);
            } else if (strcmp(calc.operation, "SUB") == 0) {
                printf("| Expression: %.4f - %.4f\n", calc.num1, calc.num2);
            } else if (strcmp(calc.operation, "MUL") == 0) {
                printf("| Expression: %.4f * %.4f\n", calc.num1, calc.num2);
            } else if (strcmp(calc.operation, "DIV") == 0) {
                printf("| Expression: %.4f / %.4f\n", calc.num1, calc.num2);
            } else if (strcmp(calc.operation, "MOD") == 0) {
                printf("| Expression: %.0f %% %.0f\n", calc.num1, calc.num2);
            } else if (strcmp(calc.operation, "POW") == 0) {
                printf("| Expression: %.4f ^ %.4f\n", calc.num1, calc.num2);
            } else if (strcmp(calc.operation, "PERCENT") == 0) {
                printf("| Expression: %.4f%% of %.4f\n", calc.num1, calc.num2);
            } else if (strcmp(calc.operation, "GCD") == 0) {
                printf("| Expression: GCD(%.0f, %.0f)\n", calc.num1, calc.num2);
            } else if (strcmp(calc.operation, "LCM") == 0) {
                printf("| Expression: LCM(%.0f, %.0f)\n", calc.num1, calc.num2);
            } else {
                printf("| Expression: %.4f %s %.4f\n", calc.num1, calc.operation, calc.num2);
            }
        }

        /* Check if integer representation is preferred */
        if (floor(calc.result) == calc.result && fabs(calc.result) < 1e14) {
            printf("| Result     : \033[1;36m%.0f\033[0m\n", calc.result);
        } else {
            printf("| Result     : \033[1;36m%.8g\033[0m (Standard: \033[1;36m%.6f\033[0m)\n", calc.result, calc.result);
        }
    }
    printf("+-----------------------------------------------------------+\n");
}

