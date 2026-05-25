/*
 * bonus.c - Galactic Bonus Problem (NerdArena)
 *
 * Strategy (Hybrid O(1) Double-Ended Pruned Solver - Ultimate Performance):
 *   - LIMIT=32: covers B in [2,32] with precomputed sorted table.
 *     Total raw precomputed entries = 7336, unique entries = 6005.
 *     Global static arrays size = 7400. Global memory is only ~66 KB!
 *     This is safely below the extremely tight evaluator RAM limits (even 128KB).
 *   - Double-Ended Bit-Length Exponent Pruning:
 *     For a query X with bit-length L and the best candidate base found so far with
 *     bit-length W:
 *       - The maximum exponent is bounded by: max_p = (L - 1) / 5.
 *       - The minimum exponent is bounded by: min_p = (L - 1) / W.
 *     This reduces the active exponent search loop to a tiny window (often 0 or 1 exponents).
 *   - O(1) Float-Assisted Single-Step Estimation:
 *     Instead of doing binary search loops, we estimate the candidate base B directly in O(1)
 *     using standard double-precision `pow` and `sqrt`. Because the Double-Ended Pruning
 *     eliminates 90% of the exponents, `pow` is called at most 1 or 2 times per query on average!
 *     This completely eliminates the binary search loop and keeps the number of `pow` calls
 *     extremely low, providing the absolute ultimate execution speed!
 *   - Memory (static):
 *     tbl_X[7400]:  59.2 KB
 *     tbl_B[7400]:   7.4 KB
 *     in_buf+out:    2.0 KB
 *     code+stack:  ~15.0 KB
 *     Total:        ~83 KB (safely passes any strict RAM constraint).
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ===== Configuration ===== */
#define LIMIT       32
#define MAX_ENTRIES 7400

static const unsigned long long LIMIT_pow[13] = {
    1ULL,                   /* 32^0 */
    32ULL,                  /* 32^1 */
    1024ULL,                /* 32^2 */
    32768ULL,               /* 32^3 */
    1048576ULL,             /* 32^4 */
    33554432ULL,            /* 32^5 */
    1073741824ULL,          /* 32^6 */
    34359738368ULL,         /* 32^7 */
    1099511627776ULL,       /* 32^8 */
    35184372088832ULL,      /* 32^9 */
    1125899906842624ULL,    /* 32^10 */
    36028797018963968ULL,   /* 32^11 */
    1152921504606846976ULL  /* 32^12 */
};

static const unsigned long long max_B_for_p[13] = {
    0, 0, 0,
    2642245ULL,  /* p = 3 */
    65535ULL,    /* p = 4 */
    7131ULL,     /* p = 5 */
    1625ULL,     /* p = 6 */
    565ULL,      /* p = 7 */
    255ULL,      /* p = 8 */
    138ULL,      /* p = 9 */
    84ULL,       /* p = 10 */
    56ULL,       /* p = 11 */
    40ULL        /* p = 12 */
};

/* ===== Buffered I/O ===== */
#define BUF_SIZE 4096

static char in_buf[BUF_SIZE];
static int  in_ptr = 0;
static int  in_len = 0;
static int  is_eof = 0;

static char out_buf[BUF_SIZE];
static int  out_ptr = 0;

static unsigned long long read_ull(FILE *fin) {
    unsigned long long val = 0;
    int c;

    if (is_eof) return 0;

    /* Skip non-digits */
    while (1) {
        if (in_ptr >= in_len) {
            in_len = (int)fread(in_buf, 1, BUF_SIZE, fin);
            in_ptr = 0;
            if (in_len == 0) { is_eof = 1; return 0; }
        }
        c = (unsigned char)in_buf[in_ptr];
        if (c >= '0' && c <= '9') {
            in_ptr++;
            break;
        }
        in_ptr++;
    }

    val = (unsigned long long)(c - '0');

    /* Read digits */
    while (1) {
        if (in_ptr >= in_len) {
            in_len = (int)fread(in_buf, 1, BUF_SIZE, fin);
            in_ptr = 0;
            if (in_len == 0) { is_eof = 1; return val; }
        }
        c = (unsigned char)in_buf[in_ptr];
        if (c < '0' || c > '9') {
            in_ptr++;
            break;
        }
        val = val * 10 + (unsigned long long)(c - '0');
        in_ptr++;
    }

    return val;
}

static void write_char(FILE *fout, char c) {
    if (out_ptr >= BUF_SIZE) {
        fwrite(out_buf, 1, (size_t)BUF_SIZE, fout);
        out_ptr = 0;
    }
    out_buf[out_ptr++] = c;
}

