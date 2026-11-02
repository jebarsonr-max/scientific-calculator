#include "calculator.h"

/* Terminal Colors & Styles */
#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_CYAN    "\033[1;36m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_BLUE    "\033[1;34m"
#define COLOR_MAGENTA "\033[1;35m"


/**
 * Safely reads a double value from the console with input validation.
 */
static int readDouble(const char *prompt, double *outValue) {
    char buffer[128];
    char *endptr;

    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            return 0;
        }

        /* Remove trailing newline */
        buffer[strcspn(buffer, "\r\n")] = '\0';

        /* Skip empty lines */
        if (strlen(buffer) == 0) {
            printf(COLOR_YELLOW "   [!] Input cannot be empty. Please enter a valid number.\n" COLOR_RESET);
            continue;
        }

        *outValue = strtod(buffer, &endptr);

        /* Ensure entire string was parsed as a number */
        while (*endptr == ' ' || *endptr == '\t') endptr++;
        if (*endptr != '\0') {
            printf(COLOR_RED "   [!] Invalid input! Please enter a valid numerical value.\n" COLOR_RESET);
            continue;
        }

        return 1;
    }
}

/**
 * Safely reads an integer value from the console with input validation.
 */
static int readInteger(const char *prompt, int *outValue) {
    double temp;
    if (readDouble(prompt, &temp)) {
        *outValue = (int)temp;
        return 1;
    }
    return 0;
}

void printHeader(void) {
    printf(COLOR_CYAN);
    printf("=====================================================================\n");
    printf("   ____   ____ ___ _____ _   _ _____ ___ _____ ___ ____   ____ _    _\n");
    printf("  / ___| / ___|_ _| ____| \\ | |_   _|_ _|  ___|_ _/ ___| / ___| |  | |\n");
    printf("  \\___ \\| |    | ||  _| |  \\| | | |  | || |_   | | |    | |   | |  | |\n");
    printf("   ___) | |___ | || |___| |\\  | | |  | ||  _|  | | |___ | |___| |__| |\n");
    printf("  |____/ \\____|___|_____|_| \\_| |_| |___|_|   |___\\____| \\____|_____|\n");
    printf("                 SCIENTIFIC CALCULATOR FOR STUDENTS                  \n");
    printf("           Developed with C Structures, Functions & Math.h           \n");
    printf("=====================================================================\n");
    printf(COLOR_RESET);
}

void displayMenu(int angleMode) {
    const char *modeStr = (angleMode == MODE_DEGREE) ? "DEGREES (DEG)" : "RADIANS (RAD)";

    printf(COLOR_BOLD "\n-------------------------- [ MAIN MENU ] ---------------------------\n" COLOR_RESET);
    printf(" Current Angle Mode: " COLOR_GREEN "[%s]" COLOR_RESET "\n\n", modeStr);

    printf(COLOR_YELLOW " [1] Basic Arithmetic Operations" COLOR_RESET "\n");
    printf("   1. Addition (x + y)                 2. Subtraction (x - y)\n");
    printf("   3. Multiplication (x * y)           4. Division (x / y)\n");
    printf("   5. Modulus for Integers (x %% y)\n\n");

    printf(COLOR_CYAN " [2] Advanced Scientific Operations" COLOR_RESET "\n");
    printf("   6. Square (x^2)                     7. Cube (x^3)\n");
    printf("   8. Square Root (vx)                 9. Cube Root (3vx)\n");
    printf("  10. Power (x^y)                     11. Reciprocal (1/x)\n");
    printf("  12. Factorial (n!)                  13. Absolute Value (|x|)\n\n");

    printf(COLOR_MAGENTA " [3] Trigonometric Functions" COLOR_RESET "\n");
    printf("  14. Sine: sin(x)                    15. Cosine: cos(x)\n");
    printf("  16. Tangent: tan(x)                 17. Inverse Sine: asin(x)\n");
    printf("  18. Inverse Cosine: acos(x)         19. Inverse Tangent: atan(x)\n\n");

    printf(COLOR_BLUE " [4] Logarithmic & Exponential" COLOR_RESET "\n");
    printf("  20. Natural Logarithm: ln(x)        21. Logarithm Base 10: log10(x)\n");
    printf("  22. Exponential Function: e^x\n\n");

    printf(COLOR_GREEN " [5] Additional Tools & History" COLOR_RESET "\n");
    printf("  23. Percentage Calculator (x%% of y) 24. Greatest Common Divisor (GCD)\n");
    printf("  25. Least Common Multiple (LCM)     26. Toggle Angle Mode (DEG/RAD)\n");
    printf("  27. View Calculation History        28. Clear Calculation History\n\n");

    printf(COLOR_RED "   0. Exit Calculator\n" COLOR_RESET);
    printf("---------------------------------------------------------------------\n");
}

