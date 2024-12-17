#include "Archive.h"

Archive::Archive(const std::string& archiver_name, const int byte_block_size)
: archiver_name_(archiver_name)
, byte_block_(byte_block_size) {
}

Archive::Archive(const int byte_block_size, const std::string& archiver_name):
                                      Archive(archiver_name, byte_block_size) {}

Archive::Archive(const std::string& archiver_name)
: archiver_name_(archiver_name) {
}

void Archive::WriteInteger(std::ofstream& archiver, const int number, const int number_bytes) const {
  std::vector<bool> vec = Utility::GetBitsNumber(number, number_bytes);
  Utility util(vec, number_bytes);
  util.HafCode(archiver);
}

void Archive::WriteAllBytes(std::ifstream& archiver, std::ofstream& of_file) const {
  char data;
  while (archiver.get(data)) {
    of_file.put(data);
  }
}

void Archive::CreateArchive(const std::vector<std::string>& files) const {
  std::ofstream archiver(archiver_name_, std::ios::out | std::ios::binary);
  if (!archiver) {
    std::cerr << "ERROR:\nThe archiver can not be opened.\n";
    std::exit(EXIT_FAILURE);
  }
  WriteInteger(archiver, byte_block_, kBytesBlock - 1);
  for (const auto& archive_name: files) {
    HafData my_file(byte_block_, archive_name);
    my_file.WriteFile(archiver);
  }
  archiver.close();
}

int Archive::ReadInteger(std::ifstream& archive, const int bytes_needed) const {
  char buff[bytes_needed];
  archive.read(buff, bytes_needed);
  uint32_t mult = 1;
  int result = 0;
  std::vector<bool> vec;
  for (int32_t i = 0; i < bytes_needed; ++i) {
    uint8_t curr_char = static_cast<uint8_t>(buff[i]);
    for (int32_t j = 0; j < kBitsInByte; ++j) {
      vec.push_back(static_cast<bool>(curr_char % 2));
      curr_char /= 2;
    }
  }
  Utility util(vec, bytes_needed - 1);
  std::vector<bool> number_vec = util.DecodeHafCode();
  for (std::size_t i = 0; i < number_vec.size() - 1; ++i) {
    if (number_vec[i]) {
      result += mult;
    }
    mult *= 2;
  }

  return result;
}

void Archive::AppendFiles(const std::vector<std::string>& files) const {
  std::string temp_archiver_name = archiver_name_ + ".tmp";
  std::ofstream temp_archiver(temp_archiver_name, std::ios::out | std::ios::binary | std::ios::app);
  std::ifstream archiver(archiver_name_, std::ios::in | std::ios::binary);
  if (!archiver) {
    std::cerr << "ERROR:\nThe archiver can not be opened.\n";
    std::exit(EXIT_FAILURE);
  }
  int byte_block = ReadInteger(archiver, kBytesBlock);
  WriteInteger(temp_archiver, byte_block, kBytesBlock - 1);
  WriteAllBytes(archiver, temp_archiver);
  for (const auto& file: files) {
    HafData my_file(file, byte_block);
    my_file.WriteFile(temp_archiver);
  }
  archiver.close();
  temp_archiver.close();
  std::remove(archiver_name_.c_str());
  std::rename(temp_archiver_name.c_str(), archiver_name_.c_str());
}

void Archive::PrintFileNames() const {
  std::ifstream archiver(archiver_name_, std::ios::in | std::ios::binary);
  if (!archiver) {
    std::cerr << "ERROR:\nThe archiver can not be opened.\n";
    std::exit(EXIT_FAILURE);
  }
  int block_size = ReadInteger(archiver, kBytesBlock);
  std::cout << "Files in archiver " << archiver_name_ << ":\n";
  while (!archiver.eof()) {
    HafData my_file(block_size);
    int32_t file_full_size = my_file.ReadIntegerFromFile(archiver, kBitsInByte - 1);
    if (!archiver.gcount()) {
      break;
    }
    int32_t file_name_size = my_file.ReadIntegerFromFile(archiver, 1);
    std::string file_name(file_name_size, kNullChar);
    my_file.ReadFileName(archiver, file_name);
    std::cout << file_name << "\n";
    int32_t new_byte_block = block_size + Utility::CountParityBits(block_size) / kBitsInByte + 1;
    archiver.seekg(new_byte_block * ((file_full_size - file_name_size - 1) / block_size + 1), std::ios::cur);
  }
  archiver.close();
}

