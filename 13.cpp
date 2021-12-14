#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#define FILENAME "input.txt"

#include <unordered_set>
// I highkey don't feel like writing a hashset from scratch today

struct pair_hash {
    template <typename T>
    size_t operator() (const std::pair<T, T>& p) const {
       return std::hash<T>()(p.first) ^ std::hash<T>()(p.second);
    }
};


typedef struct {
    bool axis;
    int pos;
} fold;


void solve() {
    freopen(FILENAME, "r", stdin);
    std::unordered_set<std::pair<int, int>, pair_hash> hs;

    fold folds[128];
    int numFolds = 0;

    char line[32];
    while (scanf("%[^\n]\n", line) != EOF) {
        if (line[0] == 'f') {
            break;
        }
        std::pair<int, int> p;
        sscanf(line, "%d,%d", &p.first, &p.second);
        hs.insert(p);
    }
    do {
        char axis; int pos;
        sscanf(line, "fold along %c=%d",
            &axis, &folds[numFolds].pos);
        folds[numFolds++].axis = axis == 'x';
    } while (scanf("%[^\n]\n", line) != EOF);

    for (int i = 0; i < numFolds; ++i) {
        fold f = folds[i];
        auto cur = hs;
        for (auto p : hs) {
            if (f.axis && p.first > f.pos) {
                cur.emplace(f.pos - (p.first - f.pos), p.second);
                cur.erase(p);
            }
            else if (!f.axis && p.second > f.pos) {
                cur.emplace(p.first, f.pos - (p.second - f.pos));
                cur.erase(p);
            }
        }
        hs = cur;
        if (i == 0) {
            printf("%d\n", hs.size());
        }
    }

    int mx = 0, my = 0;
    for (auto p : hs) {
        if (p.first > mx) mx = p.first;
        if (p.second > my) my = p.second;
    }
    for (int i = 0; i <= my; ++i) {
        for (int j = 0; j <= mx; ++j)
            putchar(hs.find({j, i}) != hs.end() ? '#' : '.');
        putchar('\n');
    }
}

int main() {
    solve();
}
