#include <stdio.h>
#include <string.h>
#include <assert.h>

#define FILENAME "input.txt"
typedef long long ll;

void solve(int numDays) {
    ll totalFish = 0;
    ll numFish[7], diff[7];
    memset(numFish, 0, 7 * sizeof(ll));
    memset(diff, 0, 7 * sizeof(ll));

    for (int curr; scanf("%d,", &curr) != EOF;) {
        ++numFish[curr];
    }
    for (int i = 0; i < 7; ++i) {
        totalFish += numFish[i];
    }
    for (int day = 0; day < numDays; ++day) {
        numFish[day % 7] += diff[day % 7];
        totalFish += numFish[day % 7];
        if (day > 1) {
            int x = (day - 2) % 7;
            diff[day % 7] = numFish[x];
        }
    }
    
    printf("%lld\n", totalFish);
}


void part1() {
    freopen(FILENAME, "r", stdin);
    solve(80);
}

void part2() {
    freopen(FILENAME, "r", stdin);
    solve(256);
}

int main() {
    part1();
    part2();
}
