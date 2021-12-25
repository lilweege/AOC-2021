#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define FILENAME "input/19.txt"

typedef struct {
    int x, y, z;
} point;

int pointCmp(point p, point q) {
    if (p.x != q.x) return p.x - q.x;
    if (p.y != q.y) return p.y - q.y;
    return p.z - q.z;
}

int pointHash(point p) {
    int val = 5381;
    val = ((val << 5) + val) + p.x;
    val = ((val << 5) + val) + p.y;
    val = ((val << 5) + val) + p.z;
    return val > 0 ? val : -val;
}

typedef struct {
    point* data;
    int size;
    int cap;
} list;

void listInit(list* l, int cap) {
    l->data = (point*) malloc(cap*sizeof(point));
    l->size = 0;
    l->cap = cap;
}
void listFree(list* l) {
    free(l->data);
}

void listPush(list* l, point x) {
    assert(l->size < l->cap-1);
    l->data[l->size++] = x;
}

bool listErase(list* l, point x) {
    for (int i = 0; i < l->size; ++i)
        if (pointCmp(x, l->data[i]) == 0) {
            memcpy(&l->data[i], &l->data[i+1],
                    (l->size-i-1)*sizeof(point));
            --l->size;
            return true;
        }
    return false;
}

int listIndex(list* l, point x) {
    for (int i = 0; i < l->size; ++i)
        if (pointCmp(x, l->data[i]) == 0)
            return i;
    return -1;
}

typedef struct {
    list* data;
    int size;
    int cap;
} hashset;

void hashsetInit(hashset* set, int cap, int lstCap) {
    set->data = (list*) malloc(cap*sizeof(list));
    for (int i = 0; i < cap; ++i)
        listInit(&set->data[i], lstCap);
    set->size = 0;
    set->cap = cap;
}

void hashsetFree(hashset* set) {
    for (int i = 0; i < set->cap; ++i)
        listFree(&set->data[i]);
    free(set->data);
}

void hashsetClear(hashset* set) {
    for (int i = 0; i < set->cap; ++i)
        set->data[i].size = 0;
    set->size = 0;
}

bool hashsetInsert(hashset* set, point x) {
    int hv = pointHash(x) % set->cap;
    if (listIndex(&set->data[hv], x) != -1)
        return false;
    listPush(&set->data[hv], x);
    ++set->size;
    return true;
}

bool hashsetContains(hashset* set, point x) {
    int hv = pointHash(x) % set->cap;
    return listIndex(&set->data[hv], x) != -1;
}

bool hashsetRemove(hashset* set, point x) {
    int hv = pointHash(x) % set->cap;
    if (listErase(&set->data[hv], x)) {
        --set->size;
        return true;
    }
    return false;
}

typedef struct {
    hashset set;
    unsigned char** cnt;
} counter;

void counterInit(counter* map, int cap, int lstCap) {
    hashsetInit(&map->set, cap, lstCap);
    map->cnt = (unsigned char**) malloc(cap*sizeof(unsigned char*));
    for (int i = 0; i < cap; ++i) {
        map->cnt[i] = (unsigned char*) malloc(lstCap*sizeof(unsigned char));
        memset(map->cnt[i], 0, lstCap*sizeof(unsigned char));
    }
}

void counterFree(counter* map) {
    hashsetFree(&map->set);
    for (int i = 0; i < map->set.cap; ++i)
        free(map->cnt[i]);
    free(map->cnt);
}

void counterClear(counter* map) {
    for (int i = 0; i < map->set.cap; ++i) {
        for (int j = 0; j < map->set.data[i].size; ++j) {
            map->set.data[i].data[j] = (point) { 0, 0, 0 };
        }
        map->set.data[i].size = 0;
        memset(map->cnt[i], 0, map->set.data[0].cap*sizeof(unsigned char));
    }
}

int counterIncrement(counter* map, point x) {
    int hv = pointHash(x) % map->set.cap;
    int idx = listIndex(&map->set.data[hv], x);
    if (idx == -1) {
        idx = map->set.data[hv].size;
        listPush(&map->set.data[hv], x);
    }
    return ++map->cnt[hv][idx];
}

int counterGet(counter* map, point x) {
    int hv = pointHash(x) % map->set.cap;
    int idx = listIndex(&map->set.data[hv], x);
    return idx == -1 ? 0 : map->cnt[hv][idx];
}

