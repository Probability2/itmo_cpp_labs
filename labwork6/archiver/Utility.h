#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>

const uint8_t kBitsInByte = 8;

class Utility {
public:
  explicit Utility(const std::vector<bool>& number, const int byte_block);
  explicit Utility(const int byte_block, const std::vector<bool>& number);
  void HafCode(std::ofstream& of_file) const;
  static uint8_t CountParityBits(const int byte_block);
  std::vector<bool> DecodeHafCode();
  static std::vector<bool> GetBitsNumber(int coding_number, const int number_bits);

private:
  void FactorizeNumber(int number, int a[]) const;
  long long GetPowerTwo(const int degree) const;
  void FindParityBits(const std::vector<bool> parity_bits, int a[], const int number_parity_bits) const;

  std::vector<bool> number_;
  int32_t byte_block_;
};
