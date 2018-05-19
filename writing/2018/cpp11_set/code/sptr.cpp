#include <memory>
#include <iostream>

struct Data {
  static size_t count;
  Data() { count++; }
  ~Data() { count--; }
};
size_t Data::count = 0;

struct Worker {
  std::shared_ptr<Data> data;
  void work () {
    std::cout << "in Worker::work(): data.use_count() = " << data.use_count()
              << ", Data::count = " << Data::count << std::endl;
  }
};

std::shared_ptr<Data> new_shared_ptr() {
  std::shared_ptr<Data> data(new Data);
  std::cout << "in new_shared_ptr(): data.use_count() = " << data.use_count()
            << ", Data::count = " << Data::count << std::endl;
  return data;
}

void copy_shared_ptr(std::shared_ptr<Data> data) {
  std::cout << "in copy_shared_ptr(): data.use_count() = "
            << data.use_count()
            << ", Data::count = " << Data::count << std::endl;
}

void reference_shared_ptr(std::shared_ptr<Data> const & data) {
  std::cout << "in reference_shared_ptr(): data.use_count() = "
            << data.use_count()
            << ", Data::count = " << Data::count << std::endl;
}

int main(int argc, char ** argv) {
  auto data = new_shared_ptr();
  copy_shared_ptr(data);
  reference_shared_ptr(data);
  Worker worker = {data};
  worker.work();
  data = nullptr;
  std::cout << "after reset the local shared_ptr to null, "
            << "the shared_ptr::use_count drops to 0" << std::endl;
  reference_shared_ptr(data);
  std::cout << "the shared_ptr in worker isn't affected"
            << std::endl;
  reference_shared_ptr(worker.data);
  return 0;
}
