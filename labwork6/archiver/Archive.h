#pragma once

#include "HafData.h"
#include "Utility.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const uint8_t kBytesBlock = 3;

struct FileParameters {
  int number_parity_bytes_;
  int archive_block_writer_size_;
  int file_size_;
  int block_size_;
};

class Archive {
public:
  explicit Archive(const std::string& archiver_name, const int byte_block_size);
  explicit Archive(const int byte_block_size, const std::string& archiver_name);
  explicit Archive(const std::string& archiver_name);
  void CreateArchive(const std::vector<std::string>& files) const;
  void ExtractAllFiles() const;
  void ExtractFiles(const std::vector<std::string>& files) const;
  void AppendFiles(const std::vector<std::string>& files) const;
  void PrintFileNames() const;
  void DeleteFiles(const std::vector<std::string>& files) const;
  void MergeArchives(const std::vector<std::string>& archives) const;

private:
  void WriteInteger(std::ofstream& archiver, const int number, const int number_bytes) const;
  int ReadInteger(std::ifstream& archiver, const int bytes_needed) const;
  void WriteAllBytes(std::ifstream& archiver, std::ofstream& of_file) const;
  void OverwriteFile(std::ifstream& archiver, std::ofstream& of_archiver, const FileParameters& my_parameters) const;
  std::string archiver_name_;
  int byte_block_;
};
