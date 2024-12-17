#pragma once

#include <cinttypes>
#include <iostream>
#include <cstring>
#include <cstdint>
#include <charconv>

enum class ParsingResultType : uint8_t {
   kMaxIterError,
   kFreqError,
   kUnnecessaryDataError,
   kNoErrors,
 };

struct ParsingArguments {
  char* file_name = nullptr;
  char* output_directory = nullptr;
  uint64_t max_iteration = 0;
  uint64_t frequency = 0;
};

bool ReadNumber(const char* str, size_t& dest);

ParsingArguments Parsing(const int argc, char* argv[], ParsingResultType& curr_error);

void CheckParsingResultType(ParsingResultType& curr_error);
