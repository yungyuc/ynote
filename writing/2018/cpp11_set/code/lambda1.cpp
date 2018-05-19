#include <unordered_map>
#include <functional>
#include <cstdio>

int main(int argc, char ** argv) {
  // Python: fmap = dict()
  std::unordered_map<int, std::function<void(int)>> fmap;

  // Python: fmap[1] = lambda v: print("v = %d" % v)
  fmap.insert({
    1, [](int v) -> void { std::printf("v = %d\n", v); }
  });

  // Python: fmap[5] = lambda v: print("v*5 = %d" % (v*5))
  fmap.insert({
    5, [](int v) -> void { std::printf("v*5 = %d\n", v*5); }
  });

  std::unordered_map<int, std::function<void(int)>>::iterator search;

  // Python: fmap[1](100)
  search = fmap.find(1);
  search->second(100);
  // OUT: v = 100

  // Python: fmap[5](500)
  search = fmap.find(5);
  search->second(500);
  // OUT: v*5 = 2500

  return 0;
}
