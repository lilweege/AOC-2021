#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FILENAME "input.txt"
#define MAXN 1000

void top3(int x, int* x1, int* x2, int* x3) {
    if (x >= *x1) {
        *x3 = *x2;
        *x2 = *x1;
        *x1 = x;
    }
    else if (x >= *x2) {
        *x3 = *x2;
        *x2 = x;
    }
    else if (x > *x3) {
        *x3 = x;
    }
}

int dfs(char grid[MAXN][MAXN], bool vis[MAXN][MAXN], int m, int n, int i, int j) {
    if (i < 0 || i >= n || j < 0 || j >= m || grid[i][j] >= '9' || vis[i][j]) {
        return 0;
    }
    vis[i][j] = true;
    return 1 +
        dfs(grid, vis, m, n, i-1, j) +
        dfs(grid, vis, m, n, i+1, j) +
        dfs(grid, vis, m, n, i, j-1) +
        dfs(grid, vis, m, n, i, j+1);
}

void solve() {
    freopen(FILENAME, "r", stdin);
    char grid[MAXN][MAXN];
    int n = 0;
    while (scanf("%s", grid[n]) != EOF) {
        ++n;
    }
    int m = strlen(grid[0]);
    bool vis[MAXN][MAXN];
    memset(vis, 0, MAXN*MAXN*sizeof(bool));

    int totalRisk = 0;
    int max1 = 0, max2 = 0, max3 = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            char cur = grid[i][j];
            if (i > 0   && cur >= grid[i-1][j] ||
                i < n-1 && cur >= grid[i+1][j] ||
                j > 0   && cur >= grid[i][j-1] ||
                j < m-1 && cur >= grid[i][j+1])
                continue;
            totalRisk += 1 + cur - '0';
            int size = dfs(grid, vis, m, n, i, j);
            top3(size, &max1, &max2, &max3);
        }
    }
    int largestProd = max1 * max2 * max3;
    printf("%d\n", totalRisk);
    printf("%d\n", largestProd);
}

int main() {
    solve();
}
