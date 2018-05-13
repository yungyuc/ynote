#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

class Series : public std::enable_shared_from_this<Series> {
  std::vector<int> m_data;
public:
  int sum() const {
    const int ret = std::accumulate(m_data.begin(), m_data.end(), 0);
    std::cout << "Series::sum() = " << ret << std::endl;
    return ret;
  }
  static size_t count;
  Series(size_t size, int lead) : m_data(size) {
    for (size_t it=0; it<size; it++) { m_data[it] = lead+it; }
    count++;
  }
  ~Series() { count--; }
  std::shared_ptr<Series> get_this_bad() {
    // this will result in double free! don't do this
    return std::shared_ptr<Series>(this);
  }
  std::shared_ptr<Series> get_this_good() {
    return shared_from_this(); // by std::enable_shared_from_this
  }
};
size_t Series::count = 0;

std::shared_ptr<Series> make_shared_pointer(size_t size, int lead) {
  return std::shared_ptr<Series>(new Series(size, lead));
}

int main(int argc, char ** argv) {
  // create a shared pointer
  auto sp1 = make_shared_pointer(10, 2);
  std::cout << "sp1.use_count() = "
            << sp1.use_count() << std::endl;
  // OUT: sp1.use_count() = 1
  // recreate a new shared pointer from the existing one
  auto sp2 = sp1->get_this_good();
  std::cout << "sp2.use_count() = "
            << sp2.use_count() << std::endl;
  // OUT: sp1.use_count() = 2
  return 0;
}
