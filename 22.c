#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#define FILENAME "input/22.txt"

typedef long long ll;
typedef struct {
    bool on;
    int x0, x1, y0, y1, z0, z1;
} step;

int cmpInt(const void *a, const void *b) {
    return *((int*)a) - *((int*)b);
}

int bSearch(int* a, int n, int x) {
    int l = 0, r = n-1;
    while (l < r) {
        int m = (l + r) / 2;
        if (a[m] == x)
            return m;
        if (a[m] < x)
            l = m+1;
        else
            r = m-1;
    }
    return l;
}


#define MAXN 1024
bool grid[MAXN][MAXN][MAXN];
step steps[MAXN/2];
int x[MAXN], y[MAXN], z[MAXN];
int numSteps = 0;

// TODO: optimize this
// it currently takes ~1 second on my pc atm
// coordinate compression is probably not suitable for the given input
// (there are less larger cuboids rather than more smaller cuboids)
// perhaps maintaining a list of all cuboids and breaking up intersecting
// cuboids into smaller discrete cuboids whenever a collision occurs would be faster

void solve() {
    freopen(FILENAME, "r", stdin);
    for (char on[4]; scanf("%[^ ] ", on) != EOF; ) {
        step* s = &steps[numSteps];
        s->on = on[1] == 'n';
        scanf("x=%d..%d,y=%d..%d,z=%d..%d\n", &s->x0, &s->x1, &s->y0, &s->y1, &s->z0, &s->z1);
        x[numSteps*2] = s->x0;
        y[numSteps*2] = s->y0;
        z[numSteps*2] = s->z0;
        x[numSteps*2+1] = ++s->x1;
        y[numSteps*2+1] = ++s->y1;
        z[numSteps*2+1] = ++s->z1;
        ++numSteps;
    }
    int N = numSteps*2;
    qsort(x, N, sizeof(int), cmpInt);
    qsort(y, N, sizeof(int), cmpInt);
    qsort(z, N, sizeof(int), cmpInt);
    
    // this is still slow when N is even a couple hundred
    for (int i = 0; i < numSteps; ++i) {
        step s = steps[i];
        int x0 = bSearch(x, N, s.x0);
        int x1 = bSearch(x, N, s.x1);
        int y0 = bSearch(y, N, s.y0);
        int y1 = bSearch(y, N, s.y1);
        int z0 = bSearch(z, N, s.z0);
        int z1 = bSearch(z, N, s.z1);
        for (int i = x0; i < x1; ++i)
            for (int j = y0; j < y1; ++j)
                for (int k = z0; k < z1; ++k)
                    grid[i][j][k] = s.on;
    }
    
    int ans1 = 0;
    ll ans2 = 0;
    for (int i = 0; i < N-1; ++i)
        for (int j = 0; j < N-1; ++j)
            for (int k = 0; k < N-1; ++k)
                if (grid[i][j][k]) {
                    ll num = 1LL * (x[i+1]-x[i]) * (y[j+1]-y[j]) * (z[k+1]-z[k]);
                    ans1 += (x[i] >= -50 && x[i] <= 50 &&
                                y[j] >= -50 && y[j] <= 50 &&
                                z[k] >= -50 && z[k] <= 50) * num;
                    ans2 += num;
                }
    printf("%d %lld\n", ans1, ans2);
}

int main() {
    solve();
}
