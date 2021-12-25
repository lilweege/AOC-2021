#include <stdio.h>
#include <string.h>
#include <assert.h>

#define GRID_SZ 1000
int grid[GRID_SZ][GRID_SZ];
int gridDiag[GRID_SZ][GRID_SZ];

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void solve() {
    memset(grid, 0, GRID_SZ*GRID_SZ*sizeof(int));
    memset(gridDiag, 0, GRID_SZ*GRID_SZ*sizeof(int));

    int x1, y1, x2, y2;
    while (scanf("%d,%d -> %d,%d", &x1, &y1, &x2, &y2) != EOF) {
        if (x1 == x2) {
            if (y1 > y2) {
                swap(&y1, &y2);
            }
            for (int i = y1; i <= y2; ++i) {
                ++grid[i][x1];
            }
        }
        else if (y1 == y2) {
            if (x1 > x2) {
                swap(&x1, &x2);
            }
            for (int j = x1; j <= x2; ++j) {
                ++grid[y1][j];
            }
        }
        else {
            int n = y1 - y2;
            if (n < 0) {
                n *= -1;
            }
            int di = y1 < y2 ? 1 : -1;
            int dj = x1 < x2 ? 1 : -1;
            int i = y1, j = x1;
            for (int x = 0; x <= n; ++x) {
                ++gridDiag[i][j];
                i += di;
                j += dj;
            }
        }
    }

    int cnt1 = 0, cnt2 = 0;
    for (int i = 0; i < GRID_SZ; ++i) {
        for (int j = 0; j < GRID_SZ; ++j) {
            cnt1 += grid[i][j] > 1;
            cnt2 += (grid[i][j] + gridDiag[i][j]) > 1;
        }
    }
    printf("%d\n%d\n", cnt1, cnt2);
}

int main() {
    solve();
}
