import random

def main():
    N = 1000
    with open("ssdj.in", "w") as f:
        f.write(f"{N}\n")
        for _ in range(N):
            row = "".join(random.choice("abcdefghijklmnopqrstuvwxyz") for _ in range(N))
            f.write(row + "\n")

if __name__ == "__main__":
    main()
