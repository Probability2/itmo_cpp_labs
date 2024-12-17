#include "17_bit_class.h"

ArrayBitsClass::ArrayBitsClass(uint32_t x_coord, uint32_t y_coord, uint32_t z_coord)
: max_x_coord_(x_coord)
, max_y_coord_(y_coord)
, max_z_coord_(z_coord) {
  array_bits_ = new unsigned char [(x_coord * y_coord * z_coord * 17 + 7) / 8] { 0 };
}

ArrayBitsClass::~ArrayBitsClass() {
  delete[] array_bits_;
}

ArrayBitsClass ArrayBitsClass::MakeArray(const uint32_t x_coord, const uint32_t y_coord, const uint32_t z_coord) {
  return ArrayBitsClass(x_coord, y_coord, z_coord);
}

void SetValueToArray(int value, int ind, ArrayBitsClass::Array2DClass::Array1DClass::NumberClass& ArrayBits) {
  unsigned char first_char_value = 0;
  unsigned char second_char_value = 0;
  int pow2 = 1;
  int i = 0;
  while (i < 8) {
    first_char_value += pow2 * ((value >> i) % 2);
    ++i;
    pow2 *= 2;
  }
  pow2 = 1;
  while (i < 16) {
    second_char_value += pow2 * ((value >> i) % 2);
    ++i;
    pow2 *= 2;
  }
  if (value >= kInt16Max) {
    ArrayBits.array_bits_[ArrayBits.ind_extra_bits_ + ind / kCharBitsSize] +=
                          kPowerTwo[kPowerTwoLength - ind % kCharBitsSize - 1];
  }
  ArrayBits.array_bits_[2 * ind] = first_char_value;
  ArrayBits.array_bits_[2 * ind + 1] = second_char_value;
}

int GetIntValue(const ArrayBitsClass::Array2DClass::Array1DClass::NumberClass& ArrayBits, int ind) {
  int number = static_cast<int>(ArrayBits.array_bits_[2 * ind] +
               ((ArrayBits.array_bits_[2 * ind + 1]) << kCharBitsSize));
  number += (((ArrayBits.array_bits_[ArrayBits.ind_extra_bits_ + ind / kCharBitsSize]
            >> (kPowerTwoLength - 1 - ind % kCharBitsSize)) % 2) << (2 * kCharBitsSize));
  return number;
}

ArrayBitsClass::Array2DClass::Array1DClass::Array1DClass(uint32_t index, uint32_t z_coord,
                                                         unsigned char* array, uint32_t ind_extra_bits)
: curr_index_(index)
, max_z_coord_(z_coord)
, array_bits_(array)
, ind_extra_bits_(ind_extra_bits) {
}

ArrayBitsClass::Array2DClass::Array2DClass(uint32_t index, uint32_t y_coord, uint32_t z_coord,
                                           unsigned char* array, uint32_t ind_extra_bits)
: curr_index_(index)
, max_y_coord_(y_coord)
, max_z_coord_(z_coord)
, array_bits_(array)
, ind_extra_bits_(ind_extra_bits) {
}

ArrayBitsClass::Array2DClass::Array1DClass::NumberClass::NumberClass(uint32_t index,
                                                                    unsigned char* array, uint32_t ind_extra_bits)
: curr_index_(index)
, array_bits_(array)
, ind_extra_bits_(ind_extra_bits) {
}

ArrayBitsClass::Array2DClass::Array1DClass::NumberClass ArrayBitsClass::Array2DClass::Array1DClass::operator[](const uint32_t value) const {
  return Array1DClass::NumberClass(curr_index_ + value, array_bits_, ind_extra_bits_);
}

ArrayBitsClass::Array2DClass::Array1DClass ArrayBitsClass::Array2DClass::operator[](const uint32_t value) const {
  return Array2DClass::Array1DClass(curr_index_ + value * max_z_coord_, max_z_coord_, array_bits_, ind_extra_bits_);
}

ArrayBitsClass::Array2DClass ArrayBitsClass::operator[](const uint32_t value) const {
  return Array2DClass(value * max_y_coord_ * max_z_coord_, max_y_coord_, max_z_coord_,
                      array_bits_, 2 * max_x_coord_ * max_y_coord_ * max_z_coord_);
}

ArrayBitsClass::Array2DClass::Array1DClass::NumberClass& ArrayBitsClass::Array2DClass::Array1DClass::NumberClass::operator=(ArrayBitsClass::Array2DClass::Array1DClass::NumberClass bits_class) {
  if (this == &bits_class) {
    return *this;
  }
  SetValueToArray(GetIntValue(bits_class, bits_class.curr_index_), curr_index_, *this);

  return *this;
}

