#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAXN 100005
#define BUF_SIZE 4096

// Packed representation for x and Q to reduce memory footprint.
// High 17th bit is packed into a bit-array (using uint32_t).
uint16_t *x_low;
uint32_t *x_high;

uint16_t *bit;
int bit_65536;

static inline void set_x(int idx, int val) {
    x_low[idx] = val & 0xFFFF;
    if (val & 0x10000) {
        x_high[idx >> 5] |= (1U << (idx & 31));
    } else {
        x_high[idx >> 5] &= ~(1U << (idx & 31));
    }
}

static inline int get_x(int idx) {
    int low = x_low[idx];
    int high = (x_high[idx >> 5] >> (idx & 31)) & 1;
    return low | (high << 16);
}

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
        if (idx == 65536) {
            bit_65536 += val;
        } else {
            bit[idx] += val;
        }
    }
}

static inline int bit_query(int idx) {
    int sum = 0;
    for (; idx > 0; idx -= idx & -idx) {
        if (idx == 65536) {
            sum += bit_65536;
        } else {
            sum += bit[idx];
        }
    }
    return sum;
}

// Binary lifting on Fenwick tree to find the k-th empty slot
static inline int bit_find_kth(int n, int k) {
    int idx = 0;
    int sum = 0;
    for (int step = 131072; step > 0; step >>= 1) {
        if (idx + step <= n) {
            int val = (idx + step == 65536) ? bit_65536 : bit[idx + step];
            if (sum + val < k) {
                idx += step;
                sum += val;
            }
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

    x_low = malloc((n + 1) * sizeof(uint16_t));
    x_high = calloc((n + 32) / 32, sizeof(uint32_t));
    bit = calloc(n + 1, sizeof(uint16_t));
    bit_65536 = 0;

    // Step 1: Compute the insertion positions x_i on the fly
    for (int j = 0; j < n; ++j) {
        int val;
        read_int(fin, &val);
        set_x(val, bit_query(val - 1));
        bit_update(n, val, 1);
    }
    fclose(fin);

    // Step 2: Add K to the mixed-radix number x
    long long carry = k;
    for (int i = n; i >= 2 && carry > 0; --i) {
        long long val = get_x(i) + carry;
        set_x(i, (int)(val % i));
        carry = val / i;
    }
    set_x(1, 0);

    // Step 3: Reconstruct Q in-place in x array to save memory
    // Re-initialize Fenwick tree for Q reconstruction: each element is 1 (empty slot)
    for (int i = 1; i <= n; ++i) {
        if (i == 65536) {
            bit_65536 = i & -i;
        } else {
            bit[i] = i & -i;
        }
    }

    // Place i from n down to 1 into the (x[i])-th empty slot
    for (int i = n; i >= 1; --i) {
        // Follow the chain if the value was moved to a larger index
        int curr = i;
        while (get_x(curr) > curr) {
            curr = get_x(curr);
        }
        int original_x_i = get_x(curr);

        int target_slot = original_x_i + 1;
        int p = bit_find_kth(n, target_slot);
        int dest_idx = p - 1; // 0-based

        if (dest_idx < i) {
            // Move the alive value at 1-based index (dest_idx + 1) to index i (which is now dead)
            set_x(i, get_x(dest_idx + 1));
            // Write the final value i to 1-based index (dest_idx + 1)
            set_x(dest_idx + 1, i);
        } else {
            // Write the final value i to 1-based index (dest_idx + 1)
            set_x(dest_idx + 1, i);
        }
        bit_update(n, p, -1);
    }

    // Write to arbperm.out (the output is in positions 1 to n of x)
    FILE *fout = fopen("arbperm.out", "w");
    if (fout) {
        for (int i = 1; i <= n; ++i) {
            fprintf(fout, "%d%c", get_x(i), (i == n) ? '\n' : ' ');
        }
        fclose(fout);
    }

    free(x_low);
    free(x_high);
    free(bit);

    return 0;
}
