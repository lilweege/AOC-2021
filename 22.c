#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#define FILENAME "input/22.txt"
#define SOLVE_SLOW 0

typedef long long ll;
typedef struct {
#if SOLVE_SLOW
    bool on;
#endif
    int x0, x1, y0, y1, z0, z1;
} prism;


#if SOLVE_SLOW
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

#define MAXP 512
bool grid[MAXP*2][MAXP*2][MAXP*2];
int x[MAXP*2], y[MAXP*2], z[MAXP*2];
prism prisms[MAXP*2];
// this is essentially a glorified brute force solution
// in hindsight, I'm surpsised it's as fast as it is
void solve() {
    freopen(FILENAME, "r", stdin);
    int numPrisms = 0;
    for (char on[4]; scanf("%[^ ] ", on) != EOF;) {
        prism* p = &prisms[numPrisms];
        p->on = on[1] == 'n';
        scanf("x=%d..%d,y=%d..%d,z=%d..%d\n", &p->x0, &p->x1, &p->y0, &p->y1, &p->z0, &p->z1);
        x[numPrisms*2] = p->x0;
        y[numPrisms*2] = p->y0;
        z[numPrisms*2] = p->z0;
        x[numPrisms*2+1] = ++p->x1;
        y[numPrisms*2+1] = ++p->y1;
        z[numPrisms*2+1] = ++p->z1;
        ++numPrisms;
    }
    int N = numPrisms*2;
    qsort(x, N, sizeof(int), cmpInt);
    qsort(y, N, sizeof(int), cmpInt);
    qsort(z, N, sizeof(int), cmpInt);
    
    // this is still slow when N is even a couple hundred
    for (int i = 0; i < numPrisms; ++i) {
        prism p = prisms[i];
        int x0 = bSearch(x, N, p.x0);
        int x1 = bSearch(x, N, p.x1);
        int y0 = bSearch(y, N, p.y0);
        int y1 = bSearch(y, N, p.y1);
        int z0 = bSearch(z, N, p.z0);
        int z1 = bSearch(z, N, p.z1);
        for (int i = x0; i < x1; ++i)
            for (int j = y0; j < y1; ++j)
                for (int k = z0; k < z1; ++k)
                    grid[i][j][k] = p.on;
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
    printf("%d\n%lld\n", ans1, ans2);
}

#else
#define MAXP 4096

bool intersect(prism p1, prism p2) {
    return p1.x0 <= p2.x1 && p1.x1 >= p2.x0 &&
            p1.y0 <= p2.y1 && p1.y1 >= p2.y0 &&
            p1.z0 <= p2.z1 && p1.z1 >= p2.z0;
}

bool valid(prism p) {
    return p.x0 <= p.x1 && p.y0 <= p.y1 && p.z0 <= p.z1;
}

long long volume(prism p) {
    return 1LL * (p.x1-p.x0+1) * (p.y1-p.y0+1) * (p.z1-p.z0+1);
}

#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)
prism intersection(prism p1, prism p2) {
    return (prism) {
        max(p1.x0, p2.x0), min(p1.x1, p2.x1),
        max(p1.y0, p2.y0), min(p1.y1, p2.y1),
        max(p1.z0, p2.z0), min(p1.z1, p2.z1),
    };
}

void addPrismDiffs(prism* prisms, int* numPrisms, prism p1, prism p2) {
    if (!intersect(p1, p2)) {
        prisms[(*numPrisms)++] = p1;
        return;
    }
    prism hit = intersection(p1, p2);
    prism subDivisions[6] = { // wow I hate geometry
        { p1.x0, p1.x1, p1.y0, p1.y1, p1.z0, hit.z0-1 },
        { p1.x0, p1.x1, p1.y0, p1.y1, hit.z1+1, p1.z1 },
        { hit.x0, hit.x1, p1.y0, hit.y0-1, hit.z0, hit.z1 },
        { hit.x0, hit.x1, hit.y1+1, p1.y1, hit.z0, hit.z1 },
        { p1.x0, hit.x0-1, p1.y0, p1.y1, hit.z0, hit.z1 },
        { hit.x1+1, p1.x1, p1.y0, p1.y1, hit.z0, hit.z1 },
    };
    for (int i = 0; i < 6; ++i)
        if (valid(subDivisions[i]))
            prisms[(*numPrisms)++] = subDivisions[i];
}

prism buf[2][MAXP];
void solve() {
    freopen(FILENAME, "r", stdin);
    prism* prisms = buf[0];
    prism* newPrisms = buf[1];
    int numPrisms = 0, numNewPrisms = 0;
    for (char on[4]; scanf("%[^ ] ", on) != EOF;) {
        prism cur;
        scanf("x=%d..%d,y=%d..%d,z=%d..%d\n", &cur.x0, &cur.x1, &cur.y0, &cur.y1, &cur.z0, &cur.z1);
        for (int i = 0; i < numPrisms; ++i) {
            prism p = prisms[i];
            addPrismDiffs(newPrisms, &numNewPrisms, p, cur);
        }
        if (on[1] == 'n') {
            newPrisms[numNewPrisms++] = cur;
        }
        prism* tmp = prisms;
        prisms = newPrisms;
        newPrisms = tmp;
        numPrisms = numNewPrisms;
        numNewPrisms = 0;
    }

    int ans1;
    long long ans2;
    for (int i = 0; i < numPrisms; ++i) {
        prism p = prisms[i];
        long long v = volume(prisms[i]);
        ans1 += (p.x0 >= -50 && p.x1 <= 50 &&
                p.y0 >= -50 && p.y1 <= 50 &&
                p.z0 >= -50 && p.z1 <= 50) * v;
        ans2 += v;
    }
    printf("%d\n%lld\n", ans1, ans2);
}
#endif

int main() {
    solve();
}
