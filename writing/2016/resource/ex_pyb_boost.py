import ex_pyb_boost as pymod
def pycount(obj): import sys; return sys.getrefcount(obj) - 3
# get shared pointers
objs = [pymod.Resource.make() for it in range(10)]; obj = objs[0]
assert 1 == pymod.Resource.get_count()
assert 11 == pycount(obj); print("  pycount(obj) =", pycount(obj))
holder = pymod.Holder(obj)
assert 1 == holder.get_count()
assert 2 == pymod.Resource.get_count_from_python("obj", obj)
del objs, obj
assert 0 == pymod.Resource.get_count()
assert 0 == holder.get_count()
