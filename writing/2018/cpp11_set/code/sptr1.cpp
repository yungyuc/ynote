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

void use_raw_pointer() {
  Series * series_ptr = new Series(10, 2);
  series_ptr->sum(); // call member function
  // OUT: Series::sum() = 65
  // remember to delete the object or we leak memory
  std::cout << "before explicit deletion, Series::count = "
            << Series::count << std::endl;
  // OUT: before explicit deletion, Series::count = 1
  delete series_ptr;
  std::cout << "after the resource is manually freed, Series::count = "
            << Series::count << std::endl;
  // OUT: after the resource is manually freed, Series::count = 0
}

void use_shared_pointer() {
  std::shared_ptr<Series> series_sptr(new Series(10, 3));
  series_sptr->sum(); // call member function
  // OUT: Series::sum() = 75
  // note shared_ptr handles deletion for series_sptr
}

int main(int argc, char ** argv) {
  // the common raw pointer
  use_raw_pointer();
  // now, shared_ptr
  use_shared_pointer();
  std::cout << "no memory leak: Series::count = "
            << Series::count << std::endl;
  // OUT: no memory leak: Series::count = 0
  return 0;
}
