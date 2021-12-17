#include <stdio.h>
#include <string.h>

#define FILENAME "input/14.txt"

typedef unsigned long long ull;

void solve(int numIters) {
    freopen(FILENAME, "r", stdin);
    // thankfully the alphabet is small
    char rules[26][26];
    ull pairCnt[26][26], nextPairCnt[26][26];
    ull charCnt[26];
    memset(rules, -1, 26*26);
    memset(pairCnt, 0, 26*26*sizeof(ull));
    memset(charCnt, 0, 26*sizeof(ull));

    char polymer[32], line[32];
    int n = 0;
    scanf("%[^\n]%n\n", polymer, &n);
    for (int i = 0; i < n-1; ++i) {
        pairCnt[polymer[i]-'A'][polymer[i+1]-'A']++;
        charCnt[polymer[i]-'A']++;
    }
    charCnt[polymer[n-1]-'A']++;

    while (scanf("%[^\n]\n", line) != EOF) {
        char a, b, c;
        sscanf(line, "%c%c -> %c", &a, &b, &c);
        rules[a-'A'][b-'A'] = c-'A';
    }

    for (int iter = 0; iter < numIters; ++iter) {
        memcpy(nextPairCnt, pairCnt, 26*26*sizeof(ull));
        for (int a = 0; a < 26; ++a)
            for (int b = 0; b < 26; ++b) {
                char c = rules[a][b];
                if (c == -1) continue;
                charCnt[c] += pairCnt[a][b];
                ull cnt = pairCnt[a][b];
                nextPairCnt[a][c] += cnt;
                nextPairCnt[c][b] += cnt;
                nextPairCnt[a][b] -= cnt;
            }
        memcpy(pairCnt, nextPairCnt, 26*26*sizeof(ull));
    }

    ull ma = 0, mi = 1LL<<62;
    for (int i = 0; i < 26; ++i) {
        ull cnt = charCnt[i];
        if (cnt == 0) continue;
        if (cnt > ma) ma = cnt;
        if (cnt < mi) mi = cnt;
    }
    printf("%llu\n", ma - mi);
}

int main() {
    solve(10);
    solve(40);
}
