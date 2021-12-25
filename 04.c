#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define MAX_BOARDS 100
#define BOARD_W 5
#define BOARD_H 5
// note numbers range 0-99
// weak constraints => naive

void solve() {
    char nums[300];
    fgets(nums, 300, stdin);

    int numBoards = 0;
    int boards[MAX_BOARDS][BOARD_H][BOARD_W];
    for (char c; (c = getchar()) != EOF;) {
        for (int i = 0; i < BOARD_H; ++i) {
            for (int j = 0; j < BOARD_W; ++j) {
                scanf("%d", &boards[numBoards][i][j]);
            }
        }
        ++numBoards;
    }
    --numBoards;

    int numWinners = 0;
    bool won[numBoards];
    memset(won, false, numBoards);
    int winner = -1;
    int winningNum = -1;
    int loser = -1;

    int curr;
    for (int x, p = 0; sscanf(nums + p, "%d,%n", &curr, &x) != EOF; p += x) {
        for (int b = 0; b < numBoards; ++b) {
            // this could be improved by keeping lists of indices
            if (won[b]) {
                continue;
            }
            for (int i = 0; i < BOARD_H; ++i) {
                for (int j = 0; j < BOARD_W; ++j) {
                    if (boards[b][i][j] == curr) {
                        boards[b][i][j] = ~boards[b][i][j];
                        bool yes = true;
                        for (int ii = 0; ii < BOARD_H; ++ii) {
                            if (boards[b][ii][j] >= 0) {
                                yes = false;
                                break;
                            }
                        }
                        if (yes) {
                            loser = b;
                            ++numWinners;
                            won[b] = true;
                            if (winner == -1) {
                                winningNum = curr;
                                winner = b;
                            }
                            goto next;
                        }
                        yes = true;
                        for (int jj = 0; jj < BOARD_W; ++jj) {
                            if (boards[b][i][jj] >= 0) {
                                yes = false;
                                break;
                            }
                        }
                        if (yes) {
                            loser = b;
                            ++numWinners;
                            won[b] = true;
                            if (winner == -1) {
                                winningNum = curr;
                                winner = b;
                            }
                            goto next;
                        }
                    }
                }
next:;
                if (numWinners == numBoards)
                    goto done;
            }
        }
    }
done:;

    assert(numWinners == numBoards);
    int sum1 = 0, sum2 = 0;
    for (int i = 0; i < BOARD_H; ++i)
        for (int j = 0; j < BOARD_W; ++j) {
            if (boards[winner][i][j] >= 0)
                sum1 += boards[winner][i][j];
            if (boards[loser][i][j] >= 0)
                sum2 += boards[loser][i][j];
        }
    printf("%d\n%d\n", sum1 * winningNum, sum2 * curr);
}

int main() {
    solve();
}