void Archive::ExtractAllFiles() const {
  std::ifstream archiver(archiver_name_, std::ios::in | std::ios::binary);
  if (!archiver) {
    std::cerr << "ERROR:\nThe archiver can not be opened.\n";
    std::exit(EXIT_FAILURE);
  }
  int block_size = ReadInteger(archiver, kBytesBlock);
  while (!archiver.eof()) {
    HafData my_file(block_size);
    my_file.DecodeFile(archiver);
  }
  archiver.close();
}

void Archive::ExtractFiles(const std::vector<std::string>& files) const {
  std::ifstream archiver(archiver_name_, std::ios::in | std::ios::binary);
  if (!archiver) {
    std::cerr << "ERROR:\nThe archiver can not be opened.\n";
    std::exit(EXIT_FAILURE);
  }
  int block_size = ReadInteger(archiver, kBytesBlock);
  while (!archiver.eof()) {
    HafData my_file(block_size);
    int32_t file_full_size = my_file.ReadIntegerFromFile(archiver, kBitsInByte - 1);
    if (!archiver.gcount()) {
      break;
    }
    int32_t file_name_size = my_file.ReadIntegerFromFile(archiver, 1);
    std::string file_name(file_name_size, kNullChar);
    my_file.ReadFileName(archiver, file_name);
    int32_t new_byte_block = block_size + Utility::CountParityBits(block_size) / kBitsInByte + 1;
    if (std::find(files.begin(), files.end(), file_name) != files.end()) {
      std::ofstream of_file(file_name, std::ios::out | std::ios::binary);
      my_file.ReadFileBlocks(archiver, of_file, file_full_size - file_name_size);
    } else {
      archiver.seekg(new_byte_block * ((file_full_size - file_name_size - 1) / block_size + 1), std::ios::cur);
    }
  }
  archiver.close();
}

void Archive::DeleteFiles(const std::vector<std::string>& files) const {
  std::string temp_archiver_name = archiver_name_ + ".tmp";
  std::ofstream temp_archiver(temp_archiver_name, std::ios::out | std::ios::binary);
  std::ifstream archiver(archiver_name_, std::ios::in | std::ios::binary);
  if (!archiver) {
    std::cerr << "ERROR:\nThe archiver can not be opened.\n";
    std::exit(EXIT_FAILURE);
  }
  int block_size = ReadInteger(archiver, kBytesBlock);
  WriteInteger(temp_archiver, block_size, kBytesBlock - 1);
  while (!archiver.eof()) {
    HafData my_file(block_size);
    int32_t file_full_size = my_file.ReadIntegerFromFile(archiver, kBitsInByte - 1);
    if (!archiver.gcount()) {
      break;
    }
    int32_t file_name_size = my_file.ReadIntegerFromFile(archiver, 1);
    std::string file_name(file_name_size, kNullChar);
    my_file.ReadFileName(archiver, file_name);
    if (std::find(files.begin(), files.end(), file_name) == files.end()) {
      my_file.WriteInteger(temp_archiver, kBitsInByte - 1, file_full_size);
      my_file.WriteInteger(temp_archiver, 1, file_name_size);
      my_file.SetFileName(file_name);
      my_file.WriteFileName(temp_archiver, file_name_size);
      int32_t new_byte_block = block_size + Utility::CountParityBits(block_size) / kBitsInByte + 1;
      for (int j = 0; j < (file_full_size - file_name_size - 1) / block_size + 1; ++j) {
        char writer[new_byte_block] { kNullChar };
        archiver.read(writer, new_byte_block);
        temp_archiver.write(writer, new_byte_block);
      }
    } else {
      int new_byte_block = block_size + (Utility::CountParityBits(block_size) + kBitsInByte) / kBitsInByte;
      archiver.seekg(new_byte_block * ((file_full_size - file_name_size - 1) / block_size + 1), std::ios::cur);
    }
  }
  archiver.close();
  temp_archiver.close();
  std::remove(archiver_name_.c_str());
  std::rename(temp_archiver_name.c_str(), archiver_name_.c_str());
}

