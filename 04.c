#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#define FILENAME "input/04.txt"

#define MAX_BOARDS 100
#define BOARD_W 5
#define BOARD_H 5
// note numbers range 0-99
// weak constraints => naive

void part1() {
    freopen(FILENAME, "r", stdin);
    char nums[300];
    fgets(nums, 300, stdin);

    int numBoards = 0;
    int boards[MAX_BOARDS][BOARD_H][BOARD_W];
    for (char c; (c = getchar()) != EOF;) {
        ungetc(c, stdin); // lol
        for (int i = 0; i < BOARD_H; ++i) {
            for (int j = 0; j < BOARD_W; ++j) {
                scanf("%d", &boards[numBoards][i][j]);
            }
        }
        ++numBoards;
    }

    int winner = -1;
    int curr;
    for (int x, p = 0; sscanf(nums + p, "%d,%n", &curr, &x) != EOF; p += x) {
        for (int b = 0; b < numBoards; ++b) {
            // this could be improved by keeping lists of indices
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
                            winner = b;
                            goto done;
                        }
                        yes = true;
                        for (int jj = 0; jj < BOARD_W; ++jj) {
                            if (boards[b][i][jj] >= 0) {
                                yes = false;
                                break;
                            }
                        }
                        if (yes) {
                            winner = b;
                            goto done;
                        }
                    }
                }
            }
        }
    }
done:;

    assert(winner != -1);
    int sum = 0;
    for (int i = 0; i < BOARD_H; ++i) {
        for (int j = 0; j < BOARD_W; ++j) {
            if (boards[winner][i][j] >= 0) {
                sum += boards[winner][i][j];
            }
        }
    }
    int score = sum * curr;
    printf("%d\n", score);
}


void part2() {
    freopen(FILENAME, "r", stdin);
    char nums[300];
    fgets(nums, 300, stdin);

    int numBoards = 0;
    int boards[MAX_BOARDS][BOARD_H][BOARD_W];
    for (char c; (c = getchar()) != EOF;) {
        ungetc(c, stdin); // lol
        for (int i = 0; i < BOARD_H; ++i) {
            for (int j = 0; j < BOARD_W; ++j) {
                scanf("%d", &boards[numBoards][i][j]);
            }
        }
        ++numBoards;
    }

    int winners = 0;
    bool won[numBoards];
    for (int i = 0; i < numBoards; ++i)
        won[i] = false;
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
                            ++winners;
                            won[b] = true;
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
                            ++winners;
                            won[b] = true;
                            goto next;
                        }
                    }
                }
next:;
                if (winners == numBoards)
                    goto done;
            }
        }
    }
done:;

    assert(winners == numBoards);
    int sum = 0;
    for (int i = 0; i < BOARD_H; ++i) {
        for (int j = 0; j < BOARD_W; ++j) {
            if (boards[loser][i][j] >= 0) {
                sum += boards[loser][i][j];
            }
        }
    }
    int score = sum * curr;
    printf("%d\n", score);
}

int main() {
    part1();
    part2();
}
