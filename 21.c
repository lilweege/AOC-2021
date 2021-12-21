#include <stdio.h>
#include <string.h>

#define FILENAME "input/21.txt"

int p1start, p2start;
void readInput() {
    freopen(FILENAME, "r", stdin);
    scanf("Player 1 starting position: %d\n", &p1start);
    scanf("Player 2 starting position: %d\n", &p2start);
}

int numRolls = 0;
int roll = 0;
int nextRoll() {
    ++numRolls;
    return roll = roll % 100 + 1;
}

void part1() {
    int turn = 0;
    int p1pos = p1start, p2pos = p2start;
    int p1score = 0, p2score = 0;
    while (p1score < 1000 && p2score < 1000) {
        int val = 0;
        for (int i = 0; i < 3; ++i)
            val += nextRoll();
        if (turn == 0) {
            p1pos = (p1pos + val - 1) % 10 + 1;
            p1score += p1pos;
        }
        else {
            p2pos = (p2pos + val - 1) % 10 + 1;
            p2score += p2pos;
        }
        turn = (turn + 1) % 2;
    }
    int loseScore = p1score < p2score ? p1score : p2score;
    printf("%d\n", loseScore * numRolls);
}

typedef unsigned long long ull;
typedef struct {
    ull i, j;
} pair;

pair dp[10][10][21][21];
pair play(int p1pos, int p2pos, int p1score, int p2score) {
    if (p1score >= 21) return (pair) { 1, 0 };
    if (p2score >= 21) return (pair) { 0, 1 };
    pair* wins = &dp[p1pos-1][p2pos-1][p1score][p2score];
    if (wins->i != 0 || wins->j != 0) return *wins;

    for (int r1 = 1; r1 <= 3; ++r1)
        for (int r2 = 1; r2 <= 3; ++r2)
            for (int r3 = 1; r3 <= 3; ++r3) {
                int np1pos = (p1pos + r1 + r2 + r3 - 1) % 10 + 1;
                int np1score = p1score + np1pos;
                pair res = play(p2pos, np1pos, p2score, np1score);
                wins->i += res.j;
                wins->j += res.i;
            }
    return *wins;
}

void part2() {
    memset(dp, 0, sizeof(dp));
    pair wins = play(p1start, p2start, 0, 0);
    ull moreWins = wins.i > wins.j ? wins.i : wins.j;
    printf("%llu\n", moreWins);
}

int main() {
    readInput();
    part1();
    part2();
}
