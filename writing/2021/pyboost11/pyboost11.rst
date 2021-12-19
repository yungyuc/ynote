================================================
Interoperation between pybind11 and Boost.Python
================================================

:date: 2021/12/19

.. _pybind11: https://pybind11.readthedocs.io/
.. _Boost.Python: https://www.boost.org/doc/libs/1_78_0/libs/python/doc/html/index.html

pybind11_ and Boost.Python_ are two different tools offering similar
capabilities to wrap between C++ and Python and to extend and embed Python.
They can wrap the C++ constructs to Python, and allow C++ code to invoke Python
code.  At the center of the capabilities lies the system for bi-directional
conversion between C++ and Python.  By using *both* tools simultaneously, we
have delicate 4-way conversion:

#. Pass **pybind11-wrapped** (C++) objects to **Boost.Python-wrapped** (C++)
   functions.

#. Return **pybind11-wrapped** (C++) objects from **Boost.Python-wrapped**
   (C++) functions.

#. Pass **Boost.Python-wrapped** (C++) objects to **pybind11-wrapped** (C++)
   functions.

#. Return **Boost.Python-wrapped** (C++) objects from **pybind11-wrapped**
   (C++) functions.

Why Mixing Two Wrapping Systems?
================================

It is very rare to need to use pybind11 and Boost.Python together.  When it
happens, I guess you either use Boost.Python already and want or need to use
pybind11 to replace it, or need to combine two systems that unfortunately chose
the different tools.

If you are creating a new wrapping layer, choose pybind11_, because modern C++
is the way to go, and pybind11 offers much cleaner and powerful API.
Boost.Python is a good tool, but you usually do not want the complexity
incorporated in Boost, as it is described by the pybind11_ project:

  The main issue with Boost.Python—and the reason for creating such a similar
  project—is Boost.

I think you only need to consider Boost.Python_ when you cannot use C++11.

Conversion for pybind11 and Boost.Python
========================================

pybind11 and Boost.Python use different design to convert between C++ and
Python.  pybind11 uses its ":doc:`casters <pybind11:advanced/cast/custom>`"
while Boost.Python uses its "`converters
<https://www.boost.org/doc/libs/1_78_0/libs/python/doc/html/reference/to_from_python_type_conversion.html>`__".
When wrapping using pybind11 and Boost.Python, the tools will create the
"casters" and "converters" behind the scenes, so that you do not need to worry
about the conversion of the C++ objects they wrap to and from Python.  We
cannot expect the tools to automatically recognize each other, and need to do
something special in the "casters" and "converters".

Let's write two classes: ``ItemPyb`` (:ref:`pyboost11-ItemPyb`) and ``ItemBpy``
(:ref:`pyboost11-ItemBpy`).  The two classes may return new instances of their
own type and the other type, as shown in the highlighted lines.

.. code-block:: cpp
   :name: pyboost11-ItemPyb
   :caption: Class to be wrapped by pybind11
   :linenos:
   :emphasize-lines: 13-17

    struct ItemPyb
    {
        ItemPyb(int v) : m_value(v) {}
        ItemPyb() = default;
        ItemPyb(ItemPyb const & other) : m_value(other.m_value) {}
        ItemPyb & operator=(ItemPyb const & other) { m_value = other.m_value; return *this; }

        int value() const { return m_value; }
        void set_value(int v) { m_value = v; }

        intptr_t address() const { return reinterpret_cast<intptr_t>(this); }

        void take_pyb(ItemPyb const & other);
        void take_bpy(ItemBpy const & other);

        ItemPyb make_pyb();
        ItemBpy make_bpy();

    private:

        int m_value = 0;
    };

.. code-block:: cpp
   :name: pyboost11-ItemBpy
   :caption: Class to be wrapped by Boost.Python
   :linenos:
   :emphasize-lines: 13-17

    struct ItemBpy
    {
        ItemBpy(int v) : m_value(v) {}
        ItemBpy() = default;
        ItemBpy(ItemBpy const & other) : m_value(other.m_value) {}
        ItemBpy & operator=(ItemBpy const & other) { m_value = other.m_value; return *this; }

        int value() const { return m_value; }
        void set_value(int v) { m_value = v; }

        intptr_t address() const { return reinterpret_cast<intptr_t>(this); }

        void take_pyb(ItemPyb const & other);
        void take_bpy(ItemBpy const & other);

        ItemPyb make_pyb();
        ItemBpy make_bpy();

    private:

        int m_value = 0;
    };

