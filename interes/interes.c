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

// Tree properties
int *depth;
int *first_occurrence;
int tour_len = 0;

// Iterative Segment Tree for RMQ
int *tree;
int n_tree;

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

// DFS to compute Euler Tour, depths and first occurrences directly inside Segment Tree leaf array
void dfs(int u, int p, int d) {
    depth[u] = d;
    first_occurrence[u] = tour_len;
    tree[n_tree + tour_len++] = u;
    for (int e = head[u]; e != -1; e = next_edge[e]) {
        int v = to[e];
        if (v != p) {
            dfs(v, u, d + 1);
            tree[n_tree + tour_len++] = u;
        }
    }
}

// LCA using Iterative Segment Tree
static inline int get_lca(int u, int v) {
    int l = first_occurrence[u];
    int r = first_occurrence[v];
    if (l > r) {
        int tmp = l;
        l = r;
        r = tmp;
    }
    
    int ans = tree[n_tree + l];
    for (l += n_tree, r += n_tree + 1; l < r; l >>= 1, r >>= 1) {
        if (l & 1) {
            int cur = tree[l++];
            if (depth[cur] < depth[ans]) ans = cur;
        }
        if (r & 1) {
            int cur = tree[--r];
            if (depth[cur] < depth[ans]) ans = cur;
        }
    }
    return ans;
}

// Distance in tree
static inline int get_dist(int u, int v) {
    return depth[u] + depth[v] - 2 * depth[get_lca(u, v)];
}

// Comparison function for sorting query nodes by first_occurrence using qsort
static int compare_nodes(const void *a, const void *b) {
    int u = *(const int *)a;
    int v = *(const int *)b;
    return first_occurrence[u] - first_occurrence[v];
}

// Custom sort function optimized for small arrays
static inline void sort_query_nodes(int *arr, int n) {
    if (n == 2) {
        if (first_occurrence[arr[0]] > first_occurrence[arr[1]]) {
            int tmp = arr[0];
            arr[0] = arr[1];
            arr[1] = tmp;
        }
        return;
    }
    if (n == 3) {
        if (first_occurrence[arr[0]] > first_occurrence[arr[1]]) {
            int tmp = arr[0]; arr[0] = arr[1]; arr[1] = tmp;
        }
        if (first_occurrence[arr[1]] > first_occurrence[arr[2]]) {
            int tmp = arr[1]; arr[1] = arr[2]; arr[2] = tmp;
            if (first_occurrence[arr[0]] > first_occurrence[arr[1]]) {
                int tmp2 = arr[0]; arr[0] = arr[1]; arr[1] = tmp2;
            }
        }
        return;
    }
    if (n <= 32) {
        for (int i = 1; i < n; ++i) {
            int key = arr[i];
            int key_fo = first_occurrence[key];
            int j = i - 1;
            while (j >= 0 && first_occurrence[arr[j]] > key_fo) {
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

    // Dynamically allocate all arrays to perfectly fit the current N
    head = malloc((n + 1) * sizeof(int));
    depth = malloc((n + 1) * sizeof(int));
    first_occurrence = malloc((n + 1) * sizeof(int));
    to = malloc(2 * n * sizeof(int));
    next_edge = malloc(2 * n * sizeof(int));
    tree = malloc(4 * n * sizeof(int));

    for (int i = 1; i <= n; ++i) {
        head[i] = -1;
    }

    for (int i = 0; i < n - 1; ++i) {
        int u = read_int();
        int v = read_int();
        add_edge(u, v);
        add_edge(v, u);
    }

    // Set leaf alignment offset in iterative segment tree before DFS
    n_tree = 2 * n - 1;

    // Run DFS starting from node 1 (writes directly to tree leaf array)
    dfs(1, 1, 0);

    // Build Iterative Segment Tree
    for (int i = n_tree - 1; i > 0; --i) {
        int u = tree[i << 1];
        int v = tree[i << 1 | 1];
        tree[i] = (depth[u] < depth[v]) ? u : v;
    }

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
    free(first_occurrence);
    free(to);
    free(next_edge);
    free(tree);
    if (query_nodes) free(query_nodes);

    return 0;
}
