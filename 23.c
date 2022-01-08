#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))
int N;

#define MEM_CAP (1<<27)
char mem[MEM_CAP];
size_t memSize = 0;
void* badMalloc(size_t numBytes) {
    assert(memSize + numBytes < MEM_CAP);
    void* start = mem + memSize;
    memSize += numBytes;
    return start;
}

void memClear() {
    memSize = 0;
}

typedef struct {
    int cost;
    char* map;
} state;

/*
01 2 3 4 56
  7 9 b d
  8 a c e
*/

int idxToPos(int i) {
    if (i == 0) return 0;
    if (i == 6) return 10;
    return i*2-1;
}

int posToIdx(int p) {
    if (p == 0) return 0;
    if (p == 10) return 6;
    return (p+1)/2;
}

state stateNew() {
    return (state) { 0, (char*) badMalloc(7+4*N) };
}

void stateFree(state s) {
    (void) s;
    // free(s.map);
}

// for heap
int stateCmpCost(state a, state b) {
    return a.cost - b.cost;
}

// for hashmap
int stateCmpMap(state a, state b) {
    return memcmp(a.map, b.map, 7+4*N);
}

#define FNV1_32_INIT ((uint32_t)0x811c9dc5)
#define FNV_32_PRIME ((uint32_t)0x01000193)
uint32_t stateHash(state s) {
    // for (int i = 0; i < 7+4*N; ++i)
    //     hash = ((hash << 5) + hash) + s.map[i];
    uint32_t hash = FNV1_32_INIT;
    for (int i = 0; i < 7+4*N; ++i) {
	    hash ^= s.map[i];
	    hash *= FNV_32_PRIME;
    }
    return hash;
}


state stateCpy(state s) {
    state n = stateNew();
    n.cost = s.cost;
    memcpy(n.map, s.map, 7+4*N);
    return n;
}

typedef struct {
    state* data;
    int cap;
    int size;
} list;

list listNew(int cap) {
    return (list) { (state*) badMalloc(cap*sizeof(state)), cap, 0 };
}

void listFree(list l) {
    (void) l;
    // for (int i = 0; i < l.size; ++i)
    //     stateFree(l.data[i]);
    // free(l.data);
}

void listClear(list* l) {
    // for (int i = 0; i < l->size; ++i)
    //     stateFree(l->data[i]);
    l->size = 0;
}


// utility functions for heap
void upHeapify(list* h, int idx) {
    int par = (idx-1)/2;
    if (stateCmpCost(h->data[par], h->data[idx]) > 0) {
        state tmp = h->data[par];
        h->data[par] = h->data[idx];
        h->data[idx] = tmp;
        upHeapify(h, par);
    }
}

void downHeapify(list* h, int par) {
    int l = par*2+1, r = par*2+2;
    if (l >= h->size || l < 0) l = -1;
    if (r >= h->size || r < 0) r = -1;
    int min = (l != -1 && stateCmpCost(h->data[l], h->data[par]) < 0) ? l : par;
    if (r != -1 && stateCmpCost(h->data[r], h->data[min]) < 0) min = r;
    if (min != par) {
        state tmp = h->data[min];
        h->data[min] = h->data[par];
        h->data[par] = tmp;
        downHeapify(h, min);
    }
}

// heap api
void heapPush(list* h, state x) {
    assert(h->size < h->cap-1);
    h->data[h->size++] = x;
    upHeapify(h, h->size-1);
}

state heapPop(list* h) {
    assert(h->size > 0);
    state x = h->data[0];
    h->data[0] = h->data[--h->size];
    downHeapify(h, 0);
    return x;
}

// api / utility functions for hashmap
void listPush(list* l, state x) {
    assert(l->size < l->cap-1);
    l->data[l->size++] = x;
}

bool listErase(list* l, state x) {
    for (int i = 0; i < l->size; ++i)
        if (stateCmpMap(x, l->data[i]) == 0) {
            memmove(&l->data[i], &l->data[i+1],
                    (l->size-i-1)*sizeof(state));
            --l->size;
            return true;
        }
    return false;
}

int listIndex(list* l, state x) {
    for (int i = 0; i < l->size; ++i)
        if (stateCmpMap(x, l->data[i]) == 0)
            return i;
    return -1;
}

typedef struct {
    list* data;
    int size;
    int cap;
} hashset;
// in this case it's more of a hashmap really

int hashval(hashset* set, state x) {
    // return stateHash(x) % set->cap; // slow
    return stateHash(x) & (set->cap - 1); // fast (set cap must be power of 2)
}

hashset hashsetNew(int setCap, int lstCap) {
    hashset set;
    set.data = (list*) badMalloc(setCap*sizeof(list));
    for (int i = 0; i < setCap; ++i)
        set.data[i] = listNew(lstCap);
    set.size = 0;
    set.cap = setCap;
    return set;
}

void hashsetFree(hashset set) {
    (void) set;
    // for (int i = 0; i < set.cap; ++i)
    //     listFree(set.data[i]);
    // free(set.data);
}

void hashsetClear(hashset* set) {
    for (int i = 0; i < set->cap; ++i)
        listClear(&set->data[i]);
    set->size = 0;
}

bool hashsetInsertHint(hashset* set, state x, int hv) {
    if (listIndex(&set->data[hv], x) != -1)
        return false;
    listPush(&set->data[hv], x);
    ++set->size;
    return true;
}

bool hashsetInsert(hashset* set, state x) {
    return hashsetInsertHint(set, x, hashval(set, x));
}

bool hashsetFind(hashset* set, state x, int* i, int* j) {
    int hv = hashval(set, x);
    int id = listIndex(&set->data[hv], x);
    if (i) *i = hv;
    if (j) *j = id;
    return id != -1;
}

