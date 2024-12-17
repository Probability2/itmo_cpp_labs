#pragma once

#include "Utility.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

const char kNullChar = '\0';

class HafData {
public:
  explicit HafData(const std::string& file_name, const int block_size);
  explicit HafData(const int block_size, const std::string& file_name);
  explicit HafData(const int block_size);
  void WriteFile(std::ofstream& of_file);
  void DecodeFile(std::ifstream& archiver) const;
  int ReadIntegerFromFile(std::ifstream& archiver, const int number_bits) const;
  void ReadFileName(std::ifstream& archiver, std::string& file_name) const;
  void ReadFileBlocks(std::ifstream& archiver, std::ofstream& file, const int full_file_size) const;
  void WriteInteger(std::ofstream& archiver, const int number_bits, const int number) const;
  void WriteFileName(std::ofstream& archiver, const std::size_t file_name_length);
  void WriteFileBlocks(std::ofstream& archiver, std::ifstream& file);
  void SetFileName(const std::string& file_name);

private:
  std::vector<bool> GetVectorFromString(const std::string& str) const;
  std::string file_name_;
  std::string archive_name_;
  int32_t byte_block_;
};

inline std::size_t FileSize(std::ifstream& my_file) {
  my_file.seekg(0, std::ios::end);
  std::size_t file_size = my_file.tellg();
  my_file.clear();
  my_file.seekg(0, std::ios::beg);
  return file_size;
}
