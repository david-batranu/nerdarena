#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

#define MAXN 100005
#define BUF_SIZE 4096
#define EOF -1

// Packed representation for x and Q to reduce memory footprint.
// High 17th bit is packed into a bit-array (using uint32_t).
uint16_t *x_low;
uint32_t *x_high;

uint16_t *Q_low;
uint32_t *Q_high;

uint16_t *bit;
int bit_65536;

static inline void set_Q(int idx, int val) {
    Q_low[idx] = val & 0xFFFF;
    if (val & 0x10000) {
        Q_high[idx >> 5] |= (1U << (idx & 31));
    } else {
        Q_high[idx >> 5] &= ~(1U << (idx & 31));
    }
}

static inline int get_Q(int idx) {
    int low = Q_low[idx];
    int high = (Q_high[idx >> 5] >> (idx & 31)) & 1;
    return low | (high << 16);
}

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

static inline char get_char(int fd) {
    if (buf_ptr >= buf_len) {
        buf_len = read(fd, buf, BUF_SIZE);
        buf_ptr = 0;
        if (buf_len <= 0) return EOF;
    }
    return buf[buf_ptr++];
}

static inline int read_int(int fd, int *val) {
    char c = get_char(fd);
    while (c != EOF && (c < '0' || c > '9')) {
        c = get_char(fd);
    }
    if (c == EOF) return 0;
    int res = 0;
    while (c >= '0' && c <= '9') {
        res = res * 10 + (c - '0');
        c = get_char(fd);
    }
    *val = res;
    return 1;
}

static inline int read_long(int fd, long long *val) {
    char c = get_char(fd);
    while (c != EOF && (c < '0' || c > '9')) {
        c = get_char(fd);
    }
    if (c == EOF) return 0;
    long long res = 0;
    while (c >= '0' && c <= '9') {
        res = res * 10 + (c - '0');
        c = get_char(fd);
    }
    *val = res;
    return 1;
}

char out_buf[BUF_SIZE];
int out_ptr = 0;

static inline void write_char(int fd, char c) {
    if (out_ptr >= BUF_SIZE) {
        write(fd, out_buf, out_ptr);
        out_ptr = 0;
    }
    out_buf[out_ptr++] = c;
}

static inline void write_int(int fd, int val) {
    if (val == 0) {
        write_char(fd, '0');
        return;
    }
    char temp[10];
    int t_ptr = 0;
    while (val > 0) {
        temp[t_ptr++] = (val % 10) + '0';
        val /= 10;
    }
    while (t_ptr > 0) {
        write_char(fd, temp[--t_ptr]);
    }
}

static inline void flush_out(int fd) {
    if (out_ptr > 0) {
        write(fd, out_buf, out_ptr);
        out_ptr = 0;
    }
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
    int fd_in = open("arbperm.in", O_RDONLY);
    if (fd_in < 0) return 0;

    int n;
    long long k;
    if (!read_int(fd_in, &n) || !read_long(fd_in, &k)) {
        close(fd_in);
        return 0;
    }

    x_low = malloc((n + 1) * sizeof(uint16_t));
    x_high = calloc((n + 32) / 32, sizeof(uint32_t));
    Q_low = malloc((n + 1) * sizeof(uint16_t));
    Q_high = calloc((n + 32) / 32, sizeof(uint32_t));
    bit = calloc(n + 1, sizeof(uint16_t));
    bit_65536 = 0;

    // Step 1: Compute the insertion positions x_i on the fly
    for (int j = 0; j < n; ++j) {
        int val;
        read_int(fd_in, &val);
        set_x(val, bit_query(val - 1));
        bit_update(n, val, 1);
    }
    close(fd_in);

    // Step 2: Add K to the mixed-radix number x
    long long carry = k;
    for (int i = n; i >= 2 && carry > 0; --i) {
        long long val = get_x(i) + carry;
        set_x(i, (int)(val % i));
        carry = val / i;
    }
    set_x(1, 0);

    // Step 3: Reconstruct Q
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
        int target_slot = get_x(i) + 1;
        int p = bit_find_kth(n, target_slot);
        set_Q(p - 1, i);
        bit_update(n, p, -1);
    }

    // Write to arbperm.out
    int fd_out = open("arbperm.out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out >= 0) {
        for (int i = 0; i < n; ++i) {
            write_int(fd_out, get_Q(i));
            write_char(fd_out, (i == n - 1) ? '\n' : ' ');
        }
        flush_out(fd_out);
        close(fd_out);
    }

    free(x_low);
    free(x_high);
    free(Q_low);
    free(Q_high);
    free(bit);

    return 0;
}