pybind11 Caster for Boost.Python
++++++++++++++++++++++++++++++++

By default, pybind11 does not know what to do with a class that is wrapped by
Boost.Python.  So we need to create pybind11 casters for the
Boost.Python-wrapped classes.  In the highlighted lines, I copy code from the
macro ``PYBIND11_TYPE_CASTER`` to make it work.

.. code-block:: cpp
   :name: pyboost11-caster
   :caption: pybind11 caster for Boost.Python-wrapped classes
   :linenos:
   :emphasize-lines: 42-58

    namespace pyboost11
    {

    // Pybind11 cast by using Boost.Python.
    template <typename T> struct caster
    {

        caster(pybind11::handle src)
          : obj(boost::python::handle<>(boost::python::borrowed(src.ptr())))
          , ext(obj)
        {}

        bool check() const { return ext.check(); }

        // From-Python conversion.
        operator T() { return ext(); }
        T operator()() { return ext(); }

        // To-Python conversion.
        static pybind11::handle to_python(T & src)
        {
            namespace bpy = boost::python;
            return bpy::incref(bpy::object(src).ptr());
        }

        boost::python::object obj;
        boost::python::extract<T> ext;

    };

    } // end namespace pyboost11

    namespace pybind11
    {

    namespace detail
    {

    template <typename type> struct pyboost11_type_caster
    {

    // Expanded from PYBIND11_TYPE_CASTER.
    protected:
        type value;
    public:
        template <typename T_, enable_if_t<std::is_same<type, remove_cv_t<T_>>::value, int> = 0>
        static handle cast(T_ *src, return_value_policy policy, handle parent) {
            if (!src) return none().release();
            if (policy == return_value_policy::take_ownership) {
                auto h = cast(std::move(*src), policy, parent); delete src; return h;
            } else {
                return cast(*src, policy, parent);
            }
        }
        operator type*() { return &value; }
        operator type&() { return value; }
        operator type&&() && { return std::move(value); }
        template <typename T_> using cast_op_type = pybind11::detail::movable_cast_op_type<T_>;

        // Boilerplate.
        bool load(handle src, bool)
        {
            if (!src)
            {
                return false;
            }
            pyboost11::caster<type> ext(src);
            if (!ext.check())
            {
                return false;
            }
            value = ext();
            return true;
        }
        static handle cast(type * src, return_value_policy /* policy */, handle /* parent */)
        {
            return pyboost11::caster<type>::to_python(src);
        }
        static handle cast(type src, return_value_policy /* policy */, handle /* parent */)
        {
            return pyboost11::caster<type>::to_python(src);
        }

    };

    #define PYBOOST11_TYPE_CASTER(type, py_name) \
        template <> struct type_caster<type> : public pyboost11_type_caster<type> \
        { static constexpr auto name = py_name; }

    } // end namespace detail

    } // end namespace pybind11

The pybind11 casters is the template specialization created with the new macro
``PYBOOST11_TYPE_CASTER``.  To "use" it, insert the macro to create the
specialization in the pybind11-wrapped module:

.. code-block:: cpp

    PYBOOST11_TYPE_CASTER(ItemBpy, _("ItemBpy"));

The pybind11 library will automatically find it during runtime to perform the
to- and from-Python conversion.

See :ref:`pyboost11-code-pyboost11-hpp` and :ref:`pyboost11-code-modpyb-cpp`
for the full code.

Boost.Python Converter for pybind11
+++++++++++++++++++++++++++++++++++

Boost.Python does not know anything about pybind11, either.  So we need to
create Boost.Python converters for the pybind11-wrapped classes.  The converter
supports both to-python and from-python conversion.

