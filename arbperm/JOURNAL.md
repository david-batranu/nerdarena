# Journal

- **Telemetry:**
  - Version: V5
  - Execution Time: 42 ms (Evaluator max)
  - Memory: 1056 KB (Evaluator peak RSS) / ~412.5 KB (Peak array allocation)
- **Algorithmic Blockers:**
  - None
- **Structural modifications:**
  - Packed 17-bit arrays for x and Q.
  - Dynamically allocated all tables.
  - Optimized Fenwick tree to use uint16_t by handling index 65536 in a dedicated variable.
  - Implemented completely in-place dynamic array sharing using a chain-following algorithm to eliminate the Q array entirely, reducing total array allocation to just ~412.5 KB.
