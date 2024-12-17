#include <iostream>
#include <fstream>
#include <cstring>

const char* kShortLinesCommand = "-l";
const char* kLongLinesCommand = "--lines=";
const char* kShortTailCommand = "-t";
const char* kLongTailCommand = "--tail";
const char* kShortDelimeterCommand = "-d";
const char* kLongDelimeterCommand = "--delimeter=";
const char kIncorrectDelimeter = '\a';
const std::size_t kLinesLen = strlen(kLongLinesCommand);
const std::size_t kDelimeterLen = strlen(kLongDelimeterCommand);
const int kSize2 = 2;
const int kSize3 = 3;
const int kSize4 = 4;
const int kBuffSize = 16 * 1024;

enum class ParsingResultType : uint8_t {
  kFileNameError,
  kDelimeterError,
  kLinesError,
  kTailError,
  kNumbersError,
  kNoErrors,
};

struct Parameters {
  bool is_tail_set = false;
  bool is_lines_set = false;
  bool is_delimeter_set = false;
  char delimeter = '\n';
  char* file_name = nullptr;
  size_t lines_number = 0;
};

bool CheckNumber(const char* str, size_t& dost, int divider) {
    char* end;
    errno = 0;
    dost = std::strtoll(str + divider, &end, 10);
    const bool kRangeError = errno |= ERANGE;
    if (!(kRangeError) || (end) != (str)+strlen(str)) {
      return false;
    }

    return true;
}

bool ParseLongLines(const char* str, size_t& dost) {
  if (strncmp(str, kLongLinesCommand, kLinesLen)) {
    return false;
  }

  return CheckNumber(str, dost, kLinesLen);
}

bool ReadLongDelimeter(const char* str, size_t& dost) {
  if (strncmp(str, kLongDelimeterCommand, kDelimeterLen)) {
    return false;
  }
  const bool kRangeError = errno;
  if (!kRangeError) {
    return false;
  }

  return true;
}

char EscapeDelimeter(char a) {
  switch (a) {
    case 't': return '\t';
    case 'n': return '\n';
    case 'b': return '\b';
    case 'v': return '\v';
    case '?': return '\?';
    case '\\': return '\\';
    case '\'': return '\'';
    case 'f': return '\f';
    default: return kIncorrectDelimeter;
  }
}

void PrintLeftToRight(char delimeter, int lines_number, std::ifstream& file) {
  int delimeter_number = 0;
  size_t i = 0;
  while (!file.eof() && delimeter_number < lines_number) {
    char buff[kBuffSize];
    file.read(buff, kBuffSize);
    i = 0;
    while (i < file.gcount() && delimeter_number < lines_number) {
      if (buff[i] == delimeter) {
        delimeter_number++;
      }
      std::cout << buff[i];
      i++;
    }
  }
}

void PrintRightToLeft(char delimeter, int lines_number, std::ifstream &file) {
  int number_all_delimeters = 0;
  size_t i = 0;
  int curr_delimeter_number = 0;
  int delimeter_number_begin = 0;
  bool check = false;
  while (!file.eof()) {
    char buff[kBuffSize];
    file.read(buff, kBuffSize);
    i = 0;
    while (i < file.gcount()) {
      if (buff[i] == delimeter) {
        number_all_delimeters++;
      }
      i++;
    }
  }
  file.clear();
  file.seekg(0, std::ios::beg);
  if (lines_number > number_all_delimeters) {
      lines_number = number_all_delimeters + 1;
  }
  if (lines_number - 1 > number_all_delimeters) {
    delimeter_number_begin = number_all_delimeters;
  } else {
    delimeter_number_begin = number_all_delimeters - lines_number + 1;
  }

  while (!file.eof()) {
    char buff[kBuffSize];
    file.read(buff, kBuffSize);
    i = 0;
    int ind_buff = 0;
    while (i < file.gcount()) {
      if (buff[i] == delimeter) {
        curr_delimeter_number++;
      }
      if (curr_delimeter_number == delimeter_number_begin && buff[i] == delimeter) {
        ind_buff = i;
      }
      if (curr_delimeter_number >= delimeter_number_begin && i != ind_buff) {
        std::cout << buff[i];
      }
      i++;
    }
  }

}

char ReadDelimeter(char* a, ParsingResultType &curr_error) {
  if ((strlen(a) <= kSize2) || (strlen(a) > kSize4)) {
    curr_error = ParsingResultType::kDelimeterError;
    return a[0];
  } else if (strlen(a) == kSize3 && a[0] == '\'' && a[2] == '\'') {
    return a[1];
  } else if (strlen(a) == kSize4 && a[0] == '\'' && a[3] == '\'' && a[1] == '\\') {
    return EscapeDelimeter(a[2]);
  }
}