void Archive::OverwriteFile(std::ifstream& archiver, std::ofstream& of_archiver,
                                            const FileParameters& my_parameters) const {
  std::vector<bool> buff;
  char buff_writer[my_parameters.block_size_ + my_parameters.number_parity_bytes_];
  int n = 0;
  int summ = 0;
  while (n != (my_parameters.file_size_ - 1) / my_parameters.block_size_ + 1) {
    archiver.read(buff_writer, my_parameters.block_size_ + my_parameters.number_parity_bytes_);
    std::vector<bool> vec;
    int32_t ind = 0;
    while (vec.size() != kBitsInByte * (my_parameters.number_parity_bytes_ + my_parameters.block_size_)) {
      std::vector<bool> curr_char_vec = Utility::GetBitsNumber(static_cast<uint8_t>(buff_writer[ind]), 1);
      for (int32_t j = 0; j < kBitsInByte; ++j) {
        vec.push_back(curr_char_vec[j]);
      }
      ++ind;
    }
    Utility util(vec, my_parameters.block_size_);
    std::vector<bool> result_vec = util.DecodeHafCode();
    for (std::size_t j = 0; j < kBitsInByte * my_parameters.block_size_; ++j) {
      buff.push_back(result_vec[j]);
      if (buff.size() == kBitsInByte * byte_block_ && summ < my_parameters.archive_block_writer_size_
                * ((my_parameters.file_size_ - 1) / byte_block_) + 1) {
        Utility util(buff, byte_block_);
        util.HafCode(of_archiver);
        buff.clear();
        summ += my_parameters.archive_block_writer_size_;
      }
    }
    ++n;
  }
}

void Archive::MergeArchives(const std::vector<std::string>& archives) const {
  std::ofstream of_archiver(archiver_name_, std::ios::out | std::ios::binary);
  if (!of_archiver) {
    std::cerr << "ERROR:\nThe archiver can not be opened.\n";
    std::exit(EXIT_FAILURE);
  }
  WriteInteger(of_archiver, byte_block_, kBytesBlock - 1);
  int archive_block_writer_size = byte_block_ + Utility::CountParityBits(byte_block_) / kBitsInByte + 1;
  for (const auto& archive_name: archives) {
    FileParameters my_parameters;
    std::ifstream archiver(archive_name, std::ios::in | std::ios::binary);
    my_parameters.block_size_ = ReadInteger(archiver, kBytesBlock);
    my_parameters.number_parity_bytes_ = Utility::CountParityBits(my_parameters.block_size_) / kBitsInByte + 1;
    char buff_writer[my_parameters.block_size_ + my_parameters.number_parity_bytes_];
    while (!archiver.eof()) {
      HafData my_file(my_parameters.block_size_);
      HafData my_archiver(byte_block_);
      int file_full_size = my_file.ReadIntegerFromFile(archiver, kBitsInByte - 1);
      if (!archiver.gcount()) {
        break;
      }
      my_archiver.WriteInteger(of_archiver, kBitsInByte - 1, file_full_size);
      int file_name_size = my_file.ReadIntegerFromFile(archiver, 1);
      my_archiver.WriteInteger(of_archiver, 1, file_name_size);
      std::string file_name(file_name_size, kNullChar);
      my_file.ReadFileName(archiver, file_name);
      my_archiver.SetFileName(file_name);
      my_archiver.WriteFileName(of_archiver, file_name_size);
      my_parameters.file_size_ = file_full_size - file_name_size;
      my_parameters.archive_block_writer_size_ = archive_block_writer_size;
      OverwriteFile(archiver, of_archiver, my_parameters);
    }
    archiver.close();
  }
  of_archiver.close();
}
