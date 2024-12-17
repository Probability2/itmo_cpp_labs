#include "bmp_image.h"

const int32_t kFileHeaderSize = 14;
const int32_t kInformationHeaderSize = 40;
const uint8_t kPalletteSize = 16 * 4;

Image::Image(const int32_t height, const int32_t width)
: image_height_(height)
, image_width_(width) {
  colors_ = new int32_t [height * width] { 0 };
}

Image::~Image() {
  delete[] colors_;
  delete[] file_header_;
  delete[] information_header_;
}

void Image::SetColor(const int32_t color_type, const uint64_t curr_pixel) {
  colors_[curr_pixel] = color_type;
}

void Image::WriteHeader(const uint8_t ind_begin, uint8_t header[], int32_t size) {
  header[ind_begin] = size;
  header[ind_begin + 1] = size >> 8;
  header[ind_begin + 2] = size >> 16;
  header[ind_begin + 3] = size >> 24;
}

void Image::FillFileHeader(uint8_t file_header_[]) {
  file_header_[0] = 'B';
  file_header_[1] = 'M';
  WriteHeader(2, file_header_, file_size_);
  file_header_[6] = 0;
  file_header_[7] = 0;
  file_header_[8] = 0;
  file_header_[9] = 0;
  WriteHeader(10, file_header_, file_offset_);
}

void Image::FillInformationHeader(uint8_t information_header_[]) {
  std::fill(information_header_, information_header_ + kInformationHeaderSize, 0);
  information_header_[0] = kInformationHeaderSize;
  WriteHeader(4, information_header_, image_width_);
  WriteHeader(8, information_header_, image_height_);
  information_header_[12] = 1;
  information_header_[14] = 4;
  information_header_[32] = 16;
}

void Image::WriteIntoFile(std::ofstream& of, const uint64_t number_pixels) {
  Pallette pallette;
  of.write(reinterpret_cast<char*>(file_header_), kFileHeaderSize);
  of.write(reinterpret_cast<char*>(information_header_), kInformationHeaderSize);
  of.write(reinterpret_cast<char*>(pallette.white), 4);
  of.write(reinterpret_cast<char*>(pallette.green), 4);
  of.write(reinterpret_cast<char*>(pallette.yellow), 4);
  of.write(reinterpret_cast<char*>(pallette.purple), 4);
  of.write(reinterpret_cast<char*>(pallette.black), 48);
  for (int32_t i = 0; i < number_pixels; ++i) {
    of.write(reinterpret_cast<char*>(&colors_[i]), 1);
  }
}

void Image::Export(const char* path, const uint64_t number_pixels) {
  std::ofstream of(path, std::ios::out | std::ios::binary);
  if (!of.is_open()) {
    std::cerr << "ERROR:\n The output directory has not found.";
    std::exit(EXIT_FAILURE);
  }
  const int32_t kPaddingBytes = (4 - ((image_width_ + 1) / 2) % 4) % 4;
  file_size_ = kFileHeaderSize + kInformationHeaderSize + kPalletteSize + (image_width_ + 1) * image_height_ / 2 + kPaddingBytes * image_height_;
  file_offset_ = kFileHeaderSize + kInformationHeaderSize + kPalletteSize;
  file_header_ = new uint8_t [kFileHeaderSize];
  information_header_ = new uint8_t [kInformationHeaderSize];
  FillFileHeader(file_header_);
  FillInformationHeader(information_header_);
  WriteIntoFile(of, number_pixels);
}