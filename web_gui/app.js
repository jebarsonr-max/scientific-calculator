/**
 * SCIENTIFIC CALCULATOR - FRONTEND CONTROLLER
 * 
 * IMPORTANT: In accordance with project requirements, ALL mathematical
 * calculations are executed solely by the compiled C executable (calculator.exe).
 * This frontend script strictly acts as an input dispatcher and UI visualizer.
 * No mathematical computations are performed in JavaScript.
 */

// State
let currentInput = "0";
let pendingOperand = null;
let pendingOperation = null;
let angleMode = 1; // 1 = DEGREE, 2 = RADIAN
let shouldResetDisplay = false;
let historyRecords = [];

// DOM Elements
const resultDisplay = document.getElementById("resultDisplay");
const expressionDisplay = document.getElementById("expressionDisplay");
const currentModeLabel = document.getElementById("currentModeLabel");
const modeDegBtn = document.getElementById("modeDegBtn");
const modeRadBtn = document.getElementById("modeRadBtn");
const errorBanner = document.getElementById("errorBanner");
const errorMessage = document.getElementById("errorMessage");
const historyList = document.getElementById("historyList");
const historyPanel = document.getElementById("historyPanel");
const toggleHistoryBtn = document.getElementById("toggleHistoryBtn");
const closeHistoryBtn = document.getElementById("closeHistoryBtn");
const clearHistoryBtn = document.getElementById("clearHistoryBtn");
const clearAllBtn = document.getElementById("clearAllBtn");
const backspaceBtn = document.getElementById("backspaceBtn");
const negateBtn = document.getElementById("negateBtn");
const equalsBtn = document.getElementById("equalsBtn");

// Unary operations list (executed immediately on current number)
const UNARY_OPS = [
    "SIN", "COS", "TAN", "ASIN", "ACOS", "ATAN",
    "SQRT", "CBRT", "SQUARE", "CUBE", "POW_INV", "RECIPROCAL",
    "FACTORIAL", "ABS", "LN", "LOG10", "EXP"
];

// Binary operations list (requires two operands)
const BINARY_OPS = [
    "ADD", "SUB", "MUL", "DIV", "MOD", "POW", "PERCENT", "GCD", "LCM"
];

const OP_SYMBOLS = {
    ADD: "+",
    SUB: "−",
    MUL: "×",
    DIV: "÷",
    MOD: "mod",
    POW: "^",
    PERCENT: "% of",
    GCD: "GCD",
    LCM: "LCM"
};

// Initialize
function init() {
    setupEventListeners();
    updateDisplay();
}

function setupEventListeners() {
    // Mode toggles
    modeDegBtn.addEventListener("click", () => setAngleMode(1));
    modeRadBtn.addEventListener("click", () => setAngleMode(2));

    // Number keys
    document.querySelectorAll("[data-num]").forEach(btn => {
        btn.addEventListener("click", () => appendNumber(btn.dataset.num));
    });

    // Operation keys (Scientific & Basic)
    document.querySelectorAll("[data-op]").forEach(btn => {
        btn.addEventListener("click", () => handleOperation(btn.dataset.op));
    });

    // Controls
    clearAllBtn.addEventListener("click", clearAll);
    backspaceBtn.addEventListener("click", handleBackspace);
    negateBtn.addEventListener("click", handleNegate);
    equalsBtn.addEventListener("click", handleEquals);

    // History controls
    toggleHistoryBtn.addEventListener("click", toggleHistory);
    closeHistoryBtn.addEventListener("click", () => historyPanel.classList.add("collapsed"));
    clearHistoryBtn.addEventListener("click", clearHistory);

    // Keyboard support
    window.addEventListener("keydown", handleKeyboard);
}

function setAngleMode(mode) {
    angleMode = mode;
    if (mode === 1) {
        modeDegBtn.classList.add("active");
        modeRadBtn.classList.remove("active");
        currentModeLabel.textContent = "DEG";
    } else {
        modeDegBtn.classList.remove("active");
        modeRadBtn.classList.add("active");
        currentModeLabel.textContent = "RAD";
    }
}

function appendNumber(num) {
    hideError();
    if (shouldResetDisplay) {
        currentInput = (num === ".") ? "0." : num;
        shouldResetDisplay = false;
    } else {
        if (num === "." && currentInput.includes(".")) {
            return;
        }
        if (currentInput === "0" && num !== ".") {
            currentInput = num;
        } else {
            currentInput += num;
        }
    }
    updateDisplay();
}

function handleBackspace() {
    hideError();
    if (shouldResetDisplay) {
        currentInput = "0";
        shouldResetDisplay = false;
    } else {
        currentInput = currentInput.slice(0, -1);
        if (currentInput === "" || currentInput === "-") {
            currentInput = "0";
        }
    }
    updateDisplay();
}

function handleNegate() {
    hideError();
    if (currentInput !== "0") {
        if (currentInput.startsWith("-")) {
            currentInput = currentInput.slice(1);
        } else {
            currentInput = "-" + currentInput;
        }
        updateDisplay();
    }
}

