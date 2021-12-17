#include <stdio.h>

#define FILENAME "input.txt"
#define MAX_IT 500
#define MAX_DY 200

int x0, x1, y0, y1;
int simulate(int dx, int dy) {
    int x = 0, y = 0;
    int by = 0;
    for (int it = 0; it < MAX_IT; ++it) {
        if (x > x1 || y < y0)
            break;
        x += dx;
        y += dy;
        if (y > by) by = y;
        if (x0 <= x && x <= x1 && y0 <= y && y <= y1)
            return by;
        dx -= (dx > 0) - (dx < 0);
        dy -= 1;
    }
    return -1;
}

void solve() {
    freopen(FILENAME, "r", stdin);
    scanf("target area: x=%d..%d, y=%d..%d",
        &x0, &x1, &y0, &y1);
    int ans1 = 0, ans2 = 0;
    // not sure if there's a mathematical way to solve this
    for (int dx = 0; dx <= x1; ++dx)
        for (int dy = y0; dy <= MAX_DY; ++dy) {
            int by = simulate(dx, dy);
            if (by != -1) {
                if (ans1 < by)
                    ans1 = by;
                ++ans2;
            }
        }
    printf("%d\n%d\n", ans1, ans2);
}

int main() {
    solve();
}