.. code-block:: cpp
   :name: pyboost11-converter
   :caption: Boost.Python converter for pybind11-wrapped classes
   :linenos:

    namespace pyboost11
    {

    // Boost.python convert by using pybind11.
    template <typename T> struct converter
    {

    public:

        converter() { init(); }

        void init()
        {
            static bool initialized = false;
            if (!initialized)
            {
                namespace bpy = boost::python;
                // From-Python conversion.
                bpy::converter::registry::push_back
                (
                    &convertible
                  , &construct
                  , bpy::type_id<T>()
                );
                // To-Python conversion.
                bpy::to_python_converter<T, converter>();

                initialized = true;
            }
        }

        // From-Python convertibility.
        static void * convertible(PyObject * objptr)
        {
            namespace pyb = pybind11;
            try
            {
                pyb::handle(objptr).cast<T>();
                return objptr;
            }
            catch (pyb::cast_error const &)
            {
                return nullptr;
            }
        }

        // From-Python conversion.
        static void construct
        (
            PyObject * objptr
          , boost::python::converter::rvalue_from_python_stage1_data * data
        )
        {
            namespace pyb = pybind11;
            void * storage = reinterpret_cast
            <
                boost::python::converter::rvalue_from_python_storage<T> *
            >(data)->storage.bytes;
            new (storage) T(pyb::handle(objptr).cast<T>());
            data->convertible = storage;
        }

        // To-Python conversion.
        static PyObject * convert(T const & t)
        {
            return pybind11::cast(t).inc_ref().ptr();
        }

    };

    } // end namespace pyboost11

Boost.Python requires us to register the conversion code using its API during
runtime.  The following code needs to be invoked from a Boost.Python-wrapped
module:

.. code-block:: cpp

    pyboost11::converter<ItemPyb>();

See :ref:`pyboost11-code-pyboost11-hpp` and :ref:`pyboost11-code-modbpy-cpp`
for the full code.

The 4-Way Conversion
====================

To this point, we made two helpers.  One is the pybind11 caster that knows how
to convert Boost.Python-wrapped classes to and from Python.  The other is the
Boost.Python converter that knows how to convert pybind11-wrapped classes to
and from Python.  We can test how the helpers work.  In the test classes
``ItemPyb`` and ``ItemBpy`` there are the setter functions to help testing:

.. code-block:: cpp
   :name: pyboost11-test-setters
   :caption: The C++ setters to help testing the wrappers
   :linenos:

    inline void ItemBpy::take_pyb(ItemPyb const & other) { m_value = other.value(); }
    inline void ItemBpy::take_bpy(ItemBpy const & other) { m_value = other.value(); }

    inline ItemPyb ItemBpy::make_pyb() { return ItemPyb(value()); }
    inline ItemBpy ItemBpy::make_bpy() { return ItemBpy(value()); }

    inline void ItemPyb::take_pyb(ItemPyb const & other) { m_value = other.value(); }
    inline void ItemPyb::take_bpy(ItemBpy const & other) { m_value = other.value(); }

    inline ItemPyb ItemPyb::make_pyb() { return ItemPyb(value()); }
    inline ItemBpy ItemPyb::make_bpy() { return ItemBpy(value()); }

Wrappers
++++++++

``ItemPyb`` is wrapped to Python using the standard pybind11 class wrapper:

.. code-block:: cpp

    PYBIND11_MODULE(modpyb, mod)
    {

        namespace pyb = pybind11;

        mod.doc() = "extension module by pybind11";

        pyb::class_<ItemPyb>(mod, "ItemPyb")
            .def(pyb::init<>())
            .def(pyb::init<int>())
            .def_property("value", &ItemPyb::value, &ItemPyb::set_value)
            .def_property_readonly("address", &ItemPyb::address)
            .def("take_pyb", &ItemPyb::take_pyb)
            .def("take_bpy", &ItemPyb::take_bpy)
            .def("make_pyb", &ItemPyb::make_pyb)
            .def("make_bpy", &ItemPyb::make_bpy)
        ;

    }

Similarly, ``ItemBpy`` is wrapped to Python using the standard Boost.Python
class wrapper:

.. code-block:: cpp

    BOOST_PYTHON_MODULE(modbpy)
    {

        namespace bpy = boost::python;

        bpy::class_<ItemBpy>("ItemBpy")
            .def(bpy::init<>())
            .def(bpy::init<int>())
            .add_property("value", &ItemBpy::value, &ItemBpy::set_value)
            .add_property("address", &ItemBpy::address)
            .def("take_pyb", &ItemBpy::take_pyb)
            .def("take_bpy", &ItemBpy::take_bpy)
            .def("make_pyb", &ItemBpy::make_pyb)
            .def("make_bpy", &ItemBpy::make_bpy)
        ;

    }

.. note::

  The full code of the wrappers can be found in :ref:`pyboost11-code-modpyb-cpp`
  and :ref:`pyboost11-code-modbpy-cpp`.

