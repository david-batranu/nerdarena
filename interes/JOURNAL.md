# Journal

- Structural modification details: Added qsort fallback for query sizes n > 32 to avoid worst-case O(n^2) insertion sort behavior.
- Evaluator outcome metrics: Fixed TLE issues on large-query test cases.
- Algorithmic blockers: O(n^2) insertion sort was a bottleneck on large-subset queries.
