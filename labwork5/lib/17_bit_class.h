#pragma once

#include <iostream>

const int kInt16Max = 65536;
const int kPowerTwo[8] = {1, 2, 4, 8, 16, 32, 64, 128};
const int kPowerTwoLength = sizeof(kPowerTwo) / sizeof(kPowerTwo[0]);
const int kMaxValue = 131071;
const int kCharBitsSize = 8;

int GetIndexNumber(int, int, int, int, int);

class ArrayBitsClass {
friend std::ostream& operator<< (std::ostream&, ArrayBitsClass&);
friend std::istream& operator>> (std::istream&, ArrayBitsClass&);

public:
  class Array2DClass {
    friend std::ostream& operator<< (std::ostream&, Array2DClass);
    friend std::istream& operator>> (std::istream&, Array2DClass);
    public:
      class Array1DClass {
        friend std::ostream& operator<< (std::ostream&, Array1DClass);
        friend std::istream& operator>> (std::istream&, Array1DClass);
        public:
          Array1DClass(uint32_t index, uint32_t z_coord, unsigned char* array_bits, uint32_t ind_extra_bits);
          class NumberClass {
            friend std::ostream& operator<< (std::ostream&, NumberClass);
            friend std::istream& operator>> (std::istream&, NumberClass);
            friend int GetIntValue (const NumberClass&, int);
            friend void SetValueToArray(int value, int ind, NumberClass&);
            public:
              NumberClass(uint32_t index, unsigned char* array_bits, uint32_t ind_extra_bits);
              NumberClass& operator= (NumberClass bits_class);
              NumberClass& operator= (const int value);
              operator int() const;
            private:
              unsigned char* array_bits_;
              uint32_t curr_index_;
              uint32_t ind_extra_bits_;
          };
          Array1DClass& operator= (Array1DClass bits_class);
          NumberClass operator[] ( const uint32_t ) const;
        private:
          unsigned char* array_bits_;
          uint32_t curr_index_;
          uint32_t max_z_coord_;
          uint32_t ind_extra_bits_;
      };
      Array2DClass(uint32_t index, uint32_t y_coord, uint32_t z_coord,
                   unsigned char* array_bits, uint32_t ind_extra_bits);
      Array2DClass& operator= (Array2DClass bits_class);
      Array1DClass operator[] ( const uint32_t ) const;
    private:
      unsigned char* array_bits_;
      uint32_t curr_index_;
      uint32_t max_y_coord_;
      uint32_t max_z_coord_;
      uint32_t ind_extra_bits_;
  };
  explicit ArrayBitsClass(uint32_t x_coord, uint32_t y_coord, uint32_t z_coord);
  ~ArrayBitsClass();
  Array2DClass operator[] ( const uint32_t ) const;
  ArrayBitsClass& operator= (const ArrayBitsClass& bits_class);
  ArrayBitsClass operator+ (ArrayBitsClass& bits_class);
  ArrayBitsClass operator- (ArrayBitsClass& bits_class);
  ArrayBitsClass operator* (ArrayBitsClass& bits_class);
  ArrayBitsClass operator* (const uint32_t value);
  static ArrayBitsClass MakeArray(const uint32_t x_coord, const uint32_t y_coord, const uint32_t z_coord);

private:
  unsigned char* array_bits_;
  uint32_t max_x_coord_;
  uint32_t max_y_coord_;
  uint32_t max_z_coord_;
};
