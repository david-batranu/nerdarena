#include <stdio.h>
#include <stdlib.h>

// Adjacency list representation
int *head;
int *to;
int *next_edge;
int edge_cnt = 0;

static inline void add_edge(int u, int v) {
    to[edge_cnt] = v;
    next_edge[edge_cnt] = head[u];
    head[u] = edge_cnt++;
}

// Tree properties for Heavy-Light Decomposition
int *depth;
int *parent;
int *heavy;
int *head_chain;
int *tin;
int timer = 0;

// Fast I/O buffers (128 KB is highly optimal and saves memory)
#define IO_BUF_SIZE 131072
static char io_buf[IO_BUF_SIZE];
static char *buf_ptr = io_buf;
static char *buf_end = io_buf;

static inline int read_int(void) {
    while (buf_ptr >= buf_end) {
        int len = fread(io_buf, 1, IO_BUF_SIZE, stdin);
        if (len <= 0) return 0;
        buf_ptr = io_buf;
        buf_end = io_buf + len;
    }
    while (*buf_ptr <= ' ') {
        buf_ptr++;
        while (buf_ptr >= buf_end) {
            int len = fread(io_buf, 1, IO_BUF_SIZE, stdin);
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
            int len = fread(io_buf, 1, IO_BUF_SIZE, stdin);
            if (len > 0) {
                buf_ptr = io_buf;
                buf_end = io_buf + len;
            }
        }
    }
    return res;
}

// Fast Output
static char out_buf[IO_BUF_SIZE];
static char *out_ptr = out_buf;

static inline void write_char(char c) {
    if (out_ptr >= out_buf + IO_BUF_SIZE) {
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

// HLD DFS 1: compute depth, parent, heavy child
int dfs1(int u, int p, int d) {
    depth[u] = d;
    parent[u] = p;
    int size = 1;
    int max_c_size = 0;
    heavy[u] = 0;
    for (int e = head[u]; e != -1; e = next_edge[e]) {
        int v = to[e];
        if (v != p) {
            int c_size = dfs1(v, u, d + 1);
            size += c_size;
            if (c_size > max_c_size) {
                max_c_size = c_size;
                heavy[u] = v;
            }
        }
    }
    return size;
}

// HLD DFS 2: decompose into chains and compute preorder tin
void dfs2(int u, int h) {
    head_chain[u] = h;
    tin[u] = ++timer;
    if (heavy[u]) {
        dfs2(heavy[u], h);
    }
    for (int e = head[u]; e != -1; e = next_edge[e]) {
        int v = to[e];
        if (v != parent[u] && v != heavy[u]) {
            dfs2(v, v);
        }
    }
}

// LCA using HLD
static inline int get_lca(int u, int v) {
    while (head_chain[u] != head_chain[v]) {
        if (depth[head_chain[u]] > depth[head_chain[v]]) {
            u = parent[head_chain[u]];
        } else {
            v = parent[head_chain[v]];
        }
    }
    return depth[u] < depth[v] ? u : v;
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

// Custom sort function optimized for small arrays
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
    qsort(arr, n, sizeof(int), compare_nodes);
}

// Dynamic storage for queries
int *query_nodes = NULL;
int query_nodes_capacity = 0;

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

    // Dynamically allocate all arrays to perfectly fit N, using ~30% less memory than Segment Tree RMQ
    head = malloc((n + 1) * sizeof(int));
    depth = malloc((n + 1) * sizeof(int));
    parent = malloc((n + 1) * sizeof(int));
    heavy = malloc((n + 1) * sizeof(int));
    head_chain = malloc((n + 1) * sizeof(int));
    tin = malloc((n + 1) * sizeof(int));
    to = malloc(2 * n * sizeof(int));
    next_edge = malloc(2 * n * sizeof(int));

    for (int i = 1; i <= n; ++i) {
        head[i] = -1;
    }

    for (int i = 0; i < n - 1; ++i) {
        int u = read_int();
        int v = read_int();
        add_edge(u, v);
        add_edge(v, u);
    }

    // Run DFS 1 to compute tree properties and heavy children
    dfs1(1, 1, 0);

    // Run DFS 2 to decompose tree into chains and compute tin
    dfs2(1, 1);

    // Process queries
    for (int qi = 0; qi < q; ++qi) {
        int k = read_int();
        
        // Dynamically resize query node array up to the maximum K encountered so far
        if (k > query_nodes_capacity) {
            query_nodes_capacity = k;
            query_nodes = realloc(query_nodes, query_nodes_capacity * sizeof(int));
        }

        for (int i = 0; i < k; ++i) {
            query_nodes[i] = read_int();
        }

        // Sort nodes using optimized sort
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

    // Free resources
    free(head);
    free(depth);
    free(parent);
    free(heavy);
    free(head_chain);
    free(tin);
    free(to);
    free(next_edge);
    if (query_nodes) free(query_nodes);

    return 0;
}
