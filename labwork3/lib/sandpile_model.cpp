#include "sandpile_model.h"
#include "bmp_image.h"

const char kSeparator = '\t';
const uint8_t kMaxNumberSands = 4;
const char* begin_file_name = "/sandpile_model_";
const std::size_t kLengthBeginFileName = std::strlen(begin_file_name);
const char* kFileExtension = ".bmp";
const uint8_t kExtraLength = 25;

SandpileModel ExpendSands(ExtensionSandpile& my_extension, SandpileModel& sandpile_model) {
  uint64_t** new_sands = new uint64_t* [sandpile_model.height + my_extension.up_direction + my_extension.down_direction] { 0 };
  for (int64_t i = 0; i < sandpile_model.height + my_extension.up_direction + my_extension.down_direction; ++i) {
    new_sands[i] = new uint64_t[sandpile_model.width + my_extension.left_direction + my_extension.right_direction] { 0 };
  }
  for (int32_t i = 0; i < sandpile_model.height; ++i) {
    for (int32_t j = 0; j < sandpile_model.width; ++j) {
      new_sands[i + my_extension.up_direction][j + my_extension.left_direction] = sandpile_model.sands[i][j];
    }
    delete[] sandpile_model.sands[i];
  }
  delete[] sandpile_model.sands;
  sandpile_model.sands = new_sands;
  sandpile_model.height += (my_extension.up_direction + my_extension.down_direction);
  sandpile_model.width += (my_extension.left_direction + my_extension.right_direction);
  return sandpile_model;
}

void CreateFileName(const char* output_directory, char*& filename, uint64_t number_picture) {
  const uint16_t kLengthOutputDirectory = std::strlen(output_directory);
  uint16_t number_picture_length = kLengthOutputDirectory + kLengthBeginFileName;
  uint64_t number_picture_copy = number_picture;
  filename = new char [kLengthOutputDirectory + kLengthBeginFileName + kExtraLength];
  for (int16_t i = 0; i < kLengthOutputDirectory; ++i) {
    filename[i] = output_directory[i];
  }
  for (int32_t i = 0; i < kLengthBeginFileName; ++i) {
    filename[i + kLengthOutputDirectory] = begin_file_name[i];
  }
  while (number_picture) {
    number_picture_length++;
    number_picture /= 10;
  }
  std::to_chars(filename + kLengthOutputDirectory + kLengthBeginFileName, filename + kLengthOutputDirectory
                + kLengthBeginFileName + number_picture_length, number_picture_copy);
  for (int32_t i = 0; i <= std::strlen(kFileExtension); ++i) {
    filename[i + number_picture_length] = kFileExtension[i];
  }
}

void SaveBMPImage(SandpileModel& sandpile_model, const char* output_directory, const uint64_t number_picture) {
  Image Image(sandpile_model.height, sandpile_model.width);
  uint64_t curr_pixel = 0;
  int32_t left_color;
  int32_t right_color;
  int32_t** colors = new int32_t* [sandpile_model.height] { 0 };
  for (int32_t i = 0; i < sandpile_model.height; ++i) {
    colors[i] = new int32_t[sandpile_model.width + 1] { 0 };
  }
  bool is_positive = false;
  for (int32_t i = 0; i < sandpile_model.height; ++i) {
    for (int32_t j = 0; j < sandpile_model.width; ++j) {
      if (sandpile_model.sands[i][j]) {
        is_positive = true;
        break;
      }
    }
  }
  for (int32_t row = 0; row < sandpile_model.height; ++row) {
    for (int32_t column = 0; column < sandpile_model.width; column += 2) {
      if (sandpile_model.sands[row][column] >= kMaxNumberSands) {
        left_color = kMaxNumberSands;
      } else {
        left_color = sandpile_model.sands[row][column];
      }
      colors[row][column] = left_color;
      if (sandpile_model.width % 2 && column == sandpile_model.width - 1) {
        right_color = 0;
        colors[row][column + 1] = right_color;
        continue;
      }
      if (sandpile_model.sands[row][column + 1] >= kMaxNumberSands) {
        right_color = kMaxNumberSands;
      } else {
        right_color = sandpile_model.sands[row][column + 1];
      }
      colors[row][column + 1] = right_color;
    }
  }
  for (int32_t row = 0; row < sandpile_model.height; ++row) {
    for (int32_t column = 0; column < sandpile_model.width; column += 2) {
      Image.SetColor((colors[row][column] << 4) + colors[row][column + 1], curr_pixel++);
    }
    curr_pixel += (4 - ((sandpile_model.width + 1) / 2) % 4) % 4;
  }
  char* filename;
  CreateFileName(output_directory, filename, number_picture);
  Image.Export(filename, curr_pixel);
  for (int32_t i = 0; i < sandpile_model.height; ++i) {
    delete colors[i];
  }
  delete colors;
}