ArrayBitsClass::Array2DClass::Array1DClass::NumberClass& ArrayBitsClass::Array2DClass::Array1DClass::NumberClass::operator=(const int value) {
  SetValueToArray(value, curr_index_, *this);

  return *this;
}

ArrayBitsClass::Array2DClass::Array1DClass& ArrayBitsClass::Array2DClass::Array1DClass::operator=(ArrayBitsClass::Array2DClass::Array1DClass bits_class) {
  if (this == &bits_class) {
    return *this;
  }
  if (max_z_coord_ != bits_class.max_z_coord_) {
    std::cerr << "ERROR\nThe sizes of two arrays are not equal.\n";
    std::exit(EXIT_FAILURE);
  }
  for (uint32_t i = 0; i < bits_class.max_z_coord_; ++i) {
    (*this)[i] = bits_class[i];
  }

  return *this;
}

ArrayBitsClass::Array2DClass& ArrayBitsClass::Array2DClass::operator=(ArrayBitsClass::Array2DClass bits_class) {
  if (this == &bits_class) {
    return *this;
  }
  if (max_y_coord_ != bits_class.max_y_coord_ ||
      max_z_coord_ != bits_class.max_z_coord_) {
      std::cerr << "ERROR\nThe sizes of two arrays are not equal.\n";
      std::exit(EXIT_FAILURE);
  }
  for (uint32_t i = 0; i < bits_class.max_y_coord_; ++i) {
    (*this)[i] = bits_class[i];
  }

  return *this;
}

ArrayBitsClass& ArrayBitsClass::operator=(const ArrayBitsClass& bits_class) {
  if (this == &bits_class) {
    return *this;
  }
  if (max_x_coord_ != bits_class.max_x_coord_ ||
      max_y_coord_ != bits_class.max_y_coord_ ||
      max_z_coord_ != bits_class.max_z_coord_) {
      std::cerr << "ERROR\nThe sizes of two arrays are not equal.\n";
      std::exit(EXIT_FAILURE);
  }
  for (uint32_t i = 0; i < max_x_coord_; ++i) {
    Array2DClass bit(i * bits_class.max_y_coord_ * bits_class.max_z_coord_, bits_class.max_y_coord_, bits_class.max_z_coord_,
                      bits_class.array_bits_, 2 * bits_class.max_x_coord_ * bits_class.max_y_coord_ * bits_class.max_z_coord_);
    (*this)[i] = bit;
  }

  return *this;
}

int GetIndexNumber(int max_y_coord, int max_z_coord, int x_coord, int y_coord, int z_coord) {
  return x_coord * max_y_coord * max_z_coord + y_coord * max_z_coord + z_coord;
}

ArrayBitsClass::Array2DClass::Array1DClass::NumberClass::operator int() const {
  return GetIntValue(*this, curr_index_);
}

ArrayBitsClass ArrayBitsClass::operator+(ArrayBitsClass& bits_class) {
  ArrayBitsClass result(max_x_coord_, max_y_coord_, max_z_coord_);
  if (max_x_coord_ != bits_class.max_x_coord_ || max_y_coord_ != bits_class.max_y_coord_ ||
      max_z_coord_ != bits_class.max_z_coord_) {
        std::cerr << "ERROR:\nIncorrect input data for operator+ function.\n";
        std::exit(EXIT_FAILURE);
  }
  for (uint32_t i = 0; i < max_x_coord_; ++i) {
    for (uint32_t j = 0; j < max_y_coord_; ++j) {
      for (uint32_t k = 0; k < max_z_coord_; ++k) {
        int addition_value = GetIntValue((*this)[i][j][k], GetIndexNumber(max_y_coord_, max_z_coord_, i, j ,k)) +
                             GetIntValue(bits_class[i][j][k], GetIndexNumber(max_y_coord_, max_z_coord_, i, j ,k));
        if (addition_value > kMaxValue) {
          std::cerr << "ERROR:\nOut of range in the operator+ function.\n";
          std::exit(EXIT_FAILURE);
        }
        result[i][j][k] = addition_value;
      }
    }
  }

  return result;
}

