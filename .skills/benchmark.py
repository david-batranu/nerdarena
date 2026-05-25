#!/usr/bin/env python3
import sys
import os
import subprocess
import time
import shutil

def benchmark(c_source, input_file, expected_output, time_limit=0.15):
    # 1. Compile C file
    binary = "./temp_bin"
    compile_cmd = ["gcc", "-O3", "-Wall", "-ansi", "-pedantic", "-o", binary, c_source, "-lm"]
    
    comp_res = subprocess.run(compile_cmd, capture_output=True, text=True)
    if comp_res.returncode != 0:
        print(f"Compilation Failed!\n{comp_res.stderr}")
        return False

    prog_input_path = "bonus.in"
    prog_output_path = "bonus.out"
    
    if os.path.exists(prog_input_path):
        os.remove(prog_input_path)
    if os.path.exists(prog_output_path):
        os.remove(prog_output_path)
        
    shutil.copy2(input_file, prog_input_path)
    
    # 2. Run binary in a separate python process to isolate resource usage measurement
    # We pass the binary path and the timeout to a python helper that runs it and prints
    # the runtime and the peak RSS of only that child process.
    py_code = f"""
import subprocess, resource, time, sys
start = time.perf_counter()
try:
    proc = subprocess.run(["{binary}"], timeout={time_limit}, capture_output=True)
    elapsed = (time.perf_counter() - start) * 1000.0
    usage = resource.getrusage(resource.RUSAGE_CHILDREN)
    maxrss = usage.ru_maxrss
    print(f"{{elapsed:.2f}}|{{maxrss}}|{{proc.returncode}}")
except subprocess.TimeoutExpired:
    print("TLE|0|0")
except Exception as e:
    print(f"ERROR|0|0")
"""
    
    proc_res = subprocess.run([sys.executable, "-c", py_code], capture_output=True, text=True)
    
    if os.path.exists(binary):
        os.remove(binary)
        
    if os.path.exists(prog_input_path):
        os.remove(prog_input_path)
        
    if proc_res.returncode != 0 or not proc_res.stdout.strip():
        print(f"Execution wrapper failed: {proc_res.stderr}")
        return False
        
    parts = proc_res.stdout.strip().split('|')
    if len(parts) < 3:
        print(f"Unexpected output from wrapper: {proc_res.stdout}")
        return False
        
    status, rss_str, ret_str = parts[0], parts[1], parts[2]
    
    if status == "TLE":
        print("Grader Verdict: Time Limit Exceeded (TLE)")
        return False
    elif status == "ERROR":
        print("Execution failed.")
        return False
        
    run_time_ms = float(status)
    raw_rss = float(rss_str)
    ret_code = int(ret_str)
    
    if ret_code != 0:
        print(f"Program exited with non-zero status: {ret_code}")
        return False
        
    # on macOS, ru_maxrss is in bytes. on Linux, it is in kilobytes.
    if sys.platform == 'darwin':
        peak_rss_kb = raw_rss / 1024.0
    else:
        peak_rss_kb = raw_rss
        
    # 3. Verify output
    if not os.path.exists(prog_output_path):
        print("Error: Output file not created by program.")
        return False
        
    diff_res = subprocess.run(["diff", "-q", prog_output_path, expected_output], capture_output=True)
    
    if diff_res.returncode == 0:
        print(f"Verdict: PASS | Time: {run_time_ms:.1f} ms | Peak RSS: {peak_rss_kb:.1f} KB")
        return True
    else:
        print("Verdict: FAIL (Output Mismatch)")
        sub_diff = subprocess.run(["diff", prog_output_path, expected_output], capture_output=True, text=True)
        print("Diff mismatch sample:")
        lines = sub_diff.stdout.splitlines()
        for line in lines[:10]:
            print(f"  {line}")
        return False

if __name__ == "__main__":
    if len(sys.argv) < 4:
        print("Usage: python3 benchmark.py <source.c> <input.in> <expected.ok> [time_limit_sec]")
        sys.exit(1)
        
    src = sys.argv[1]
    inp = sys.argv[2]
    exp = sys.argv[3]
    t_lim = float(sys.argv[4]) if len(sys.argv) > 4 else 0.15
    
    benchmark(src, inp, exp, t_lim)
