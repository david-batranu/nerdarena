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

// Bounded-recursion HLD DFS 2 (recursion depth bounded by log N <= 18)
void dfs2(int u, int h) {
    while (u) {
        head_chain[u] = h;
        tin[u] = ++timer;
        int hv = heavy[u];
        for (int e = head[u]; e != -1; e = next_edge[e]) {
            int v = to[e];
            if (v != parent[u] && v != hv) {
                dfs2(v, v); // Recurse on light children (maximum depth log N)
            }
        }
        u = hv; // Tail call optimization for heavy child (0 stack overhead)
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

    // Dynamically allocate tree arrays
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

    // BFS Top-Down to compute depth and parent (0 stack memory)
    int *queue = malloc((n + 1) * sizeof(int));
    int q_head = 0, q_tail = 0;
    queue[q_tail++] = 1;
    depth[1] = 0;
    parent[1] = 1;

    while (q_head < q_tail) {
        int u = queue[q_head++];
        for (int e = head[u]; e != -1; e = next_edge[e]) {
            int v = to[e];
            if (v != parent[u]) {
                depth[v] = depth[u] + 1;
                parent[v] = u;
                queue[q_tail++] = v;
            }
        }
    }

    // Bottom-Up size and heavy child computation (0 stack memory)
    int *sub_size = calloc(n + 1, sizeof(int));
    for (int i = 1; i <= n; ++i) {
        heavy[i] = 0;
    }
    for (int i = n - 1; i >= 0; --i) {
        int u = queue[i];
        sub_size[u] += 1;
        int p = parent[u];
        if (u != 1) {
            sub_size[p] += sub_size[u];
            if (heavy[p] == 0 || sub_size[u] > sub_size[heavy[p]]) {
                heavy[p] = u;
            }
        }
    }

    // Run DFS2 with dynamically bounded recursion (stack depth <= 18)
    dfs2(1, 1);

    // Free temporary memory used for BFS and sizes before query processing
    free(queue);
    free(sub_size);

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

    // Free final resources
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
