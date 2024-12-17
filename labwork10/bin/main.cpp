#include "lib/TTaskScheduler.hpp"

int main() {
  TTaskScheduler scheduler;

  auto id2 = scheduler.add([](){return 2.0;});

  std::cout << scheduler.getResult<double>(id2) << '\n';

  return EXIT_SUCCESS;
}