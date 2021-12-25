#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef long long ll;

void solve(int numDays1, int numDays2) {
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
    for (int day = 0; day < numDays2; ++day) {
        if (day == numDays1) {
            printf("%lld\n", totalFish);
        }
        numFish[day % 7] += diff[day % 7];
        totalFish += numFish[day % 7];
        if (day > 1) {
            int x = (day - 2) % 7;
            diff[day % 7] = numFish[x];
        }
    }
    
    printf("%lld\n", totalFish);
}


int main() {
    solve(80, 256);
}
