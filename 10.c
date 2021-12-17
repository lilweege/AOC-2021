#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FILENAME "input/10.txt"
#define MAX 128
typedef long long ll;

int cmpLL(const void *a, const void *b) {
    ll* x = (ll*) a;
    ll* y = (ll*) b;
    return *x > *y ? 1 : -1;
}

void solve() {
    freopen(FILENAME, "r", stdin);
    char lines[MAX][MAX];
    int sz = 0;
    while (scanf("%s", lines[sz]) != EOF) {
        ++sz;
    }
    char paren_stack[MAX];

    ll scores[MAX];
    int numScores = 0;
    int syntaxScore = 0;
    for (int i = 0; i < sz; ++i) {
        int top = 0;
        bool complete = true;
        for (int j = 0; lines[i][j]; ++j) {
            char curr = lines[i][j];
            if (curr == '(' || curr == '[' || curr == '{' || curr == '<') {
                paren_stack[top++] = curr;
            }
            else {
                if (top <= 0) {
                    complete = false;
                    break;
                }
                char last = paren_stack[--top];
                if (last == '(' && curr != ')' ||
                    last == '[' && curr != ']' ||
                    last == '{' && curr != '}' ||
                    last == '<' && curr != '>'
                ) {
                    if (curr == ')')
                        syntaxScore += 3;
                    else if (curr == ']')
                        syntaxScore += 57;
                    else if (curr == '}')
                        syntaxScore += 1197;
                    else if (curr == '>')
                        syntaxScore += 25137;
                    complete = false;
                    break;
                }
            }
        }
        if (complete && top > 0) {
            // incomplete
            ll score = 0;
            while (top > 0) {
                char curr = paren_stack[--top];
                if (curr == '(')
                    score = score * 5 + 1;
                else if (curr == '[')
                    score = score * 5 + 2;
                else if (curr == '{')
                    score = score * 5 + 3;
                else if (curr == '<')
                    score = score * 5 + 4;
            }
            scores[numScores++] = score;
        }
    }

    printf("%d\n", syntaxScore);
    qsort(scores, numScores, sizeof(ll), cmpLL);
    printf("%lld\n", scores[numScores/2]);
}

int main() {
    solve();
}
