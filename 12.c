#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

// this is probably the most tedious problem to solve in c
// adjacency list of strings 😻
#define MAX_LINES 32
#define MAX_STRLEN 8

#define LIST_CAP 16
typedef struct {
    int lst[LIST_CAP];
    int sz;
} list;

void pb(list* l, int s) {
    assert(l->sz < LIST_CAP-1);
    l->lst[l->sz++] = s;
}

#define TABLE_SIZE 64
// http://www.cse.yorku.ca/~oz/hash.html
int hash(char* s) {
    int val = 5381;
    for (char c; (c = *s++);)
        val = ((val << 5) + val) + c; /* val * 33 + c */
    return val % TABLE_SIZE;
}

bool isLower(char* s) {
    for (char c; (c = *s++);)
        if (!('a' <= c && c <= 'z'))
            return false;
    return true;
}

int visCnt[TABLE_SIZE];
char strs[TABLE_SIZE][MAX_STRLEN];
int hshs[TABLE_SIZE];
bool lower[TABLE_SIZE];

list adj[TABLE_SIZE];
int start = -1, end = -1;

void readInput() {
    char a[MAX_STRLEN], b[MAX_STRLEN];
    memset(adj, 0, TABLE_SIZE*sizeof(list));
    memset(strs, 0, TABLE_SIZE*MAX_STRLEN);
    memset(hshs, 0, TABLE_SIZE*sizeof(int));
    memset(lower, 0, TABLE_SIZE);
    int numStrs = 0;
    while (scanf("%[^-]-%[^\n]\n", a, b) != EOF) {
        int ha = hash(a), hb = hash(b);
        // printf("%s (%llu) - %s (%llu)\n", a, ha, b, hb);
        if (strs[ha][0] == 0) {
            strcpy(strs[ha], a);
            hshs[numStrs++] = ha;
            lower[ha] = isLower(a);
            if (strcmp(a, "start") == 0)
                start = ha;
            else if (strcmp(a, "end") == 0)
                end = ha;
        }
        if (strs[hb][0] == 0) {
            strcpy(strs[hb], b);
            hshs[numStrs++] = hb;
            lower[hb] = isLower(b);
            if (strcmp(b, "start") == 0)
                start = hb;
            else if (strcmp(b, "end") == 0)
                end = hb;
        }
        assert(strcmp(strs[ha], a) == 0);
        assert(strcmp(strs[hb], b) == 0);
        pb(&adj[ha], hb);
        pb(&adj[hb], ha);
    }
    assert(start != -1);
    assert(end != -1);
}

void dfs(bool curTwoSmall, int cur, int* cnt, bool part1) {
    if (cur == end) {
        ++*cnt;
        return;
    }
    for (int i = 0; i < adj[cur].sz; ++i) {
        int nxt = adj[cur].lst[i];
        bool twoSmall = curTwoSmall;
        if (part1) {
            if (lower[nxt] && visCnt[nxt]) {
                continue;
            }
        }
        else {
            if (nxt == start) {
                continue;
            }
            if (lower[nxt] && nxt != end) {
                if (visCnt[nxt] == 1) {
                    if (twoSmall)
                        continue;
                    twoSmall = true;
                }
                else if (visCnt[nxt] >= 2)
                    continue;
            }
        }
        ++visCnt[nxt];
        dfs(twoSmall, nxt, cnt, part1);
        --visCnt[nxt];
    }
}

void solve(bool part1) {
    memset(visCnt, 0, sizeof(visCnt));
    visCnt[start] = 1;
    int ans = 0;
    dfs(false, start, &ans, part1);
    printf("%d\n", ans);
}

int main() {
    readInput();
    solve(true);
    solve(false);
}
