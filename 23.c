#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))
int N;

#define MEM_CAP (1<<28)
char mem[MEM_CAP];
size_t memSize = 0;
void* badMalloc(size_t numBytes) {
    assert(memSize + numBytes < MEM_CAP);
    void* start = mem + memSize;
    memSize += numBytes;
    return start;
}

void memClear() {
    memset(mem, 0, sizeof(MEM_CAP));
    memSize = 0;
}

typedef struct {
    int cost;
    char* map;
} state;

state stateNew() {
    return (state) { 0, (char*) badMalloc(11+4*N) };
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
    return memcmp(a.map, b.map, 11+4*N);
}

size_t stateHash(state s) {
    size_t val = 31;
    for (int i = 0; i < 11+4*N; ++i)
        val = ((val << 5) + val) + s.map[i]; /* val * 33 + c */
    return val;
}


state stateCpy(state s) {
    state n = stateNew();
    n.cost = s.cost;
    memcpy(n.map, s.map, 11+4*N);
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
            memcpy(&l->data[i], &l->data[i+1],
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

bool hashsetInsert(hashset* set, state x) {
    int hv = stateHash(x) % set->cap;
    if (listIndex(&set->data[hv], x) != -1)
        return false;
    listPush(&set->data[hv], x);
    ++set->size;
    return true;
}

bool hashsetFind(hashset* set, state x, int* i, int* j) {
    int hv = stateHash(x) % set->cap;
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
    int hv = stateHash(x) % set->cap;
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


int hallways[7] = {0, 1, 3, 5, 7, 9, 10};
int numHallways = 7;
int pows[4] = { 1, 10, 100, 1000 };
list pq;
hashset minCost;

void addNewMoves(state s) {
    for (int hallwayIdx = 0; hallwayIdx < numHallways; ++hallwayIdx) {
        int hallway = hallways[hallwayIdx];
        if (s.map[hallway] == '.') {
            // try to move all guys to this position
            for (int room = 0; room < 4; ++room) {
                int under = room * 2 + 2;
                int mi = min(hallway, under);
                int ma = max(hallway, under);
                bool ok = true;
                for (int i=mi; i <= ma; ++i) {
                    if (s.map[i] != '.') {
                        ok = false;
                        break;
                    }
                }
                if (!ok) continue;
                for (int dep = 0; dep < N; ++dep) {
                    // take only the top guy
                    int cellIdx = 11 + (room*N+dep);
                    if (s.map[cellIdx] != '.') {
                        int dist = abs(hallway - under) + dep+1;
                        state newS = stateCpy(s);
                        char guy = newS.map[cellIdx];
                        newS.map[cellIdx] = '.';
                        newS.map[hallway] = guy;
                        newS.cost = s.cost + dist*pows[guy-'A'];
                        int hv, id;
                        if (!hashsetFind(&minCost, newS, &hv, &id)) {
                            // hashsetInsert(&minCost, stateCpy(newS));
                            hashsetInsert(&minCost, newS);
                            heapPush(&pq, newS);
                        }
                        else if (newS.cost < hashsetGet(&minCost, hv, id)) {
                            hashsetUpdate(&minCost, hv, id, newS.cost);
                            heapPush(&pq, newS);
                        }
                        break;
                    }
                }
            }
        }
        else {
            // try to move this guy to his room
            int room = s.map[hallway]-'A';
            int under = room * 2 + 2;
            int mi = min(hallway, under);
            int ma = max(hallway, under);
            bool ok = true;
            for (int i=mi; i <= ma; ++i) {
                if (i == hallway) continue;
                if (s.map[i] != '.') {
                    ok = false;
                    break;
                }
            }
            if (!ok) {
                continue;
            }
            for (int dep = N-1; dep >= 0; --dep) {
                // take only lowest empty cell
                int cellIdx = 11 + (room*N+dep);
                if (s.map[cellIdx] == '.') {
                    int dist = abs(hallway - under) + dep+1;
                    state newS = stateCpy(s);
                    char guy = newS.map[hallway];
                    newS.map[hallway] = '.';
                    newS.map[cellIdx] = guy;
                    newS.cost = s.cost + dist*pows[guy-'A'];
                    int hv, id;
                    if (!hashsetFind(&minCost, newS, &hv, &id)) {
                        // hashsetInsert(&minCost, stateCpy(newS));
                        hashsetInsert(&minCost, newS);
                        heapPush(&pq, newS);
                    }
                    else if (newS.cost < hashsetGet(&minCost, hv, id)) {
                        hashsetUpdate(&minCost, hv, id, newS.cost);
                        heapPush(&pq, newS);
                    }
                    break;
                }
                if (s.map[cellIdx] != s.map[hallway]) break;
            }
        }

    }
}

void solve(const char* start) {
    // listClear(&pq);
    // hashsetClear(&minCost);
    memClear();
    pq = listNew(1<<15);
    minCost = hashsetNew(1<<17, 64);

    N = (strlen(start) - 11) / 4;
    
    state initState = stateNew();
    memcpy(initState.map, start, 11+4*N);
    heapPush(&pq, initState);
    // hashsetInsert(&minCost, stateCpy(initState));
    hashsetInsert(&minCost, initState);

    int ans = -1;
    while (pq.size > 0) {
        state s = heapPop(&pq);
        // printf("%d '%19s'\n", s.cost, s.map);
        if (s.cost > hashsetGetOrDefault(&minCost, s, 1<<30)) {
            continue;
        }
        bool done = true;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < N; ++j) {
                if (s.map[11+i*N+j] != 'A'+i) {
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

char part1start[11+4*2+1];
char part2start[11+4*4+1];
void readInput() { // lol
    scanf("#############\n#...........#\n###%c#%c#%c#%c###\n  #%c#%c#%c#%c",
            &part1start[11], &part1start[13], &part1start[15], &part1start[17],
            &part1start[12], &part1start[14], &part1start[16], &part1start[18]);
    for (int i = 0; i < 4; ++i) {
        part2start[11+i*4] = part1start[11+i*2]; // 11 13 15 17
        part2start[14+i*4] = part1start[12+i*2]; // 14 18 22 26
    }
    sscanf("  #D#C#B#A#\n  #D#B#A#C#", "  #%c#%c#%c#%c#\n  #%c#%c#%c#%c#",
            &part2start[12], &part2start[16], &part2start[20], &part2start[24],
            &part2start[13], &part2start[17], &part2start[21], &part2start[25]);
    memset(part1start, '.', 11);
    memset(part2start, '.', 11);
    part1start[11+4*2] = 0;
    part2start[11+4*4] = 0;
}

// TODO: fix this terrible program
// - add some kind of heuristic function
// - remove unnecessary branches
//   - if straight path from cell to final room, don't bother with hallway
//   - some other smart stuff probably
// - don't store unused hallway positions (can save 4 bytes per state)
// - improve hash function
// x probably don't malloc or free / fix leaks
// - improve hashmap api
// - clean up

void test() {
    solve("...........BACDBCDA"); // sample - 12521/44169
    solve("...........DBACCBDA"); // excel guy - 14148/43814
    solve("...........ADCABDCB"); // xdavidliu - 16300/48676
    solve("...........DDCCABBA"); // me - 19160/47232
    solve("...........BDDACCBDBBACDACA"); // sample - 12521/44169
    solve("...........ADDDCCBABBADCACB"); // xdavidliu - 16300/48676
    solve("...........DDDBACBCCBABDACA"); // excel guy - 14148/43814
    solve("...........DDDDCCBCABABBACA"); // me - 19160/47232
}

int main() {
    // pq = listNew(1<<15);
    // minCost = hashsetNew(1<<17, 64); // this is slow as balls
    // test();
    readInput();
    solve(part1start);
    solve(part2start);
    // listFree(pq);
    // hashsetFree(minCost);
}
