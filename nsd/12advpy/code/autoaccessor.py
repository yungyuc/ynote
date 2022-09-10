class AutoAccessor:
    """Routing access to all instance attributes to alternate names on the
    instance."""
    def __init__(self):
        self.name = None
    def __get__(self, obj, objtype):
        print('On object {} , auto retrieve: {}'.format(obj, self.name))
        varname = '_acs' + self.name
        if not hasattr(obj, varname):
            setattr(obj, varname, None)
        return getattr(obj, varname)
    def __set__(self, obj, val):
        print('On object {} , auto update: {}'.format(obj, self.name))
        varname = '_acs' + self.name
        return setattr(obj, varname, val)


class AutoAccessorMeta(type):
    """Metaclass to use the new automatic descriptor."""
    def __new__(cls, name, bases, namespace):
        print('DEBUG before names:', name)
        print('DEBUG before bases:', bases)
        print('DEBUG before namespace:', namespace)
        for k, v in namespace.items():
            if isinstance(v, AutoAccessor):
                v.name = k
        # Create the class object for MyAutoClass.
        newcls = super(AutoAccessorMeta, cls).__new__(
                cls, name, bases, namespace)
        print('DEBUG after names:', name)
        print('DEBUG after bases:', bases)
        print('DEBUG after namespace:', namespace)
        return newcls

# vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4: