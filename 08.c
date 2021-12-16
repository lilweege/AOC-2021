#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define FILENAME "input.txt"

/*
  0:      1:      2:      3:      4:
 aaaa    ....    aaaa    aaaa    ....
b    c  .    c  .    c  .    c  b    c
b    c  .    c  .    c  .    c  b    c
 ....    ....    dddd    dddd    dddd
e    f  .    f  e    .  .    f  .    f
e    f  .    f  e    .  .    f  .    f
 gggg    ....    gggg    gggg    ....

  5:      6:      7:      8:      9:
 aaaa    aaaa    aaaa    aaaa    aaaa
b    .  b    .  .    c  b    c  b    c
b    .  b    .  .    c  b    c  b    c
 dddd    dddd    ....    dddd    dddd
.    f  e    f  .    f  e    f  .    f
.    f  e    f  .    f  e    f  .    f
 gggg    gggg    ....    gggg    gggg
*/

#define MAX_LINES 200

char lines[MAX_LINES][14][8];
int sizes[MAX_LINES][14];
int numLines = 0;
char* orig[10] = {
    "abcefg",
    "cf",
    "acdeg",
    "acdfg",
    "bcdf",
    "abdfg",
    "abdefg",
    "acf",
    "abcdefg",
    "abcdfg"
};
int origSizes[10] = { 6, 2, 5, 5, 4, 5, 6, 3, 7, 6 };


void readInput() {
    freopen(FILENAME, "r", stdin);
    while (scanf("%s", lines[numLines][0]) != EOF) {
        sizes[numLines][0] = strlen(lines[numLines][0]);
        for (int i = 1; i < 14; ++i) {
            if (i == 10) scanf(" | ");
            scanf("%s", lines[numLines][i]);
            sizes[numLines][i] = strlen(lines[numLines][i]);
        }
        ++numLines;
    }
}

void part1() {
    int ans = 0;
    for (int l = 0; l < numLines; ++l) {
        for (int i = 10; i < 14; ++i) {
            int n = sizes[l][i];
            if (n == 2 || n == 3 || n == 4 || n == 7) {
                ++ans;
            }
        }
    }
    printf("%d\n", ans);
}

void part2() {
    int ans = 0;
    for (int l = 0; l < numLines; ++l) {
        // len: 2 3 4 5 5 5 6 6 6 7
        // num: 1 7 4 x x x y y y 8
        // x: [2,3,5], y: [0,6,9]
        int key[7]; // a b c d e f g
        memset(key, -1, 7*sizeof(int));

        int one = -1, seven = -1, four = -1;
        for (int i = 0; i < 10; ++i) {
            int n = sizes[l][i];
            if (n == 2) one = i;
            else if (n == 3) seven = i;
            else if (n == 4) four = i;
        }
        assert(one != -1 && seven != -1 && four != -1);

        for (int i = 0; i < 3; ++i) {
            char ch = lines[l][seven][i];
            bool in = false;
            for (int j = 0; j < 2; ++j) {
                in |= ch == lines[l][one][j];
            }
            if (!in) {
                key[ch-'a'] = 0;
                goto done1;
            }
        }
        assert(0);
done1:;

        for (int i = 0; i < 10; ++i) {
            if (sizes[l][i] != 6) continue;
            char ch0 = lines[l][one][0];
            char ch1 = lines[l][one][1];
            bool ch0in = false, ch1in = false;
            for (int j = 0; j < 6; ++j) {
                ch0in |= ch0 == lines[l][i][j];
                ch1in |= ch1 == lines[l][i][j];
            }
            if (ch0in != ch1in) {
                if (ch0in) {
                    key[ch0-'a'] = 5;
                    key[ch1-'a'] = 2;
                }
                else {
                    key[ch0-'a'] = 2;
                    key[ch1-'a'] = 5;
                }
                goto done2;
            }
        }
        assert(0);
done2:;

        char diff[2];
        int diffIdx = 0;
        for (int i = 0; i < 4; ++i) {
            char ch = lines[l][four][i];
            bool in = false;
            for (int j = 0; j < 2; ++j) {
                in |= ch == lines[l][one][j];
            }
            if (!in) {
                diff[diffIdx++] = ch;
            }
        }
        assert(diffIdx == 2);

        for (int i = 0; i < 10; ++i) {
            if (sizes[l][i] != 6) continue;
            char ch0 = diff[0], ch1 = diff[1];
            bool ch0in = false, ch1in = false;
            for (int j = 0; j < 6; ++j) {
                ch0in |= ch0 == lines[l][i][j];
                ch1in |= ch1 == lines[l][i][j];
            }
            if (ch0in != ch1in) {
                if (ch0in) {
                    key[ch0-'a'] = 1;
                    key[ch1-'a'] = 3;
                }
                else {
                    key[ch0-'a'] = 3;
                    key[ch1-'a'] = 1;
                }
                goto done3;
            }
        }
        assert(0);
done3:;

        diffIdx = 0;
        for (int i = 0; i < 7; ++i) {
            char ch = 'a' + i;
            bool in = false;
            for (int j = 0; j < 7; ++j) {
                if (key[j] != -1)
                    in |= ch == j + 'a';
            }
            if (!in) {
                diff[diffIdx++] = ch;
            }
        }
        assert(diffIdx == 2);

        for (int i = 0; i < 10; ++i) {
            if (sizes[l][i] != 6) continue;
            char ch0 = diff[0], ch1 = diff[1];
            bool ch0in = false, ch1in = false;
            for (int j = 0; j < 6; ++j) {
                ch0in |= ch0 == lines[l][i][j];
                ch1in |= ch1 == lines[l][i][j];
            }
            if (ch0in != ch1in) {
                if (ch0in) {
                    key[ch0-'a'] = 6;
                    key[ch1-'a'] = 4;
                }
                else {
                    key[ch0-'a'] = 4;
                    key[ch1-'a'] = 6;
                }
                goto done4;
            }
        }
        assert(0);
done4:;


        int val = 0;
        for (int i = 10; i < 14; ++i) {
            bool has[7];
            memset(has, 0, 7);
            for (int j = 0; j < sizes[l][i]; ++j) {
                has[key[lines[l][i][j]-'a']] = true;
            }
            int origIdx = -1;
            for (int j = 0; j < 10; ++j) {
                if (sizes[l][i] != origSizes[j]) {
                    continue;
                }
                bool yes = true;
                for (int k = 0; k < origSizes[j]; ++k) {
                    if (!has[orig[j][k]-'a']) {
                        yes = false;
                        break;
                    }
                }
                if (yes) {
                    origIdx = j;
                    break;
                }
            }
            assert(origIdx != -1);
            val = val * 10 + origIdx;
        }
        ans += val;
    }
    printf("%d\n", ans);
}

int main() {
    readInput();
    part1();
    part2();
}
