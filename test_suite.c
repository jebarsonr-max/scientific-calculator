#include "calculator.h"
#include <assert.h>

static int totalTests = 0;
static int passedTests = 0;

void assertDoubleEqual(const char *testName, double actual, double expected, double tolerance) {
    totalTests++;
    if (fabs(actual - expected) <= tolerance) {
        printf("  [PASS] %-35s Expected: %.6g, Got: %.6g\n", testName, expected, actual);
        passedTests++;
    } else {
        printf("  \033[1;31m[FAIL]\033[0m %-35s Expected: %.6g, Got: %.6g\n", testName, expected, actual);
    }
}

void assertStatusEqual(const char *testName, int actualStatus, int expectedStatus) {
    totalTests++;
    if (actualStatus == expectedStatus) {
        printf("  [PASS] %-35s Status matched: %d (%s)\n", testName, actualStatus, getStatusDescription(actualStatus));
        passedTests++;
    } else {
        printf("  \033[1;31m[FAIL]\033[0m %-35s Expected status: %d, Got: %d\n", testName, expectedStatus, actualStatus);
    }
}

int main(void) {
    printf("\n=====================================================================\n");

    /* 1. Prompt Mandatory Test Cases */
    printf("--- [1] MANDATORY PROMPT TEST CASES ---\n");
    
    /* 10 + 20 = 30 */
    Calculator c1 = createCalculator(10.0, 20.0, "ADD", 0);
    c1 = executeCalculation(c1, MODE_DEGREE);
    assertDoubleEqual("Addition: 10 + 20", c1.result, 30.0, 1e-9);
    assertStatusEqual("Addition status: SUCCESS", c1.status, CALC_SUCCESS);

    /* 5 * 4 = 20 */
    Calculator c2 = createCalculator(5.0, 4.0, "MUL", 0);
    c2 = executeCalculation(c2, MODE_DEGREE);
    assertDoubleEqual("Multiplication: 5 * 4", c2.result, 20.0, 1e-9);

    /* sqrt(64) = 8 */
    Calculator c3 = createCalculator(64.0, 0.0, "SQRT", 1);
    c3 = executeCalculation(c3, MODE_DEGREE);
    assertDoubleEqual("Square Root: sqrt(64)", c3.result, 8.0, 1e-9);

    /* 2^5 = 32 */
    Calculator c4 = createCalculator(2.0, 5.0, "POW", 0);
    c4 = executeCalculation(c4, MODE_DEGREE);
    assertDoubleEqual("Power: 2^5", c4.result, 32.0, 1e-9);

    /* 5! = 120 */
    Calculator c5 = createCalculator(5.0, 0.0, "FACTORIAL", 1);
    c5 = executeCalculation(c5, MODE_DEGREE);
    assertDoubleEqual("Factorial: 5!", c5.result, 120.0, 1e-9);

    /* sin(30 deg) = 0.5 */
    Calculator c6 = createCalculator(30.0, 0.0, "SIN", 1);
    c6 = executeCalculation(c6, MODE_DEGREE);
    assertDoubleEqual("Sine: sin(30 deg)", c6.result, 0.5, 1e-9);

    /* cos(60 deg) = 0.5 */
    Calculator c7 = createCalculator(60.0, 0.0, "COS", 1);
    c7 = executeCalculation(c7, MODE_DEGREE);
    assertDoubleEqual("Cosine: cos(60 deg)", c7.result, 0.5, 1e-9);

    /* log10(100) = 2 */
    Calculator c8 = createCalculator(100.0, 0.0, "LOG10", 1);
    c8 = executeCalculation(c8, MODE_DEGREE);
    assertDoubleEqual("Logarithm: log10(100)", c8.result, 2.0, 1e-9);

    /* 10 / 0 = Error */
    Calculator c9 = createCalculator(10.0, 0.0, "DIV", 0);
    c9 = executeCalculation(c9, MODE_DEGREE);
    assertStatusEqual("Division: 10 / 0 (Error)", c9.status, CALC_ERR_DIV_ZERO);

    /* sqrt(-9) = Error */
    Calculator c10 = createCalculator(-9.0, 0.0, "SQRT", 1);
    c10 = executeCalculation(c10, MODE_DEGREE);
    assertStatusEqual("Square Root: sqrt(-9) (Error)", c10.status, CALC_ERR_NEG_SQRT);

    /* 2. Comprehensive Boundary & Error Testing */
    printf("\n--- [2] EXTENDED ARITHMETIC & SCIENTIFIC OPERATIONS ---\n");
    
    /* Subtraction */
    Calculator c11 = createCalculator(50.0, 15.0, "SUB", 0);
    c11 = executeCalculation(c11, MODE_DEGREE);
    assertDoubleEqual("Subtraction: 50 - 15", c11.result, 35.0, 1e-9);

    /* Modulus */
    Calculator c12 = createCalculator(29.0, 7.0, "MOD", 0);
    c12 = executeCalculation(c12, MODE_DEGREE);
    assertDoubleEqual("Modulus: 29 % 7", c12.result, 1.0, 1e-9);

    /* Modulus by Zero */
    Calculator c13 = createCalculator(25.0, 0.0, "MOD", 0);
    c13 = executeCalculation(c13, MODE_DEGREE);
    assertStatusEqual("Modulus by 0 (Error)", c13.status, CALC_ERR_MOD_ZERO);

    /* Square and Cube */
    Calculator c14 = createCalculator(12.0, 0.0, "SQUARE", 1);
    c14 = executeCalculation(c14, MODE_DEGREE);
    assertDoubleEqual("Square: 12^2", c14.result, 144.0, 1e-9);

    Calculator c15 = createCalculator(4.0, 0.0, "CUBE", 1);
    c15 = executeCalculation(c15, MODE_DEGREE);
    assertDoubleEqual("Cube: 4^3", c15.result, 64.0, 1e-9);

    /* Cube root */
    Calculator c16 = createCalculator(-27.0, 0.0, "CBRT", 1);
    c16 = executeCalculation(c16, MODE_DEGREE);
    assertDoubleEqual("Cube Root: cbrt(-27)", c16.result, -3.0, 1e-9);

    /* Reciprocal */
    Calculator c17 = createCalculator(4.0, 0.0, "RECIPROCAL", 1);
    c17 = executeCalculation(c17, MODE_DEGREE);
    assertDoubleEqual("Reciprocal: 1/4", c17.result, 0.25, 1e-9);

    Calculator c18 = createCalculator(0.0, 0.0, "RECIPROCAL", 1);
    c18 = executeCalculation(c18, MODE_DEGREE);
    assertStatusEqual("Reciprocal of 0 (Error)", c18.status, CALC_ERR_DIV_ZERO);

    /* Factorial 0! and boundary */
    Calculator c19 = createCalculator(0.0, 0.0, "FACTORIAL", 1);
    c19 = executeCalculation(c19, MODE_DEGREE);
    assertDoubleEqual("Factorial: 0!", c19.result, 1.0, 1e-9);

    Calculator c20 = createCalculator(-5.0, 0.0, "FACTORIAL", 1);
    c20 = executeCalculation(c20, MODE_DEGREE);
    assertStatusEqual("Factorial of -5 (Error)", c20.status, CALC_ERR_FACTORIAL_INVALID);

    Calculator c21 = createCalculator(25.0, 0.0, "FACTORIAL", 1);
    c21 = executeCalculation(c21, MODE_DEGREE);
    assertStatusEqual("Factorial > 20 overflow (Error)", c21.status, CALC_ERR_FACTORIAL_OVERFLOW);

    /* Absolute value */
    Calculator c22 = createCalculator(-15.75, 0.0, "ABS", 1);
    c22 = executeCalculation(c22, MODE_DEGREE);
    assertDoubleEqual("Absolute: |-15.75|", c22.result, 15.75, 1e-9);

    /* 3. Trigonometric and Radian/Degree conversions */
    printf("\n--- [3] TRIGONOMETRIC OPERATIONS (DEG & RAD) ---\n");

    /* tan(45 deg) = 1 */
    Calculator c23 = createCalculator(45.0, 0.0, "TAN", 1);
    c23 = executeCalculation(c23, MODE_DEGREE);
    assertDoubleEqual("Tangent: tan(45 deg)", c23.result, 1.0, 1e-9);

    /* tan(90 deg) = Error */
    Calculator c24 = createCalculator(90.0, 0.0, "TAN", 1);
    c24 = executeCalculation(c24, MODE_DEGREE);
    assertStatusEqual("Tangent: tan(90 deg) (Undefined)", c24.status, CALC_ERR_TAN_UNDEFINED);

    /* Inverse Sine */
    Calculator c25 = createCalculator(0.5, 0.0, "ASIN", 1);
    c25 = executeCalculation(c25, MODE_DEGREE);
    assertDoubleEqual("Inverse Sine: asin(0.5) in DEG", c25.result, 30.0, 1e-9);

    /* Inverse Sine domain error */
    Calculator c26 = createCalculator(1.5, 0.0, "ASIN", 1);
    c26 = executeCalculation(c26, MODE_DEGREE);
    assertStatusEqual("Inverse Sine: asin(1.5) (Domain Error)", c26.status, CALC_ERR_INVALID_TRIG_DOMAIN);

    /* Inverse Cosine */
    Calculator c27 = createCalculator(0.5, 0.0, "ACOS", 1);
    c27 = executeCalculation(c27, MODE_DEGREE);
    assertDoubleEqual("Inverse Cosine: acos(0.5) in DEG", c27.result, 60.0, 1e-9);

    /* Inverse Tangent */
    Calculator c28 = createCalculator(1.0, 0.0, "ATAN", 1);
    c28 = executeCalculation(c28, MODE_DEGREE);
    assertDoubleEqual("Inverse Tangent: atan(1) in DEG", c28.result, 45.0, 1e-9);

    /* 4. Logarithmic & Exponential */
    printf("\n--- [4] LOGARITHMIC & EXPONENTIAL FUNCTIONS ---\n");

    /* Natural Log ln(e) = 1 */
    Calculator c29 = createCalculator(exp(1.0), 0.0, "LN", 1);
    c29 = executeCalculation(c29, MODE_DEGREE);
    assertDoubleEqual("Natural Log: ln(e)", c29.result, 1.0, 1e-9);

    /* Log of zero */
    Calculator c30 = createCalculator(0.0, 0.0, "LN", 1);
    c30 = executeCalculation(c30, MODE_DEGREE);
    assertStatusEqual("Logarithm of 0 (Error)", c30.status, CALC_ERR_INVALID_LOG);

    /* Log of negative */
    Calculator c31 = createCalculator(-10.0, 0.0, "LOG10", 1);
    c31 = executeCalculation(c31, MODE_DEGREE);
    assertStatusEqual("Log10 of -10 (Error)", c31.status, CALC_ERR_INVALID_LOG);

    /* Exponential e^0 = 1 */
    Calculator c32 = createCalculator(0.0, 0.0, "EXP", 1);
    c32 = executeCalculation(c32, MODE_DEGREE);
    assertDoubleEqual("Exponential: e^0", c32.result, 1.0, 1e-9);

    /* 5. Additional Tools: Percentage, GCD, LCM */
    printf("\n--- [5] ADDITIONAL TOOLS: PERCENTAGE, GCD, LCM ---\n");

    Calculator c33 = createCalculator(15.0, 200.0, "PERCENT", 0);
    c33 = executeCalculation(c33, MODE_DEGREE);
    assertDoubleEqual("Percentage: 15% of 200", c33.result, 30.0, 1e-9);

    Calculator c34 = createCalculator(48.0, 18.0, "GCD", 0);
    c34 = executeCalculation(c34, MODE_DEGREE);
    assertDoubleEqual("GCD(48, 18)", c34.result, 6.0, 1e-9);

    Calculator c35 = createCalculator(12.0, 18.0, "LCM", 0);
    c35 = executeCalculation(c35, MODE_DEGREE);
    assertDoubleEqual("LCM(12, 18)", c35.result, 36.0, 1e-9);

    /* 6. History Management Buffer (20 records limit) */
    printf("\n--- [6] HISTORY BUFFER MANAGEMENT (MAX 20) ---\n");
    initHistory();
    for (int i = 1; i <= 25; i++) {
        Calculator hTest = createCalculator((double)i, (double)i, "ADD", 0);
        hTest = executeCalculation(hTest, MODE_DEGREE);
        addHistory(hTest);
    }
    
    totalTests++;
    if (getHistoryCount() == 20) {
        printf("  [PASS] History capped at MAX_HISTORY (20). Count = %d\n", getHistoryCount());
        passedTests++;
    } else {
        printf("  \033[1;31m[FAIL]\033[0m History count expected 20, got %d\n", getHistoryCount());
    }

    /* Verify oldest retained entry is calculation #6 (num1 = 6.0) */
    History oldest = getHistoryAt(0);
    totalTests++;
    if (oldest.num1 == 6.0) {
        printf("  [PASS] FIFO queue correctly purged oldest 5 records (First element is 6.0 + 6.0)\n");
        passedTests++;
    } else {
        printf("  \033[1;31m[FAIL]\033[0m First history element expected 6.0, got %.4g\n", oldest.num1);
    }

    clearHistory();
    totalTests++;
    if (getHistoryCount() == 0) {
        printf("  [PASS] History clear successfully reset count to 0.\n");
        passedTests++;
    } else {
        printf("  \033[1;31m[FAIL]\033[0m History clear failed, count is %d\n", getHistoryCount());
    }

    /* Summary */
    printf("\n=====================================================================\n");
    printf(" TEST RESULTS SUMMARY: %d / %d PASSED (%.1f%%)\n",
           passedTests, totalTests, (float)passedTests / totalTests * 100.0f);
    printf("=====================================================================\n\n");

    return (passedTests == totalTests) ? 0 : 1;
}
