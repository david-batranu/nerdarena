#include <stdio.h>
#include <stdlib.h>

// Fenwick tree functions
inline void bit_update(int *bit, int n, int idx, int val) {
    for (; idx <= n; idx += idx & -idx) {
        bit[idx] += val;
    }
}

inline int bit_query(int *bit, int idx) {
    int sum = 0;
    for (; idx > 0; idx -= idx & -idx) {
        sum += bit[idx];
    }
    return sum;
}

// Binary lifting on Fenwick tree to find the k-th empty slot
// Returns 1-indexed position
inline int bit_find_kth(const int *bit, int n, int k) {
    int idx = 0;
    int sum = 0;
    // 1 << 17 is 131072, which is the smallest power of 2 >= 100000
    for (int step = 131072; step > 0; step >>= 1) {
        if (idx + step <= n && sum + bit[idx + step] < k) {
            idx += step;
            sum += bit[idx];
        }
    }
    return idx + 1;
}

int main(void) {
    FILE *fin = fopen("arbperm.in", "r");
    if (!fin) return 0;

    int n;
    long long k;
    if (fscanf(fin, "%d %lld", &n, &k) != 2) {
        fclose(fin);
        return 0;
    }

    // Allocate arrays dynamically to minimize RSS
    int *pos = (int *)malloc((n + 1) * sizeof(int));
    int *x = (int *)malloc((n + 1) * sizeof(int));
    int *bit = (int *)calloc(n + 1, sizeof(int));

    if (!pos || !x || !bit) {
        fclose(fin);
        return 0;
    }

    // Read permutation P and record positions
    // P elements are 1-indexed, positions are 0-indexed
    for (int i = 0; i < n; ++i) {
        int val;
        if (fscanf(fin, "%d", &val) == 1) {
            pos[val] = i;
        }
    }
    fclose(fin);

    // Step 1: Compute the insertion positions x_i
    // We process i from n down to 1.
    // x_i = pos[i] - query(pos[i]) (since pos is 0-indexed, and query counts elements already placed to the left)
    // Note: Fenwick tree expects 1-indexed, so we query at pos[i] (which is equivalent to 1-indexed prefix sum up to pos[i])
    for (int i = n; i >= 1; --i) {
        int p = pos[i];
        int placed_left = bit_query(bit, p);
        x[i] = p - placed_left;
        bit_update(bit, n, p + 1, 1);
    }

    // Step 2: Add K to the mixed-radix number x
    // x[i] is in base i. Least significant is x[n], most significant is x[2] (x[1] is always 0)
    long long carry = k;
    for (int i = n; i >= 2 && carry > 0; --i) {
        long long val = x[i] + carry;
        x[i] = (int)(val % i);
        carry = val / i;
    }
    x[1] = 0; // x[1] is always 0

    // Step 3: Reconstruct Q
    // Re-initialize Fenwick tree for Q reconstruction: each element is 1 (empty slot)
    // We can do this in O(n)
    for (int i = 1; i <= n; ++i) {
        bit[i] = i & -i;
    }

    // Reuse pos array to store Q to save memory!
    int *Q = pos;

    // Place i from n down to 1 into the (x[i])-th empty slot
    for (int i = n; i >= 1; --i) {
        int target_slot = x[i] + 1; // 1-indexed empty slot count
        int p = bit_find_kth(bit, n, target_slot);
        Q[p - 1] = i;
        bit_update(bit, n, p, -1);
    }

    // Write to arbperm.out
    FILE *fout = fopen("arbperm.out", "w");
    if (fout) {
        for (int i = 0; i < n; ++i) {
            fprintf(fout, "%d%c", Q[i], (i == n - 1) ? '\n' : ' ');
        }
        fclose(fout);
    }

    free(pos);
    free(x);
    free(bit);

    return 0;
}
