#include <stdio.h>
#include <stdlib.h>

#define MAXN 200005
#define LOGN 18

// Adjacency list representation
int head[MAXN];
int to[MAXN * 2];
int next_edge[MAXN * 2];
int edge_cnt = 0;

static inline void add_edge(int u, int v) {
    to[edge_cnt] = v;
    next_edge[edge_cnt] = head[u];
    head[u] = edge_cnt++;
}

// Tree properties
int depth[MAXN];
int tin[MAXN];
int timer = 0;
int up[MAXN][LOGN];
int max_depth = 0;
int max_log = 0;

// Fast I/O buffers
static char io_buf[1 << 20];
static char *buf_ptr = io_buf;
static char *buf_end = io_buf;

static inline int read_int(void) {
    while (buf_ptr >= buf_end) {
        int len = fread(io_buf, 1, sizeof(io_buf), stdin);
        if (len <= 0) return 0;
        buf_ptr = io_buf;
        buf_end = io_buf + len;
    }
    while (*buf_ptr <= ' ') {
        buf_ptr++;
        while (buf_ptr >= buf_end) {
            int len = fread(io_buf, 1, sizeof(io_buf), stdin);
            if (len <= 0) return 0;
            buf_ptr = io_buf;
            buf_end = io_buf + len;
        }
    }
    int res = 0;
    while (buf_ptr < buf_end && *buf_ptr > ' ') {
        res = res * 10 + (*buf_ptr - '0');
        buf_ptr++;
        if (buf_ptr >= buf_end) {
            int len = fread(io_buf, 1, sizeof(io_buf), stdin);
            if (len > 0) {
                buf_ptr = io_buf;
                buf_end = io_buf + len;
            }
        }
    }
    return res;
}

// Fast Output
static char out_buf[1 << 20];
static char *out_ptr = out_buf;

static inline void write_char(char c) {
    if (out_ptr >= out_buf + sizeof(out_buf)) {
        fwrite(out_buf, 1, out_ptr - out_buf, stdout);
        out_ptr = out_buf;
    }
    *out_ptr++ = c;
}

static inline void write_int(long long n) {
    if (n == 0) {
        write_char('0');
        return;
    }
    char temp[25];
    int temp_ptr = 0;
    while (n > 0) {
        temp[temp_ptr++] = (n % 10) + '0';
        n /= 10;
    }
    while (temp_ptr > 0) {
        write_char(temp[--temp_ptr]);
    }
}

static inline void flush_output(void) {
    if (out_ptr > out_buf) {
        fwrite(out_buf, 1, out_ptr - out_buf, stdout);
        out_ptr = out_buf;
    }
}

// DFS to compute depths and entry times
void dfs(int u, int p) {
    tin[u] = ++timer;
    up[u][0] = p;
    if (depth[u] > max_depth) {
        max_depth = depth[u];
    }
    for (int e = head[u]; e != -1; e = next_edge[e]) {
        int v = to[e];
        if (v != p) {
            depth[v] = depth[u] + 1;
            dfs(v, u);
        }
    }
}

// LCA using binary lifting
static inline int get_lca(int u, int v) {
    if (depth[u] < depth[v]) {
        int tmp = u;
        u = v;
        v = tmp;
    }
    
    int diff = depth[u] - depth[v];
    while (diff) {
        int i = __builtin_ctz(diff);
        u = up[u][i];
        diff &= diff - 1;
    }
    
    if (u == v) return u;
    
    for (int i = max_log; i >= 0; --i) {
        if (up[u][i] != up[v][i]) {
            u = up[u][i];
            v = up[v][i];
        }
    }
    
    return up[u][0];
}

// Distance in tree
static inline int get_dist(int u, int v) {
    return depth[u] + depth[v] - 2 * depth[get_lca(u, v)];
}

// Comparison function for sorting query nodes by tin using qsort
static int compare_nodes(const void *a, const void *b) {
    int u = *(const int *)a;
    int v = *(const int *)b;
    return tin[u] - tin[v];
}

// Custom sort function optimized for small arrays & DFS preorder tin sorting
static inline void sort_query_nodes(int *arr, int n) {
    if (n == 2) {
        if (tin[arr[0]] > tin[arr[1]]) {
            int tmp = arr[0];
            arr[0] = arr[1];
            arr[1] = tmp;
        }
        return;
    }
    if (n == 3) {
        if (tin[arr[0]] > tin[arr[1]]) {
            int tmp = arr[0]; arr[0] = arr[1]; arr[1] = tmp;
        }
        if (tin[arr[1]] > tin[arr[2]]) {
            int tmp = arr[1]; arr[1] = arr[2]; arr[2] = tmp;
            if (tin[arr[0]] > tin[arr[1]]) {
                int tmp2 = arr[0]; arr[0] = arr[1]; arr[1] = tmp2;
            }
        }
        return;
    }
    if (n <= 32) {
        // Insertion sort for small sub-arrays
        for (int i = 1; i < n; ++i) {
            int key = arr[i];
            int key_tin = tin[key];
            int j = i - 1;
            while (j >= 0 && tin[arr[j]] > key_tin) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
        return;
    }
    // Fallback to standard library qsort for larger n to avoid O(n^2) worst case
    qsort(arr, n, sizeof(int), compare_nodes);
}

// Temporary storage for queries
int query_nodes[MAXN];

int main(void) {
    if (freopen("interes.in", "r", stdin) == NULL) {
        // Fallback
    }
    if (freopen("interes.out", "w", stdout) == NULL) {
        // Fallback
    }

    int n = read_int();
    int q = read_int();
    if (n == 0) return 0;

    for (int i = 1; i <= n; ++i) {
        head[i] = -1;
    }

    for (int i = 0; i < n - 1; ++i) {
        int u = read_int();
        int v = read_int();
        add_edge(u, v);
        add_edge(v, u);
    }

    // Run DFS starting from node 1
    depth[1] = 0;
    dfs(1, 1);

    // Compute max log limit
    max_log = 31 - __builtin_clz(max_depth | 1);

    // Fill binary lifting table iteratively and cache-friendly
    for (int i = 1; i <= max_log; ++i) {
        for (int u = 1; u <= n; ++u) {
            up[u][i] = up[up[u][i - 1]][i - 1];
        }
    }

    // Process queries
    for (int qi = 0; qi < q; ++qi) {
        int k = read_int();
        for (int i = 0; i < k; ++i) {
            query_nodes[i] = read_int();
        }

        // Sort nodes by tin using optimized sort
        sort_query_nodes(query_nodes, k);

        long long total_dist = 0;
        for (int i = 0; i < k; ++i) {
            int next_idx = (i == k - 1) ? 0 : i + 1;
            total_dist += get_dist(query_nodes[i], query_nodes[next_idx]);
        }

        write_int(total_dist / 2);
        write_char('\n');
    }

    flush_output();
    return 0;
}