// omg I love writing everything from scratch

#define MAX_BEACONS 32
typedef struct {
    point beacons[MAX_BEACONS];
    int n;
} scanner;


// double euclid2(point p, point q) {
//     return (p.x-q.x)*(p.x-q.x) + (p.y-q.y)*(p.y-q.y) + (p.z-q.z)*(p.z-q.z);
// }

int manhattan(point p, point q) {
    int dx = p.x-q.x; if (dx < 0) dx = -dx;
    int dy = p.y-q.y; if (dy < 0) dy = -dy;
    int dz = p.z-q.z; if (dz < 0) dz = -dz;
    return dx + dy + dz;
}

point dirs[6] = {
    {0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}
};

void solve() {
    freopen(FILENAME, "r", stdin);
    scanner scanners[32];
    memset(scanners, 0, sizeof(scanners));
    int numScanners = -1;
    
    char line[32];
    while (scanf("%[^\n]\n", line) != EOF) {
        if (line[1] == '-') {
            ++numScanners;
            continue;
        }
        point* p = &scanners[numScanners].beacons[scanners[numScanners].n++];
        sscanf(line, "%d,%d,%d\n", &p->x, &p->y, &p->z);
    }
    ++numScanners;

    hashset points;
    hashsetInit(&points, 512, 8);
    for (int i = 0; i < scanners[0].n; ++i)
        hashsetInsert(&points, scanners[0].beacons[i]);
    
    counter offset;
    counterInit(&offset, 8192, 16);
    
    point cur[32];
    bool found[numScanners];
    memset(found, 0, numScanners);
    int numFound = 0;
    point scannerPos[32];
    scannerPos[0] = (point) { 0, 0, 0 };
    int numScannerPos = 1;

    // this is probably close to the fastest brute force
    // I am aware that there are smarter ways to solve this
    // perhaps something to do with sorting by distance and stopping some checks early
    // or even some geometry (which at this point I am pretty sick of)
    // regardless, this is as good as it's gonna get for now
    while (numFound < numScanners-1) {
        for (int i = 1; i < numScanners; ++i) {
            if (found[i]) continue;
            scanner* s = &scanners[i];
            // NOTE: checking all 48 instead of 24
            for (int sign = 0; sign < (1<<3); ++sign) {
                for (int dir = 0; dir < 6; ++dir) {
                    int curSize = 0;
                    counterClear(&offset);

                    point origin;
                    int maxCnt = 0;
                    for (int j = 0; j < s->n; ++j) {
                        int nx = *((int*) &s->beacons[j] + dirs[dir].x) * ((sign&(1<<0)) ? -1 : 1);
                        int ny = *((int*) &s->beacons[j] + dirs[dir].y) * ((sign&(1<<1)) ? -1 : 1);
                        int nz = *((int*) &s->beacons[j] + dirs[dir].z) * ((sign&(1<<2)) ? -1 : 1);
                        cur[curSize++] = (point) { nx, ny, nz };

                        for (int n = 0; n < points.cap; ++n)
                            for (int m = 0; m < points.data[n].size; ++m) {
                                point p = points.data[n].data[m];
                                point q = (point) { nx + p.x, ny + p.y, nz + p.z };
                                int cnt = counterIncrement(&offset, q);
                                if (cnt > maxCnt) {
                                    maxCnt = cnt;
                                    origin = q;
                                }
                            }
                    }
                    if (maxCnt >= 12) {
                        found[i] = true;
                        ++numFound;
                        for (int k = 0; k < curSize; ++k) {
                            point p = { origin.x - cur[k].x, origin.y - cur[k].y,  origin.z - cur[k].z };
                            hashsetInsert(&points, p);
                        }
                        scannerPos[numScannerPos++] = origin;
                        break;
                    }
                }
                if (found[i]) break;
            }
        }
    }
    printf("%d\n", points.size);
    hashsetFree(&points);
    counterFree(&offset);

    int ans2 = 0;
    for (int i = 0; i < numScannerPos; ++i)
        for (int j = 0; j < numScannerPos; ++j) {
            int dist = manhattan(scannerPos[i], scannerPos[j]);
            if (dist > ans2)
                ans2 = dist;
        }
    printf("%d\n", ans2);
}

int main() {
    solve();
}
