# AGENTS.md

## Scope & Target Architecture
- **Environment:** Solve algorithmic problems from https://www.nerdarena.ro/. Compile with C89 `-O2`.
- **Sandbox Baseline:** Assume a low-end CPU capped at SSE2 (no AVX/AVX2/BMI2). Strict fallbacks: 512KB RAM / 0.15s runtime execution limit unless `PROBLEM.md` explicitly overrides.
- **Isolation:** Execute operations inside the target problem directory. Maintain a global git repository in the root directory. Stay blind to adjacent subfolders.

## Automated Scraping & Setup
- **Automation Skill:** Use/maintain a parametric Python skill (`scripts/fetch_nerdarena.py`) to parse problem pages. Extract target variables from `task-header` (limits) and content from `wiki_text_block`. Save directly to `./PROBLEM_ID/PROBLEM.md`.
- **Initial Checkpoint:** Generate `PROBLEM.md` and initialize an empty `JOURNAL.md` trace file before drafting any code or executing binaries.

## Workspace Hygiene & Silence Rules (Token-Conservation)
- **Silent Tooling:** Run all compiler, test, and shell tools with strict silence flags (suppress verbose warnings/status notices). 
- **Absolute Redirection:** NEVER run local binaries without redirecting stdout/stderr (`> /dev/null` or `> tmp.out`). Never cat full output data into the console.
- **Truncated Verification:** Evaluate test results using binary pass/fail flags (`diff -q`) or strictly capped heads (`diff file1 file2 | head -n 20`). 
- **Git Ledger Precision:** Commit changes per iteration. Commit messages MUST be single-line imperative strings (max 50 characters). Never run unpaginated `git log` or verbose tracking status commands.
- **Dataset Generation:** Use Python skills to build large synthetic test inputs. Never open, read, or print these dataset files into the terminal console context.

## Algorithmic Circuit Breaker
- **Trigger:** If code fails 3 consecutive iterations under the same core logic, halt.
- **Action:** Write an isolated Big-O Time/Space complexity profile and identify the mathematical bottleneck. Write this analysis directly into `JOURNAL.md`. Do not dump this explanation into the primary chat stream.

## Performance Engineering (SSE2 Guardrails)
- **Zero Float Tax:** Reject standard math libraries (`pow`, `sqrt`, `log`). Extract mathematical roots ($\lfloor\sqrt[p]{X}\rfloor$) up to $2^{64}-1$ via bitwise digit-by-digit binary masking construction.
- **Branchless Logic:** Minimize nested condition jumps inside loops to protect small CPU Branch Target Buffers (BTB). Use bitwise masking calculations to eliminate unpredictable `if-else` branching.
- **Division-Free Primitives:** Ban 64-bit integer divisions (`/`) and modulos (`%`) inside loops. Substitute with bit shifts, pre-calculated reciprocals, or boundary bounds.
- **Hardware Probing:** Inject compile-time guards and lightweight runtime CPUID probes (`__builtin_cpu_supports`) at `main` to dynamically select optimal acceleration tiers (Tier 1: BMI2/AVX, Tier 2: SSE4/POPCNT, Tier 3: Safe Baseline SSE2).
- **Memory Profiling:** Zero dynamic allocations (`malloc`). Keep execution stacks under $O(\log N)$. Stream incoming files through an inlined 4KB chunk-buffered `fread` parser using manual accumulation integer logic.

## Journaling & Operational Skills
- **Journal Format:** Keep `JOURNAL.md` below 15 lines per iteration. Use bulleted data points only: (1) Modification details, (2) Evaluator metric, (3) Blockers. No source code or logs.
- **Python Toolkit Matrix:** Reuse parametric scripts in `scripts/`. Keep execution output sparse, actionable, and strictly parsed to safeguard token limits.

### Dynamic Constraints