#include <stdio.h>

#define INPUT_BUF_SIZE 4194304
char input_buf[INPUT_BUF_SIZE];
char *ptr = input_buf;

static inline int read_int(void) {
    while (*ptr < '0') {
        ptr++;
    }
    int res = 0;
    while (*ptr >= '0') {
        res = res * 10 + (*ptr - '0');
        ptr++;
    }
    return res;
}

static inline void read_string(char *s, int n) {
    while (*ptr < '0') {
        ptr++;
    }
    for (int i = 0; i < n; ++i) {
        s[i] = *ptr;
        ptr++;
    }
}

#define OUTPUT_BUF_SIZE 1048576
char out_buf[OUTPUT_BUF_SIZE];
int out_ptr = 0;

static inline void flush_out(void) {
    if (out_ptr > 0) {
        fwrite(out_buf, 1, out_ptr, stdout);
        out_ptr = 0;
    }
}

static inline void write_char(char c) {
    if (out_ptr >= OUTPUT_BUF_SIZE) {
        flush_out();
    }
    out_buf[out_ptr++] = c;
}

static inline void write_int(int x) {
    if (x == 0) {
        write_char('0');
        return;
    }
    char temp[12];
    int tp = 0;
    while (x > 0) {
        temp[tp++] = (char)('0' + (x % 10));
        x /= 10;
    }
    while (tp > 0) {
        write_char(temp[--tp]);
    }
}

typedef struct {
    int max_len;
    int pref_len;
    int suff_len;
} Node;

#define MAXN 200005
#define MAX_NODES 524288

Node tree[MAX_NODES];
char initial_state[MAXN];

static inline Node merge(Node A, Node B, int len_A, int len_B) {
    Node res;
    int cross = A.suff_len + B.pref_len;
    int max_child = A.max_len > B.max_len ? A.max_len : B.max_len;
    res.max_len = cross > max_child ? cross : max_child;
    res.pref_len = (A.pref_len == len_A) ? (len_A + B.pref_len) : A.pref_len;
    res.suff_len = (B.suff_len == len_B) ? (len_B + A.suff_len) : B.suff_len;
    return res;
}

int main(void) {
    if (freopen("spiridusi.in", "r", stdin) == NULL) return 0;
    if (freopen("spiridusi.out", "w", stdout) == NULL) return 0;

    int bytes_read = fread(input_buf, 1, INPUT_BUF_SIZE - 1, stdin);
    input_buf[bytes_read] = '\0';

    int n = read_int();
    int q = read_int();
    if (n <= 0) return 0;

    read_string(initial_state, n);

    // Find the smallest power of 2 >= n
    int k = 1;
    while (k < n) {
        k <<= 1;
    }

    int clz_k = __builtin_clz(k);

    // Build the leaf level of the iterative segment tree
    for (int i = 0; i < k; ++i) {
        int val = (i < n) ? (initial_state[i] - '0') : 0;
        tree[i + k].max_len = val;
        tree[i + k].pref_len = val;
        tree[i + k].suff_len = val;
    }

    // Build the upper levels bottom-up
    for (int i = k - 1; i > 0; --i) {
        int child_len = 1 << (__builtin_clz(i) - clz_k - 1);
        tree[i] = merge(tree[i << 1], tree[i << 1 | 1], child_len, child_len);
    }

    for (int i = 0; i < q; ++i) {
        int type = read_int();
        if (type == 1) {
            int x = read_int();
            int p = x - 1 + k;
            int val = tree[p].max_len ^ 1;
            tree[p].max_len = val;
            tree[p].pref_len = val;
            tree[p].suff_len = val;
            for (p >>= 1; p > 0; p >>= 1) {
                int child_len = 1 << (__builtin_clz(p) - clz_k - 1);
                tree[p] = merge(tree[p << 1], tree[p << 1 | 1], child_len, child_len);
            }
        } else if (type == 2) {
            int x = read_int();
            int y = read_int();
            int l = x - 1;
            int r = y - 1;

            Node res_l = (Node){0, 0, 0};
            Node res_r = (Node){0, 0, 0};
            int len_l = 0;
            int len_r = 0;

            for (l += k, r += k + 1; l < r; l >>= 1, r >>= 1) {
                if (l & 1) {
                    int cur_len = 1 << (__builtin_clz(l) - clz_k);
                    res_l = merge(res_l, tree[l], len_l, cur_len);
                    len_l += cur_len;
                    l++;
                }
                if (r & 1) {
                    --r;
                    int cur_len = 1 << (__builtin_clz(r) - clz_k);
                    res_r = merge(tree[r], res_r, cur_len, len_r);
                    len_r += cur_len;
                }
            }
            Node ans = merge(res_l, res_r, len_l, len_r);
            write_int(ans.max_len);
            write_char('\n');
        }
    }

    flush_out();
    return 0;
}
