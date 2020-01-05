from cs50 import get_int

while True:
    size = get_int("Height: ")
    if size > 0 and size < 9:
        break

for i in range(size):
    for j in range(size):
        if j >= size - i - 1:
            print("#", end="")
        else:
            print(" ", end="")

    print("  ", end="")

    for k in range(size):
        if k <= i:
            print("#", end="")

    print()
