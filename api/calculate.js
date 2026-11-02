const { execFile, execSync } = require('child_process');
const path = require('path');
const fs = require('fs');

/**
 * Vercel Serverless Function: /api/calculate
 * Strictly executes the compiled C binary (calculator) to perform all mathematical computations.
 */
module.exports = async (req, res) => {
    // Enable CORS
    res.setHeader('Access-Control-Allow-Credentials', 'true');
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.setHeader('Access-Control-Allow-Methods', 'GET,OPTIONS,PATCH,DELETE,POST,PUT');
    res.setHeader(
        'Access-Control-Allow-Headers',
        'X-CSRF-Token, X-Requested-With, Accept, Accept-Version, Content-Length, Content-MD5, Content-Type, Date, X-Api-Version'
    );

    if (req.method === 'OPTIONS') {
        return res.status(200).end();
    }

    if (req.method !== 'POST') {
        return res.status(405).json({ status: 10, error: 'Method Not Allowed. Use POST.' });
    }

    let body = req.body;
    if (typeof body === 'string') {
        try {
            body = JSON.parse(body);
        } catch (e) {
            body = {};
        }
    }

    const operation = (body && body.operation) ? String(body.operation) : 'ADD';
    const num1 = (body && body.num1 !== undefined) ? String(body.num1) : '0';
    const num2 = (body && body.num2 !== undefined) ? String(body.num2) : '0';
    const angleMode = (body && body.angleMode !== undefined) ? String(body.angleMode) : '1';

    // Locate or prepare the C executable
    const isWindows = process.platform === 'win32';
    const rootDir = path.resolve(__dirname, '..');
    const winBin = path.join(rootDir, 'calculator.exe');
    const bundledLinuxBin = path.join(__dirname, 'calculator_linux');
    const tmpBin = path.join('/tmp', 'calculator_linux');

    let binaryToRun = null;

    if (isWindows && fs.existsSync(winBin)) {
        binaryToRun = winBin;
    } else {
        // Linux / Vercel Serverless environment
        if (fs.existsSync(bundledLinuxBin)) {
            try {
                // Copy to /tmp to ensure execution permissions inside AWS Lambda
                if (!fs.existsSync(tmpBin)) {
                    fs.copyFileSync(bundledLinuxBin, tmpBin);
                }
                fs.chmodSync(tmpBin, 0o755);
                binaryToRun = tmpBin;
            } catch (err) {
                binaryToRun = bundledLinuxBin;
            }
        } else if (fs.existsSync(tmpBin)) {
            binaryToRun = tmpBin;
        } else {
            // Attempt on-the-fly compilation if source files are available
            try {
                const srcMain = path.join(rootDir, 'main.c');
                const srcCalc = path.join(rootDir, 'calculator.c');
                const srcHist = path.join(rootDir, 'history.c');

                if (fs.existsSync(srcMain) && fs.existsSync(srcCalc)) {
                    execSync(`gcc -O2 "${srcMain}" "${srcCalc}" "${srcHist}" -o "${tmpBin}" -lm`, { timeout: 10000 });
                    fs.chmodSync(tmpBin, 0o755);
                    binaryToRun = tmpBin;
                }
            } catch (compileErr) {
                // Compilation error or gcc missing
            }
        }
    }

    if (!binaryToRun || !fs.existsSync(binaryToRun)) {
        return res.status(500).json({
            status: 10,
            error: 'Compiled C executable not found on server. Please check Vercel build step.'
        });
    }

    // Execute compiled C binary with --json arguments
    execFile(binaryToRun, ['--json', operation, num1, num2, angleMode], { timeout: 4000 }, (error, stdout, stderr) => {
        const output = (stdout || '').trim();

        if (!output) {
            return res.status(500).json({
                status: 10,
                error: `C engine execution failed: ${stderr || (error && error.message) || 'Empty output'}`
            });
        }

        try {
            const parsed = JSON.parse(output);
            return res.status(200).json(parsed);
        } catch (jsonErr) {
            return res.status(200).send(output);
        }
    });
};
