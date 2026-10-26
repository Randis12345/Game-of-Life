import sys

if __name__ == "__main__":
    assert len(sys.argv) == 2
    rle = sys.argv[1].splitlines()[-1]
    x = 0
    y = 0
    count = ""
    
    for char in rle:
      if char.isdigit():
        count += char
      elif char in "bo":
        n = int(count) if count else 1
        if char == "o":
          for i in range(n):
            print(f"{x+i} {y}")
        x += n
        count = ""
      elif char == "$":
        y += int(count) if count else 1
        x = 0
        count = ""
      elif char == "!":
        break
