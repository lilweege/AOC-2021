#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
// #include <stdlib.h>

typedef struct node node;
struct node {
    int val;
    node *l, *r;
};

#define MEM_CAP (1<<20)
node mem[MEM_CAP];
int memSz = 0;
node* createNode(node* l, node* r, int val) {
    // node* n = malloc(sizeof(node));
    // faster
    assert(memSz < MEM_CAP-1);
    node* n = mem + memSz++;
    n->val = val;
    n->l = l;
    n->r = r;
    return n;
}

node* newLeaf(int val) { return createNode(NULL, NULL, val); }
node* newNode(node* l, node* r) { return createNode(l, r, 0); }
node* copyTree(node* n) {
    if (n == NULL) return NULL;
    return createNode(copyTree(n->l), copyTree(n->r), n->val);
}
bool isLeaf(node* n) { return n->l == NULL; }

void printNode(node* n) {
    assert(n != NULL);
    if (isLeaf(n)) {
        printf("%d", n->val);
    }
    else {
        putchar('[');
        printNode(n->l);
        putchar(',');
        printNode(n->r);
        putchar(']');
    }
}

node* fromStr(char** s) {
    char c = **s; ++*s;
    if (c == '[') {
        node* l = fromStr(s); ++*s; // ,
        node* r = fromStr(s); ++*s; // ]
        return newNode(l, r);
    }
    else if (c != 0) {
        return newLeaf(c - '0');
    }
    assert(0 && "Unreachable");
    return NULL;
}


bool explodeLeft(node* n, node* me, node** prev, bool* stop) {
    if (n == me) {
        *stop = true;
    }
    if (*stop) {
        return true;
    }
    if (isLeaf(n)) {
        *prev = n;
        return false;
    }
    return explodeLeft(n->l, me, prev, stop) ||
            explodeLeft(n->r, me, prev, stop);
}

bool explodeRight(node* n, node* me, node** prev, bool* stop) {
    if (n == me) {
        *stop = true;
    }
    else if (*stop && isLeaf(n)) {
        *prev = n;
        return true;
    }
    if (isLeaf(n)) {
        return false;
    }
    return explodeRight(n->l, me, prev, stop) ||
            explodeRight(n->r, me, prev, stop);
}

bool explode(node* root, node* n, int dep) {
    if (isLeaf(n)) {
        return false;
    }
    if (dep >= 4) {
        bool found;
        node *l = NULL, *r = NULL;
        
        found = false;
        explodeLeft(root, n, &l, &found);
        if (l != NULL) l->val += n->l->val;
        found = false;
        explodeRight(root, n->r, &r, &found);
        if (r != NULL) r->val += n->r->val;
        
        n->val = 0;
        n->l = NULL;
        n->r = NULL;
        return true;
    }
    return explode(root, n->l, dep + 1) ||
            explode(root, n->r, dep + 1);
}

bool split(node *n) {
    if (isLeaf(n)) {
        if (n->val < 10) {
            return false;
        }
        n->l = newLeaf(n->val/2);
        n->r = newLeaf(n->val/2 + (n->val&1));
        return true;
    }
    return split(n->l) || split(n->r);
}


int magnitude(node* n) {
    if (isLeaf(n))
        return n->val;
    return 3 * magnitude(n->l) + 2 * magnitude(n->r);
}

node* addNode(node* l, node* r) {
    node* n = newNode(l, r);
    while (true) {
        if (explode(n, n, 0))
            continue;
        if (!split(n))
            break;
    }
    return n;
}


node* nodes[128];
int numNodes = 0;
void readInput() {
    char line[64];
    while (scanf("%s", line) != EOF) {
        char* p = line;
        nodes[numNodes++] = fromStr(&p);
    }
}

void part1() {
    node* cur = copyTree(nodes[0]);
    for (int i = 1; i < numNodes; ++i)
        cur = addNode(cur, copyTree(nodes[i]));
    printf("%d\n", magnitude(cur));
}

void part2() {
    int ans = 0;
    for (int i = 0; i < numNodes; ++i) {
        for (int j = 0; j < numNodes; ++j) {
            if (i == j) continue;
            node* n = addNode(copyTree(nodes[i]), copyTree(nodes[j]));
            int mag = magnitude(n);
            if (mag > ans)
                ans = mag;
        }
    }
    printf("%d\n", ans);
}

int main() {
    readInput();
    part1();
    part2();
}
