#include <stdio.h>

#define MAXN 1005

/* Global buffers to avoid stack allocation/overflow and malloc */
static char A[MAXN][MAXN];
static short left_one[MAXN][MAXN];

/* Fast I/O buffers */
static char buf[1 << 16];
static size_t buf_ptr = 0;
static size_t buf_len = 0;

static char next_char(FILE *in) {
    if (buf_ptr >= buf_len) {
        buf_len = fread(buf, 1, sizeof(buf), in);
        buf_ptr = 0;
        if (buf_len == 0) {
            return EOF;
        }
    }
    return buf[buf_ptr++];
}

static int read_int(FILE *in) {
    int val = 0;
    char c = next_char(in);
    while (c != EOF && (c < '0' || c > '9')) {
        c = next_char(in);
    }
    while (c != EOF && c >= '0' && c <= '9') {
        val = val * 10 + (c - '0');
        c = next_char(in);
    }
    return val;
}

int main(void) {
    FILE *in = fopen("ssdj.in", "r");
    FILE *out = fopen("ssdj.out", "w");
    int N, r, c, ch_val, r2, c2;
    long long ans = 0;

    if (!in || !out) {
        if (in) fclose(in);
        if (out) fclose(out);
        return 0;
    }

    N = read_int(in);

    /* Read matrix, skipping whitespace */
    for (r = 0; r < N; r++) {
        for (c = 0; c < N; c++) {
            char ch = next_char(in);
            while (ch != EOF && (ch < 'a' || ch > 'z')) {
                ch = next_char(in);
            }
            A[r][c] = ch;
        }
    }

    /* Core logic */
    for (ch_val = 'b'; ch_val <= 'z'; ch_val++) {
        /* Build left_one for current ch_val */
        for (r = 0; r < N; r++) {
            short last = -1;
            for (c = 0; c < N; c++) {
                left_one[r][c] = last;
                if (A[r][c] >= ch_val) {
                    last = c;
                }
            }
        }

        /* Count valid submatrices */
        for (r2 = 0; r2 < N; r2++) {
            for (c2 = 0; c2 < N; c2++) {
                if (A[r2][c2] >= ch_val) {
                    int c_min = left_one[r2][c2] + 1;
                    for (r = r2 - 1; r >= 0; r--) {
                        int c_cand = left_one[r][c2];
                        if (c_cand >= c_min) {
                            if (A[r][c2] < ch_val) {
                                if (A[r][c_cand] == ch_val || A[r2][c2] == ch_val) {
                                    ans++;
                                }
                            }
                        }
                        if (A[r][c2] >= ch_val) {
                            c_min = c2;
                        } else if (c_cand != -1) {
                            if (c_cand + 1 > c_min) {
                                c_min = c_cand + 1;
                            }
                        }
                        if (c_min >= c2) {
                            break;
                        }
                    }
                }
            }
        }
    }

    fprintf(out, "%lld\n", ans);

    fclose(in);
    fclose(out);
    return 0;
}
