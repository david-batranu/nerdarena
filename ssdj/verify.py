import random
import sys

def solve_naive(N, A):
    ans = 0
    # A is list of strings
    for r1 in range(N):
        for c1 in range(N):
            for r2 in range(r1 + 1, N):
                for c2 in range(c1 + 1, N):
                    v_tl = ord(A[r1][c1])
                    v_br = ord(A[r2][c2])
                    val = min(v_tl, v_br)
                    # check all other elements in submatrix are <= val - 1
                    ok = True
                    for r in range(r1, r2 + 1):
                        for c in range(c1, c2 + 1):
                            if (r == r1 and c == c1) or (r == r2 and c == c2):
                                continue
                            if ord(A[r][c]) >= val:
                                ok = False
                                break
                        if not ok:
                            break
                    if ok:
                        ans += 1
    return ans

def solve_optimized(N, A):
    ans = 0
    grid = [[ord(c) for c in row] for row in A]
    
    for ch_val in range(ord('b'), ord('z') + 1):
        # build left_one
        left_one = [[-1] * N for _ in range(N)]
        for r in range(N):
            last = -1
            for c in range(N):
                left_one[r][c] = last
                if grid[r][c] >= ch_val:
                    last = c
                    
        for r2 in range(N):
            for c2 in range(N):
                if grid[r2][c2] >= ch_val:
                    c_min = left_one[r2][c2] + 1
                    for r in range(r2 - 1, -1, -1):
                        c_cand = left_one[r][c2]
                        if c_cand >= c_min:
                            if grid[r][c2] < ch_val:
                                if grid[r][c_cand] == ch_val or grid[r2][c2] == ch_val:
                                    ans += 1
                        if grid[r][c2] >= ch_val:
                            c_min = c2
                        elif c_cand != -1:
                            c_min = max(c_min, c_cand + 1)
                        if c_min >= c2:
                            break
    return ans

def main():
    # Run random test cases
    for t in range(100):
        N = random.randint(2, 15)
        # Generate grid with random letters
        # We can also restrict letters to 'a', 'b', 'c' to increase chance of matches
        A = []
        for _ in range(N):
            row = "".join(random.choice("abcde") for _ in range(N))
            A.append(row)
            
        ans_naive = solve_naive(N, A)
        ans_opt = solve_optimized(N, A)
        if ans_naive != ans_opt:
            print(f"FAILED on test {t}:")
            print(f"N = {N}")
            for row in A:
                print(row)
            print(f"Naive: {ans_naive}, Optimized: {ans_opt}")
            sys.exit(1)
            
    print("ALL TESTS PASSED!")

if __name__ == "__main__":
    main()
