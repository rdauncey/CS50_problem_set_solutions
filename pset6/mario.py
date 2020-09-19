from cs50 import get_int

# Get user input to determine height
while True:
    height = get_int("Height: ")
    if height > 0 and height < 9:
        break

# Print pyramid accordingly
for i in range(1, height + 1):
    print((" " * (height - i)) + ("#" * i), end="  ")
    print("#" * i)