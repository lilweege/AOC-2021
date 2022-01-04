#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

typedef unsigned long long ull;

#define ARR_CAP 64
typedef struct {
    ull data[ARR_CAP];
    int size;
} arr;

void pb(arr* a, ull x) {
    assert(a->size < ARR_CAP-1);
    a->data[a->size++] = x;
}


#define MAX_STRLEN 4096
bool bitSeq[MAX_STRLEN*4];
int seqSize;

int getInt(int idx, int num) {
    assert(num < 31 && idx+num <= seqSize);
    int x = 0;
    for (int i = idx; i < idx+num; ++i) {
        x = (x << 1) | bitSeq[i];
    }
    return x;
}

int vSum = 0;
ull parsePacket(int* idx) {
    int V = getInt(*idx, 3); vSum += V;
    int T = getInt(*idx+3, 3);
    // printf("V=%d T=%d\n", V, T);
    *idx += 6;
    if (T == 4) {
        // value literal packet
        ull val = 0;
        while (*idx < seqSize - 5) {
            val = (val << 4) + getInt(*idx+1, 4);
            *idx += 5;
            if (bitSeq[*idx - 5] == 0) {
                break;
            }
        }
        return val;
    }
    else { // operator
        bool I = bitSeq[(*idx)++];
        arr packets;
        packets.size = 0;

        if (I == 0) {
            // next 15 bits = num bits of packets
            int numBits = getInt(*idx, 15);
            *idx += 15;
            int end = *idx + numBits;
            while (*idx < end) {
                pb(&packets, parsePacket(idx));
            }
        }
        else {
            // next 11 bits = num packets
            int numPackets = getInt(*idx, 11);
            *idx += 11;
            for (int i = 0; i < numPackets; ++i) {
                pb(&packets, parsePacket(idx));
            }
        }

        ull val = 0;
        if (T == 0) {
            val = 0;
            for (int i = 0; i < packets.size; ++i)
                val += packets.data[i];
        }
        else if (T == 1) {
            val = 1;
            for (int i = 0; i < packets.size; ++i)
                val *= packets.data[i];
        }
        else if (T == 2) {
            val = 1<<30;
            for (int i = 0; i < packets.size; ++i)
                if (packets.data[i] < val)
                    val = packets.data[i];
        }
        else if (T == 3) {
            val = 0;
            for (int i = 0; i < packets.size; ++i)
                if (packets.data[i] > val)
                    val = packets.data[i];
        }
        else if (T == 5) {
            assert(packets.size == 2);
            val = packets.data[0] > packets.data[1] ? 1 : 0;
        }
        else if (T == 6) {
            assert(packets.size == 2);
            val = packets.data[0] < packets.data[1] ? 1 : 0;
        }
        else if (T == 7) {
            assert(packets.size == 2);
            val = packets.data[0] == packets.data[1] ? 1 : 0;
        }
        else {
            assert(0 && "Unreachable");
        }
        return val;
    }
    assert(0 && "Unreachable");
}

void solve() {
    char packet[MAX_STRLEN];
    scanf("%s%n", packet, &seqSize);
    
    for (int i = 0; i < seqSize; ++i) {
        char c = packet[i];
        c -= c >= 'A' ? 'A'-10 : '0';
        bitSeq[i*4+0] = (c>>3&1);
        bitSeq[i*4+1] = (c>>2&1);
        bitSeq[i*4+2] = (c>>1&1);
        bitSeq[i*4+3] = (c   &1);
    }
    seqSize *= 4;
    int idx = 0;
    ull value = parsePacket(&idx);
    printf("%d\n", vSum);
    printf("%llu\n", value);
}

int main() {
    solve();
}