void clearScreen(void) {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

/**
 * Handles CLI/JSON evaluation mode for external frontends or automated tests.
 * Usage: calculator.exe --json <OPERATION> <NUM1> [NUM2] [ANGLE_MODE]
 */
static int handleJsonMode(int argc, char *argv[]) {
    if (argc < 4) {
        printf("{\"status\": 10, \"error\": \"Insufficient arguments for evaluation.\"}\n");
        return 1;
    }

    const char *op = argv[2];
    double num1 = atof(argv[3]);
    double num2 = (argc >= 5) ? atof(argv[4]) : 0.0;
    int angleMode = (argc >= 6) ? atoi(argv[5]) : MODE_DEGREE;

    /* Determine if operation is unary */
    int is_unary = 0;
    if (strcmp(op, "SQUARE") == 0 || strcmp(op, "CUBE") == 0 ||
        strcmp(op, "SQRT") == 0 || strcmp(op, "CBRT") == 0 ||
        strcmp(op, "RECIPROCAL") == 0 || strcmp(op, "FACTORIAL") == 0 ||
        strcmp(op, "ABS") == 0 || strcmp(op, "SIN") == 0 ||
        strcmp(op, "COS") == 0 || strcmp(op, "TAN") == 0 ||
        strcmp(op, "ASIN") == 0 || strcmp(op, "ACOS") == 0 ||
        strcmp(op, "ATAN") == 0 || strcmp(op, "LN") == 0 ||
        strcmp(op, "LOG10") == 0 || strcmp(op, "EXP") == 0) {
        is_unary = 1;
    }

    Calculator calc = createCalculator(num1, num2, op, is_unary);
    calc = executeCalculation(calc, angleMode);

    printf("{\"status\": %d, \"operation\": \"%s\", \"num1\": %.8g, \"num2\": %.8g, \"result\": %.8g, \"is_unary\": %d, \"error\": \"%s\"}\n",
           calc.status, calc.operation, calc.num1, calc.num2, calc.result, calc.is_unary, calc.error_msg);

    return 0;
}

int main(int argc, char *argv[]) {
    /* Check for JSON evaluation mode */
    if (argc >= 2 && strcmp(argv[1], "--json") == 0) {
        return handleJsonMode(argc, argv);
    }

    int choice = -1;
    int angleMode = MODE_DEGREE;

    initHistory();
    clearScreen();
    printHeader();

    printf(COLOR_GREEN " Welcome to the C Scientific Calculator!\n" COLOR_RESET);
    printf(" This program demonstrates modular C programming, structures, and math functions.\n");

    while (1) {
        displayMenu(angleMode);

        if (!readInteger(" Enter your choice (0-28): ", &choice)) {
            continue;
        }

        if (choice == 0) {
            printf("\n" COLOR_CYAN "Thank you for using the Scientific Calculator. Goodbye!\n\n" COLOR_RESET);
            break;
        }

        /* Handle Angle Mode Toggle */
        if (choice == 26) {
            angleMode = (angleMode == MODE_DEGREE) ? MODE_RADIAN : MODE_DEGREE;
            printf(COLOR_GREEN "\n [i] Angle mode switched to: %s\n" COLOR_RESET,
                   (angleMode == MODE_DEGREE) ? "DEGREES" : "RADIANS");
            continue;
        }

        /* Handle History Display */
        if (choice == 27) {
            displayHistory();
            continue;
        }

        /* Handle History Clear */
        if (choice == 28) {
            clearHistory();
            continue;
        }

        /* Validate Choice Range */
        if (choice < 1 || choice > 25) {
            printf(COLOR_RED "\n [!] Invalid choice (%d)! Please select a valid option from the menu.\n" COLOR_RESET, choice);
            continue;
        }

        Calculator currentCalc;
        double n1 = 0.0, n2 = 0.0;
        const char *angleUnit = (angleMode == MODE_DEGREE) ? "degrees" : "radians";

        /* Structure-based workflow with switch-case */
        switch (choice) {
            /* --- Basic Arithmetic --- */
            case 1: /* Addition */
                printf("\n--- Addition (x + y) ---\n");
                readDouble(" Enter first number (x): ", &n1);
                readDouble(" Enter second number (y): ", &n2);
                currentCalc = createCalculator(n1, n2, "ADD", 0);
                break;

            case 2: /* Subtraction */
                printf("\n--- Subtraction (x - y) ---\n");
                readDouble(" Enter first number (x): ", &n1);
                readDouble(" Enter second number (y): ", &n2);
                currentCalc = createCalculator(n1, n2, "SUB", 0);
                break;

            case 3: /* Multiplication */
                printf("\n--- Multiplication (x * y) ---\n");
                readDouble(" Enter first number (x): ", &n1);
                readDouble(" Enter second number (y): ", &n2);
                currentCalc = createCalculator(n1, n2, "MUL", 0);
                break;

            case 4: /* Division */
                printf("\n--- Division (x / y) ---\n");
                readDouble(" Enter numerator (x): ", &n1);
                readDouble(" Enter denominator (y): ", &n2);
                currentCalc = createCalculator(n1, n2, "DIV", 0);
                break;

            case 5: /* Modulus */
                printf("\n--- Modulus for Integers (x %% y) ---\n");
                readDouble(" Enter integer dividend (x): ", &n1);
                readDouble(" Enter integer divisor (y): ", &n2);
                currentCalc = createCalculator((long long)n1, (long long)n2, "MOD", 0);
                break;

            /* --- Scientific Operations --- */
            case 6: /* Square */
                printf("\n--- Square (x^2) ---\n");
                readDouble(" Enter number (x): ", &n1);
                currentCalc = createCalculator(n1, 0.0, "SQUARE", 1);
                break;

            case 7: /* Cube */
                printf("\n--- Cube (x^3) ---\n");
                readDouble(" Enter number (x): ", &n1);
                currentCalc = createCalculator(n1, 0.0, "CUBE", 1);
                break;

            case 8: /* Square Root */
                printf("\n--- Square Root (vx) ---\n");
                readDouble(" Enter non-negative number (x): ", &n1);
                currentCalc = createCalculator(n1, 0.0, "SQRT", 1);
                break;

            case 9: /* Cube Root */
                printf("\n--- Cube Root (3vx) ---\n");
                readDouble(" Enter number (x): ", &n1);
                currentCalc = createCalculator(n1, 0.0, "CBRT", 1);
                break;

            case 10: /* Power */
                printf("\n--- Power (x^y) ---\n");
                readDouble(" Enter base (x): ", &n1);
                readDouble(" Enter exponent (y): ", &n2);
                currentCalc = createCalculator(n1, n2, "POW", 0);
                break;

            case 11: /* Reciprocal */
                printf("\n--- Reciprocal (1/x) ---\n");
                readDouble(" Enter non-zero number (x): ", &n1);
                currentCalc = createCalculator(n1, 0.0, "RECIPROCAL", 1);
                break;

            case 12: /* Factorial */
                printf("\n--- Factorial (n!) ---\n");
                readDouble(" Enter non-negative integer (0 <= n <= 20): ", &n1);
                currentCalc = createCalculator(n1, 0.0, "FACTORIAL", 1);
                break;

            case 13: /* Absolute Value */
                printf("\n--- Absolute Value (|x|) ---\n");
                readDouble(" Enter number (x): ", &n1);
                currentCalc = createCalculator(n1, 0.0, "ABS", 1);
                break;

            /* --- Trigonometric Functions --- */
            case 14: /* Sine */
                printf("\n--- Sine: sin(x) in %s ---\n", angleUnit);
                readDouble(" Enter angle (x): ", &n1);
                currentCalc = createCalculator(n1, 0.0, "SIN", 1);
                break;

            case 15: /* Cosine */
                printf("\n--- Cosine: cos(x) in %s ---\n", angleUnit);
                readDouble(" Enter angle (x): ", &n1);
                currentCalc = createCalculator(n1, 0.0, "COS", 1);
                break;

            case 16: /* Tangent */
                printf("\n--- Tangent: tan(x) in %s ---\n", angleUnit);
                readDouble(" Enter angle (x): ", &n1);
                currentCalc = createCalculator(n1, 0.0, "TAN", 1);
                break;

            case 17: /* Inverse Sine */
                printf("\n--- Inverse Sine: asin(x) -> Output in %s ---\n", angleUnit);
                readDouble(" Enter value (-1.0 <= x <= 1.0): ", &n1);
                currentCalc = createCalculator(n1, 0.0, "ASIN", 1);
                break;

            case 18: /* Inverse Cosine */
                printf("\n--- Inverse Cosine: acos(x) -> Output in %s ---\n", angleUnit);
                readDouble(" Enter value (-1.0 <= x <= 1.0): ", &n1);
                currentCalc = createCalculator(n1, 0.0, "ACOS", 1);
                break;

            case 19: /* Inverse Tangent */
                printf("\n--- Inverse Tangent: atan(x) -> Output in %s ---\n", angleUnit);
                readDouble(" Enter value (x): ", &n1);
                currentCalc = createCalculator(n1, 0.0, "ATAN", 1);
                break;

            /* --- Logarithmic & Exponential --- */
            case 20: /* Natural Log */
                printf("\n--- Natural Logarithm: ln(x) ---\n");
                readDouble(" Enter positive number (x > 0): ", &n1);
                currentCalc = createCalculator(n1, 0.0, "LN", 1);
                break;

            case 21: /* Log Base 10 */
                printf("\n--- Logarithm Base 10: log10(x) ---\n");
                readDouble(" Enter positive number (x > 0): ", &n1);
                currentCalc = createCalculator(n1, 0.0, "LOG10", 1);
                break;

            case 22: /* Exponential */
                printf("\n--- Exponential: e^x ---\n");
                readDouble(" Enter exponent (x): ", &n1);
                currentCalc = createCalculator(n1, 0.0, "EXP", 1);
                break;

            /* --- Additional Functions --- */
            case 23: /* Percentage */
                printf("\n--- Percentage Calculator (x%% of y) ---\n");
                readDouble(" Enter percentage rate (x): ", &n1);
                readDouble(" Enter base total (y): ", &n2);
                currentCalc = createCalculator(n1, n2, "PERCENT", 0);
                break;

            case 24: /* GCD */
                printf("\n--- Greatest Common Divisor (GCD) ---\n");
                readDouble(" Enter first integer (x): ", &n1);
                readDouble(" Enter second integer (y): ", &n2);
                currentCalc = createCalculator((long long)n1, (long long)n2, "GCD", 0);
                break;

            case 25: /* LCM */
                printf("\n--- Least Common Multiple (LCM) ---\n");
                readDouble(" Enter first integer (x): ", &n1);
                readDouble(" Enter second integer (y): ", &n2);
                currentCalc = createCalculator((long long)n1, (long long)n2, "LCM", 0);
                break;

            default:
                continue;
        }

        /* Execute calculation using structures */
        currentCalc = executeCalculation(currentCalc, angleMode);

        /* Display result */
        displayResult(currentCalc, angleMode);

        /* Record into calculation history */
        addHistory(currentCalc);

        /* Prompt user to continue */
        printf("\nDo you want to perform another calculation? (y/n): ");
        char answer[16];
        if (fgets(answer, sizeof(answer), stdin)) {
            if (answer[0] == 'n' || answer[0] == 'N') {
                printf(COLOR_CYAN "\nThank you for using the Scientific Calculator. Goodbye!\n\n" COLOR_RESET);
                break;
            }
        }
    }

    return 0;
}
