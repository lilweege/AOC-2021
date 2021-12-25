#include <stdio.h>
#include <stdlib.h>

#define MAXN 1000

int cmpInt(const void *a, const void *b) {
    int* x = (int*) a;
    int* y = (int*) b;
    return *x - *y;
}

int cost1(int p, int c) {
    int x = p - c;
    if (x < 0) x = -x;
    return x;
}

int cost2(int p, int c) {
    int x = p - c;
    if (x < 0) x = -x;
    return x * (x+1) / 2;
}

int lo = 1 << 30, hi = -(1 << 30);
int nums[MAXN], sz = 0;
void readInput() {
    // lo is just zero, but this is more general
    while (scanf("%d,", &nums[sz]) != EOF) {
        if (hi < nums[sz]) hi = nums[sz];
        if (lo > nums[sz]) lo = nums[sz];
        ++sz;
    }
    qsort(nums, sz, sizeof(int), cmpInt);
}

void solve(int (*cost)(int, int)) {
    int l = lo, r = hi;
    while (l < r) {
        int m = (l + r) / 2;
        int diff = 0;
        for (int i = 0; i < sz; ++i) {
            diff += cost(nums[i], m-1) - cost(nums[i], m);
        }
        if (diff > 0) {
            l = m + 1;
        }
        else {
            r = m - 1;
        }
    }
    int ans = 0;
    for (int i = 0; i < sz; ++i) {
        ans += cost(nums[i], r);
    }
    
    printf("%d\n", ans);
}

int main() {
    readInput();
    solve(cost1);
    solve(cost2);
}
