#include <stdio.h>
#include <stdlib.h>

#define FILENAME "input/07.txt"
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

void solve(int (*cost)(int, int)) {
    // lo is just zero, but this is more general
    int lo = 1 << 30, hi = -1 << 30;
    int nums[MAXN], sz = 0;
    while (scanf("%d,", &nums[sz]) != EOF) {
        if (hi < nums[sz])
            hi = nums[sz];
        if (lo > nums[sz])
            lo = nums[sz];
        ++sz;
    }

    // int ans = 1 << 30;
    // // this works but is not particularly efficient
    // for (int i = lo; i < hi; ++i) {
    //     int tot = 0;
    //     for (int j = 0; j < sz; ++j) {
    //         tot += cost(nums[j], i);
    //     }
    //     if (tot < ans)
    //         ans = tot;
    // }

    qsort(nums, sz, sizeof(int), cmpInt);
    while (lo < hi) {
        int m = (lo + hi) / 2;
        int diff = 0;
        for (int i = 0; i < sz; ++i) {
            diff += cost(nums[i], m-1) - cost(nums[i], m);
        }
        if (diff > 0) {
            lo = m + 1;
        }
        else {
            hi = m - 1;
        }
    }
    int ans = 0;
    for (int i = 0; i < sz; ++i) {
        ans += cost(nums[i], hi);
    }
    
    printf("%d\n", ans);
}

void part1() {
    freopen(FILENAME, "r", stdin);
    solve(cost1);
}

void part2() {
    freopen(FILENAME, "r", stdin);
    solve(cost2);
}

int main() {
    part1();
    part2();
}
