#include <stdio.h>
#include <stdlib.h>

#define MAXN 100005
#define BUF_SIZE 4096

// Static global arrays to eliminate heap allocation overhead (BSS segment)
int x[MAXN];
int bit[MAXN];
int Q[MAXN];

char buf[BUF_SIZE];
int buf_ptr = BUF_SIZE;
int buf_len = BUF_SIZE;

static inline char get_char(FILE *fin) {
    if (buf_ptr >= buf_len) {
        buf_len = fread(buf, 1, BUF_SIZE, fin);
        buf_ptr = 0;
        if (buf_len == 0) return EOF;
    }
    return buf[buf_ptr++];
}

static inline int read_int(FILE *fin, int *val) {
    char c = get_char(fin);
    while (c != EOF && (c < '0' || c > '9')) {
        c = get_char(fin);
    }
    if (c == EOF) return 0;
    int res = 0;
    while (c >= '0' && c <= '9') {
        res = res * 10 + (c - '0');
        c = get_char(fin);
    }
    *val = res;
    return 1;
}

static inline int read_long(FILE *fin, long long *val) {
    char c = get_char(fin);
    while (c != EOF && (c < '0' || c > '9')) {
        c = get_char(fin);
    }
    if (c == EOF) return 0;
    long long res = 0;
    while (c >= '0' && c <= '9') {
        res = res * 10 + (c - '0');
        c = get_char(fin);
    }
    *val = res;
    return 1;
}

// Fenwick tree functions
static inline void bit_update(int n, int idx, int val) {
    for (; idx <= n; idx += idx & -idx) {
        bit[idx] += val;
    }
}

static inline int bit_query(int idx) {
    int sum = 0;
    for (; idx > 0; idx -= idx & -idx) {
        sum += bit[idx];
    }
    return sum;
}

// Binary lifting on Fenwick tree to find the k-th empty slot
static inline int bit_find_kth(int n, int k) {
    int idx = 0;
    int sum = 0;
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
    if (!read_int(fin, &n) || !read_long(fin, &k)) {
        fclose(fin);
        return 0;
    }

    // Step 1: Compute the insertion positions x_i on the fly
    for (int j = 0; j < n; ++j) {
        int val;
        read_int(fin, &val);
        x[val] = bit_query(val - 1);
        bit_update(n, val, 1);
    }
    fclose(fin);

    // Step 2: Add K to the mixed-radix number x
    long long carry = k;
    for (int i = n; i >= 2 && carry > 0; --i) {
        long long val = x[i] + carry;
        x[i] = (int)(val % i);
        carry = val / i;
    }
    x[1] = 0;

    // Step 3: Reconstruct Q
    // Re-initialize Fenwick tree for Q reconstruction: each element is 1 (empty slot)
    for (int i = 1; i <= n; ++i) {
        bit[i] = i & -i;
    }

    // Place i from n down to 1 into the (x[i])-th empty slot
    for (int i = n; i >= 1; --i) {
        int target_slot = x[i] + 1;
        int p = bit_find_kth(n, target_slot);
        Q[p - 1] = i;
        bit_update(n, p, -1);
    }

    // Write to arbperm.out
    FILE *fout = fopen("arbperm.out", "w");
    if (fout) {
        for (int i = 0; i < n; ++i) {
            fprintf(fout, "%d%c", Q[i], (i == n - 1) ? '\n' : ' ');
        }
        fclose(fout);
    }

    return 0;
}