Tests
+++++

Test an ``ItemPyb`` that **takes** and **makes** another ``ItemPyb``:

.. code-block:: python
   :emphasize-lines: 3,5

    self.ipyb = modpyb.ItemPyb()
    self.ipyb.value = 10
    self.ipyb.take_pyb(modpyb.ItemPyb(11))
    self.assertIsInstance(self.ipyb.make_pyb(), modpyb.ItemPyb)
    self.assertEqual(11, self.ipyb.make_pyb().value)

Test the ``ItemPyb`` to **take** and **make** an ``ItemBpy``:

.. code-block:: python
   :emphasize-lines: 3,5

    self.ipyb = modpyb.ItemPyb()
    self.ipyb.value = 10
    self.ipyb.take_bpy(modbpy.ItemBpy(11))
    self.assertIsInstance(self.ipyb.make_bpy(), modbpy.ItemBpy)
    self.assertEqual(11, self.ipyb.value)

Similarly, test an ``ItemBpy`` that **takes** and **makes** an ``ItemPby``:

.. code-block:: python
   :emphasize-lines: 3,5

    self.ibpy = modbpy.ItemBpy()
    self.ibpy.value = 10
    self.ibpy.take_pyb(modpyb.ItemPyb(11))
    self.assertIsInstance(self.ibpy.make_pyb(), modpyb.ItemPyb)
    self.assertEqual(11, self.ibpy.make_pyb().value)

Test the ``ItemBpy`` to **take** and **make** another ``ItemBpy``:

.. code-block:: python
   :emphasize-lines: 3,5

    self.ibpy = modbpy.ItemBpy()
    self.ibpy.value = 10
    self.ibpy.take_bpy(modbpy.ItemBpy(11))
    self.assertIsInstance(self.ibpy.make_bpy(), modbpy.ItemBpy)
    self.assertEqual(11, self.ibpy.make_bpy().value)

Ownership
=========

The 4-way conversion works fine except that the item classes are passed by
value.  We will see the conversion also works well when the classes are managed
by shared pointers.

We need new item classes and the containers to hold them.  Let us start to
build the test infrastructure with the item classes.  ``ItemPybSS``
(:ref:`pyboost11-ItemPybSS`):

.. code-block:: cpp
   :name: pyboost11-ItemPybSS
   :caption: Class managed by shared pointer and wrapped by pybind11
   :linenos:

    /* To be wrapped by pybind11 */
    struct ItemPybSS : std::enable_shared_from_this<ItemPybSS>
    {
    private:

        struct ctor_passkey {};

    public:

        template < typename ... Args >
        static std::shared_ptr<ItemPybSS> make(Args && ... args)
        {
            return std::make_shared<ItemPybSS>(std::forward<Args>(args) ..., ctor_passkey());
        }

        ItemPybSS(int v, ctor_passkey const &) : m_value(v) {}
        ItemPybSS() = delete;
        ItemPybSS(ItemPybSS const &) = delete;
        ItemPybSS & operator=(ItemPybSS const &) = delete;

        int value() const { return m_value; }
        void set_value(int v) { m_value = v; }

        intptr_t address() const { return reinterpret_cast<intptr_t>(this); }

        void take_pybss(ItemPybSS const & other);
        void take_bpybs(ItemBpyBS const & other);

        std::shared_ptr<ItemPybSS> make_pybss();
        boost::shared_ptr<ItemBpyBS> make_bpybs();

    private:

        int m_value = 0;
    };

Then we build ``ItemBpyBS`` (:ref:`pyboost11-ItemBpyBS`).  To make it work with
older version of Boost.Python, we used ``boost::shared_ptr`` instead of
``std::shared_ptr``.

