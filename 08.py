FILENAME = "input.txt"

# this problem is annoying

'''

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

'''

def solve1(inp, out):
    return sum(1 for x in out.split() if
        (n := len(x)) == 2 or n == 3 or n == 4 or n == 7)


def segToInt(seg):
    return [
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
    ].index(''.join(sorted(seg)))

def remap(num, key):
    x = ""
    for char in num:
        x += key[char]
    return x

def solve2(inp, out):
    # brute forcing all permutations is somehow less interesting
    # than this painful ad hoc solution

    inp = sorted(inp.split(), key=len)
    out = out.split()
    # len: 2 3 4 5 5 5 6 6 6 7
    # num: 1 7 4 x x x y y y 8
    # x: [2,3,5], y: [0,6,9]

    key = {}
    one = inp[0]
    seven = inp[1]
    four = inp[2]
    for char in seven:
        if char not in one:
            key[char] = 'a'
            break
    assert len(key) == 1


    for num in inp[6:9]: # len=6
        if (one[0] in num) != (one[1] in num):
            # num is 6
            if one[0] in num:
                key[one[0]] = 'f'
                key[one[1]] = 'c'
            else:
                key[one[0]] = 'c'
                key[one[1]] = 'f'
            break
    assert len(key) == 3
    

    diff = ""
    for char in four:
        if char not in one:
            diff += char
    assert len(diff) == 2

    for num in inp[6:9]: # len = 6
        if (diff[0] in num) != (diff[1] in num):
            # num is 0
            if diff[0] in num:
                key[diff[0]] = 'b'
                key[diff[1]] = 'd'
            else:
                key[diff[0]] = 'd'
                key[diff[1]] = 'b'
    assert len(key) == 5


    diff = ""
    for char in "abcdefg":
        if char not in key:
            diff += char
    assert len(diff) == 2

    for num in inp[6:9]: # len = 6
        if (diff[0] in num) != (diff[1] in num):
            # num is 9
            if diff[0] in num:
                key[diff[0]] = 'g'
                key[diff[1]] = 'e'
            else:
                key[diff[0]] = 'e'
                key[diff[1]] = 'g'
    assert len(key) == 7

    tot = 0
    for num in out:
        tot = tot * 10 + segToInt(remap(num, key))
    return tot


with open(FILENAME, "r") as f:
    stuff = [x.split(" | ") for x in f.read().split("\n")]
    print(sum(solve1(first, second) for first, second in stuff))
    print(sum(solve2(first, second) for first, second in stuff))
