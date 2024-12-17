#include "HafData.h"

HafData::HafData(const int block_size)
: byte_block_(block_size) {
}

HafData::HafData(const std::string& file_name, const int block_size): HafData(block_size) {
  file_name_ = file_name;
}

HafData::HafData(const int block_size, const std::string& file_name): HafData(file_name, block_size) {}

void HafData::SetFileName(const std::string& file_name) {
  file_name_ = file_name;
}

std::vector<bool> HafData::GetVectorFromString(const std::string& str) const {
  std::vector<bool> binary_vector;
  for (char c: str) {
    for (std::size_t j = 0; j < kBitsInByte; ++j) {
      binary_vector.push_back(((c >> j) & 1) ? true : false);
    }
  }

  return binary_vector;
}

void HafData::WriteInteger(std::ofstream& archiver, const int number_bits, const int number) const {
  std::vector<bool> vec = Utility::GetBitsNumber(number, (1 + number_bits / byte_block_) * byte_block_);
  int i = 0;
  int begin_index = 0;
  while (i < 1 + number_bits / byte_block_) {
    std::vector<bool> subVector;
    for (std::size_t j = begin_index; j < begin_index + byte_block_ * kBitsInByte; ++j) {
      subVector.push_back(vec[j]);
    }
    Utility util(subVector, byte_block_);
    util.HafCode(archiver);
    ++i;
    begin_index += byte_block_ * kBitsInByte;
  }
}

void HafData::WriteFileName(std::ofstream& archiver, const std::size_t file_name_length) {
  std::vector<bool> binary_vector = GetVectorFromString(file_name_);
  int i = 0;
  int begin_index = 0;
  while (i < (file_name_length - 1) / byte_block_ + 1) {
    std::vector<bool> subVector(kBitsInByte * byte_block_);
    std::fill(subVector.begin(), subVector.end(), false);
    for (int32_t j = 0; j < kBitsInByte * byte_block_; ++j) {
      subVector[j] = binary_vector[j + begin_index];
    }
    Utility util(subVector, byte_block_);
    util.HafCode(archiver);
    ++i;
    begin_index += kBitsInByte * byte_block_;
  }
}

void HafData::WriteFileBlocks(std::ofstream& archiver, std::ifstream& my_file) {
  char buff[byte_block_];
  while (my_file.read(buff, byte_block_)) {
    Utility util(GetVectorFromString(std::string(buff)), byte_block_);
    util.HafCode(archiver);
  }
}

void HafData::WriteFile(std::ofstream& of_file) {
  std::ifstream my_file(file_name_, std::ios::in | std::ios::binary);
  if (!my_file) {
    std::cerr << "ERROR:\nThe file " << file_name_ << " can not be opened.\n";
    return;
  }
  std::size_t file_name_length = file_name_.length();
  WriteInteger(of_file, kBitsInByte - 1, file_name_length + FileSize(my_file));
  WriteInteger(of_file, 1, file_name_length);
  WriteFileName(of_file, file_name_length);
  WriteFileBlocks(of_file, my_file);
  my_file.close();
}

int HafData::ReadIntegerFromFile(std::ifstream& archiver, const int number_bits) const {
  char buff[byte_block_ + 1];
  int32_t i = 0;
  int32_t result = 0;
  int32_t mult = 1;
  while (i < 1 + number_bits / byte_block_ && archiver.read(buff, byte_block_ + 1)) {
    std::vector<bool> vec;
    int32_t ind = 0;
    while (vec.size() != kBitsInByte * (byte_block_ + 1)) {
      std::vector<bool> curr_char_vec = Utility::GetBitsNumber(static_cast<uint8_t>(buff[ind]), 1);
      for (int32_t j = 0; j < kBitsInByte; ++j) {
        vec.push_back(curr_char_vec[j]);
      }
      ++ind;
    }
    Utility util(vec, byte_block_);
    std::vector<bool> result_vec = util.DecodeHafCode();
    for (std::size_t j = 0; j < kBitsInByte * byte_block_; ++j) {
      result += mult * static_cast<int>(result_vec[j]);
      mult *= 2;
    }
    ++i;
  }

  return result;
}

void HafData::ReadFileName(std::ifstream& archiver, std::string& file_name) const {
  char buff[byte_block_ + 1];
  int i = 0;
  int curr_ind_filename = -1;
  int mult = 1;
  int file_name_size = file_name.length();
  while (i < (file_name_size - 1) / byte_block_ + 1 && archiver.read(buff, byte_block_ + 1)) {
    std::vector<bool> vec;
    int32_t ind = 0;
    while (vec.size() != kBitsInByte * (byte_block_ + 1)) {
      std::vector<bool> curr_char_vec = Utility::GetBitsNumber(static_cast<uint8_t>(buff[ind]), 1);
      for (int32_t j = 0; j < kBitsInByte; ++j) {
        vec.push_back(curr_char_vec[j]);
      }
      ++ind;
    }
    Utility util(vec, byte_block_ + 1);
    std::vector<bool> result_vec = util.DecodeHafCode();
    for (std::size_t j = 0; j < kBitsInByte * byte_block_; ++j) {
      if (j % kBitsInByte == 0) {
        mult = 1;
        ++curr_ind_filename;
      }
      if (curr_ind_filename >= file_name_size) {
        break;
      }
      if (curr_ind_filename < file_name_size) {
        file_name[curr_ind_filename] += mult * static_cast<int>(result_vec[j]);
      }
      mult *= 2;
    }
    ++i;
  }
}

void HafData::ReadFileBlocks(std::ifstream& archiver, std::ofstream& my_file, const int full_file_size) const {
  int i = 0;
  int new_byte_block = byte_block_ + static_cast<int>(Utility::CountParityBits(byte_block_)) / kBitsInByte + 1;
  char buff[new_byte_block] {kNullChar};
  while (i != (full_file_size - 1) / byte_block_ + 1 && archiver.read(buff, new_byte_block)) {
    char writer[byte_block_] { kNullChar };
    std::vector<bool> vec;
    int32_t ind = 0;
    while (vec.size() != kBitsInByte * new_byte_block) {
      uint8_t curr_char = static_cast<uint8_t>(buff[ind]);
      std::vector<bool> curr_char_vec = Utility::GetBitsNumber(static_cast<int>(curr_char), 1);
      for (int32_t j = 0; j < kBitsInByte; ++j) {
        vec.push_back(curr_char_vec[j]);
      }
      ++ind;
    }
    Utility util(vec, byte_block_);
    std::vector<bool> result_vec = util.DecodeHafCode();
    int32_t curr_ind = -1;
    int32_t mult = 1;
    for (std::size_t j = 0; j < kBitsInByte * byte_block_; ++j) {
      if (j % kBitsInByte == 0) {
        mult = 1;
        ++curr_ind;
      }
      writer[curr_ind] += mult * static_cast<int32_t>(result_vec[j]);
      mult *= 2;
    }
    ++i;
    if (i != (full_file_size - 1) / byte_block_ + 1 || full_file_size % byte_block_ == 0) {
      my_file.write(writer, byte_block_);
      continue;
    }
    my_file.write(writer, full_file_size % byte_block_);
  }
  my_file.close();
}

void HafData::DecodeFile(std::ifstream& archiver) const {
  int32_t file_full_size = ReadIntegerFromFile(archiver, kBitsInByte - 1);
  int32_t file_name_size = ReadIntegerFromFile(archiver, 1);
  std::string file_name(file_name_size, kNullChar);
  ReadFileName(archiver, file_name);
  std::ofstream my_file(file_name, std::ios::out | std::ios::binary);
  ReadFileBlocks(archiver, my_file, file_full_size - file_name_size);
  my_file.close();
}
