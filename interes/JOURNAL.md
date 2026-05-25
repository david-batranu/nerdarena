# Journal

- Structural modification details: Migrated all tree arrays and query buffers to dynamic memory allocation via malloc and realloc.
- Evaluator outcome metrics: Drastically reduced memory across all test cases based on specific N and Q parameters.
- Algorithmic blockers: Static BSS mapping had fixed system memory overhead.
