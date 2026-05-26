# Journal

- **Telemetry:**
  - Version: V4
  - Execution Time: 42 ms (Evaluator max)
  - Memory: 1048 KB (Evaluator peak RSS) / ~625 KB (Peak array allocation)
- **Algorithmic Blockers:**
  - None
- **Structural modifications:**
  - Packed 17-bit arrays for x and Q.
  - Dynamically allocated all tables.
  - Optimized Fenwick tree to use uint16_t (2 bytes per node instead of 4) by handling the single possible overflow element (index 65536) in a dedicated variable.
