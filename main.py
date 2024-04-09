from functools import lru_cache

c = [-1,1,4,5,7,8,12,13,18,19,21]

@lru_cache(maxsize=None)
def m(n, k):
    global c
    if n == 1:
        return k*c[k]
    else:
        return min((k - d)*c[k] + m(n - 1, d) for d in range(n - 1, k))

result = m(3, 10)
print(result)

n = 2
p = {}
size = len(c) - 1
while (n > 0):
  counter = 1
  while (m(n, size - counter) != result - (counter*c[size])):
      counter += 1
  p[size] = counter
  result -= counter*c[size]
  size -= counter
  n -= 1

p[size] = size
print(p)