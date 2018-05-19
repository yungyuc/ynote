#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

class Series : public std::enable_shared_from_this<Series> {
  std::vector<int> m_data;
  // private constructor
  Series(size_t size, int lead) : m_data(size) {
    for (size_t it=0; it<size; it++) { m_data[it] = lead+it; }
    count++;
  }
public:
  int sum() const {
    const int ret = std::accumulate(m_data.begin(), m_data.end(), 0);
    std::cout << "Series::sum() = " << ret << std::endl;
    return ret;
  }
  static size_t count;
  ~Series() { count--; }
  // factory method to construct the object
  // and put it in the shared pointer
  static std::shared_ptr<Series> make(size_t size, int lead) {
    return std::shared_ptr<Series>(new Series(size, lead));
  }
};
size_t Series::count = 0;

int main(int argc, char ** argv) {
  // create a shared pointer
  auto sp1 = Series::make(10, 2);
  std::cout << "sp1.use_count() = "
            << sp1.use_count() << std::endl;
  // OUT: sp1.use_count() = 1
  Series o2(*sp1); // uhoh, we forgot copy construction!
  o2.sum();
  // OUT: Series::sum() = 65
  return 0;
}
