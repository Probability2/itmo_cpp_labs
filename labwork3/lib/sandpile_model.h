#pragma once

#include <cinttypes>
#include <iostream>
#include <cstdint>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <charconv>

struct SandpileModel {
  uint32_t height = 0;
  uint32_t width = 0;
  uint64_t** sands;
  const char* output_directory;
};

struct ExtensionSandpile {
  bool is_expand = false;
  uint8_t up_direction = 0;
  uint8_t right_direction = 0;
  uint8_t down_direction = 0;
  uint8_t left_direction = 0;
  bool is_freq_zero = false;
};

SandpileModel GetSandpileModel(const char* filename, SandpileModel& sandpile_model);

void DoIterations(const uint64_t max_iteration, uint64_t frequency, SandpileModel& sandpile_model, ExtensionSandpile& my_extension);

SandpileModel ExpendSands(ExtensionSandpile& my_extension, SandpileModel& sandpile_model);

void SaveBMPImage(SandpileModel& sandpile_model, const char* output_directory, const uint64_t number_picture);

void CreateFileName(const char* output_directory, char*& filename, uint64_t number_picture);
