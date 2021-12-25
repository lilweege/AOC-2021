#include <stdio.h>

void solve() {
	int cnt1 = 0, cnt2 = 0;
	int minus1, minus2, minus3;
	scanf("%d", &minus3);
	scanf("%d", &minus2); if (minus2 > minus3) ++cnt1;
	scanf("%d", &minus1); if (minus1 > minus2) ++cnt1;

	int currSum = minus1 + minus2 + minus3;
	int lastSum = currSum;
	int lastDep = minus1;
	int currDep = 0;
	while (scanf("%d", &currDep) != EOF) {
		currSum -= minus3;
		currSum += currDep;
		minus3 = minus2;
		minus2 = minus1;
		minus1 = currDep;
		if (currDep > lastDep)
			++cnt1;
		if (currSum > lastSum)
			++cnt2;
		lastDep = currDep;
		lastSum = currSum;
	}
	printf("%d\n%d\n", cnt1, cnt2);
}


int main() {
	solve();
}
