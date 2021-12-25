#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAXN 256
char grid[MAXN][MAXN];
int N, M;

typedef struct {
    int i, j;
} pair;

void solve() {
    N = 0;
    while (scanf("%s", grid[N]) != EOF) {
        ++N;
    }
    M = strlen(grid[0]);
    
    // save moves to avoid copying grid
    pair moves[4096];
    int numMoves;
    int iter = 0;
    bool done = false;
    for (; !done; ++iter) {
        done = true;

        numMoves = 0;
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j)
                if (grid[i][j] == '>' && grid[i][(j+1)%M] == '.')
                    moves[numMoves++] = (pair) { i, j };
        done &= numMoves == 0;
        for (int m = 0; m < numMoves; ++m) {
            int i = moves[m].i, j = moves[m].j;
            grid[i][j] = '.';
            grid[i][(j+1)%M] = '>';
        }
        
        numMoves = 0;
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j)
                if (grid[i][j] == 'v' && grid[(i+1)%N][j] == '.')
                    moves[numMoves++] = (pair) { i, j };
        done &= numMoves == 0;
        for (int m = 0; m < numMoves; ++m) {
            int i = moves[m].i, j = moves[m].j;
            grid[i][j] = '.';
            grid[(i+1)%N][j] = 'v';
        }
    }
    printf("%d\n", iter);
}

int main() {
    solve();
}
