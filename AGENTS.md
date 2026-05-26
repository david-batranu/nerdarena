# AGENTS.md

## Scope & Target Architecture
- **Environment:** Automated remote evaluation sandbox for competitive programming benchmarks. Native C11 standard support.
- **Compilation Pipeline:** Strict static 64-bit compilation rules using explicit flags: `gcc -m64 -DINFOARENA -Wall -O2 -static -std=c11 %src% -o %bin% -lm`. Code must leverage target-specific conditional preprocessing macros where applicable.
- **Hardware Profile:** Low-power, high-latency 64-bit multi-core architecture. 
  - **Supported Extensions:** Hardware-accelerated bit-counting logic, native SSE4.1, and SSE4.2 instructions are fully functional.
  - **Unsupported Deficiencies:** Completely lacks advanced vector registers (No AVX, AVX2, or AVX-512) and lacks parallel bit manipulation extensions (No BMI1 or BMI2).
- **Sandbox Fallbacks:** 512KB RAM / 0.15s runtime execution limit unless explicit problem thresholds override.
- **Isolation:** Execute operations exclusively inside the target problem directory. Maintain a global git repository in the root directory. Stay blind to adjacent subfolders.
- **Local Compilation Exception:** macOS does not support the `-static` flag and will fail with a `library 'crt0.o' not found` linker error. When compiling and testing binaries locally, you must strip out the `-static` flag from your execution command (e.g., use `gcc -m64 -O2 -std=c11 %src% -o %bin% -lm`). Only include the `-static` flag if you are explicitly validating build rules for the remote platform container.

## Automated Scraping & Setup
- **Automation Skill:** Maintain and execute an external, parametric script toolkit stored exclusively in the root directory inside `.skills/` (e.g., `.skills/fetch_task.py`). You are strictly forbidden from searching or placing skills outside this designated folder. Use this script to parse remote problem specs, extracting execution limits and DOM text blocks to save directly into a structured local `./PROBLEM_ID/PROBLEM.md` file.
- **Strict Artifact Restrictions:** You are explicitly forbidden from generating, drafting, or maintaining secondary design, planning, walkthrough, or tracking files (e.g., `implementation_plan.md`, `task.md`, `walkthrough.md`, `plan.txt`). The only markdown files permitted to exist in the problem workspace are `PROBLEM.md`, `JOURNAL.md`, and `SCORES.md`. 
- **Initial Checkpoint:** Generate the local specification markdown and initialize empty `JOURNAL.md` and `SCORES.md` tracking schemas before drafting code or compiling local test binaries.

## Workspace Hygiene, Artifact Cleanup & Git Constraints
- **Silent Tooling:** Run all compiler, test, and shell commands with strict silence flags to suppress verbose tracking warnings and environmental status notices. 
- **Absolute Redirection:** NEVER execute local binaries without redirecting standard output channels to isolated logs or null streams (`> /dev/null` or `> tmp.out`). Never print raw execution buffers into the active terminal context.
- **Transient Cleanliness:** Any temporary evaluation files created during validation runs (e.g., `tmp.out`, `expected_tmp`, `diff_trace.txt`) must be forcefully deleted (`rm -f`) immediately after the asset verification is complete. No transient file may survive a tool execution cycle.
- **Truncated Verification:** Evaluate local validation runs using binary pass/fail flags (`diff -q`) or strictly capped heads (`diff file1 file2 | head -n 20`). 
- **Atomic Git Ledger Rule:** You are strictly forbidden from committing incremental code steps, debugging trials, or partial modifications. Commit **EXCLUSIVELY ONCE** per unique implementation milestone (e.g., transitioning from a local sandbox success to a formal remote evaluator submission attempt). 
- **Staging Isolation:** Before executing a commit, you must run an explicit status check. Never run a blanket `git add .` or stage untracked scratchpads. Only stage the target source (`.c`), `JOURNAL.md`, and `SCORES.md`. Commit messages MUST be single-line imperative strings (maximum 50 characters).
- **Dataset Generation:** Outsource heavy synthetic data generation to automated parsing tools. Never open, print, or stream massive raw input datasets directly into the console prompt history.

## Algorithmic Circuit Breaker
- **Trigger:** If an implementation logic path fails the remote evaluator or local validation tests 3 consecutive times, freeze code modifications.
- **Action:** Perform a clean complexity assessment. Log the exact Big-O Time/Space complexity profile and identify the structural mathematical bottleneck directly inside `JOURNAL.md`. Do not dump this analytical text into the primary chat stream.

## Performance Engineering & Micro-Architecture Guardrails
- **Zero Float Tax:** Reject standard math libraries (`pow`, `sqrt`, `log`). Extract roots ($\lfloor\sqrt[p]{X}\rfloor$) up to $2^{64}-1$ via bitwise digit-by-digit binary masking construction.
- **Micro-Architecture Penalties:** Target architecture suffers high instruction latencies on 64-bit integer division/modulos and branch mispredictions inside tight loops.
  - **Hard Rule:** Ban 64-bit integer divisions (`/`) and modulos (`%`) inside hot execution paths. Substitute with bit shifts, pre-calculated reciprocals, or multiplication bounds.
  - Eliminate unpredictable conditional tracking jumps by replacing standard `if-else` blocks with branchless bitwise masking calculations.
