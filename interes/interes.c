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

// Fast I/O
#define BUFFER_SIZE 65536
char buffer[BUFFER_SIZE];
int buffer_ptr = 0;
int buffer_len = 0;

static inline char read_char(void) {
    if (buffer_ptr >= buffer_len) {
        buffer_len = fread(buffer, 1, BUFFER_SIZE, stdin);
        buffer_ptr = 0;
        if (buffer_len == 0) {
            return EOF;
        }
    }
    return buffer[buffer_ptr++];
}

static inline int read_int(void) {
    char c = read_char();
    while (c <= ' ' && c != EOF) {
        c = read_char();
    }
    if (c == EOF) return 0;
    int res = 0;
    while (c >= '0' && c <= '9') {
        res = res * 10 + (c - '0');
        c = read_char();
    }
    return res;
}

// DFS to compute depths, entry times, and initial ancestors
void dfs(int u, int p) {
    tin[u] = ++timer;
    up[u][0] = p;
    for (int i = 1; i < LOGN; ++i) {
        up[u][i] = up[up[u][i - 1]][i - 1];
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
int get_lca(int u, int v) {
    if (depth[u] < depth[v]) {
        int tmp = u;
        u = v;
        v = tmp;
    }
    
    // Lift u to the same depth as v
    int diff = depth[u] - depth[v];
    for (int i = 0; i < LOGN; ++i) {
        if ((diff >> i) & 1) {
            u = up[u][i];
        }
    }
    
    if (u == v) return u;
    
    for (int i = LOGN - 1; i >= 0; --i) {
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

// Comparison function for sorting query nodes by tin
int compare_nodes(const void *a, const void *b) {
    int u = *(const int *)a;
    int v = *(const int *)b;
    return tin[u] - tin[v];
}

// Temporary storage for queries
int query_nodes[MAXN];

int main(void) {
    // Redirect standard files for NerdArena compatibility
    if (freopen("interes.in", "r", stdin) == NULL) {
        // Fallback to standard input if file doesn't exist
    }
    if (freopen("interes.out", "w", stdout) == NULL) {
        // Fallback to standard output if file cannot be opened
    }

    int n = read_int();
    int q = read_int();
    if (n == 0) return 0;

    // Initialize adjacency list
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

    // Process queries
    for (int qi = 0; qi < q; ++qi) {
        int k = read_int();
        for (int i = 0; i < k; ++i) {
            query_nodes[i] = read_int();
        }

        // Sort nodes by their entry time (tin)
        qsort(query_nodes, k, sizeof(int), compare_nodes);

        long long total_dist = 0;
        for (int i = 0; i < k; ++i) {
            int next_idx = (i == k - 1) ? 0 : i + 1;
            total_dist += get_dist(query_nodes[i], query_nodes[next_idx]);
        }

        printf("%lld\n", total_dist / 2);
    }

    return 0;
}
