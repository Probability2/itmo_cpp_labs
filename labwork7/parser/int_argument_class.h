#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

namespace ArgumentParser{

class IntArguments {
  public:
    char short_parameter_;
    std::string long_parameter_;
    std::string description_;
    int32_t value_;
    int32_t* valueptr_;
    std::vector<int32_t> values_;
    std::vector<int32_t>* valuesptr_;
    bool is_value_set_ = false;
    bool is_valueptr_set_ = false;
    bool is_valuesptr_set_ = false;
    bool is_positional_ = false;
    bool is_multi_value_ = false;
    uint64_t min_argc_count_ = 1;

    IntArguments(const char short_argument, const std::string& long_argument, const std::string& description);
    IntArguments& StoreValue(int32_t& value);
    IntArguments& StoreValues(std::vector<int32_t>& values);
    IntArguments& Positional();
    IntArguments& MultiValue(const uint64_t min_args_count);
    IntArguments& MultiValue();
};
}
