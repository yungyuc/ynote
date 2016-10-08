#include <boost/python.hpp>
#include <stdlib.h>
#include <boost/utility.hpp>
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
BOOST_PYTHON_MODULE(ex_bpy_from_python)
{
  using namespace boost::python;
  class_<Resource, SHARED_PTR<Resource>, boost::noncopyable>
  ("Resource", no_init)
    .def("make", &Resource::make)
    .staticmethod("make")
    .def("get_count", &Resource::get_count)
    .staticmethod("get_count")
    .def("get_count_from_python", &Resource::get_count_from_python)
    .staticmethod("get_count_from_python")
  ;
}
