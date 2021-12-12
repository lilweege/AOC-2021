#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define FILENAME "input.txt"

// this is probably the most tedious problem to solve in c
// adjacency list of strings plus queue from scratch 😻
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
    for (char c; c = *s++;)
        val = ((val << 5) + val) + c; /* val * 33 + c */
    return val % TABLE_SIZE;
}

typedef struct {
    int pos;
    int visCnt[TABLE_SIZE];
    bool twoSmall;
} path;

bool isLower(char* s) {
    for (char c; c = *s++;)
        if (!('a' <= c && c <= 'z'))
            return false;
    return true;
}

// this would probably be easier to implement recursively
// but I already started this so I'm not changing it
#define Q_SZ (1 << 17)
path q[Q_SZ];
char strs[TABLE_SIZE][MAX_STRLEN];
int hshs[TABLE_SIZE];
bool lower[TABLE_SIZE];

list adj[TABLE_SIZE];
int start = -1, end = -1;

void readInput() {
    freopen(FILENAME, "r", stdin);
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

void solve(bool part1) {
    int ans = 0;
    q[0].pos = start;
    q[0].twoSmall = 0;
    q[0].visCnt[start] = true;
    int front = 0, back = 1;
    while (back != front) {
        path cur = q[front];
        front = (front + 1) % Q_SZ;
        if (cur.pos == end) {
            ++ans;
            continue;
        }
        for (int i = 0; i < adj[cur.pos].sz; ++i) {
            int nxt = adj[cur.pos].lst[i];
            // assert(nxt != 0);
            bool twoSmall = cur.twoSmall;
            if (nxt == start) {
                continue;
            }
            if (part1) {
                if (lower[nxt] && cur.visCnt[nxt]) {
                    continue;
                }
            }
            else {
                if (lower[nxt] && nxt != end) {
                    if (cur.visCnt[nxt] == 1) {
                        if (twoSmall)
                            continue;
                        twoSmall = true;
                    }
                    else if (cur.visCnt[nxt] >= 2)
                        continue;
                }
            }
            // sheeesh
            memcpy(&q[back].visCnt, &cur.visCnt, TABLE_SIZE*sizeof(int));
            q[back].pos = nxt;
            q[back].visCnt[nxt]++;
            q[back].twoSmall = twoSmall;
            back = (back + 1) % Q_SZ;
        }
    }

    printf("%d\n", ans);
}

int main() {
    readInput();
    solve(true);
    solve(false);
}
