// compile with -std=c++11 -stdlib=libc++ to get C++11 support
#include <stdlib.h>
#ifdef USE_BOOST_SHARED_PTR
  #include <boost/shared_ptr.hpp>
  #define SHARED_PTR boost::shared_ptr
#else
  #include <memory>
  #define SHARED_PTR std::shared_ptr
#endif
struct Resource {
  Resource() { printf("Resource %p constructed\n", this); }
  ~Resource() { printf("Resource %p destructed\n", this); }
};
int main(int argc, char ** argv) {
  auto obj = SHARED_PTR<Resource>(new Resource);
  printf("address %p, count %lu\n",
         obj.get(),      // getting the raw pointer
         obj.use_count() // getting the reference count
  );
  // output: address 0x7fa970c03420, count 1
  printf("reset obj\n");
  obj = nullptr; // this resets the shared_ptr and thus release the resource object.
  printf("program ends\n");
  return 0;
}
