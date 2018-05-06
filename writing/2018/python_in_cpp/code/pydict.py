import _pydict
d = _pydict.do("a", 7, "b", "name", 10, 4.2)
print(d)
d2 = _pydict.do2(d, "b", "d")
print("The returned dictionary is empty:", d2)
print("The first dictionary becomes empty too:", d)
print("Are the two dictionaries the same?", d2 is d)
