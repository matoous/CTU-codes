from collections import Counter
from os.path import splitext


def count_chars_in_string(arg):
    cnt = Counter()
    for s in range(len(arg)):
        cnt[arg[s]] += 1
    cnt.pop("\n")
    return cnt.most_common(len(cnt))


def count_chars_in_file(arg):
    with open(arg, 'r', encoding="utf-8") as f:
        read = f.read()
    cnt = count_chars_in_string(read)
    filename = splitext(arg)[0]
    with open(filename + ".freq", 'w', encoding="utf-8") as f:
        for key, freq in cnt:
            f.write(str(key + " " + str(freq) + "\n"))


if __name__ == "__main__":
    count_chars_in_file("in.txt")