#include <stdio.h>

int main() {
	freopen("input.txt", "r", stdin);
	
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

