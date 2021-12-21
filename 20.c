#include <stdio.h>
#include <assert.h>
#include <string.h>

#define FILENAME "input/20.txt"

#define MAXIT 51
#define N 100
#define MAXN (N+2*MAXIT)

char grid[N][N];
char grids[2][MAXN][MAXN];
char algo[512];

void readInput() {
    freopen(FILENAME, "r", stdin);
    scanf("%s", algo);
    for (int i = 0; i < N; ++i)
        assert(scanf("%s", grid[i]) != EOF);
}

void solve(int iters) {
    assert(iters < MAXIT);
    if (iters & 1) {
        puts("inf");
        return;
    }
    memset(grids, '.', sizeof(grids));
    for (int i = 0; i < N; ++i)
        memcpy(&grids[0][i+MAXIT][MAXIT], grid[i], N);
    
    for (int it = 0; it < iters; ++it) {
        char allOn = it & 1;
        for (int i = MAXIT-it-1; i < MAXIT+it+N+1; ++i)
            for (int j = MAXIT-it-1; j < MAXIT+it+N+1; ++j) {
                int idx = 0;
                for (int ni = i-1; ni <= i+1; ++ni)
                    for (int nj = j-1; nj <= j+1; ++nj)
                        idx = (idx << 1) | ((grids[allOn][ni][nj] == '#') != allOn);
                grids[!allOn][i][j] = allOn ? algo[idx] : (algo[idx] == '#' ? '.' : '#');
            }
    }

    int cnt = 0;
    for (int i = MAXIT-iters; i < MAXIT+iters+N; ++i)
        for (int j = MAXIT-iters; j < MAXIT+iters+N; ++j)
            cnt += grids[0][i][j] == '#';
    printf("%d\n", cnt);
}

int main() {
    readInput();
    solve(2);
    solve(50);
}
