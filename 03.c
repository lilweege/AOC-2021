#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define NUM_BITS 12
#define MAX_VALUES 1000

void solve() {
    char values[MAX_VALUES+1][NUM_BITS+1];
    int numValues = 0;
    int bitCount[NUM_BITS];
    memset(bitCount, 0, NUM_BITS*sizeof(int));

    while (scanf("%s", values[numValues]) != EOF) {
        for (int i = 0; i < NUM_BITS; ++i)
            if (values[numValues][i] == '1')
                ++bitCount[i];
        ++numValues;
    }
    int gamma = 0;
    for (int i = 0; i < NUM_BITS; ++i)
        if (bitCount[i] > numValues / 2)
            gamma += 1 << (NUM_BITS-i-1);
    int epsilon = ~(gamma - (1 << NUM_BITS));
    printf("%d\n", gamma * epsilon);

    bool OxyOk[numValues], CO2Ok[numValues];
    for (int i = 0; i < numValues; ++i)
        OxyOk[i] = CO2Ok[i] = 1;
    int numOxy = numValues;
    int numCO2 = numValues;

    for (int i = 0; i < NUM_BITS; ++i) {
        int OxyBitCount = 0;
        int CO2BitCount = 0;
        for (int j = 0; j < numValues; ++j) {
            if (values[j][i] == '1') {
                if (OxyOk[j]) ++OxyBitCount;
                if (CO2Ok[j]) ++CO2BitCount;
            }
        }
        int OxyCommon = '0' + (OxyBitCount >= numOxy - OxyBitCount);
        int CO2Common = '0' + (CO2BitCount >= numCO2 - CO2BitCount);
        for (int j = 0; j < numValues; ++j) {
            if (OxyOk[j] && numOxy > 1 && values[j][i] != OxyCommon) {
                OxyOk[j] = 0;
                --numOxy;
            }
            if (CO2Ok[j] && numCO2 > 1 && values[j][i] == CO2Common) {
                CO2Ok[j] = 0;
                --numCO2;
            }
        }
        if (numOxy == 1 && numCO2 == 1) {
            break;
        }
    }

    assert(numOxy == 1 && numCO2 == 1);
    int OxyIdx = -1, CO2Idx = -1;
    for (int i = 0; i < numValues; ++i) {
        if (OxyOk[i]) OxyIdx = i;
        if (CO2Ok[i]) CO2Idx = i;
    }
    int OxyRating = 0, CO2Rating = 0;
    for (int i = 0; i < NUM_BITS; ++i) {
        if (values[OxyIdx][i] == '1')
            OxyRating += 1 << (NUM_BITS-i-1);
        if (values[CO2Idx][i] == '1')
            CO2Rating += 1 << (NUM_BITS-i-1);
    }
    printf("%d\n", OxyRating * CO2Rating);
}

int main() {
	solve();
}
