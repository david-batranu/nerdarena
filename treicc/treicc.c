#include <stdio.h>
#include <stdlib.h>

/* DP table: dp[rem][p][c]
   rem: 0 to 8
   p: 0 to 1
   c: 1 to 2
*/
static long dp[9][2][3];

static void precompute(int N) {
    int rem, p, c;
    /* Initialize base cases for rem = 0 */
    for (p = 0; p <= 1; ++p) {
        for (c = 1; c <= 2; ++c) {
            dp[0][p][c] = 1;
        }
    }
    
    for (rem = 1; rem <= N; ++rem) {
        for (p = 0; p <= 1; ++p) {
            for (c = 1; c <= 2; ++c) {
                long ways = 0;
                int np;
                for (np = 0; np <= 1; ++np) {
                    if (np == p) {
                        if (c < 2) {
                            ways += 5 * dp[rem - 1][np][2];
                        }
                    } else {
                        ways += 5 * dp[rem - 1][np][1];
                    }
                }
                dp[rem][p][c] = ways;
            }
        }
    }
}

static void solve_one(int N, long M, int *digits) {
    int i, d;
    int cur_p = -1, cur_c = 0;
    
    /* First digit: 1 to 9 */
    for (d = 1; d <= 9; ++d) {
        int p = d % 2;
        long W = dp[N - 1][p][1];
        if (M <= W) {
            digits[0] = d;
            cur_p = p;
            cur_c = 1;
            break;
        }
        M -= W;
    }
    
    /* Subsequent digits: 0 to 9 */
    for (i = 1; i < N; ++i) {
        int rem = N - 1 - i;
        for (d = 0; d <= 9; ++d) {
            int np = d % 2;
            if (np == cur_p && cur_c == 2) {
                continue;
            }
            
            {
                long W = 0;
                if (np == cur_p) {
                    W = dp[rem][np][2];
                } else {
                    W = dp[rem][np][1];
                }
                
                if (M <= W) {
                    digits[i] = d;
                    cur_c = (np == cur_p) ? 2 : 1;
                    cur_p = np;
                    break;
                }
                M -= W;
            }
        }
    }
}

int main(void) {
    FILE *fin, *fout;
    int N, K, i, j;
    long A[5];
    int digits[10];
    
    fin = fopen("treicc.in", "r");
    if (!fin) {
        return 1;
    }
    
    if (fscanf(fin, "%d %d", &N, &K) != 2) {
        fclose(fin);
        return 1;
    }
    
    for (i = 0; i < K; ++i) {
        if (fscanf(fin, "%ld", &A[i]) != 1) {
            fclose(fin);
            return 1;
        }
    }
    fclose(fin);
    
    precompute(N);
    
    fout = fopen("treicc.out", "w");
    if (!fout) {
        return 1;
    }
    
    for (i = 0; i < K; ++i) {
        solve_one(N, A[i], digits);
        for (j = 0; j < N; ++j) {
            fprintf(fout, "%d", digits[j]);
        }
        fprintf(fout, "\n");
    }
    
    fclose(fout);
    return 0;
}
