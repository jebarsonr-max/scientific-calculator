#include "calculator.h"

static History historyRecords[MAX_HISTORY];
static int historyCount = 0;

/**
 * Initializes the history buffer.
 */
void initHistory(void) {
    historyCount = 0;
    for (int i = 0; i < MAX_HISTORY; i++) {
        historyRecords[i].num1 = 0.0;
        historyRecords[i].num2 = 0.0;
        historyRecords[i].result = 0.0;
        historyRecords[i].operation[0] = '\0';
        historyRecords[i].is_unary = 0;
        historyRecords[i].status = CALC_SUCCESS;
        historyRecords[i].error_msg[0] = '\0';
    }
}

/**
 * Adds a calculation record into the history array.
 * If history exceeds MAX_HISTORY (20), shifts oldest entries out (FIFO).
 */
void addHistory(Calculator calc) {
    if (historyCount < MAX_HISTORY) {
        /* Add at next available slot */
        historyRecords[historyCount].num1 = calc.num1;
        historyRecords[historyCount].num2 = calc.num2;
        historyRecords[historyCount].result = calc.result;
        historyRecords[historyCount].is_unary = calc.is_unary;
        historyRecords[historyCount].status = calc.status;
        strncpy(historyRecords[historyCount].operation, calc.operation, sizeof(historyRecords[historyCount].operation) - 1);
        historyRecords[historyCount].operation[sizeof(historyRecords[historyCount].operation) - 1] = '\0';
        strncpy(historyRecords[historyCount].error_msg, calc.error_msg, sizeof(historyRecords[historyCount].error_msg) - 1);
        historyRecords[historyCount].error_msg[sizeof(historyRecords[historyCount].error_msg) - 1] = '\0';
        historyCount++;
    } else {
        /* Shift array left by 1 to discard the oldest calculation */
        for (int i = 0; i < MAX_HISTORY - 1; i++) {
            historyRecords[i] = historyRecords[i + 1];
        }
        /* Insert the new calculation at the end (index 19) */
        historyRecords[MAX_HISTORY - 1].num1 = calc.num1;
        historyRecords[MAX_HISTORY - 1].num2 = calc.num2;
        historyRecords[MAX_HISTORY - 1].result = calc.result;
        historyRecords[MAX_HISTORY - 1].is_unary = calc.is_unary;
        historyRecords[MAX_HISTORY - 1].status = calc.status;
        strncpy(historyRecords[MAX_HISTORY - 1].operation, calc.operation, sizeof(historyRecords[MAX_HISTORY - 1].operation) - 1);
        historyRecords[MAX_HISTORY - 1].operation[sizeof(historyRecords[MAX_HISTORY - 1].operation) - 1] = '\0';
        strncpy(historyRecords[MAX_HISTORY - 1].error_msg, calc.error_msg, sizeof(historyRecords[MAX_HISTORY - 1].error_msg) - 1);
        historyRecords[MAX_HISTORY - 1].error_msg[sizeof(historyRecords[MAX_HISTORY - 1].error_msg) - 1] = '\0';
    }
}

/**
 * Displays the calculation history table.
 */
void displayHistory(void) {
    printf("\n+===================================================================================+\n");
    printf("|                         CALCULATION HISTORY (LAST %d)                              |\n", MAX_HISTORY);
    printf("+===================================================================================+\n");

    if (historyCount == 0) {
        printf("| No calculations recorded yet in this session.                                    |\n");
        printf("+-----------------------------------------------------------------------------------+\n");
        return;
    }

    printf("| #  | Operation   | Operand 1      | Operand 2      | Result / Status              |\n");
    printf("+----+-------------+----------------+----------------+------------------------------+\n");

    for (int i = 0; i < historyCount; i++) {
        History h = historyRecords[i];
        char op1Str[20];
        char op2Str[20];
        char resStr[32];

        snprintf(op1Str, sizeof(op1Str), "%.4g", h.num1);
        if (h.is_unary) {
            snprintf(op2Str, sizeof(op2Str), "N/A");
        } else {
            snprintf(op2Str, sizeof(op2Str), "%.4g", h.num2);
        }

        if (h.status != CALC_SUCCESS) {
            snprintf(resStr, sizeof(resStr), "ERROR (%d)", h.status);
        } else {
            snprintf(resStr, sizeof(resStr), "%.6g", h.result);
        }

        printf("| %-2d | %-11s | %-14s | %-14s | %-28s |\n",
               i + 1, h.operation, op1Str, op2Str, resStr);
    }
    printf("+----+-------------+----------------+----------------+------------------------------+\n");
}

/**
 * Clears the calculation history.
 */
void clearHistory(void) {
    initHistory();
    printf("\n\033[1;32m[SUCCESS] Calculation history has been cleared successfully.\033[0m\n");
}

int getHistoryCount(void) {
    return historyCount;
}

History getHistoryAt(int index) {
    if (index >= 0 && index < historyCount) {
        return historyRecords[index];
    }
    History empty;
    memset(&empty, 0, sizeof(empty));
    return empty;
}

