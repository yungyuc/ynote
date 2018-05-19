#include <algorithm>
#include <cstdio>

struct Storage {
  int * data = nullptr;
  size_t size = 0;
  size_t reserved = 0;
  Storage() = delete;
  Storage(size_t size, size_t reserved)
  : data(new int[reserved]), size(size), reserved(reserved) {
    std::printf("Storage(this=%p)::Storage(size_t, size_t)\n", this);
  }
  Storage(Storage const & other)
  : data(new int[other.reserved])
  , size(other.size), reserved(other.reserved) {
    // this takes time and memory
    std::printf("Storage(this=%p)::Storage(const &): costly copy\n", this);
    std::copy(other.data, other.data+other.size, data);
  }
  ~Storage() {
    std::printf("Storage(this=%p)::~Storage()\n", this);
    if (data) { delete[] data; }
  }
  void append(int v) {
    if (reserved <= size) {
      std::printf("Storage(this=%p)::append(int) add more space\n", this);
      reserved = (size+1)*2;
      int * newdata = new int[reserved];
      std::copy(data, data+size, newdata);
      std::swap(data, newdata);
      delete[] newdata;
    }
    data[size] = v;
    size++;
  }
};

Storage extend(Storage stor) {
  std::printf("extend() &stor=%p\n", &stor);
  stor.append(stor.data[stor.size-1]);
  std::printf("extend() finish\n");
  return stor;
}

int main(int argc, char ** argv) {
  std::printf("Normal construction:\n");
  Storage stor1(2, 4);
  std::printf("stor1  (this=%p).size = %ld\n", &stor1, stor1.size);
  // OUT: Storage(this=0x7ffc355e3540)::Storage(size_t, size_t)
  // OUT: stor1  (this=0x7ffc355e3540).size = 2

  std::printf("Prepare to extend stor1:\n");
  Storage stor2 = extend(stor1);
  // OUT: Storage(this=0x7ffc355e3580)::Storage(const &): costly copy
  // OUT: extend() &stor=0x7ffc355e3580
  // OUT: extend() finish
  // OUT: Storage(this=0x7ffc355e3560)::Storage(const &): costly copy
  // OUT: Storage(this=0x7ffc355e3580)::~Storage()
  std::printf("stor2  (this=%p).size = %ld\n", &stor2, stor2.size);
  // OUT: stor2  (this=0x7ffc355e3560).size = 3
  // OUT: Storage(this=0x7ffc355e3560)::~Storage()
  // OUT: Storage(this=0x7ffc355e3540)::~Storage()

  return 0;
}
