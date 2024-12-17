#pragma once

#include <cinttypes>
#include <iostream>
#include <cstdint>
#include <fstream>

struct Pallette {
  uint8_t white[4] = {255, 255, 255, 0};
  uint8_t green[4]  = {0, 255, 0, 0};
  uint8_t yellow[4] = {0, 255, 255, 0};
  uint8_t purple[4] = {255, 0, 255, 0};
  uint8_t black[4] = {0, 0, 0, 0};
};

class Image {
  public:
    Image (const int32_t height, const int32_t width);
    ~Image();
    void SetColor(const int color_type, const uint64_t pixel_count);
    void Export(const char* path, const uint64_t byte_count);
    void FillFileHeader(uint8_t fileHeader[]);
    void FillInformationHeader(uint8_t informationHeader[]);
    void WriteIntoFile(std::ofstream& of, const uint64_t number_pixels);
    void WriteHeader(const uint8_t ind_begin, uint8_t header[], int32_t size);

  private:
    int32_t image_height_;
    int32_t image_width_;
    int32_t file_size_;
    int32_t file_offset_;
    int32_t* colors_;
    uint8_t* file_header_;
    uint8_t* information_header_;
};
