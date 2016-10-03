#include <pybind11/pybind11.h>
#include <stdlib.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#define SHARED_PTR boost::shared_ptr
#define WEAK_PTR boost::weak_ptr
struct Resource {
private:
  struct ctor_passkey{};
  Resource(const ctor_passkey &) { printf("  Resource %p constructed\n", this); }
  static WEAK_PTR<Resource> g_weak;
public:
  Resource() = delete;
  Resource(Resource const & ) = delete;
  Resource(Resource       &&) = delete;
  Resource & operator=(Resource const & ) = delete;
  Resource & operator=(Resource       &&) = delete;
  ~Resource() { printf("  Resource %p destructed\n", this); }
  static SHARED_PTR<Resource> make() {
    if (g_weak.use_count()) {
      return g_weak.lock(); 
    } else {
      auto strong = SHARED_PTR<Resource>(new Resource(ctor_passkey()));
      g_weak = strong;
      return strong;
    }
  }
  static size_t get_count() {
    printf("  Resource.g_weak.use_count() = %ld\n", g_weak.use_count());
    return g_weak.use_count();
  }
  static size_t get_count_from_python(const std::string & name, const SHARED_PTR<Resource> & obj) {
    printf("  %s.use_count() = %ld .get() = %p\n", name.c_str(), obj.use_count(), obj.get());
    return obj.use_count();
  }
};
WEAK_PTR<Resource> Resource::g_weak = WEAK_PTR<Resource>();
struct Holder {
  Holder(const SHARED_PTR<Resource> & resource) : m_weak(resource) {}
  size_t get_count() {
    printf("  Holder.m_weak.use_count() = %ld\n", m_weak.use_count());
    return m_weak.use_count();
  }
private:
  WEAK_PTR<Resource> m_weak;
};
namespace py = pybind11;
PYBIND11_DECLARE_HOLDER_TYPE(T, SHARED_PTR<T>);
PYBIND11_PLUGIN(ex_pyb_boost) {
  py::module mod("ex_pyb_boost");
  py::class_<Holder>(mod, "Holder")
    .def(py::init<const SHARED_PTR<Resource> &>())
    .def("get_count", &Holder::get_count)
  ;
  py::class_<Resource, SHARED_PTR<Resource>>(mod, "Resource")
    .def_static("make", &Resource::make)
    .def_static("get_count", &Resource::get_count)
    .def_static("get_count_from_python", &Resource::get_count_from_python)
  ;
  return mod.ptr();
}
