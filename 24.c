#include <stdio.h>
#include <string.h>

/*
input matches the following pattern, once for each digit
the z register is basically a stack in base 26

inp w
mul x 0             // x = 0
add x z             // x = z
mod x 26            // x = z.peek()
div z (1|26)        // nop | z.pop()
add x (-?\d+)       // x = top + val1
eql x w             // x = top + val1 == w
eql x 0             // x = top + val1 != w
mul y 0             // y = 0
add y 25            // y = 25
mul y x             // y = top + val1 == w ? 0 : 25
add y 1             // y = top + val1 == w ? 1 : 26
mul z y             // top + val1 == w ? nop : z.push(0)
mul y 0             // y = 0
add y w             // y = w
add y (\d+)         // y = w + val2
mul y x             // y = top + val1 == w ? 0 : w + val2
add z y             // top + val1 == w ? nop : z[-1] = w + val2

for z to be zero at the end, the stack must finish empty, which in this case is
equivalent to number of pushes == number of pops (will never pop when 0 size)
each push operation will have a corresponding pop pair in which val2 push is val1 pop
*/

#define INPUT_PATTERN "inp w\nmul x 0\nadd x z\nmod x 26\ndiv z %d\nadd x %d\neql x w\neql x 0\nmul y 0\nadd y 25\nmul y x\nadd y 1\nmul z y\nmul y 0\nadd y w\nadd y %d\nmul y x\nadd z y\n"
#define LEN 14

void solve() {
    char ans1[LEN+1], ans2[LEN+1];
    memset(ans1, '9', LEN); ans1[LEN] = 0;
    memset(ans2, '1', LEN); ans2[LEN] = 0;
    int vals[LEN], idxs[LEN];
    int size = 0;
    for (int divZ, val1, val2, i = 0;
        scanf(INPUT_PATTERN, &divZ, &val1, &val2) != EOF;
        ++i)
    {
        if (divZ == 26) {
            int x = vals[--size];
            int j = idxs[size];
            int diff = x + val1;
            if (diff > 0) {
                ans1[j] = '9'-diff;
                ans2[i] = '1'+diff;
            }
            else {
                ans1[i] = '9'+diff;
                ans2[j] = '1'-diff;
            }
        }
        else { // 1
            vals[size] = val2;
            idxs[size++] = i;
        }
    }

    printf("%s\n%s\n", ans1, ans2);
}

int main() {
    solve();
}
