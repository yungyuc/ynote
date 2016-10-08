import ex_bpy_shared_from_this as pymod
def pycount(obj): import sys; return sys.getrefcount(obj) - 3
# get a shared pointers
objs = [pymod.Resource.make() for it in range(10)]; obj = objs[0]
assert 10 == pymod.Resource.get_count()
assert 2 == pycount(obj); print("  pycount(obj) =", pycount(obj))
holder = pymod.Holder(obj)
assert 10 == holder.get_count()
assert 1 == pymod.Resource.get_count_from_python("obj", obj)
del objs, obj
assert 0 == pymod.Resource.get_count()
assert 0 == holder.get_count()
