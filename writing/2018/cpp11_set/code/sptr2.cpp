#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

class Series {
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
};
size_t Series::count = 0;

std::shared_ptr<Series> make_shared_pointer(size_t size, int lead) {
  return std::shared_ptr<Series>(new Series(size, lead));
}

void use_shared_pointer(const std::shared_ptr<Series> & series_sptr) {
  series_sptr->sum(); // call member function
  // OUT: Series::sum() = 65
}

int main(int argc, char ** argv) {
  // create a shared pointer
  auto series_sptr = make_shared_pointer(10, 2);
  // now, shared_ptr
  use_shared_pointer(series_sptr);
  // the object is still alive
  std::cout << "Series::count = " << Series::count << std::endl;
  // OUT: Series::count = 1
  // reset the pointer
  series_sptr = nullptr;
  std::cout << "no memory leak: Series::count = "
            << Series::count << std::endl;
  // OUT: no memory leak: Series::count = 0
  return 0;
}
