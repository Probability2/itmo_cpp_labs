#include "parsing.h"

const char* kShortInputCommand = "-i";
const char* kLongInputCommand = "--input=";
const char* kShortOutputCommand = "-o";
const char* kLongOutputCommand = "--output=";
const char* kShortMaxIterCommand = "-m";
const char* kLongMaxIterCommand = "--max-iter=";
const char* kShortFreqCommand = "-f";
const char* kLongFreqCommand = "--freq=";
const std::size_t kInputLen = std::strlen(kLongInputCommand);
const std::size_t kOutputLen = std::strlen(kLongOutputCommand);
const std::size_t kMaxIterLen = std::strlen(kLongMaxIterCommand);
const std::size_t kFreqLen = std::strlen(kLongFreqCommand);

bool ReadNumber(const char* str, std::size_t& dest) {
  char* end;
  const std::from_chars_result result = std::from_chars(str, str + std::strlen(str), dest);
  if (result.ec == std::errc{} && result.ptr == str + std::strlen(str)) {
    return true;
  }
  return false;
}

ParsingArguments Parsing(const int argc, char* argv[], ParsingResultType& curr_error) {
  ParsingArguments my_arguments;
  std::size_t dest;
  char *end;
  for (int i = 1; i < argc; ++i) {
    if (!strcmp(argv[i], kShortFreqCommand)) {
      ++i;
      if (!ReadNumber(argv[i], dest)) {
        curr_error = ParsingResultType::kFreqError;
        break;
      }
      std::from_chars(argv[i], argv[i] + std::strlen(argv[i]), my_arguments.frequency);
    } else if (!strcmp(argv[i], kShortInputCommand)) {
      ++i;
      my_arguments.file_name = argv[i];
    } else if (!strcmp(argv[i], kShortOutputCommand)) {
      ++i;
      my_arguments.output_directory = argv[i];
    } else if (!strcmp(argv[i], kShortMaxIterCommand)) {
      ++i;
      if (!ReadNumber(argv[i], dest)) {
        curr_error = ParsingResultType::kMaxIterError;
        break;
      }
      std::from_chars(argv[i], argv[i] + std::strlen(argv[i]), my_arguments.max_iteration);
    } else if (!strncmp(argv[i], kLongFreqCommand, kFreqLen)) {
      argv[i] += kFreqLen;
      if (!ReadNumber(argv[i], dest)) {
        curr_error = ParsingResultType::kFreqError;
        break;
      }
      std::from_chars(argv[i], argv[i] + std::strlen(argv[i]), my_arguments.frequency);
    } else if (!strncmp(argv[i], kLongInputCommand, kInputLen)) {
      argv[i] += kInputLen;
      my_arguments.file_name = argv[i];
    } else if (!strncmp(argv[i], kLongOutputCommand, kOutputLen)) {
      argv[i] += kOutputLen;
      my_arguments.output_directory = argv[i];
    } else if (!strncmp(argv[i], kLongMaxIterCommand, kMaxIterLen)) {
      argv[i] += kMaxIterLen;
      if (!ReadNumber(argv[i], dest)) {
        curr_error = ParsingResultType::kMaxIterError;
        break;
      }
      std::from_chars(argv[i], argv[i] + std::strlen(argv[i]), my_arguments.max_iteration);
    } else {
      curr_error = ParsingResultType::kUnnecessaryDataError;
    }
  }
  
  return my_arguments;
}

void CheckParsingResultType(ParsingResultType& curr_error) {
  switch (curr_error) {
    case ParsingResultType::kFreqError:
      std::cerr << "ERROR: Not a correct format for '-f' ('--freq=') command.";
    case ParsingResultType::kMaxIterError:
      std::cerr << "ERROR: Not a correct format for '-m' ('--max-iter=') command.";
    case ParsingResultType::kUnnecessaryDataError:
      std::cerr << "ERROR: There is unnecessary data in the arguments.";
  }
  std::exit(EXIT_FAILURE);
}