function clearAll() {
    currentInput = "0";
    pendingOperand = null;
    pendingOperation = null;
    shouldResetDisplay = false;
    expressionDisplay.innerHTML = "&nbsp;";
    hideError();
    updateDisplay();
}

function handleOperation(op) {
    hideError();
    const num = parseFloat(currentInput);

    if (UNARY_OPS.includes(op)) {
        // Direct execution via C backend for unary operations
        expressionDisplay.textContent = `${op}(${currentInput})`;
        callCEngine(op, num, 0, angleMode);
    } else if (BINARY_OPS.includes(op)) {
        if (pendingOperation && !shouldResetDisplay) {
            // Consecutive binary operation: evaluate previous first with C engine
            callCEngine(pendingOperation, pendingOperand, num, angleMode, (res) => {
                pendingOperand = res;
                pendingOperation = op;
                shouldResetDisplay = true;
                expressionDisplay.textContent = `${res} ${OP_SYMBOLS[op] || op}`;
            });
        } else {
            pendingOperand = num;
            pendingOperation = op;
            shouldResetDisplay = true;
            expressionDisplay.textContent = `${num} ${OP_SYMBOLS[op] || op}`;
        }
    }
}

function handleEquals() {
    if (pendingOperation !== null && pendingOperand !== null) {
        const num2 = parseFloat(currentInput);
        expressionDisplay.textContent = `${pendingOperand} ${OP_SYMBOLS[pendingOperation] || pendingOperation} ${num2} =`;
        callCEngine(pendingOperation, pendingOperand, num2, angleMode, () => {
            pendingOperation = null;
            pendingOperand = null;
        });
    }
}

/**
 * Dispatches calculation request to C Calculation Engine backend.
 */
async function callCEngine(operation, num1, num2, mode, onSuccessCallback) {
    try {
        const response = await fetch("/api/calculate", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({
                operation: operation,
                num1: num1,
                num2: num2,
                angleMode: mode
            })
        });

        if (!response.ok) {
            throw new Error(`Server returned HTTP ${response.status}`);
        }

        const data = await response.json();

        if (data.status === 0) {
            // Success
            hideError();
            currentInput = String(data.result);
            shouldResetDisplay = true;
            updateDisplay();
            addHistoryRecord(data);
            if (onSuccessCallback) onSuccessCallback(data.result);
        } else {
            // Error from C validation
            showError(data.error || "Calculation error returned by C engine.");
            shouldResetDisplay = true;
            addHistoryRecord(data);
        }
    } catch (err) {
        showError(`C Engine Communication Error: ${err.message}`);
    }
}

function updateDisplay() {
    resultDisplay.textContent = currentInput;
}

function showError(msg) {
    errorMessage.textContent = msg;
    errorBanner.classList.remove("hidden");
}

function hideError() {
    errorBanner.classList.add("hidden");
}

function toggleHistory() {
    historyPanel.classList.toggle("collapsed");
}

function addHistoryRecord(record) {
    historyRecords.unshift(record);
    if (historyRecords.length > 20) {
        historyRecords.pop(); // Keep at most 20 records
    }
    renderHistory();
}

function renderHistory() {
    if (historyRecords.length === 0) {
        historyList.innerHTML = '<div class="empty-history">No calculations yet. Perform operations to record history.</div>';
        return;
    }

    historyList.innerHTML = historyRecords.map(rec => {
        let expr = "";
        if (rec.is_unary) {
            expr = `${rec.operation}(${rec.num1})`;
        } else {
            expr = `${rec.num1} ${OP_SYMBOLS[rec.operation] || rec.operation} ${rec.num2}`;
        }

        if (rec.status !== 0) {
            return `
                <div class="history-item">
                    <div class="history-item-expr">${expr}</div>
                    <div class="history-item-error">${rec.error || "Error"}</div>
                </div>
            `;
        } else {
            return `
                <div class="history-item" onclick="reuseHistory(${rec.result})">
                    <div class="history-item-expr">${expr} =</div>
                    <div class="history-item-res">${rec.result}</div>
                </div>
            `;
        }
    }).join("");
}

window.reuseHistory = function(val) {
    currentInput = String(val);
    shouldResetDisplay = true;
    updateDisplay();
};

function clearHistory() {
    historyRecords = [];
    renderHistory();
}

function handleKeyboard(e) {
    if (e.key >= "0" && e.key <= "9") {
        appendNumber(e.key);
    } else if (e.key === ".") {
        appendNumber(".");
    } else if (e.key === "+") {
        handleOperation("ADD");
    } else if (e.key === "-") {
        handleOperation("SUB");
    } else if (e.key === "*") {
        handleOperation("MUL");
    } else if (e.key === "/") {
        e.preventDefault();
        handleOperation("DIV");
    } else if (e.key === "Enter" || e.key === "=") {
        e.preventDefault();
        handleEquals();
    } else if (e.key === "Backspace") {
        handleBackspace();
    } else if (e.key === "Escape") {
        clearAll();
    }
}

document.addEventListener("DOMContentLoaded", init);

