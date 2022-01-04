#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int x, y;
} point;

int pointCmp(point p, point q) {
    if (p.x != q.x)
        return p.x - q.x;
    return p.y - q.y;
}

#define LIST_CAP 8
typedef struct {
    point data[LIST_CAP];
    int size;
} list;

void listPush(list* l, point x) {
    assert(l->size < LIST_CAP-1);
    l->data[l->size++] = x;
}

bool listErase(list* l, point x) {
    for (int i = 0; i < l->size; ++i)
        if (pointCmp(x, l->data[i]) == 0) {
            memmove(&l->data[i], &l->data[i+1],
                    (l->size-i-1) * sizeof(point));
            --l->size;
            return true;
        }
    return false;
}

bool listContains(list* l, point x) {
    for (int i = 0; i < l->size; ++i)
        if (pointCmp(x, l->data[i]) == 0)
            return true;
    return false;
}

#define TABLE_SIZE 512
typedef struct {
    list data[TABLE_SIZE];
    int size;
} hashset;

void hashsetClear(hashset* set) {
    for (int i = 0; i < TABLE_SIZE; ++i)
        set->data[i].size = 0;
    set->size = 0;
}

int pointHash(point p) {
    int val = 5381;
    val = ((val << 5) + val) + p.x;
    val = ((val << 5) + val) + p.y;
    return val % TABLE_SIZE;
}

bool hashsetInsert(hashset* set, point x) {
    int hv = pointHash(x);
    if (listContains(&set->data[hv], x))
        return false;
    listPush(&set->data[hv], x);
    ++set->size;
    return true;
}

bool hashsetContains(hashset* set, point x) {
    int hv = pointHash(x);
    return listContains(&set->data[hv], x);
}

bool hashsetRemove(hashset* set, point x) {
    int hv = pointHash(x);
    if (listErase(&set->data[hv], x)) {
        --set->size;
        return true;
    }
    return false;
}

typedef struct {
    bool axis;
    int pos;
} fold;


void solve() {
    hashset hs;
    hashsetClear(&hs);

    fold folds[128];
    int numFolds = 0;

    char line[32];
    while (scanf("%[^\n]\n", line) != EOF) {
        if (line[0] == 'f') {
            break;
        }
        point p;
        sscanf(line, "%d,%d", &p.x, &p.y);
        hashsetInsert(&hs, p);
    }
    do {
        char axis;
        sscanf(line, "fold along %c=%d",
            &axis, &folds[numFolds].pos);
        folds[numFolds++].axis = axis == 'x';
    } while (scanf("%[^\n]\n", line) != EOF);

    for (int i = 0; i < numFolds; ++i) {
        fold f = folds[i];

        hashset nxt = hs;
        for (int j = 0; j < TABLE_SIZE; ++j) {
            for (int k = 0; k < hs.data[j].size; ++k) {
                point p = hs.data[j].data[k];
                if (f.axis && p.x > f.pos) {
                    hashsetInsert(&nxt, (point){f.pos - (p.x - f.pos), p.y});
                    hashsetRemove(&nxt, p);
                }
                else if (!f.axis && p.y > f.pos) {
                    hashsetInsert(&nxt, (point){p.x, f.pos - (p.y - f.pos)});
                    hashsetRemove(&nxt, p);
                }
            }
        }
        hs = nxt;

        if (i == 0)
            printf("%d\n", hs.size);
    }

    int mx = 0, my = 0;
    for (int j = 0; j < TABLE_SIZE; ++j)
        for (int k = 0; k < hs.data[j].size; ++k) {
            point p = hs.data[j].data[k];
            if (p.x > mx) mx = p.x;
            if (p.y > my) my = p.y;
        }
    for (int i = 0; i <= my; ++i) {
        for (int j = 0; j <= mx; ++j)
            putchar(hashsetContains(&hs, (point){j, i}) ? '#' : '.');
        putchar('\n');
    }
}

int main() {
    solve();
}
