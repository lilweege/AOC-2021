#include <stdio.h>
#define FILENAME "input.txt"

#include <unordered_map>
// same as last day

typedef std::pair<char, char> pcc;
typedef unsigned long long ull;

struct pair_hash {
    template <typename T>
    size_t operator() (const std::pair<T, T>& p) const {
       return std::hash<T>()(p.first) ^ std::hash<T>()(p.second);
    }
};

void solve(int numIters) {
    freopen(FILENAME, "r", stdin);
    std::unordered_map<pcc, char, pair_hash> rules;
    std::unordered_map<pcc, ull, pair_hash> pairCnt;
    std::unordered_map<char, ull> charCnt;

    char polymer[32], line[32];
    int n = 0;
    scanf("%[^\n]%n\n", polymer, &n);
    for (int i = 0; i < n-1; ++i) {
        pairCnt[{polymer[i], polymer[i+1]}]++;
        charCnt[polymer[i]]++;
    }
    charCnt[polymer[n-1]]++;

    while (scanf("%[^\n]\n", line) != EOF) {
        char a, b, c;
        sscanf(line, "%c%c -> %c", &a, &b, &c);
        rules[{a, b}] = c;
    }

    for (int iter = 0; iter < numIters; ++iter) {
        auto nextPairCnt = pairCnt;
        for (auto [pair, _] : pairCnt) {
            auto it = rules.find(pair);
            if (it != rules.end()) {
                auto [ab, c] = *it;
                charCnt[c] += pairCnt[ab];
                pcc ac { ab.first, c },
                    cb { c, ab.second };
                ull cnt = pairCnt[ab];
                nextPairCnt[ac] += cnt;
                nextPairCnt[cb] += cnt;
                nextPairCnt[ab] -= cnt;
            }
        }
        pairCnt = nextPairCnt;
    }
    ull ma = 0, mi = 1LL<<62;
    for (auto [_, cnt] : charCnt) {
        if (cnt > ma) ma = cnt;
        if (cnt < mi) mi = cnt;
    }
    printf("%llu\n", ma - mi);
}

int main() {
    solve(10);
    solve(40);
}