void PrintFile(Parameters &my_parameters, ParsingResultType &curr_error) {
  if (curr_error == ParsingResultType::kFileNameError) {
    std::cerr << "ERROR: There is more than one file." << "\n";
  } else if (curr_error == ParsingResultType::kDelimeterError) {
    std::cerr << "ERROR: Not a correct format for '-d' ('--delimeter=') command." << "\n";
  } else if (curr_error == ParsingResultType::kLinesError) {
    std::cerr << "ERROR: Not a correct format for '-l' ('--lines=') command." << "\n";
  } else if (curr_error == ParsingResultType::kTailError) {
    std::cerr << "ERROR: Not a correct format for '-t' ('--tail=') command." << "\n";
  } else if (curr_error == ParsingResultType::kNumbersError) {
    std::cerr << "ERROR: There is more than one number." << "\n";
  } else if (!my_parameters.file_name) {
    std::cerr << "ERROR: There is no files." << "\n";
  }
  if (curr_error != ParsingResultType::kNoErrors) {
    exit(EXIT_FAILURE);
  }
  std::ifstream file(my_parameters.file_name);
  if (!file.is_open()) {
    std::cerr << "ERROR: This file hasn't found." << "\n";
    exit(EXIT_FAILURE);
  }
  if (!my_parameters.lines_number) {
    file.seekg(0, std::ios::end);
    my_parameters.lines_number = file.tellg();
    file.clear();
    file.seekg(0, std::ios::beg);
  }
  if (my_parameters.is_tail_set) {
    PrintRightToLeft(my_parameters.delimeter, my_parameters.lines_number, file);
  } else{
    PrintLeftToRight(my_parameters.delimeter, my_parameters.lines_number, file);
  }
}

Parameters Parsing(int argc, char* argv[], ParsingResultType curr_error) {
  Parameters my_parameters;
  if (argc == 1) {
    std::cerr << "ERROR: There is no file." << "\n";
    exit(EXIT_FAILURE);
  }
  if (argc == 2) {
    my_parameters.is_lines_set = true;
  }
  size_t dost;
  for (int i = 1; i < argc; ++i) {
    if (CheckNumber(argv[i], dost, 0)) {
      if (my_parameters.lines_number) {
        curr_error = ParsingResultType::kNumbersError;
        break;
      }
      char* end;
      my_parameters.lines_number = std::strtoll(argv[i], &end, 10);
    } else if (!strcmp(argv[i], kShortTailCommand) || !strcmp(argv[i], kLongTailCommand)) {
      if (my_parameters.is_tail_set) {
        curr_error = ParsingResultType::kTailError;
        break;
      }
      my_parameters.is_tail_set = true;
    } else if (!strcmp(argv[i], kShortLinesCommand)) {
      if (my_parameters.is_lines_set) {
        curr_error = ParsingResultType::kLinesError;
        break;
      }
      my_parameters.is_lines_set = true;
    } else if (!strcmp(argv[i], kShortDelimeterCommand)) {
      if (my_parameters.is_delimeter_set) {
        curr_error = ParsingResultType::kDelimeterError;
        break;
      }
      my_parameters.is_delimeter_set = true;
      ++i;
      my_parameters.delimeter = ReadDelimeter(argv[i], curr_error);
      if (my_parameters.delimeter == kIncorrectDelimeter) {
          curr_error = ParsingResultType::kDelimeterError;
          break;
      }
    } else if (ParseLongLines(argv[i], dost)) {
      char* end;
      if (my_parameters.is_lines_set) {
        curr_error = ParsingResultType::kLinesError;
        break;
      }
      my_parameters.is_lines_set = true;
      if (my_parameters.lines_number) {
        curr_error = ParsingResultType::kNumbersError;
        break;
      }
      my_parameters.lines_number = std::strtoll(argv[i] + kLinesLen, &end, 10);
    } else if (ReadLongDelimeter(argv[i], dost)) {
      if (my_parameters.is_delimeter_set) {
        curr_error = ParsingResultType::kDelimeterError;
        break;
      }
      argv[i] += kDelimeterLen;
      my_parameters.delimeter = ReadDelimeter(argv[i], curr_error);
      if (my_parameters.delimeter == '\a') {
        curr_error = ParsingResultType::kDelimeterError;
        break;
      }
      my_parameters.is_delimeter_set = true;
    } else{
      if (my_parameters.file_name) {
        curr_error = ParsingResultType::kFileNameError;
        break;
      }
      my_parameters.file_name = argv[i];
    }
  }

  return my_parameters;
}

int main(int argc, char* argv[]){
  Parameters my_parameters;
  ParsingResultType curr_error = ParsingResultType::kNoErrors;
  my_parameters = Parsing(argc, argv, curr_error);
  PrintFile(my_parameters, curr_error);

  return EXIT_SUCCESS;
}