.. note::

  Because the support for ``std::shared_ptr`` is relatively new (see
  https://www.boost.org/users/history/version_1_63_0.html),
  ``boost::shared_ptr`` support in Boost.Python is more stable.

.. code-block:: cpp
   :name: pyboost11-ItemBpyBS
   :caption: Class managed by shared pointer and wrapped by Boost.Python
   :linenos:

    /* To be wrapped by Boost.Python */
    struct ItemBpyBS : boost::enable_shared_from_this<ItemBpyBS>
    {
    private:

        struct ctor_passkey {};

    public:

        template < typename ... Args >
        static boost::shared_ptr<ItemBpyBS> make(Args && ... args)
        {
            return boost::make_shared<ItemBpyBS>(std::forward<Args>(args) ..., ctor_passkey());
        }

        ItemBpyBS(int v, ctor_passkey const &) : m_value(v) {}
        ItemBpyBS() = delete;
        ItemBpyBS(ItemBpyBS const &) = delete;
        ItemBpyBS & operator=(ItemBpyBS const &) = delete;

        int value() const { return m_value; }
        void set_value(int v) { m_value = v; }

        intptr_t address() const { return reinterpret_cast<intptr_t>(this); }

        void take_pybss(ItemPybSS const & other);
        void take_bpybs(ItemBpyBS const & other);

        std::shared_ptr<ItemPybSS> make_pybss();
        boost::shared_ptr<ItemBpyBS> make_bpybs();

    private:

        int m_value = 0;
    };

After having the item classes, we build the container classes.  The containers
will hold the above two item classes as well as the two pass-by-value item
classes we had previously.  The container classes will provide member functions
to take the ownership of the item objects managed by shared pointer.
``ContainerPyb`` (:ref:`pyboost11-ContainerPyb`):

.. code-block:: cpp
   :name: pyboost11-ContainerPyb
   :caption: Container that is wrapped by pybind11
   :linenos:
   :emphasize-lines: 23-24

    /* To be wrapped by pybind11 */
    struct ContainerPyb
    {
        ContainerPyb
        (
            ItemPyb const & pyb, ItemBpy const & bpy
          , std::shared_ptr<ItemPybSS> const & pybss
          , boost::shared_ptr<ItemBpyBS> const & bpybs
        ) : m_pyb(pyb), m_bpy(bpy), m_pybss(pybss), m_bpybs(bpybs) {}

        ItemPyb const & pyb() const { return m_pyb; }
        ItemBpy const & bpy() const { return m_bpy; }

        std::shared_ptr<ItemPybSS> const & pybss() const { return m_pybss; }
        boost::shared_ptr<ItemBpyBS> const & bpybs() const { return m_bpybs; }

        void take_pyb(ItemPyb const & other) { m_pyb.set_value(other.value()); }
        void take_bpy(ItemBpy const & other) { m_bpy.set_value(other.value()); }

        void set_pybss(std::shared_ptr<ItemPybSS> const & other) { m_pybss = other; }
        void set_bpybs(boost::shared_ptr<ItemBpyBS> const & other) { m_bpybs = other; }

        void set_pybss_ref(ItemPybSS & other) { m_pybss = other.shared_from_this(); }
        void set_bpybs_ref(ItemBpyBS & other) { m_bpybs = other.shared_from_this(); }

    private:

        ItemPyb m_pyb;
        ItemBpy m_bpy;
        std::shared_ptr<ItemPybSS> m_pybss;
        boost::shared_ptr<ItemBpyBS> m_bpybs;
    };

``ContainerBpy`` (:ref:`pyboost11-ContainerBpy`):

.. code-block:: cpp
   :name: pyboost11-ContainerBpy
   :caption: Container that is wrapped by Boost.Python
   :linenos:
   :emphasize-lines: 23-24

    /* To be wrapped by Boost.Python */
    struct ContainerBpy
    {
        ContainerBpy
        (
            ItemPyb const & pyb, ItemBpy const & bpy
          , std::shared_ptr<ItemPybSS> const & pybss
          , boost::shared_ptr<ItemBpyBS> const & bpybs
        ) : m_pyb(pyb), m_bpy(bpy), m_pybss(pybss), m_bpybs(bpybs) {}

        ItemPyb const & pyb() const { return m_pyb; }
        ItemBpy const & bpy() const { return m_bpy; }

        std::shared_ptr<ItemPybSS> const & pybss() const { return m_pybss; }
        boost::shared_ptr<ItemBpyBS> const & bpybs() const { return m_bpybs; }

        void take_pyb(ItemPyb const & other) { m_pyb.set_value(other.value()); }
        void take_bpy(ItemBpy const & other) { m_bpy.set_value(other.value()); }

        void set_pybss(std::shared_ptr<ItemPybSS> const & other) { m_pybss = other; }
        void set_bpybs(boost::shared_ptr<ItemBpyBS> const & other) { m_bpybs = other; }

        void set_pybss_ref(ItemPybSS & other) { m_pybss = other.shared_from_this(); }
        void set_bpybs_ref(ItemBpyBS & other) { m_bpybs = other.shared_from_this(); }

    private:

        ItemPyb m_pyb;
        ItemBpy m_bpy;
        std::shared_ptr<ItemPybSS> m_pybss;
        boost::shared_ptr<ItemBpyBS> m_bpybs;
    };

Wrap the Containers
+++++++++++++++++++

The pybind11 wrappers:

.. code-block:: cpp

    namespace pybind11
    {

    namespace detail
    {

    PYBOOST11_TYPE_CASTER(ItemBpy, _("ItemBpy"));
    PYBOOST11_TYPE_CASTER(boost::shared_ptr<ItemBpyBS>, _("ItemBpyBS"));

    } // end namespace detail

    } // end namespace pybind11

    PYBIND11_MODULE(modpyb, mod)
    {

        namespace pyb = pybind11;

        pyb::class_<ItemPybSS, std::shared_ptr<ItemPybSS>>(mod, "ItemPybSS")
            .def(pyb::init([](int v) { return ItemPybSS::make(v); }))
            .def_property("value", &ItemPybSS::value, &ItemPybSS::set_value)
            .def_property_readonly("address", &ItemPybSS::address)
            .def("take_pybss", &ItemPybSS::take_pybss)
            .def("take_bpybs", &ItemPybSS::take_bpybs)
            .def("make_pybss", &ItemPybSS::make_pybss)
            .def("make_bpybs", &ItemPybSS::make_bpybs)
        ;

        pyb::class_<ContainerPyb>(mod, "ContainerPyb")
            .def
            (
                pyb::init
                <
                    ItemPyb const &, ItemBpy const &
                  , std::shared_ptr<ItemPybSS> const &
                  , boost::shared_ptr<ItemBpyBS> const &
                >()
            )
            .def_property_readonly("pyb", &ContainerPyb::pyb)
            .def_property_readonly("bpy", &ContainerPyb::bpy)
            .def_property_readonly("pybss", &ContainerPyb::pybss)
            .def_property_readonly("bpybs", &ContainerPyb::bpybs)
            .def("take_pyb", &ContainerPyb::take_pyb)
            .def("take_bpy", &ContainerPyb::take_bpy)
            .def("set_pybss", &ContainerPyb::set_pybss)
            .def("set_bpybs", &ContainerPyb::set_bpybs)
        ;

    }

The Boost.Python wrappers:

.. code-block:: cpp

    BOOST_PYTHON_MODULE(modbpy)
    {

        namespace bpy = boost::python;

        pyboost11::converter<std::shared_ptr<ItemPybSS>>();

        bpy::class_<ItemBpyBS, boost::shared_ptr<ItemBpyBS>, boost::noncopyable>("ItemBpyBS", bpy::no_init)
            .def("__init__", bpy::make_constructor(&make_bpybs))
            .add_property("value", &ItemBpyBS::value, &ItemBpyBS::set_value)
            .add_property("address", &ItemBpyBS::address)
            .def("take_pybss", &ItemBpyBS::take_pybss)
            .def("take_bpybs", &ItemBpyBS::take_bpybs)
            .def("make_pybss", &ItemBpyBS::make_pybss)
            .def("make_bpybs", &ItemBpyBS::make_bpybs)
        ;

        bpy::class_<ContainerBpy>("ContainerBpy", bpy::no_init)
            .def
            (
                bpy::init
                <
                    ItemPyb const &, ItemBpy const &
                  , std::shared_ptr<ItemPybSS> const &
                  , boost::shared_ptr<ItemBpyBS> const &
                >()
            )
            .add_property
            (
                "pyb"
              , bpy::make_function
                (
                    &ContainerBpy::pyb
                  , bpy::return_value_policy<bpy::copy_const_reference>()
                )
            )
            .add_property
            (
                "bpy"
              , bpy::make_function
                (
                    &ContainerBpy::bpy
                  , bpy::return_value_policy<bpy::copy_const_reference>()
                )
            )
            .add_property
            (
                "pybss"
              , bpy::make_function
                (
                    &ContainerBpy::pybss
                  , bpy::return_value_policy<bpy::copy_const_reference>()
                )
            )
            .add_property
            (
                "bpybs"
              , bpy::make_function
                (
                    &ContainerBpy::bpybs
                  , bpy::return_value_policy<bpy::copy_const_reference>()
                )
            )
            .def("take_pyb", &ContainerBpy::take_pyb)
            .def("take_bpy", &ContainerBpy::take_bpy)
            .def("set_pybss", &ContainerBpy::set_pybss)
            .def("set_bpybs", &ContainerBpy::set_bpybs)
        ;

    }

.. note::

  The full code of the wrappers can be found in :ref:`pyboost11-code-modpyb-cpp`
  and :ref:`pyboost11-code-modbpy-cpp`.

Test the Containers
+++++++++++++++++++

Now we can check what will happen in the Python.  First we check
``ContainerPyb``:

.. code-block:: python

    pyb = modpyb.ItemPyb(100)
    bpy = modbpy.ItemBpy(101)
    pybss = modpyb.ItemPybSS(102)
    bpybs = modbpy.ItemBpyBS(103)
    cpyb = modpyb.ContainerPyb(pyb, bpy, pybss, bpybs)

    # Pass by copy cannot preserve the address.
    self.assertNotEqual(pyb.address, cpyb.pyb.address)
    self.assertNotEqual(bpy.address, cpyb.bpy.address)
    # Return with the shared pointer preserves the address.
    self.assertEqual(pybss.address, cpyb.pybss.address)
    self.assertEqual(bpybs.address, cpyb.bpybs.address)

    # Reset the contents.
    cpyb.take_pyb(modpyb.ItemPyb(200))
    cpyb.take_bpy(modbpy.ItemBpy(201))
    pybss2 = modpyb.ItemPybSS(202)
    bpybs2 = modbpy.ItemBpyBS(203)
    cpyb.set_pybss(pybss2)
    cpyb.set_bpybs(bpybs2)
    self.assertEqual(200, cpyb.pyb.value)
    self.assertEqual(201, cpyb.bpy.value)
    self.assertEqual(202, cpyb.pybss.value)
    self.assertEqual(203, cpyb.bpybs.value)

    # The container has its shared-pointer-managed objects replaced.
    self.assertNotEqual(pybss.address, cpyb.pybss.address)
    self.assertNotEqual(bpybs.address, cpyb.bpybs.address)
    self.assertEqual(pybss2.address, cpyb.pybss.address)
    self.assertEqual(bpybs2.address, cpyb.bpybs.address)

``ContainerBpy`` will work the same:

.. code-block:: python

    pyb = modpyb.ItemPyb(100)
    bpy = modbpy.ItemBpy(101)
    pybss = modpyb.ItemPybSS(102)
    bpybs = modbpy.ItemBpyBS(103)
    cbpy = modbpy.ContainerBpy(pyb, bpy, pybss, bpybs)

    # Pass by copy cannot preserve the address.
    self.assertNotEqual(pyb.address, cbpy.pyb.address)
    self.assertNotEqual(bpy.address, cbpy.bpy.address)
    # Return with the shared pointer preserves the address.
    self.assertEqual(pybss.address, cbpy.pybss.address)
    self.assertEqual(bpybs.address, cbpy.bpybs.address)

    # Reset the contents.
    cbpy.take_pyb(modpyb.ItemPyb(200))
    cbpy.take_bpy(modbpy.ItemBpy(201))
    pybss2 = modpyb.ItemPybSS(202)
    bpybs2 = modbpy.ItemBpyBS(203)
    cbpy.set_pybss(pybss2)
    cbpy.set_bpybs(bpybs2)
    self.assertEqual(200, cbpy.pyb.value)
    self.assertEqual(201, cbpy.bpy.value)
    self.assertEqual(202, cbpy.pybss.value)
    self.assertEqual(203, cbpy.bpybs.value)

    # The container has its shared-pointer-managed objects replaced.
    self.assertNotEqual(pybss.address, cbpy.pybss.address)
    self.assertNotEqual(bpybs.address, cbpy.bpybs.address)
    self.assertEqual(pybss2.address, cbpy.pybss.address)
    self.assertEqual(bpybs2.address, cbpy.bpybs.address)

Remarks
=======

Take the 4-way conversion in :ref:`pyboost11-code-pyboost11-hpp` if you think
it is useful.  It is not yet tested thoroughly, and I have not figured out all
the details in the conversion.  Any feedbacks are welcome.

Reference Code
==============

.. toctree::
  :maxdepth: 2

  code

.. vim: set ff=unix fenc=utf8 sw=2 ts=2 sts=2:
