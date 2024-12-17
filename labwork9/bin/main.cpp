#include "lib/Transform.hpp"
#include "lib/Filter.hpp"
#include "lib/Take.hpp"
#include "lib/Drop.hpp"
#include "lib/Reverse.hpp"
#include "lib/Keys.hpp"
#include "lib/Values.hpp"

int main() {
  std::vector<int> v = {1, 2, 3, 4, 5, 6};

  auto removed = v | MyFilter([](int i){return i % 2;});
  for (const auto& i: removed) {
    std::cout << i << ' ';
  }

  return EXIT_SUCCESS;
}