#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 1005
#define WORDS 16

static unsigned long long M[3][MAXN][WORDS];
static int C[3][MAXN];

static unsigned long long A_mat[MAXN][WORDS];
static int B_vec[MAXN];

static int pivot_row[MAXN];
static int solved[MAXN];

static int x[MAXN];
static int row_prev[MAXN];
static int row_curr[MAXN];
static int row_next[MAXN];

int main(void) {
    int N;
    int words;
    int i, j, w, c, r;
    int prev, curr, next;
    int final_idx;
    FILE *fin, *fout;

    fin = fopen("switch5.in", "r");
    if (!fin) {
        return 1;
    }
    if (fscanf(fin, "%d", &N) != 1) {
        fclose(fin);
        return 1;
    }
    fclose(fin);

    if (N < 1 || N > 1000) {
        return 1;
    }

    words = (N + 63) / 64;

    /* Initialize propagation state */
    /* M_prev (0) and C_prev (0) are all 0 */
    /* M_curr (1) is Identity, C_curr (1) is 0 */
    for (j = 1; j <= N; ++j) {
        int word = (j - 1) / 64;
        int bit = (j - 1) % 64;
        M[1][j][word] |= (1ULL << bit);
    }

    /* Perform propagation */
    for (i = 1; i <= N; ++i) {
        prev = (i - 1) % 3;
        curr = i % 3;
        next = (i + 1) % 3;

        /* Reset M[next] and C[next] */
        for (j = 1; j <= N; ++j) {
            for (w = 0; w < words; ++w) {
                M[next][j][w] = 0;
            }
            C[next][j] = 0;
        }

        for (j = 1; j <= N; ++j) {
            for (w = 0; w < words; ++w) {
                M[next][j][w] = M[curr][j][w] ^ M[prev][j][w];
                if (j > 1) {
                    M[next][j][w] ^= M[curr][j - 1][w];
                }
                if (j < N) {
                    M[next][j][w] ^= M[curr][j + 1][w];
                }
            }
            C[next][j] = C[curr][j] ^ C[prev][j] ^ 1;
            if (j > 1) {
                C[next][j] ^= C[curr][j - 1];
            }
            if (j < N) {
                C[next][j] ^= C[curr][j + 1];
            }
        }
    }

    final_idx = (N + 1) % 3;

    /* Set up linear system A_mat * x = B_vec */
    for (r = 0; r < N; ++r) {
        for (w = 0; w < words; ++w) {
            A_mat[r][w] = M[final_idx][r + 1][w];
        }
        B_vec[r] = C[final_idx][r + 1];
    }

    /* Gauss-Jordan Elimination */
    for (r = 0; r < N; ++r) {
        pivot_row[r] = -1;
        solved[r] = 0;
    }

    for (c = 0; c < N; ++c) {
        int p = -1;
        int word_c = c / 64;
        unsigned long long bit_c = 1ULL << (c % 64);

        for (r = 0; r < N; ++r) {
            if (!solved[r] && (A_mat[r][word_c] & bit_c)) {
                p = r;
                break;
            }
        }

        if (p == -1) {
            continue;
        }

        solved[p] = 1;
        pivot_row[c] = p;

        for (r = 0; r < N; ++r) {
            if (r != p && (A_mat[r][word_c] & bit_c)) {
                for (w = 0; w < words; ++w) {
                    A_mat[r][w] ^= A_mat[p][w];
                }
                B_vec[r] ^= B_vec[p];
            }
        }
    }

    /* Assign solution for row 1 variables */
    for (c = 0; c < N; ++c) {
        int p = pivot_row[c];
        if (p != -1) {
            x[c + 1] = B_vec[p];
        } else {
            x[c + 1] = 0;
        }
    }

    /* Write output to file */
    fout = fopen("switch5.out", "w");
    if (!fout) {
        return 1;
    }

    /* Reconstruct and print each row */
    for (j = 1; j <= N; ++j) {
        row_curr[j] = x[j];
        row_prev[j] = 0;
        fprintf(fout, "%d", row_curr[j]);
    }
    fprintf(fout, "\n");

    for (i = 1; i < N; ++i) {
        for (j = 1; j <= N; ++j) {
            row_next[j] = row_curr[j] ^ row_prev[j] ^ 1;
            if (j > 1) {
                row_next[j] ^= row_curr[j - 1];
            }
            if (j < N) {
                row_next[j] ^= row_curr[j + 1];
            }
            fprintf(fout, "%d", row_next[j]);
        }
        fprintf(fout, "\n");

        for (j = 1; j <= N; ++j) {
            row_prev[j] = row_curr[j];
            row_curr[j] = row_next[j];
        }
    }

    fclose(fout);
    return 0;
}
