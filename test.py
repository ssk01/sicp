def a():
    for i in range(3):
        for j in 'abc':
            yield (i ,j)
b = a()
print(b)
print(b.__next__())
print(b.__next__())
print(b.__next__())