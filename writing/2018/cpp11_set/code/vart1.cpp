#include <memory>
#include <cstdio>
#include <vector>
#include <list>

// C++11 doesn't have make_unique
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

int main(int argc, char ** argv) {
  auto vec = make_unique<std::vector<int>>(10);
  std::printf("vec->size() = %ld\n", vec->size());
  // OUT: vec->size() = 10

  auto lst = make_unique<std::list<int>>(vec->begin(), vec->end());
  std::printf("lst->size() = %ld\n", lst->size());
  // OUT: lst->size() = 10

  return 0;
}
