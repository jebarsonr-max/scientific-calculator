# Scientific Calculator for Students (C Language)

A modular, college-level Scientific Calculator developed strictly in **C (C99)** demonstrating the practical use of **Structures (`struct`)**, **User-Defined Functions**, **Pointers**, **Header Modularization**, **Switch-Case Statements**, and **Math Engine Algorithms** (`math.h`).

---

## 📑 Table of Contents
1. [Project Overview](#project-overview)
2. [Key Objectives](#key-objectives)
3. [Software & System Requirements](#software--system-requirements)
4. [Concepts of C Language Implemented](#concepts-of-c-language-implemented)
5. [Structure Design & Explanation](#structure-design--explanation)
6. [Functions Architecture & Explanation](#functions-architecture--explanation)
7. [Module Descriptions & Source Tree](#module-descriptions--source-tree)
8. [Features List](#features-list)
9. [Error Handling & Boundary Protection](#error-handling--boundary-protection)
10. [How to Compile and Run](#how-to-compile-and-run)
11. [Automated Verification & Test Cases](#automated-verification--test-cases)
12. [Sample CLI & GUI Output](#sample-cli--gui-output)
13. [College Evaluation & Viva Guide](#college-evaluation--viva-guide)
14. [Conclusion](#conclusion)

---

## 1. Project Overview

This project implements a fully functional **Scientific Calculator for Students** in genuine C. In college programming curricula (such as First-Year B.Tech / B.E. / BCA / B.Sc Computer Science courses), students often struggle to understand how real-world applications structure data and modularize calculations using C structures and functions.

This project bridges that gap by:
- Storing calculation operands, selected operations, status flags, and results inside user-defined C structures.
- Passing structures to functions and returning structured calculation states.
- Maintaining an in-memory session history of the previous **20 calculations** using an array of structures.
- Providing a **Dual Interface**:
  1. **Terminal Console Interface**: ANSI-styled interactive dark-theme CLI with full input validation and ASCII headers.
  2. **Modern Web GUI**: An interactive dark-theme browser interface powered by `app_server.py` where **every single mathematical calculation is executed solely by the compiled C executable (`calculator.exe`)**—strictly adhering to zero-JavaScript calculation rules.

---

## 2. Key Objectives

- **Practical Data Abstraction**: Demonstrate how `struct Calculator` and `struct History` bundle related data into single logical units.
- **Modular Programming**: Break down monolithic code into independent modules (`calculator.h`, `calculator.c`, `history.c`, `main.c`).
- **Structure-Centric Execution**: Pass structures by value and reference across function boundaries.
- **Robust Error Handling**: Handle mathematical anomalies (division by zero, negative square roots, undefined trigonometric values, factorial domain/overflow) without crashing.
- **Academic Readiness**: Clean, well-commented code suitable for college laboratory examinations, practicals, and viva assessments.

---

## 3. Software & System Requirements

| Component | Minimum Requirement | Recommended |
|---|---|---|
| **Operating System** | Windows 10/11, Linux (Ubuntu, Debian, Fedora), or macOS | Windows 10/11 |
| **C Compiler** | GCC (MinGW / MSYS2 / Clang / MSVC) supporting C99 | GCC 9.0 or higher |
| **C Standard Library** | C Standard Library (`stdio.h`, `stdlib.h`, `math.h`, `string.h`, `stdbool.h`) | Standard |
| **Linker Flags** | `-lm` (for math library linking on POSIX/MinGW) | `-lm` |
| **Optional for GUI** | Python 3.7+ (standard library only, zero external packages) | Python 3.10+ |

---

## 4. Concepts of C Language Implemented

1. **Structures (`struct`)**:
   - Encapsulation of disparate data types (`double`, `char[]`, `int`) into cohesive structures (`Calculator`, `History`).
2. **User-Defined Functions**:
   - Specialized single-responsibility functions for each operation category (basic arithmetic, scientific, trigonometric, logarithms).
3. **Function Declarations and Definitions**:
   - Prototypes declared in `calculator.h` and implemented across `calculator.c` and `history.c`.
4. **Passing Structures to Functions**:
   - Passing `Calculator` instances by value to calculation dispatchers (`executeCalculation()`, `addHistory()`, `displayResult()`).
5. **Passing Pointers for Status/Error Communication**:
   - Functions like `divide(a, b, &status)` or `squareRoot(a, &status)` return status codes via pointers.
6. **Switch-Case Statements**:
   - Clean menu navigation and operational dispatch based on user integer selection.
7. **Control Flow & Loops**:
   - `while(1)` main application loop, `for` loops for factorial calculation and FIFO history array shifting.
8. **Mathematical Library (`math.h`)**:
   - Functions utilized: `sin()`, `cos()`, `tan()`, `asin()`, `acos()`, `atan()`, `sqrt()`, `cbrt()`, `pow()`, `log()`, `log10()`, `exp()`, `fabs()`, `floor()`, `round()`.
9. **Safe Input Validation**:
   - Use of `fgets()` with `strtod()` parsing to avoid buffer overflows and eliminate infinite loops caused by non-numeric inputs.

---

## 5. Structure Design & Explanation

Two core structures form the backbone of this application (defined in `calculator.h`):

### 1. `struct Calculator`
Stores current calculation inputs, operator, result, and status.

```c
typedef struct {
    double num1;        /* First operand */
    double num2;        /* Second operand (0.0 for unary operations) */
    double result;      /* Calculated mathematical result */
    char operation[20]; /* Operation token (e.g., "ADD", "SQRT", "SIN") */
    int is_unary;       /* 1 = single operand, 0 = two operands */
    int status;         /* 0 = CALC_SUCCESS, non-zero = error code */
    char error_msg[64]; /* Descriptive error message if status != 0 */
} Calculator;
```

### 2. `struct History`
Stores individual records of past calculations.

```c
typedef struct {
    double num1;        /* First operand */
    double num2;        /* Second operand */
    char operation[20]; /* Operation identifier */
    double result;      /* Calculated result */
    int is_unary;       /* Unary flag */
    int status;         /* Status code */
    char error_msg[64]; /* Error message if calculation failed */
} History;
```

### How the History Array Works
An internal array of 20 `History` structures is maintained in `history.c`:
```c
static History historyRecords[MAX_HISTORY];
static int historyCount = 0;
```
When new calculations are added:
- If fewer than 20 records exist, the record is inserted at `historyCount++`.
- When 20 records are reached, a FIFO queue shift discards the oldest record (index 0) and appends the newest calculation at index 19.

---

## 6. Functions Architecture & Explanation

### Where Structures and Functions are Implemented

| Function Name | Location | Accepts | Returns | Description |
|---|---|---|---|---|
| `createCalculator()` | `calculator.c` | `(double, double, const char*, int)` | `Calculator` (struct) | Factory function that initializes a `Calculator` structure variable. |
| `executeCalculation()` | `calculator.c` | `(Calculator, int angleMode)` | `Calculator` (struct) | Main calculation engine: runs math functions, validates results, returns updated struct. |
| `displayResult()` | `calculator.c` | `(Calculator, int angleMode)` | `void` | Formats and prints the structure's inputs, operation, and result/error. |
| `addHistory()` | `history.c` | `(Calculator)` | `void` | Copies `Calculator` fields into the `historyRecords[]` array of structures. |
| `displayHistory()` | `history.c` | `void` | `void` | Iterates through history structure records and displays a formatted table. |
| `clearHistory()` | `history.c` | `void` | `void` | Resets the history buffer count and wipes records. |
| `add()`, `subtract()`, `multiply()` | `calculator.c` | `(double, double)` | `double` | Basic arithmetic calculations. |
| `divide()` | `calculator.c` | `(double, double, int *status)` | `double` | Division with zero-division validation via pointer. |
| `modulus()` | `calculator.c` | `(long long, long long, int *status)` | `long long` | Integer modulo with zero-divisor check. |
| `square()`, `cube()` | `calculator.c` | `(double)` | `double` | Powers of 2 and 3. |
| `squareRoot()` | `calculator.c` | `(double, int *status)` | `double` | Computes $\sqrt{x}$, flags negative input error. |
| `cubeRoot()` | `calculator.c` | `(double)` | `double` | Computes $\sqrt[3]{x}$ for all real numbers. |
| `power()` | `calculator.c` | `(double, double, int *status)` | `double` | Computes $x^y$ with domain checks. |
| `reciprocal()` | `calculator.c` | `(double, int *status)` | `double` | Computes $1/x$, guards against division by zero. |
| `factorial()` | `calculator.c` | `(int, int *status)` | `double` | Computes $n!$ for $0 \le n \le 20$, guards against overflow. |
| `sine()`, `cosine()`, `tangent()` | `calculator.c` | `(double, int mode)` | `double` | Trigonometry in DEG/RAD mode with clean floating-point rounding. |
| `arcsine()`, `arccosine()`, `arctangent()` | `calculator.c` | `(double, int mode, int *status)` | `double` | Inverse trig with domain validation $[-1.0, 1.0]$. |
| `naturalLog()`, `logBase10()` | `calculator.c` | `(double, int *status)` | `double` | Computes $\ln(x)$ and $\log_{10}(x)$ for $x > 0$. |
| `exponential()` | `calculator.c` | `(double)` | `double` | Computes $e^x$. |
| `gcd()`, `lcm()` | `calculator.c` | `(long long, long long)` | `long long` | Euclidean GCD and LCM calculations. |

---

## 7. Module Descriptions & Source Tree

```text
SCIENTIFIC CALCULATOR/
│
├── calculator.h         # Header file: Structure definitions, error codes, function prototypes
├── calculator.c         # Calculation engine: Math functions and structure-based dispatcher
├── history.c            # History module: 20-slot history buffer and table rendering
├── main.c               # Application entry point: CLI menu loop, input handling, --json CLI mode
│
├── test_suite.c         # Automated unit test suite verifying all 39 test conditions
│
├── web_gui/             # Modern Dark-Themed GUI Frontend (zero calculation in JS)
│   ├── index.html       # HTML5 structure with responsive scientific keypad
│   ├── style.css        # Professional dark-theme stylesheet
│   └── app.js           # Event listener & IPC dispatcher to calculator.exe
│
├── app_server.py        # Python HTTP bridge connecting web GUI to compiled C executable
├── build.bat            # Windows 1-click build script
├── run_cli.bat          # Windows 1-click launcher for Terminal CLI
├── run_gui.bat          # Windows 1-click launcher for Web GUI
├── Makefile             # Multi-platform build file for GCC/Make
└── README.md            # Comprehensive project documentation
```

---

## 8. Features List

### 1. Basic Arithmetic
- Addition ($x + y$)
- Subtraction ($x - y$)
- Multiplication ($x \times y$)
- Division ($x / y$)
- Integer Modulus ($x \pmod y$)

### 2. Advanced Scientific Operations
- Square ($x^2$) and Cube ($x^3$)
- Square Root ($\sqrt{x}$) and Cube Root ($\sqrt[3]{x}$)
- Power ($x^y$)
- Reciprocal ($1/x$)
- Factorial ($n!$)
- Absolute Value ($|x|$)

### 3. Trigonometric Functions
- Sine ($\sin x$)
- Cosine ($\cos x$)
- Tangent ($\tan x$)
- Inverse Sine ($\arcsin x$)
- Inverse Cosine ($\arccos x$)
- Inverse Tangent ($\arctan x$)
- **Degree / Radian Toggle**: Users can switch angle mode anytime. Angles in degrees are cleanly converted: $\text{rad} = \text{deg} \times \frac{\pi}{180}$.

### 4. Logarithmic & Exponential
- Natural Logarithm ($\ln x$)
- Common Logarithm Base 10 ($\log_{10} x$)
- Exponential Function ($e^x$)

### 5. Number Theory & Additional Tools
- Percentage Calculation ($x\%$ of $y$)
- Greatest Common Divisor ($\gcd(a, b)$)
- Least Common Multiple ($\text{lcm}(a, b)$)

### 6. History Management
- Tracks the previous **20 calculations** in memory.
- Displays operation name, operands, result, and status.
- "Clear History" option to reset the record buffer.

---

## 9. Error Handling & Boundary Protection

The calculator never crashes. Input validation and error protection include:

| Potential Error | Validation Condition | Calculator Action |
|---|---|---|
| **Division by Zero** | $|b| < 10^{-15}$ | Sets `CALC_ERR_DIV_ZERO`, displays *"Error: Division by zero is undefined."* |
| **Negative Square Root** | $x < 0.0$ | Sets `CALC_ERR_NEG_SQRT`, displays *"Error: Square root of a negative number is undefined in real numbers."* |
| **Non-positive Logarithm** | $x \le 0.0$ | Sets `CALC_ERR_INVALID_LOG`, displays *"Error: Logarithm is only defined for strictly positive numbers (> 0)."* |
| **Inverse Trig Domain Error** | $x < -1.0$ or $x > 1.0$ | Sets `CALC_ERR_INVALID_TRIG_DOMAIN`, displays *"Error: Input value for inverse trigonometric function must be between -1 and 1."* |
| **Factorial of Negative** | $n < 0$ | Sets `CALC_ERR_FACTORIAL_INVALID`, displays *"Error: Factorial is only defined for non-negative integers."* |
| **Factorial Overflow** | $n > 20$ | Sets `CALC_ERR_FACTORIAL_OVERFLOW`, prevents 64-bit integer overflow. |
| **Tangent Undefined Angles** | $\theta = 90^\circ, 270^\circ$ | Sets `CALC_ERR_TAN_UNDEFINED`, detects when $\cos(\theta) = 0$. |
| **Modulus by Zero** | $y = 0$ | Sets `CALC_ERR_MOD_ZERO`, displays *"Error: Modulo by zero is undefined."* |
| **Invalid Numeric Input** | Strings, letters, symbols | `fgets` + `strtod` detects parsing failure, clears buffer, prompts again without looping. |

---

## 10. How to Compile and Run

### Option A: Using the Automated Batch Scripts (Windows)
1. Double click **`build.bat`** to compile the C programs.
2. Double click **`run_cli.bat`** to launch the interactive terminal calculator.
3. Double click **`run_gui.bat`** to launch the modern dark-theme Web GUI.

### Option B: Manual Command-Line Compilation (GCC)
Open Command Prompt or Terminal in the project directory:

```bash
# 1. Compile the main application
gcc -Wall -Wextra -std=c99 main.c calculator.c history.c -o calculator.exe -lm

# 2. Run the interactive CLI calculator
./calculator.exe

# 3. (Optional) Run the automated unit test suite
gcc -Wall -Wextra -std=c99 test_suite.c calculator.c history.c -o test_suite.exe -lm
./test_suite.exe
```

### Option C: Launching the Web GUI (Connected to C Engine)
To launch the modern dark-theme web interface:
```bash
python app_server.py
```
This starts the local bridge server and opens `http://localhost:5000` in your browser. All button clicks invoke `calculator.exe --json` under the hood!

### Option D: Deploying to Vercel (1-Click Cloud Deployment)
This repository is configured for automated Vercel deployment:
1. Go to [Vercel Dashboard](https://vercel.com/new).
2. Click **"Import Project"** and select your GitHub repository (`jebarsonr-max/scientific-calculator`).
3. Vercel automatically detects `vercel.json` and `package.json`.
4. Click **Deploy**. Vercel compiles the C engine on Linux (`gcc main.c calculator.c history.c -o api/calculator_linux -lm`) and serves both the static UI and the serverless C calculation endpoint (`/api/calculate`)!

---

## 11. Automated Verification & Test Cases

The test suite in `test_suite.c` validates every required operation and boundary case against expected outputs:

| Test Case | Expression | Expected Output | Status |
|---|---|---|---|
| **Addition** | $10 + 20$ | `30` | **PASS** |
| **Multiplication** | $5 \times 4$ | `20` | **PASS** |
| **Square Root** | $\sqrt{64}$ | `8` | **PASS** |
| **Power** | $2^5$ | `32` | **PASS** |
| **Factorial** | $5!$ | `120` | **PASS** |
| **Sine** | $\sin(30^\circ)$ | `0.5` | **PASS** |
| **Cosine** | $\cos(60^\circ)$ | `0.5` | **PASS** |
| **Logarithm** | $\log_{10}(100)$ | `2` | **PASS** |
| **Division by Zero** | $10 / 0$ | `Error (Status 1)` | **PASS** |
| **Negative Square Root** | $\sqrt{-9}$ | `Error (Status 2)` | **PASS** |
| **Subtraction** | $50 - 15$ | `35` | **PASS** |
| **Modulus** | $29 \pmod 7$ | `1` | **PASS** |
| **Modulus by Zero** | $25 \pmod 0$ | `Error (Status 8)` | **PASS** |
| **Tangent** | $\tan(45^\circ)$ | `1.0` | **PASS** |
| **Tangent at 90°** | $\tan(90^\circ)$ | `Error (Status 7)` | **PASS** |
| **Inverse Sine** | $\arcsin(0.5)$ in DEG | `30°` | **PASS** |
| **Inverse Sine Domain** | $\arcsin(1.5)$ | `Error (Status 4)` | **PASS** |
| **Natural Log** | $\ln(e)$ | `1.0` | **PASS** |
| **Logarithm of 0** | $\ln(0)$ | `Error (Status 3)` | **PASS** |
| **Percentage** | $15\%$ of $200$ | `30` | **PASS** |
| **GCD** | $\gcd(48, 18)$ | `6` | **PASS** |
| **LCM** | $\text{lcm}(12, 18)$ | `36` | **PASS** |
| **History Buffer FIFO** | Add 25 records | Capped at 20, oldest purged | **PASS** |
| **History Clear** | Clear history | Buffer resets to 0 | **PASS** |

**Test Result Summary: 39 / 39 Tests Passed (100.0%)**

---

## 12. Sample CLI & GUI Output

### Terminal CLI Interface:
```text
=====================================================================
   ____   ____ ___ _____ _   _ _____ ___ _____ ___ ____   ____ _    _
  / ___| / ___|_ _| ____| \ | |_   _|_ _|  ___|_ _/ ___| / ___| |  | |
  \___ \| |    | ||  _| |  \| | | |  | || |_   | | |    | |   | |  | |
   ___) | |___ | || |___| |\  | | |  | ||  _|  | | |___ | |___| |__| |
  |____/ \____|___|_____|_| \_| |_| |___|_|   |___\____| \____|_____|
                 SCIENTIFIC CALCULATOR FOR STUDENTS                  
           Developed with C Structures, Functions & Math.h           
=====================================================================

-------------------------- [ MAIN MENU ] ---------------------------
 Current Angle Mode: [DEGREES (DEG)]

 [1] Basic Arithmetic Operations
   1. Addition (x + y)                 2. Subtraction (x - y)
   3. Multiplication (x * y)           4. Division (x / y)
   5. Modulus for Integers (x % y)

 [2] Advanced Scientific Operations
   6. Square (x^2)                     7. Cube (x^3)
   8. Square Root (vx)                 9. Cube Root (3vx)
  10. Power (x^y)                     11. Reciprocal (1/x)
  12. Factorial (n!)                  13. Absolute Value (|x|)

 [3] Trigonometric Functions
  14. Sine: sin(x)                    15. Cosine: cos(x)
  16. Tangent: tan(x)                 17. Inverse Sine: asin(x)
  18. Inverse Cosine: acos(x)         19. Inverse Tangent: atan(x)

 [4] Logarithmic & Exponential
  20. Natural Logarithm: ln(x)        21. Logarithm Base 10: log10(x)
  22. Exponential Function: e^x

 [5] Additional Tools & History
  23. Percentage Calculator (x% of y) 24. Greatest Common Divisor (GCD)
  25. Least Common Multiple (LCM)     26. Toggle Angle Mode (DEG/RAD)
  27. View Calculation History        28. Clear Calculation History

   0. Exit Calculator
---------------------------------------------------------------------
 Enter your choice (0-28): 14

--- Sine: sin(x) in degrees ---
 Enter angle (x): 30

+-----------------------------------------------------------+
|                    CALCULATION RESULT                     |
+-----------------------------------------------------------+
| Status : SUCCESS
| Expression: SIN(30.0000 deg)
| Result     : 0.5 (Standard: 0.500000)
+-----------------------------------------------------------+
```

### Calculation History Display:
```text
+===================================================================================+
|                         CALCULATION HISTORY (LAST 20)                             |
+===================================================================================+
| #  | Operation   | Operand 1      | Operand 2      | Result / Status              |
+----+-------------+----------------+----------------+------------------------------+
| 1  | ADD         | 10             | 20             | 30                           |
| 2  | MUL         | 5              | 4              | 20                           |
| 3  | SQRT        | 64             | N/A            | 8                            |
| 4  | POW         | 2              | 5              | 32                           |
| 5  | FACTORIAL   | 5              | N/A            | 120                          |
| 6  | SIN         | 30             | N/A            | 0.5                          |
| 7  | DIV         | 10             | 0              | ERROR (1)                    |
+----+-------------+----------------+----------------+------------------------------+
```

---

## 13. College Evaluation & Viva Guide

When presenting this project to a professor or lab examiner, use the following points:

### Q1: Why did you use structures instead of regular variables?
> **Answer:** *"A calculator operation involves multiple interrelated attributes: operand 1, operand 2, the operation name, the computed result, and an error status. Using `struct Calculator` bundles all these related attributes into a single user-defined datatype. This avoids passing 5 separate variables to functions and prevents variable mismatch."*

### Q2: How does the calculation history work?
> **Answer:** *"History is implemented as an array of structures `History historyRecords[20]`. When a new calculation occurs, `addHistory(Calculator calc)` copies the structure fields into the array. If the number of calculations exceeds 20, a loop shifts the array elements left by one (FIFO queue behavior), maintaining the most recent 20 calculations."*

### Q3: How is modular programming achieved?
> **Answer:** *"The project is divided into 4 modular files: `calculator.h` acts as the interface containing structure definitions and function prototypes with header guards (`#ifndef CALCULATOR_H`). `calculator.c` contains pure mathematical implementations. `history.c` manages storage. `main.c` orchestrates user interaction and CLI execution."*

### Q4: How are degrees converted to radians for `math.h`?
> **Answer:** *"C's standard library trigonometric functions in `<math.h>` expect angles in radians. When the user selects DEGREE mode, our function `degreesToRadians()` multiplies the input by $(\pi / 180)$ before invoking `sin()`, `cos()`, or `tan()`."*

---

## 14. Conclusion

This Scientific Calculator project demonstrates mastery of foundational and intermediate C programming techniques. By combining **Structures**, **Modular Functions**, **Pointers**, **Robust Error Handling**, and **Interactive UI Design**, it provides an exemplary model for college-level software engineering in C.

