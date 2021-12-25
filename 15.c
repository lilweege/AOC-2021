#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef struct {
    int a, b, c;
} tuple;

int cmp(tuple p, tuple q) {
    return p.c - q.c;
}

#define HEAP_SZ 10000
typedef struct {
    tuple data[HEAP_SZ];
    int size;
} arr;

void upheapify(arr* h, int idx) {
    int par = (idx-1)/2;
    if (cmp(h->data[par], h->data[idx]) > 0) {
        tuple tmp = h->data[par];
        h->data[par] = h->data[idx];
        h->data[idx] = tmp;
        upheapify(h, par);
    }
}

void downheapify(arr* h, int par) {
    int l = par*2+1, r = par*2+2;
    if (l >= h->size || l < 0) l = -1;
    if (r >= h->size || r < 0) r = -1;
    int min = (l != -1 && cmp(h->data[l], h->data[par]) < 0) ? l : par;
    if (r != -1 && cmp(h->data[r], h->data[min]) < 0) min = r;
    if (min != par) {
        tuple tmp = h->data[min];
        h->data[min] = h->data[par];
        h->data[par] = tmp;
        downheapify(h, min);
    }
}

void heappush(arr* heap, tuple x) {
    assert(heap->size+1 < HEAP_SZ);
    heap->data[heap->size++] = x;
    upheapify(heap, heap->size-1);
}

tuple heappop(arr* heap) {
    assert(heap->size > 0);
    tuple x = heap->data[0];
    heap->data[0] = heap->data[--heap->size];
    downheapify(heap, 0);
    return x;
}

int b1Mod(int x, int m) { return (x - 1) % m + 1; }


#define MAXN 100
int di[4] = { 1, 0, -1, 0 };
int dj[4] = { 0, 1, 0, -1 };

int rows, cols;
int grid[MAXN][MAXN];
void readInput() {
    char input[MAXN][MAXN+1];
    rows = 0;
    while (scanf("%s", (char*) &input[rows]) != EOF) {
        ++rows;
    }
    cols = strlen(input[0]);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            grid[i][j] = input[i][j] - '0';
}

void solve(int N) {
    int dist[rows*N][cols*N];
    memset(dist, 0x3f, rows*cols*N*N*sizeof(int));
    
    arr pq;
    pq.data[0] = (tuple) { 0, 0, 0 };
    pq.size = 1;
    while (pq.size > 0) {
        tuple cur = heappop(&pq);
        int i = cur.a, j = cur.b;
        int cost = cur.c;
        if (i == rows*N-1 && j == cols*N-1) {
            printf("%d\n", cost);
            return;
        }
        if (cost >= dist[i][j])
            continue;
        dist[i][j] = cost;
        for (int d = 0; d < 4; ++d) {
            int ni = i+di[d], nj = j+dj[d];
            if (0 <= ni && ni < rows*N &&
                0 <= nj && nj < cols*N
            ) {
                heappush(&pq, (tuple) {
                    ni, nj, cost +
                    b1Mod(grid[ni%rows][nj%cols]+(ni/rows)+(nj/cols), 9)
                });
            }
        }
    }
    assert(0 && "Unreachable");
}

int main() {
    readInput();
    solve(1);
    solve(5);
}
