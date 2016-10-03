import pymod

print("Test %s::shared_ptr" % ("boost" if pymod.boost_shared_ptr else "std"))
print()

print("Initially no resource is allocated:")
assert pymod.Object.get_strong_count() == 0
assert pymod.Object.get_weak_count() == 0
pymod.Object.print_status()

print("Create the resource:")
pymod.Object.make(10)
assert pymod.Object.get_strong_count() == 1
pymod.Object.print_status()

print("Convert the shared pointer to Python:")
obj = pymod.Object.get_strong()
assert pymod.Object.get_strong_count() == 2
if pymod.wrapper_type == "bpy":
    if pymod.boost_shared_ptr:
        assert pymod.get_cpp_count(obj) == 1, pymod.get_cpp_count(obj)
    else:
        assert pymod.get_cpp_count(obj) == 2, pymod.get_cpp_count(obj)
else:
    assert pymod.get_cpp_count(obj) == 3, pymod.get_cpp_count(obj)
pymod.Object.print_status()
pymod.print_from_python(obj)

print("Reset internal strong reference:")
pymod.Object.reset_strong()
assert pymod.Object.get_strong_count() == 0
assert pymod.Object.get_weak_count() == 1
pymod.Object.print_status()

print("Remove the Python object:")
pymod.print_from_python(obj)
del obj
assert pymod.Object.get_strong_count() == 0
assert pymod.Object.get_weak_count() == 0
pymod.Object.print_status()

print()
print("Create another object:")
pymod.Object.make(20)
assert pymod.Object.get_strong_count() == 1
pymod.Object.print_status()

print("Enabled shared from this (%s)" % pymod.shared_from_this)
obj = pymod.Object.get_strong()
assert pymod.Object.get_strong_count() == 2
if pymod.wrapper_type == "bpy":
    if pymod.boost_shared_ptr:
        assert pymod.get_cpp_count(obj) == 1, pymod.get_cpp_count(obj)
    else:
        assert pymod.get_cpp_count(obj) == 2, pymod.get_cpp_count(obj)
else:
    assert pymod.get_cpp_count(obj) == 3, pymod.get_cpp_count(obj)
pymod.print_from_python(obj)
pymod.Object.print_status()

holder = pymod.Holder(obj)
if pymod.wrapper_type == "bpy" and pymod.boost_shared_ptr and not pymod.shared_from_this:
    assert holder.get_count() == 0, holder.get_count()
else:
    assert holder.get_count() == 2, holder.get_count()
print("Holder gets %d reference count" % holder.get_count())

print()
print("End of test")
