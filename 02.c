#include <stdio.h>
#include <string.h>

#define FILENAME "input.txt"

void part1() {
	freopen(FILENAME, "r", stdin);
	char buf[10];
	int x;
	int depth = 0;
	int horiz = 0;
	while (scanf("%s ", buf) != EOF) {
		scanf("%d", &x);
		if (strcmp(buf, "up") == 0) {
			depth -= x;
		}
		else if (strcmp(buf, "down") == 0) {
			depth += x;
		}
		else { // forward
			horiz += x;
		}
	}
	printf("%d\n", depth * horiz);
}

void part2() {
	freopen(FILENAME, "r", stdin);
	char buf[10];
	int x;
	int depth = 0;
	int horiz = 0;
	int aim = 0;
	while (scanf("%s ", buf) != EOF) {
		scanf("%d", &x);
		if (strcmp(buf, "up") == 0) {
			aim -= x;
		}
		else if (strcmp(buf, "down") == 0) {
			aim += x;
		}
		else { // forward
			horiz += x;
			depth += aim * x;
		}
	}
	printf("%d\n", depth * horiz);
}

int main() {
	part1();
	part2();
}