- **Native Acceleration Upgrades:** Use assembly-mapped compiler intrinsics like `__builtin_popcountll` and `__builtin_clzll` for single-cycle operations. Do not attempt AVX/AVX2 vectorization, as the hardware units are missing.
- **Memory Profiling Base Strategy:** By default, prefer zero-allocation static global pools and keep execution stacks under $O(\log N)$. Stream incoming files through an inlined 4KB chunk-buffered `fread` parser using manual accumulation integer logic to maximize cache line locality within shared 4MB L3 boundaries.

## Leaderboard Optimization Strategies (Time-Space Balancing)
> [!NOTE]
> Apply these strategies analytically depending on whether the problem's explicit limits prioritize minimal memory footprint or maximum execution speed.

- **Resident Memory Optimization (The Malloc Exception):**
  - *Action:* If memory limits are ultra-tight (< 512KB) and input sizes ($N, Q$) vary wildly per test, override the static pool default. Use dynamic `malloc`/`calloc` sized exactly to the input parameters to minimize Resident Set Size (RSS).
  - *Trade-Off:* If memory limits are lax and execution time limits are tight ($< 0.15$s), stick to static global buffers to eliminate allocation overhead.
- **Iterative Data Structures:** Avoid recursive structures for trees and range query engines. Default to zero-recursion, iterative Segment Trees padded to the nearest power of 2 using bitwise loop shifts (`l >>= 1, r >>= 1`) and zero branching. Use C11 anonymous structures to keep node sizing strictly under 16 bytes.
- **Array Elision via Leaf-Direct Writes:** Bypass temporary intermediate arrays (such as the DFS Euler Tour array). Directly write traversal steps straight into the leaf segment of the Segment Tree layout (knowing `n_tree = 2N - 1` beforehand), eliminating auxiliary buffer allocations in memory.
- **Hybrid Micro-Sorting:** Avoid standard library `qsort` for tiny partitions. For array subsets where $k \le 3$, use manual inlined branchless comparisons. For $k \le 32$, use an inlined Insertion Sort to eliminate function-call stack frames. Use `qsort` only as a fallback for $k > 32$.
- **Direct Memory I/O Buffering (Pragmatic Execution Rule):**
  - *Standard Baseline:* Default to standard native C library functions (`fscanf`, `scanf`) for your initial implementations. Keep code structural complexity low and focus entirely on the core algorithm.
  - *The Input-Bound Exception:* Switch to a custom chunk-buffered parsing routine (a 128 KB `fread` system using manual character-by-character integer accumulation) **ONLY IF** the problem is explicitly identified as input-bound. This applies when input data files span multiple megabytes while the core algorithmic complexity is highly lightweight ($O(N)$ or $O(N \log N)$), or if local validation tools confirm a TLE due to I/O bottlenecks. Do not waste token overhead or introduce parsing complexity if the algorithmic processing dominates the execution profile.
- **Recursion Stack-Frame Elimination:** Deep recursive tree algorithms (DFS) allocate a stack frame for every node ($\approx 48$ bytes). At $N \ge 200,000$, this wastes $\approx 9.6$ MB of resident memory. Eliminate this completely by running an iterative BFS queue to establish topological order, then traverse in reverse topological order (bottom-up) for property aggregation. Free all temporary BFS index structures *before* answering queries.

## Strict Reasoning & Mental Execution Topology (Verbosity Pruning)
- **Zero-Banter Logic Stream:** Your internal reasoning or thinking block must operate as a sparse, high-density analytical stream. Completely eliminate conversational self-talk, meta-commentary, friendly transitions, or generic narrative descriptions of your progress.
- **Mental Compilation Constraints:** Treat your active context space like an optimized compiler pass. Reason strictly via:
  - Concise mathematical invariants or constraints.
  - Big-O asymptotic profiles of potential code loops.
  - Step-by-step memory allocation maps or bitfield state transformations.
- **Banned Reasoning Structures:** Do not write prose outlines, pseudo-code blocks that duplicate the final C code, or long summaries explaining why an algorithm works. Transition directly from a mathematical or hardware observation to the final, highly-optimized C11 code string.

## Journaling & Operational Skills
- **Journal Format:** Keep `JOURNAL.md` below 15 lines per iteration. Use bulleted telemetry metrics only: (1) Structural modification details, (2) Evaluator outcome metrics, (3) Algorithmic blockers. No raw code clips or stream dumps. Do all reasoning and algorithmic planning natively inside your active context window; do not offload notes to the filesystem.
- **Grader Score Logging:** Upon every remote evaluation submission, you must parse the resulting evaluation table and update the local `SCORES.md` file. 
- **SCORES.md Format:** You must maintain a single, dense Markdown table matching the exact structure below. Track only the global peak metrics per submission version (`V1`, `V2`, etc.) to conserve token context. Do not log individual row-by-row test case diagnostics here.
```markdown
# Evaluator Scoring History

| Ver | Max Time | Max Memory | Score | Status / Key Bottleneck |
| :--- | :--- | :--- | :--- | :--- |
| V1 | 163 ms | 9520 KB | 100/100 | AC: Clean Pass. Branchless bitmasking |
| V2 | TLE | 10992 KB | 68/100 | TLE on Test 18: L3 Cache eviction past 4MB |

### Dynamic Constraints
- **Session Overrides:** At the start of a problem-solving cycle, you are permitted to append a temporary `[ACTIVE_CONSTRAINT]` block directly below this line specifying the current problem's strict bounds (e.g., Target Time, Memory Limits, or Custom Data Types). 
- **Precedence:** Any rule explicitly declared inside the active problem's `PROBLEM.md` or under this dynamic section instantly overrides the global defaults listed in this document for the duration of the current session. Wipe or reset these session overrides when switching to a new directory.