ArrayBitsClass ArrayBitsClass::operator-(ArrayBitsClass& bits_class) {
  if (max_x_coord_ != bits_class.max_x_coord_ || max_y_coord_ != bits_class.max_y_coord_ || 
    max_z_coord_ != bits_class.max_z_coord_) {
      std::cerr << "ERROR:\nIncorrect input data for operator- function.\n";
      std::exit(EXIT_FAILURE);
  }
  ArrayBitsClass result(max_x_coord_, max_y_coord_, max_z_coord_);
  for (uint32_t i = 0; i < max_x_coord_; ++i) {
    for (uint32_t j = 0; j < max_y_coord_; ++j) {
      for (uint32_t k = 0; k < max_z_coord_; ++k) {
        int substract_value = GetIntValue((*this)[i][j][k], GetIndexNumber(max_y_coord_, max_z_coord_, i, j ,k)) -
                             GetIntValue(bits_class[i][j][k], GetIndexNumber(max_y_coord_, max_z_coord_, i, j ,k));
        if (substract_value > kMaxValue) {
          std::cerr << "ERROR:\nOut of range in the operator- function.\n";
          std::exit(EXIT_FAILURE);
        }
        result[i][j][k] = substract_value;
      }
    }
  }

  return result;
}

ArrayBitsClass ArrayBitsClass::operator*(const uint32_t value) {
  ArrayBitsClass result(max_x_coord_, max_y_coord_, max_z_coord_);
  for (uint32_t i = 0; i < max_x_coord_; ++i) {
    for (uint32_t j = 0; j < max_y_coord_; ++j) {
      for (uint32_t k = 0; k < max_z_coord_; ++k) {
        int multiple_value = GetIntValue((*this)[i][j][k], GetIndexNumber(max_y_coord_, max_z_coord_, i, j ,k)) * value;
        if (multiple_value > kMaxValue) {
          std::cerr << "ERROR:\nOut of range in the operator* function.\n";
          std::exit(EXIT_FAILURE);
        }
        result[i][j][k] = multiple_value;
      }
    }
  }

  return result;
}

ArrayBitsClass ArrayBitsClass::operator* (ArrayBitsClass& bits_class) {
  ArrayBitsClass result(max_x_coord_, max_y_coord_, max_z_coord_);
  for (uint32_t i = 0; i < max_x_coord_; ++i) {
    for (uint32_t j = 0; j < max_y_coord_; ++j) {
      for (uint32_t k = 0; k < max_z_coord_; ++k) {
        int multiple_value = GetIntValue((*this)[i][j][k], GetIndexNumber(max_y_coord_, max_z_coord_, i, j ,k)) *
                             GetIntValue(bits_class[i][j][k], GetIndexNumber(max_y_coord_, max_z_coord_, i, j ,k));
        if (multiple_value > kMaxValue) {
          std::cerr << "ERROR:\nOut of range in the operator* function.\n";
          std::exit(EXIT_FAILURE);
        }
        result[i][j][k] = multiple_value;
      }
    }
  }

  return result;
}

std::ostream& operator<<(std::ostream& output, ArrayBitsClass::Array2DClass::Array1DClass::NumberClass ArrayBits) {
  output << GetIntValue(ArrayBits, ArrayBits.curr_index_);

  return output;
}

std::ostream& operator<<(std::ostream& output, ArrayBitsClass::Array2DClass::Array1DClass ArrayBits) {
  for (uint32_t i = 0; i < ArrayBits.max_z_coord_; ++i) {
    output << ArrayBits[i];
    output << " ";
  }

  return output;
}

std::ostream& operator<<(std::ostream& output, ArrayBitsClass::Array2DClass ArrayBits) {
  for (uint32_t i = 0; i < ArrayBits.max_y_coord_; ++i) {
    output << ArrayBits[i];
    output << "\n";
  }

  return output;
}

std::ostream& operator<<(std::ostream& output, ArrayBitsClass& ArrayBits) {
  for (uint32_t i = 0; i < ArrayBits.max_x_coord_; ++i) {
    output << ArrayBits[i];
    output << "||\n";
  }

  return output;
}

std::istream& operator>>(std::istream& input, ArrayBitsClass::Array2DClass::Array1DClass::NumberClass ArrayBits) {
  uint32_t value;
  input >> value;
  SetValueToArray(value, ArrayBits.curr_index_, ArrayBits);
 
  return input;
}

std::istream& operator>>(std::istream& input, ArrayBitsClass::Array2DClass::Array1DClass ArrayBits) {
  for (uint32_t i = 0; i < ArrayBits.max_z_coord_; ++i) {
    input >> ArrayBits[i];
  }
 
  return input;
}

std::istream& operator>>(std::istream& input, ArrayBitsClass::Array2DClass ArrayBits) {
  for (uint32_t i = 0; i < ArrayBits.max_y_coord_; ++i) {
    input >> ArrayBits[i];
  }
 
  return input;
}

std::istream& operator>>(std::istream& input, ArrayBitsClass& ArrayBits) {
  for (uint32_t i = 0; i < ArrayBits.max_x_coord_; ++i) {
    input >> ArrayBits[i];
  }
 
  return input;
}