void DoIterations(const uint64_t max_iteration, uint64_t frequency, SandpileModel& sandpile_model, ExtensionSandpile& my_extension) {
  uint64_t curr_iteration = 0;
  if (frequency == 0) {
    frequency = max_iteration + 1;
    my_extension.is_freq_zero = true;
  }
  uint64_t number_picture = 1;
  while (max_iteration > curr_iteration) {
    my_extension.is_expand = false;
    my_extension.up_direction = 0;
    my_extension.right_direction = 0;
    my_extension.down_direction = 0;
    my_extension.left_direction = 0;
    uint8_t** expend_coords = new uint8_t* [sandpile_model.height] { 0 };
    for (int32_t i = 0; i < sandpile_model.height; ++i) {
      expend_coords[i] = new uint8_t[sandpile_model.width] { 0 };
    }
    bool is_positive_number = false;
    int32_t* raws = new int32_t [sandpile_model.height] { 0 };
    int32_t* columns = new int32_t [sandpile_model.width] { 0 };
    for (int32_t i = 0; i < sandpile_model.height; ++i) {
      for (int32_t j = 0; j < sandpile_model.width; ++j) {
        if (sandpile_model.sands[i][j] >= kMaxNumberSands) {
          expend_coords[i][j] = 1;
          is_positive_number = true;
        }
      }
    }
    if (!is_positive_number) {
      std::cout << "The sandpile model has reached a stable state.\n";
      break;
    }
    for (int32_t i = 0; i < sandpile_model.height; ++i) {
      for (int32_t j = 0; j < sandpile_model.width; ++j) {
        if (!expend_coords[i][j]) {
          continue;
        }
        if (i % (sandpile_model.height - 1) == 0 || j % (sandpile_model.width - 1) == 0) {
          my_extension.is_expand = true;
        }
        sandpile_model.sands[i][j] -= kMaxNumberSands;
        if (j == 0) {
          std::cout << "";
          ++raws[i];
          my_extension.left_direction = 1;
        }
        if (j == sandpile_model.width - 1) {
          raws[i] += 2;
          my_extension.right_direction = 1;
        }
        if (i == 0) {
          ++columns[j];
          my_extension.up_direction = 1;
        }
        if (i == sandpile_model.height - 1) {
          columns[j] += 2;
          my_extension.down_direction = 1;
        }
        if (j) {
          ++sandpile_model.sands[i][j - 1];
        }
        if (j < sandpile_model.width - 1) {
          ++sandpile_model.sands[i][j + 1];
        }
        if (i) {
          ++sandpile_model.sands[i - 1][j];
        }
        if (i < sandpile_model.height - 1) {
          ++sandpile_model.sands[i + 1][j];
        }
      }
    }
    ++curr_iteration;
    if (my_extension.is_expand) {
      sandpile_model = ExpendSands(my_extension, sandpile_model);
      for (int32_t i = 0; i < sandpile_model.height - my_extension.up_direction - my_extension.down_direction; ++i) {
        if (raws[i] == 1 || raws[i] == 3) {
          sandpile_model.sands[i + my_extension.up_direction][0] = 1;
        }
        if (raws[i] == 2 || raws[i] == 3) {
          sandpile_model.sands[i + my_extension.up_direction][sandpile_model.width - 1] = 1;
        }
      }
      for (int32_t i = 0; i < sandpile_model.width - my_extension.right_direction - my_extension.left_direction; ++i) {
        if (columns[i] == 1 || columns[i] == 3) {
          sandpile_model.sands[0][i + my_extension.left_direction] = 1;
        }
        if (columns[i] == 2 || columns[i] == 3) {
          sandpile_model.sands[sandpile_model.height - 1][i + my_extension.left_direction] = 1;
        }
      }
    }
    if (curr_iteration % frequency == 0) {
      SaveBMPImage(sandpile_model, sandpile_model.output_directory, number_picture);
      ++number_picture;
    }
    delete[] raws;
    delete[] columns;
    for (int32_t i = 0; i < sandpile_model.height - my_extension.up_direction - my_extension.down_direction; ++i) {
      delete[] expend_coords[i];
    }
    delete[] expend_coords;
  }
}

SandpileModel GetSandpileModel(const char* filename, SandpileModel& sandpile_model) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "ERROR:\n This file hasn't found.\n";
    std::exit(EXIT_FAILURE);
  }
  int16_t x;
  int16_t y;
  uint64_t number_sands;
  int16_t min_x = INT16_MAX;
  int16_t min_y = INT16_MAX;
  int16_t max_x = INT16_MIN;
  int16_t max_y = INT16_MIN;
  while (file >> x >> y >> number_sands) {
    min_x = std::min(min_x, x);
    min_y = std::min(min_y, y);
    max_x = std::max(max_x, x);
    max_y = std::max(max_y, y);
  }
  sandpile_model.height = max_x - min_x + 1;
  sandpile_model.width = max_y - min_y + 1;
  sandpile_model.sands = new uint64_t* [sandpile_model.height] { 0 };
  for (int64_t i = 0; i < sandpile_model.height; ++i) {
    sandpile_model.sands[i] = new uint64_t[sandpile_model.width] { 0 };
  }
  file.clear();
  file.seekg(0, std::ios::beg);
  while (file >> x >> y >> number_sands) {
    sandpile_model.sands[x - min_x][y - min_y] = number_sands;
  }

  return sandpile_model;
}