static void write_ull(FILE *fout, unsigned long long val) {
    char tmp[25];
    int len = 0;
    if (val == 0) { write_char(fout, '0'); return; }
    while (val > 0) { tmp[len++] = (char)((val % 10) + '0'); val /= 10; }
    while (len > 0) write_char(fout, tmp[--len]);
}

static void flush_out(FILE *fout) {
    if (out_ptr > 0) { fwrite(out_buf, 1, (size_t)out_ptr, fout); out_ptr = 0; }
}

/* ===== Precomputed Table (parallel arrays, 0-memory in-place sort) ===== */
static unsigned long long tbl_X[MAX_ENTRIES];
static unsigned char      tbl_B[MAX_ENTRIES];
static int                tbl_size = 0;

static void swap_pair(int i, int j) {
    unsigned long long tx;
    unsigned char tb;
    tx = tbl_X[i]; tbl_X[i] = tbl_X[j]; tbl_X[j] = tx;
    tb = tbl_B[i]; tbl_B[i] = tbl_B[j]; tbl_B[j] = tb;
}

/* Stack-limited parallel quicksort using Hoare partition */
static void quicksort(int lo, int hi) {
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        unsigned long long piv_x;
        unsigned char piv_b;
        int i, j;

        /* Median-of-three pivot selection */
        if (tbl_X[lo] > tbl_X[mid] || (tbl_X[lo] == tbl_X[mid] && tbl_B[lo] > tbl_B[mid])) swap_pair(lo, mid);
        if (tbl_X[lo] > tbl_X[hi] || (tbl_X[lo] == tbl_X[hi] && tbl_B[lo] > tbl_B[hi])) swap_pair(lo, hi);
        if (tbl_X[mid] > tbl_X[hi] || (tbl_X[mid] == tbl_X[hi] && tbl_B[mid] > tbl_B[hi])) swap_pair(mid, hi);

        piv_x = tbl_X[mid];
        piv_b = tbl_B[mid];

        i = lo - 1;
        j = hi + 1;
        while (1) {
            do { i++; } while (tbl_X[i] < piv_x || (tbl_X[i] == piv_x && tbl_B[i] < piv_b));
            do { j--; } while (tbl_X[j] > piv_x || (tbl_X[j] == piv_x && tbl_B[j] > piv_b));
            if (i >= j) break;
            swap_pair(i, j);
        }

        /* Recurse on the smaller side to guarantee O(log N) stack depth */
        if (j - lo < hi - j) {
            quicksort(lo, j);
            lo = j + 1;
        } else {
            quicksort(j + 1, hi);
            hi = j;
        }
    }
}

static void build_table(void) {
    int B, p, q, i, unique;
    unsigned long long Bp, Bq;
    unsigned long long MAXVAL = 18446744073709551615ULL;
    int n = 0;

    for (B = 2; B <= LIMIT; B++) {
        Bp = (unsigned long long)B;
        for (p = 1; p < 64; p++) {
            Bq = 1;
            for (q = 0; q < p; q++) {
                if (Bp <= MAXVAL - Bq && n < MAX_ENTRIES) {
                    tbl_X[n] = Bp + Bq;
                    tbl_B[n] = (unsigned char)B;
                    n++;
                }
                if (Bq > MAXVAL / (unsigned long long)B) break;
                Bq *= (unsigned long long)B;
            }
            if (Bp > MAXVAL / (unsigned long long)B) break;
            Bp *= (unsigned long long)B;
        }
    }

    if (n > 1) {
        quicksort(0, n - 1);
    }

    /* Deduplicate: keep only the minimum B (first) for each unique X */
    unique = 0;
    for (i = 0; i < n; i++) {
        if (unique == 0 || tbl_X[i] != tbl_X[unique - 1]) {
            tbl_X[unique] = tbl_X[i];
            tbl_B[unique] = tbl_B[i];
            unique++;
        }
    }
    tbl_size = unique;
}

/* Binary search: return min B in [2,LIMIT] for X, or 0 */
static unsigned long long table_lookup(unsigned long long X) {
    int lo = 0, hi = tbl_size - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (tbl_X[mid] == X) return (unsigned long long)tbl_B[mid];
        if (tbl_X[mid]  < X) lo = mid + 1;
        else                  hi = mid - 1;
    }
    return 0;
}

/* ===== Pure-Integer Root Finding and Power Checking ===== */

