#include <cmath>

#include "lib/TTaskScheduler.hpp"
#include "gtest/gtest.h"

TEST(SchedulerSuiteTest, TwoArgsTest1) {
  float a = 1;
  float b = -2;
  float c = 0;
  TTaskScheduler scheduler;
  auto id1 = scheduler.add([](float a, float c){return -4 * a * c;}, a, c);
  float f_id1 = -4 * a * c;

  auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.getFutureResult<float>(id1));
  float f_id2 = b * b + f_id1;

  auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));
  float f_id3 = -b + std::sqrt(f_id2);

  auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));
  float f_id4 = -b - std::sqrt(f_id2);

  auto id5 = scheduler.add([](float b, float d){return d/(2*b);}, a, scheduler.getFutureResult<float>(id3));
  float f_id5 = f_id3 / (2 * a);

  auto id6 = scheduler.add([](float b, float d){return d/(2*b);}, a, scheduler.getFutureResult<float>(id4));
  float f_id6 = f_id4 / (2 * a);

  scheduler.executeAll();

  ASSERT_EQ(scheduler.getResult<float>(id1), f_id1);
  ASSERT_EQ(scheduler.getResult<float>(id2), f_id2);
  ASSERT_EQ(scheduler.getResult<float>(id3), f_id3);
  ASSERT_EQ(scheduler.getResult<float>(id4), f_id4);
  ASSERT_EQ(scheduler.getResult<float>(id5), f_id5);
  ASSERT_EQ(scheduler.getResult<float>(id6), f_id6);
}

TEST(SchedulerSuiteTest, NoArgsInt) {
  TTaskScheduler scheduler;
  auto id1 = scheduler.add([](){return 2 + 2;});
  int i_id1 = 2 + 2;

  auto id2 = scheduler.add([](){return 2 * 54 + 125;});
  int i_id2 = 2 * 54 + 125;

  auto id3 = scheduler.add([](){return -102 + 1 * 2 - 5 * 7;});
  int i_id3 = -102 + 1 * 2 - 5 * 7;

  auto id4 = scheduler.add([](){return 4/3 + (6 * 5) / 3;});
  int i_id4 = 4/3 + (6 * 5) / 3;

  scheduler.executeAll();

  ASSERT_EQ(scheduler.getResult<int>(id1), i_id1);
  ASSERT_EQ(scheduler.getResult<int>(id2), i_id2);
  ASSERT_EQ(scheduler.getResult<int>(id3), i_id3);
  ASSERT_EQ(scheduler.getResult<int>(id4), i_id4);
}

TEST(SchedulerSuiteTest, NoArgsFloat) {
  TTaskScheduler scheduler;
  auto id1 = scheduler.add([](){return std::sqrt(3);});
  double d_id1 = std::sqrt(3);

  auto id2 = scheduler.add([](){double d = 2 * 54 + 125; return d;});
  double d_id2 = 2 * 54 + 125;

  auto id3 = scheduler.add([](){return 3/4 + 5/7 + 8/77 + std::sqrt(2);});
  double d_id3 = 3/4 + 5/7 + 8/77 + std::sqrt(2);

  auto id4 = scheduler.add([](){double d = 4/3 + (6 * 5) / 8; return d;});
  double d_id4 = 4/3 + (6 * 5) / 8;

  scheduler.executeAll();

  ASSERT_EQ(scheduler.getResult<double>(id1), d_id1);
  ASSERT_EQ(scheduler.getResult<double>(id2), d_id2);
  ASSERT_EQ(scheduler.getResult<double>(id3), d_id3);
  ASSERT_EQ(scheduler.getResult<double>(id4), d_id4);
}

TEST(SchedulerSuiteTest, OneArgInt) {
  int a = 3;

  TTaskScheduler scheduler;
  auto id1 = scheduler.add([](int a){return a * 3 + 1;}, a);
  int i_id1 = a * 3 + 1;

  auto id2 = scheduler.add([](int a){return a * 4 - 3;}, scheduler.getFutureResult<int>(id1));
  int i_id2 = i_id1 * 4 - 3;

  auto id3 = scheduler.add([](int a){return a / 3 - 5;}, scheduler.getFutureResult<int>(id2));
  int i_id3 = i_id2 / 3 - 5;

  auto id4 = scheduler.add([](int a){return a * 91 + 81;}, scheduler.getFutureResult<int>(id3));
  int i_id4 = i_id3 * 91 + 81;

  scheduler.executeAll();

  ASSERT_EQ(scheduler.getResult<int>(id1), i_id1);
  ASSERT_EQ(scheduler.getResult<int>(id2), i_id2);
  ASSERT_EQ(scheduler.getResult<int>(id3), i_id3);
  ASSERT_EQ(scheduler.getResult<int>(id4), i_id4);
}

TEST(SchedulerSuiteTest, OneArgCycle) {
  double a = 3;

  TTaskScheduler scheduler;
  for (int i = 0; i < 1000; ++i) {
    auto id1 = scheduler.add([](double a){return a * 3 + std::sqrt(a);}, a);
    a = a * 3 + std::sqrt(a);
    ASSERT_EQ(scheduler.getResult<double>(id1), a);
  }
}

