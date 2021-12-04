#include <stdio.h>

#define FILENAME "input.txt"

void part1() {
	freopen(FILENAME, "r", stdin);
	int last = 1 << 30;
	int curr;
	int cnt = 0;
	while (scanf("%d", &curr) != EOF) {
		if (curr > last)
			++cnt;
		last = curr;
	}
	printf("%d\n", cnt);
}

void part2() {
	freopen(FILENAME, "r", stdin);
	int minus1, minus2, minus3;
	scanf("%d", &minus3);
	scanf("%d", &minus2);
	scanf("%d", &minus1);

	int currSum = minus1 + minus2 + minus3;
	int lastSum = currSum;
	
	int cnt = 0;
	int currDep = 0;
	while (scanf("%d", &currDep) != EOF) {
		currSum -= minus3;
		currSum += currDep;
		minus3 = minus2;
		minus2 = minus1;
		minus1 = currDep;
		if (currSum > lastSum)
			++cnt;
		lastSum = currSum;
	}
	printf("%d\n", cnt);
}


int main() {
	part1();
	part2();
}