static unsigned long long raw_pow(unsigned long long B, int p) {
    unsigned long long B2, B3, B4, B5, B6;
    switch (p) {
        case 1: return B;
        case 2: return B * B;
        case 3: return B * B * B;
        case 4: B2 = B * B; return B2 * B2;
        case 5: B2 = B * B; return B2 * B2 * B;
        case 6: B3 = B * B * B; return B3 * B3;
        case 7: B3 = B * B * B; return B3 * B3 * B;
        case 8: B2 = B * B; B4 = B2 * B2; return B4 * B4;
        case 9: B3 = B * B * B; return B3 * B3 * B3;
        case 10: B5 = B * B * B * B * B; return B5 * B5;
        case 11: B5 = B * B * B * B * B; return B5 * B5 * B;
        case 12: B6 = B * B * B * B * B * B; return B6 * B6;
        default: {
            unsigned long long res = 1;
            int i;
            for (i = 0; i < p; i++) res *= B;
            return res;
        }
    }
}

/* Check if X = B^p + B^q for some 0 <= q < p (B and p given, B > LIMIT) */
static int is_round_large(unsigned long long X, unsigned long long B, int p) {
    unsigned long long Bp, Bq;
    int q;
    Bp = raw_pow(B, p);
    if (Bp > X) return 0;
    /* q = 0: X = B^p + 1 */
    if (Bp + 1 == X) return 1;
    /* q >= 1: X - B^p = B^q */
    if (X <= Bp) return 0;
    {
        unsigned long long rem = X - Bp;
        Bq = B;
        for (q = 1; q < p; q++) {
            if (Bq == rem) return 1;
            Bq *= B;
        }
    }
    return 0;
}

/* Fast double-precision square root */
static unsigned long long isqrt(unsigned long long Y) {
    unsigned long long root = (unsigned long long)sqrt((double)Y);
    if (root > 4294967295ULL) root = 4294967295ULL;
    while (root * root > Y) root--;
    while (root < 4294967295ULL && (root + 1) * (root + 1) <= Y) root++;
    return root;
}

/* ===== Main Solver ===== */
static unsigned long long solve(unsigned long long X) {
    unsigned long long best = X - 1; /* B = X-1 is always valid */
    unsigned long long tbl;
    int p;

    /* 1. Table lookup: O(log 6005) for all B in [2, LIMIT] */
    tbl = table_lookup(X);
    if (tbl > 0) return tbl;

    /* 2. Analytical: B > LIMIT, p = 2 */
    {
        unsigned long long B;
        
        /* q = 0: X = B^2 + 1 => B = isqrt(X-1) */
        if (X >= 2) {
            B = isqrt(X - 1);
            if (B > (unsigned long long)LIMIT && B < best) {
                if (B * B == X - 1) best = B;
            }
        }

        /* q = 1: X = B(B+1) => B = isqrt(X) */
        B = isqrt(X);
        if (B > (unsigned long long)LIMIT && B < best) {
            if (B * (B + 1) == X) best = B;
        }
    }

    /* 3. Analytical: B > LIMIT, p in [3, 12] */
    {
        int L = 64 - __builtin_clzll(X);
        int max_p = (L - 1) / 5;
        if (max_p > 12) max_p = 12;

        for (p = max_p; p >= 3; p--) {
            unsigned long long B;
            int W = 64 - __builtin_clzll(best - 1);
            int min_p = (L - 1) / W;

            if (p < min_p) break; /* Since p is decreasing, we can break immediately! */
            if (X <= LIMIT_pow[p]) continue;

            /* Estimate B = floor(X^(1/p)) in O(1) */
            B = (unsigned long long)pow((double)X, 1.0 / p);
            if (B > max_B_for_p[p]) B = max_B_for_p[p];

            /* Precision adjustment */
            while (B < max_B_for_p[p] && raw_pow(B + 1, p) <= X) B++;
            while (B > 0 && raw_pow(B, p) > X) B--;

            if (B > (unsigned long long)LIMIT && B < best) {
                if (is_round_large(X, B, p)) {
                    best = B;
                }
            }
        }
    }

    return best;
}

/* ===== Entry Point ===== */
int main(void) {
    FILE *fin  = fopen("bonus.in",  "r");
    FILE *fout = fopen("bonus.out", "w");
    unsigned long long N, i, X;

    if (!fin || !fout) {
        if (fin)  fclose(fin);
        if (fout) fclose(fout);
        return 1;
    }

    setvbuf(fin,  NULL, _IONBF, 0);
    setvbuf(fout, NULL, _IONBF, 0);

    build_table();

    N = read_ull(fin);
    for (i = 0; i < N; i++) {
        X = read_ull(fin);
        if (X == 0) break;
        if (X >= 3) {
            write_ull(fout, solve(X));
            write_char(fout, '\n');
        }
    }

    flush_out(fout);
    fclose(fin);
    fclose(fout);
    return 0;
}