TEST(SchedulerSuiteTest, NoArgs2) {
  TTaskScheduler scheduler;
  auto id1 = scheduler.add([](){return true;});
  bool b_id1 = true;

  auto id2 = scheduler.add([](){return 1000 * 1001 / 2;});
  int i_id2 = 1000 * 1001 / 2;

  auto id3 = scheduler.add([](){char a = 'a' + 4; return a;});
  char c_id3 = 'a' + 4;

  auto id4 = scheduler.add([](){std::vector<int> v = {1, 2, 3}; return v;});
  std::vector<int> v_id4 = {1, 2, 3};

  scheduler.executeAll();

  ASSERT_EQ(scheduler.getResult<bool>(id1), b_id1);
  ASSERT_EQ(scheduler.getResult<int>(id2), i_id2);
  ASSERT_EQ(scheduler.getResult<char>(id3), c_id3);
  ASSERT_EQ(scheduler.getResult<std::vector<int>>(id4), v_id4);
}

TEST(SchedulerSuiteTest, TwoArgsTest2) {
  for (int i = 1; i < 1000; ++i) {
    TTaskScheduler scheduler;
    float a = static_cast<float>(i);
    float b = static_cast<float>(i * i);
    float c = static_cast<float>(i * 2 - 101);
    auto id1 = scheduler.add([](float a, float c){return -4 * a * c;}, a, c);
    float f_id1 = -4 * a * c;

    auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.getFutureResult<float>(id1));
    float f_id2 = b * b + f_id1;

    auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));
    float f_id3 = -b + std::sqrt(f_id2);

    auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));
    float f_id4 = -b - std::sqrt(f_id2);

    auto id5 = scheduler.add([](float b, float d){return d/(2*b);}, a, scheduler.getFutureResult<float>(id3));
    float f_id5 = f_id3 / (2 * a);

    auto id6 = scheduler.add([](float b, float d){return d/(2*b);}, a, scheduler.getFutureResult<float>(id4));
    float f_id6 = f_id4 / (2 * a);

    scheduler.executeAll();

    ASSERT_EQ(scheduler.getResult<float>(id1), f_id1);
    ASSERT_EQ(scheduler.getResult<float>(id2), f_id2);
    ASSERT_EQ(scheduler.getResult<float>(id3), f_id3);
    ASSERT_EQ(scheduler.getResult<float>(id4), f_id4);
    ASSERT_EQ(scheduler.getResult<float>(id5), f_id5);
    ASSERT_EQ(scheduler.getResult<float>(id6), f_id6);
  }
}

TEST(SchedulerSuiteTest, TwoArgsTest3) {
  for (int i = 1; i < 1000; ++i) {
    TTaskScheduler scheduler;
    int a = i;
    int b = i + i / 3 + i % 7 - i % 6;
    int c = i * 3 / 2;
    auto id1 = scheduler.add([](int a, int c){return 50 * a + c;}, a, c);
    int i_id1 = 50 * a + c;

    auto id2 = scheduler.add([](int b, int v){return b * b + v;}, b, scheduler.getFutureResult<int>(id1));
    int i_id2 = b * b + i_id1;

    auto id3 = scheduler.add([](int b, int d){return 4 * b + d * d;}, scheduler.getFutureResult<int>(id2),
                                                                          scheduler.getFutureResult<int>(id2));
    int i_id3 = 4 * i_id2 + i_id2 * i_id2;

    auto id4 = scheduler.add([](int b, int d){return 1 + 2 * b - 3 * d;}, scheduler.getFutureResult<int>(id2),
                                                                       scheduler.getFutureResult<int>(id3));
    int i_id4 = 1 + 2 * i_id2 - 3 * i_id3;

    auto id5 = scheduler.add([](int b, int d){return b + d;}, scheduler.getFutureResult<int>(id4), a);
    int i_id5 = i_id4 + a;

    ASSERT_EQ(scheduler.getResult<int>(id1), i_id1);
    ASSERT_EQ(scheduler.getResult<int>(id2), i_id2);
    ASSERT_EQ(scheduler.getResult<int>(id3), i_id3);
    ASSERT_EQ(scheduler.getResult<int>(id4), i_id4);
    ASSERT_EQ(scheduler.getResult<int>(id5), i_id5);
  }
}

TEST(SchedulerSuiteTest, OneArgCycle2) {
  TTaskScheduler scheduler;
  for (int i = 0; i < 1000; ++i) {
    auto id1 = scheduler.add([](double i){return std::sqrt(i);}, i);
    double d_id1 = std::sqrt(i);

    auto id2 = scheduler.add([](double i){return std::sqrt(i);}, i * 2);
    double d_id2 = std::sqrt(i * 2);

    auto id3 = scheduler.add([](double i){return std::sqrt(i);}, scheduler.getFutureResult<double>(id1));
    double d_id3 = std::sqrt(d_id1);

    auto id4 = scheduler.add([](double i){double a = -4 + i * i; return a;}, scheduler.getFutureResult<double>(id2));
    double d_id4 = -4 + d_id2 * d_id2;

    auto id5 = scheduler.add([](double i){return i + 5.1234;}, scheduler.getFutureResult<double>(id4));
    double d_id5 = d_id4 + 5.1234;

    ASSERT_EQ(scheduler.getResult<double>(id1), d_id1);
    ASSERT_EQ(scheduler.getResult<double>(id2), d_id2);
    ASSERT_EQ(scheduler.getResult<double>(id3), d_id3);
    ASSERT_EQ(scheduler.getResult<double>(id4), d_id4);
    ASSERT_EQ(scheduler.getResult<double>(id5), d_id5);
  }
}

TEST(SchedulerSuiteTest, OneArgCycle3) {
  TTaskScheduler scheduler;
  std::vector<int> v;
  for (int i = 0; i < 1000; ++i) {
    auto id = scheduler.add([](std::vector<int>& v){v.push_back(1); return v;}, v);
    v.push_back(1);
    ASSERT_EQ(scheduler.getResult<std::vector<int>>(id), v);
  }
}
