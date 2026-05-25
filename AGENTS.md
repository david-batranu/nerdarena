# AGENTS.md

## Scope & Target Architecture
- **Environment:** Automated remote evaluation sandbox for competitive programming benchmarks.
- **Compilation Pipeline:** Strict static 64-bit compilation rules using explicit flags: `gcc -m64 -Wall -O2 -static -std=c11 %src% -o %bin% -lm`. Code must leverage target-specific conditional preprocessing macros where applicable.
- **Hardware Profile:** Low-power, high-latency 64-bit multi-core architecture. 
  - **Supported Extensions:** Hardware-accelerated bit-counting logic, native SSE4.1, and SSE4.2 instructions are fully functional.
  - **Unsupported Deficiencies:** Completely lacks advanced vector registers (No AVX, AVX2, or AVX-512) and lacks parallel bit manipulation extensions (No BMI1 or BMI2).
- **Sandbox Fallbacks:** 512KB RAM / 0.15s runtime execution limit unless explicit problem thresholds override.
- **Isolation:** Execute operations exclusively inside the target problem directory. Maintain a global git repository in the root directory. Stay blind to adjacent subfolders.

## Automated Scraping & Setup
- **Automation Skill:** Maintain and execute an external, parametric script toolkit stored exclusively in the root directory inside `.skills/` (e.g., `.skills/fetch_task.py`). You are strictly forbidden from searching or placing skills outside this designated folder. Use this script to parse remote problem specs, extracting execution limits and DOM text blocks to save directly into a structured local `./PROBLEM_ID/PROBLEM.md` file.
- **Initial Checkpoint:** Generate the local specification markdown and initialize an empty `JOURNAL.md` tracking schema before drafting code or compiling local test binaries.

## Workspace Hygiene & Silence Rules (Token-Conservation)
- **Silent Tooling:** Run all compiler, test, and shell commands with strict silence flags to suppress verbose tracking warnings and environmental status notices. 
- **Absolute Redirection:** NEVER execute local binaries without redirecting standard output channels to isolated logs or null streams (`> /dev/null` or `> tmp.out`). Never print raw execution buffers into the active terminal context.
- **Truncated Verification:** Evaluate local validation runs using binary pass/fail flags (`diff -q`) or strictly capped heads (`diff file1 file2 | head -n 20`). 
- **Git Ledger Precision:** Commit changes sequentially per iteration loop. Commit messages MUST be single-line imperative strings (maximum 50 characters). Never run unpaginated logs or verbose repository health checks.
- **Dataset Generation:** Outsource heavy synthetic data generation to automated parsing tools. Never open, print, or stream massive raw input datasets directly into the console prompt history.

## Algorithmic Circuit Breaker
- **Trigger:** If an implementation logic path fails the remote evaluator or local validation tests 3 consecutive times, freeze code modifications.
- **Action:** Perform a clean complexity assessment. Log the exact Big-O Time/Space complexity profile and identify the structural mathematical bottleneck directly inside `JOURNAL.md`. Do not dump this analytical text into the primary chat stream.

## Performance Engineering (High-Latency Execution Guardrails)
- **Zero Float Tax:** Completely reject standard math runtime libraries (`pow`, `sqrt`, `log`). Extract mathematical roots ($\lfloor\sqrt[p]{X}\rfloor$) across large 64-bit spaces using deterministic, integer-only routines such as bitwise digit-by-digit binary masking construction.
- **Micro-Architecture Penalties:** The evaluation processor suffers severe pipeline stalls on 64-bit integer divisions and branch mispredictions inside tight loops.
  - **Hard Rule:** Ban 64-bit integer divisions (`/`) and modulos (`%`) inside hot execution paths. Substitute with bit shifts, pre-calculated reciprocals, or multiplication bounds.
  - Eliminate unpredictable conditional tracking jumps by replacing standard `if-else` loops with branchless bitwise masking calculations.
- **Hardware Pruning:** Use assembly-mapped compiler intrinsics like `__builtin_popcountll` and `__builtin_clzll` for single-cycle operations. Do not attempt vectorization loops, as the underlying silicon lacks the hardware processing units.
- **Memory Profiling:** Zero dynamic allocations (`malloc`). Keep processing stacks bounded strictly below $O(\log N)$. Stream incoming source files through an inlined 4KB chunk-buffered `fread` parser using manual accumulation integer logic to maximize cache line locality within shared L3 block boundaries.

## Journaling & Operational Skills
- **Journal Format:** Keep `JOURNAL.md` below 15 lines per iteration. Use bulleted telemetry metrics only: (1) Structural modification details, (2) Evaluator outcome metrics, (3) Algorithmic blockers. No raw code clips or stream dumps.
- **Toolkit Matrix & Discovery:** Delegate all environment profiling, testing, scraping, and tracking tasks to external parametric automation tools located inside the root `.skills/` directory. 
- **Automatic Skill Discovery:** Before writing a new automation script, you must run a silent directory listing of the `.skills/` folder to discover existing tools. Do not maintain a written `skills.md` file; rely strictly on clean script naming conventions (e.g., `.skills/fetch_task.py`, `.skills/stress_test.py`) and standard inline python docstrings for self-documentation. Keep tool response streams highly sparse and actionable to safeguard active token boundaries.

### Dynamic Constraints