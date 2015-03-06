from ctypes import CDLL, c_void_p, Structure, addressof, byref, cast
from ctypes.util import find_library
# I want to store a pointer to a C function.
class MyData(Structure):
    _fields_ = [
        ('funcptr', c_void_p),
    ]
mydata = MyData()
# Let's say the C function I want is printf.
libc = CDLL(find_library('c'))
cfunc = libc.printf
print 'It\'s the C function we have:', cfunc
print '    The addressof() is somehow interesting:', '%lx' % addressof(cfunc)
print '    byref() is also not useful:', byref(cfunc)
# Create a CArg pointer object from the C function.
funccarg = c_void_p.from_param(cfunc)
print 'This guy can be passed into another C function as parameter:'
print '   ', type(funccarg), funccarg
print 'But it cannot be set into the MyData Structure?'
try:
    # Oooops, it DOES NOT WORK.
    mydata.funcptr = funccarg
except TypeError, e:
    print '    Now you have a TypeError, saying "%s"' % e
# Create a real c_void_p object.
funcptr = cast(libc.printf, c_void_p)
print 'cast() saves the day:'
print '   ', type(funcptr), '%lx' % funcptr.value
print '   ', funcptr, funcptr.value
mydata.funcptr = funcptr.value
print 'It works:', '%lx' % mydata.funcptr
