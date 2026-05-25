#include <stdio.h>

#define BUF_SIZE 1048576
char buf[BUF_SIZE];
int buf_ptr = 0;
int buf_len = 0;

static inline char next_char(void) {
    if (buf_ptr >= buf_len) {
        buf_ptr = 0;
        buf_len = (int)fread(buf, 1, BUF_SIZE, stdin);
        if (buf_len <= 0) {
            return EOF;
        }
    }
    return buf[buf_ptr++];
}

static inline int read_int(void) {
    char c = next_char();
    while (c != EOF && (c < '0' || c > '9')) {
        c = next_char();
    }
    if (c == EOF) return 0;
    int res = 0;
    while (c >= '0' && c <= '9') {
        res = res * 10 + (c - '0');
        c = next_char();
    }
    return res;
}

static inline void read_string(char *s, int n) {
    char c = next_char();
    while (c != EOF && c != '0' && c != '1') {
        c = next_char();
    }
    for (int i = 0; i < n; ++i) {
        s[i] = c;
        c = next_char();
    }
}

char out_buf[BUF_SIZE];
int out_ptr = 0;

static inline void flush_out(void) {
    if (out_ptr > 0) {
        fwrite(out_buf, 1, out_ptr, stdout);
        out_ptr = 0;
    }
}

static inline void write_char(char c) {
    if (out_ptr >= BUF_SIZE) {
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
    // Branchless max: res.max_len = max(A.max_len, B.max_len)
    res.max_len = A.max_len ^ ((A.max_len ^ B.max_len) & -(A.max_len < B.max_len));
    
    // Branchless cross merge: max(res.max_len, A.suff_len + B.pref_len)
    int cross = A.suff_len + B.pref_len;
    int diff = res.max_len - cross;
    res.max_len ^= (res.max_len ^ cross) & -(diff < 0);
    
    // Branchless prefix and suffix length updates
    res.pref_len = A.pref_len + (B.pref_len & -(A.pref_len == len_A));
    res.suff_len = B.suff_len + (A.suff_len & -(B.suff_len == len_B));
    
    return res;
}

void build(int node, int start, int end) {
    if (start == end) {
        int val = initial_state[start - 1] - '0';
        tree[node].max_len = val;
        tree[node].pref_len = val;
        tree[node].suff_len = val;
        return;
    }
    int mid = (start + end) >> 1;
    int left = node << 1;
    int right = left | 1;
    build(left, start, mid);
    build(right, mid + 1, end);
    tree[node] = merge(tree[left], tree[right], mid - start + 1, end - mid);
}

void update(int node, int start, int end, int idx) {
    if (start == end) {
        int val = tree[node].max_len ^ 1;
        tree[node].max_len = val;
        tree[node].pref_len = val;
        tree[node].suff_len = val;
        return;
    }
    int mid = (start + end) >> 1;
    int left = node << 1;
    int right = left | 1;
    if (idx <= mid) {
        update(left, start, mid, idx);
    } else {
        update(right, mid + 1, end, idx);
    }
    tree[node] = merge(tree[left], tree[right], mid - start + 1, end - mid);
}

Node query(int node, int start, int end, int l, int r) {
    if (l <= start && end <= r) {
        return tree[node];
    }
    int mid = (start + end) >> 1;
    int left = node << 1;
    int right = left | 1;
    if (r <= mid) {
        return query(left, start, mid, l, r);
    }
    if (l > mid) {
        return query(right, mid + 1, end, l, r);
    }
    
    Node ql = query(left, start, mid, l, r);
    Node qr = query(right, mid + 1, end, l, r);
    
    int max_l = l > start ? l : start;
    int min_r = r < end ? r : end;
    
    return merge(ql, qr, mid - max_l + 1, min_r - mid);
}

int main(void) {
    if (freopen("spiridusi.in", "r", stdin) == NULL) return 0;
    if (freopen("spiridusi.out", "w", stdout) == NULL) return 0;

    int n = read_int();
    int q = read_int();
    if (n <= 0) return 0;

    read_string(initial_state, n);
    build(1, 1, n);

    for (int i = 0; i < q; ++i) {
        int type = read_int();
        if (type == 1) {
            int x = read_int();
            update(1, 1, n, x);
        } else if (type == 2) {
            int x = read_int();
            int y = read_int();
            Node ans = query(1, 1, n, x, y);
            write_int(ans.max_len);
            write_char('\n');
        }
    }

    flush_out();
    return 0;
}