int hashsetUpdate(hashset* set, int hv, int id, int x) {
    return set->data[hv].data[id].cost = x;
}

int hashsetGet(hashset* set, int hv, int id) {
    return set->data[hv].data[id].cost;
}

bool hashsetRemove(hashset* set, state x) {
    int hv = hashval(set, x);
    if (listErase(&set->data[hv], x)) {
        --set->size;
        return true;
    }
    return false;
}

int hashsetGetOrDefault(hashset* set, state x, int d) {
    int hv, id;
    if (hashsetFind(set, x, &hv, &id))
        return hashsetGet(set, hv, id);
    return d;
}


int pows[4] = { 1, 10, 100, 1000 };
list pq;
hashset minCost;

void tryAddMove(state s, int fr, int to, int dist) {
    state newS = stateCpy(s);
    char guy = newS.map[fr];
    newS.map[fr] = '.';
    newS.map[to] = guy;
    newS.cost = s.cost + dist*pows[guy-'A'];
    int hv, id;
    if (!hashsetFind(&minCost, newS, &hv, &id)) {
        // hashsetInsert(&minCost, stateCpy(newS));
        hashsetInsertHint(&minCost, newS, hv);
        heapPush(&pq, newS);
    }
    else if (newS.cost < hashsetGet(&minCost, hv, id)) {
        hashsetUpdate(&minCost, hv, id, newS.cost);
        heapPush(&pq, newS);
    }
}

unsigned char blocked(state s, int pos, int under) {
    int l = posToIdx(min(pos, under+(under<pos?1:-1)));
    int r = posToIdx(max(pos, under+(under<pos?1:-1)));
    unsigned char taken = 0;
    for (int i = l; i <= r; ++i)
        taken |= (s.map[i] != '.') * (1<<i);
    return taken;
}

void addNewMoves(state s) {
    for (int hallway = 0; hallway < 7; ++hallway) {
        if (s.map[hallway] == '.') {
            // try to move all guys to this position
            for (int room = 0; room < 4; ++room) {
                int pos = idxToPos(hallway);
                int under = room * 2 + 2;
                if (blocked(s, pos, under))
                    continue;
                for (int dep = 0; dep < N; ++dep) {
                    // take only the top guy
                    int cellIdx = 7 + (room*N+dep);
                    if (s.map[cellIdx] != '.') {
                        int dist = abs(pos - under) + dep+1;
                        tryAddMove(s, cellIdx, hallway, dist);
                        break;
                    }
                }
            }
        }
        else {
            // try to move this guy to his room
            int room = s.map[hallway]-'A';
            int under = room * 2 + 2;
            int pos = idxToPos(hallway);
            if (blocked(s, pos, under) & ~(1 << hallway))
                continue;
            for (int dep = N-1; dep >= 0; --dep) {
                // take only lowest empty cell
                int cellIdx = 7 + (room*N+dep);
                if (s.map[cellIdx] == '.') {
                    int dist = abs(pos - under) + dep+1;
                    tryAddMove(s, hallway, cellIdx, dist);
                    break;
                }
                if (s.map[cellIdx] != s.map[hallway])
                    break;
            }
        }

    }
}

void solve(const char* start) {
    // listClear(&pq);
    // hashsetClear(&minCost);
    memClear();
    pq = listNew(1<<15);
    minCost = hashsetNew(1<<18, 16);

    N = (strlen(start) - 7) / 4;
    
    state initState = stateNew();
    memcpy(initState.map, start, 7+4*N);
    heapPush(&pq, initState);
    hashsetInsert(&minCost, initState);

    // this dijkstra is good enough for now (~0.2s on my machine)
    // it's probably possible to be way faster if some heuristic
    // function is used to reweight the heap, but idc rn
    int ans = -1;
    while (pq.size > 0) {
        state s = heapPop(&pq);
        if (s.cost > hashsetGetOrDefault(&minCost, s, 1<<30)) {
            continue;
        }
        // printf("%d '%.19s'\n", s.cost, s.map);
        bool done = true;
        for (int room = 0; room < 4; ++room) {
            for (int dep = 0; dep < N; ++dep) {
                int cellIdx = 7+room*N+dep;
                if (s.map[cellIdx] != 'A'+room) {
                    done = false;
                    break;
                }
            }
            if (!done) break;
        }
        if (done) {
            ans = s.cost;
            break;
        }
        
        addNewMoves(s);
        // stateFree(s);
    }
    printf("%d\n", ans);
}

char part1start[7+4*2+1];
char part2start[7+4*4+1];
void readInput() { // lol
    scanf("#############\n#...........#\n###%c#%c#%c#%c###\n  #%c#%c#%c#%c",
            &part1start[7], &part1start[ 9], &part1start[11], &part1start[13],
            &part1start[8], &part1start[10], &part1start[12], &part1start[14]);
    for (int i = 0; i < 4; ++i) {
        part2start[ 7+i*4] = part1start[7+i*2]; //  7 11 15 19
        part2start[10+i*4] = part1start[8+i*2]; // 10 14 18 22
    }
    sscanf("  #D#C#B#A#\n  #D#B#A#C#", "  #%c#%c#%c#%c#\n  #%c#%c#%c#%c#",
            &part2start[8], &part2start[12], &part2start[16], &part2start[20],
            &part2start[9], &part2start[13], &part2start[17], &part2start[21]);
    memset(part1start, '.', 7);
    memset(part2start, '.', 7);
    part1start[7+4*2] = 0;
    part2start[7+4*4] = 0;
}

int main() {
    // pq = listNew(1<<15);
    // minCost = hashsetNew(1<<17, 64);
    readInput();
    solve(part1start);
    solve(part2start);
    // listFree(pq);
    // hashsetFree(minCost);
}
