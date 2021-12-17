#include "pyboost11.hpp"

#include "item.hpp"

boost::shared_ptr<ItemBpyBS> make_bpybs(int v)
{
    return ItemBpyBS::make(v);
}

void set_pybss_ref(ContainerBpy & self, std::shared_ptr<ItemPybSS> const & other)
{
    self.set_pybss_ref(*other);
}

BOOST_PYTHON_MODULE(modbpy)
{

    namespace bpy = boost::python;

    pyboost11::converter<ItemPyb>();
    pyboost11::converter<std::shared_ptr<ItemPybSS>>();

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
        // FIXME: How to automatically recognize ItemPybSS &?
        .def("set_pybss_ref", &set_pybss_ref)
        .def("set_bpybs_ref", &ContainerBpy::set_bpybs_ref)
        // Overload with argument name.
        .def
        (
            "overload_pyb"
          , static_cast<std::string (ContainerBpy::*)(ItemPyb const &) const>
            (&ContainerBpy::overload_pyb)
        )
        .def
        (
            "overload_pyb"
          , static_cast<std::string (ContainerBpy::*)(std::shared_ptr<ItemPybSS> const &) const>
            (&ContainerBpy::overload_pyb)
        )
        .def
        (
            "overload_bpy"
          , static_cast<std::string (ContainerBpy::*)(ItemBpy const &) const>
            (&ContainerBpy::overload_bpy)
        )
        .def
        (
            "overload_bpy"
          , static_cast<std::string (ContainerBpy::*)(boost::shared_ptr<ItemBpyBS> const &) const>
            (&ContainerBpy::overload_bpy)
        )
        // Overload with argument name.
        .def
        (
            "overload2_pyb"
          , static_cast<std::string (ContainerBpy::*)(ItemPyb const &) const>
            (&ContainerBpy::overload_pyb)
          , bpy::arg("pyb")
        )
        .def
        (
            "overload2_pyb"
          , static_cast<std::string (ContainerBpy::*)(std::shared_ptr<ItemPybSS> const &) const>
            (&ContainerBpy::overload_pyb)
          , bpy::arg("pybss")
        )
        .def
        (
            "overload2_bpy"
          , static_cast<std::string (ContainerBpy::*)(ItemBpy const &) const>
            (&ContainerBpy::overload_bpy)
          , bpy::arg("bpy")
        )
        .def
        (
            "overload2_bpy"
          , static_cast<std::string (ContainerBpy::*)(boost::shared_ptr<ItemBpyBS> const &) const>
            (&ContainerBpy::overload_bpy)
          , bpy::arg("bpybs")
        )
    ;

}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
