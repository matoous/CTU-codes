def count_rows_and_words(path):
    lines = words = 0
    with open(path,"r",encoding="utf-8") as f:
        for line in f.readlines():
            lines += 1
            words += len(line.split())
    return (lines, words)

if __name__ == "__main__":
    print(count_rows_and_words("test.txt"))