import ex_bpy_to_python as pymod
def pycount(obj): import sys; return sys.getrefcount(obj) - 3
print("before construction")
pymod.Resource.get_count()
print("construct the resource and return the shared pointers")
# get shared pointers
objs = [pymod.Resource.make() for it in range(10)]
assert 10 == pymod.Resource.get_count()
assert 1 == pycount(objs[0]); print("  pycount(obj) =", pycount(objs[0]))
print("delete Python objects")
del objs
assert 0 == pymod.Resource.get_count()
print("program ends")
"""output:
before construction
  Resource.g_weak.use_count() = 0
construct the resource and return the shared pointers
  Resource 0x100506b00 constructed
  Resource.g_weak.use_count() = 10
  pycount(obj) = 1
delete Python objects
  Resource 0x100506b00 destructed
  Resource.g_weak.use_count() = 0
program ends
"""
