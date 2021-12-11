#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define FILENAME "input.txt"
#define N 10
#define ITERS 100

typedef struct {
    int i, j;
} pair;

void solve() {
    freopen(FILENAME, "r", stdin);

    char energy[N][N+1];
    for (int i = 0; i < N; ++i) {
        scanf("%s", energy[i]);
    }
    bool flashed[N][N];
    int totFlashes = 0;
    pair q[N*N];
    for (int it = 1;; ++it) {
        int back = 0;
        int front = 0;
        memset(flashed, 0, N*N*sizeof(bool));
        
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (++energy[i][j] > '9') {
                    q[back++ % (N*N)] = (pair) { i, j };
                }
            }
        }
        int numFlashes = 0;
        while (front != back) {
            pair p = q[front++ % (N*N)];
            int i = p.i, j = p.j;
            if (flashed[i][j]) {
                continue;
            }
            flashed[i][j] = true;
            ++numFlashes;
            energy[i][j] = '0';
            for (int y = i-1; y <= i+1; ++y) {
                for (int x = j-1; x <= j+1; ++x) {
                    if (y >= 0 && y < N && x >= 0 && x < N &&
                        !flashed[y][x] &&
                        ++energy[y][x] > '9'
                    ) {
                        q[back++ % (N*N)] = (pair) { y, x };
                    }
                }
            }
        }

        totFlashes += numFlashes;
        if (it == ITERS) {
            printf("%d\n", totFlashes);
        }
        if (numFlashes == N*N) {
            printf("%d\n", it);
            break;
        }
    }
}

int main() {
    solve();
}
