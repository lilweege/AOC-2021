#include <stdio.h>
#include <string.h>

void solve() {
	char buf[10];
	int x;
	int depth1 = 0, depth2 = 0;
	int horiz = 0;
	int aim = 0;
	while (scanf("%s ", buf) != EOF) {
		scanf("%d", &x);
		if (buf[0] == 'u') {
			depth1 -= x;
			aim -= x;
		}
		else if (buf[0] == 'd') {
			depth1 += x;
			aim += x;
		}
		else { // forward
			horiz += x;
			depth2 += aim * x;
		}
	}
	printf("%d\n%d\n", depth1 * horiz, depth2 * horiz);
}

int main() {
	solve();
}
