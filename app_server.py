#!/usr/bin/env python3
"""
Scientific Calculator - GUI Web Bridge Server
Connects the modern HTML/CSS/JS frontend interface to the compiled C engine (calculator.exe).
All calculations are strictly executed by calculator.exe.
"""

import sys
import os
import json
import subprocess
import webbrowser
from http.server import HTTPServer, SimpleHTTPRequestHandler

PORT = 5000
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
WEB_DIR = os.path.join(BASE_DIR, "web_gui")

# Resolve calculator executable
CALC_EXE = os.path.join(BASE_DIR, "calculator.exe")
if not os.path.isfile(CALC_EXE):
    CALC_EXE = os.path.join(BASE_DIR, "calculator")


class CalculatorHandler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=WEB_DIR, **kwargs)

    def do_POST(self):
        if self.path == "/api/calculate":
            content_length = int(self.headers.get("Content-Length", 0))
            post_data = self.rfile.read(content_length)

            try:
                req = json.loads(post_data.decode("utf-8"))
                operation = req.get("operation", "ADD")
                num1 = str(req.get("num1", 0))
                num2 = str(req.get("num2", 0))
                mode = str(req.get("angleMode", 1))

                # Execute compiled C program with --json arguments
                cmd = [CALC_EXE, "--json", operation, num1, num2, mode]
                proc = subprocess.run(cmd, capture_output=True, text=True, timeout=5)
                output = proc.stdout.strip()

                if not output:
                    err_resp = {"status": 10, "error": f"C engine error: {proc.stderr}"}
                    response_json = json.dumps(err_resp)
                else:
                    response_json = output

                self.send_response(200)
                self.send_header("Content-Type", "application/json")
                self.send_header("Access-Control-Allow-Origin", "*")
                self.end_headers()
                self.wfile.write(response_json.encode("utf-8"))

            except Exception as e:
                self.send_response(500)
                self.send_header("Content-Type", "application/json")
                self.end_headers()
                err_data = json.dumps({"status": 10, "error": f"Server exception: {str(e)}"})
                self.wfile.write(err_data.encode("utf-8"))
        else:
            self.send_error(404, "Endpoint Not Found")

    def end_headers(self):
        self.send_header("Cache-Control", "no-cache, no-store, must-revalidate")
        super().end_headers()


def run():
    if not os.path.exists(CALC_EXE):
        print(f"[!] Warning: Compiled C executable not found at '{CALC_EXE}'.")
        print("    Please compile first with: gcc -Wall -std=c99 main.c calculator.c history.c -o calculator.exe -lm")
        sys.exit(1)

    server_address = ("", PORT)
    httpd = HTTPServer(server_address, CalculatorHandler)
    url = f"http://localhost:{PORT}"

    print("=================================================================")
    print("   SCIENTIFIC CALCULATOR - C ENGINE GUI BRIDGE                   ")
    print("=================================================================")
    print(f" * Backend C Executable: {CALC_EXE}")
    print(f" * Web Interface Directory: {WEB_DIR}")
    print(f" * Server running at: {url}")
    print(" * Press Ctrl+C in terminal to stop.")
    print("=================================================================")

    # Attempt to open default web browser
    try:
        webbrowser.open(url)
    except Exception:
        pass

    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\n[i] Server stopped by user.")
        httpd.server_close()


if __name__ == "__main__":
    run()

