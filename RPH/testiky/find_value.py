def value_count(data, value):
    # count all occurences of VALUE in matrix DATA
    count = 0
    for x in data:
        for y in x:
            if y == value:
                count += 1
    return count

def value_positions(data, value):
    # find positions of all occurences of VALUE in matrix DATA
    out = []
    for x in range(len(data)):
        for y in range(len(data[x])):
            if data[x][y] == value:
                out.append((x, y))
    return out

# test
if __name__ == "__main__":
    value = -1
    data = [[0,-1,1],[-1,0,-1],[1,0,-1]]
    count = value_count(data, value)
    print(count)
    value = 0
    positions = value_positions(data, value)
    print(positions)
