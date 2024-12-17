#include "Utility.h"

Utility::Utility(const std::vector<bool>& number, const int byte_block)
: number_(number)
, byte_block_(byte_block) {
}

Utility::Utility(const int byte_block, const std::vector<bool>& number): Utility(number, byte_block) {}

long long Utility::GetPowerTwo(const int degree) const {
  long long ans = 1;
  for (uint16_t i = 0; i < degree; ++i) {
    ans *= 2;
  }
  
  return ans;
}

void Utility::FactorizeNumber(int number, int a[]) const {
  int32_t length = 0;
  while (number) {
    a[length] ^= (number % 2);
    number /= 2;
    ++length;
  }
}

std::vector<bool> Utility::GetBitsNumber(int coding_number, const int number_bytes) {
  std::vector<bool> result;
  for (int32_t j = 0; j < kBitsInByte * number_bytes; ++j) {
    result.push_back(static_cast<bool>(coding_number % 2));
    coding_number /= 2;
  }

  return result;
}

uint8_t Utility::CountParityBits(const int byte_block) {
  int32_t parity_bit = 1;
  uint8_t number_parity_bits = 0;
  int32_t i = 0;
  int32_t curr_ind = 0;
  while (i != kBitsInByte * byte_block) {
    ++curr_ind;
    if (curr_ind != parity_bit) {
      ++i;
      continue;
    }
    parity_bit *= 2;
    ++number_parity_bits;
  }

  return number_parity_bits;
}

void Utility::FindParityBits(const std::vector<bool> number, int a[], const int number_parity_bits) const {
  int32_t i = number.size() - 1;
  int32_t parity_bit = GetPowerTwo(number_parity_bits - 1);
  int32_t curr_ind = number_parity_bits + number.size() - 1;
  while (i >= 0) {
    if (curr_ind == parity_bit - 1) {
      parity_bit /= 2;
    } else {
      if (number[i--]) {
        FactorizeNumber(curr_ind + 1, a);
      }
    }
    --curr_ind;
  }
}

void Utility::HafCode(std::ofstream& of) const {
  uint8_t hamming_result = 0;
  uint8_t number_parity_bits = CountParityBits(byte_block_);
  int32_t parity_bits[number_parity_bits] { 0 };
  FindParityBits(number_, parity_bits, number_parity_bits);
  long long end_parity_bits = 1;
  bool total_xor = false;
  int32_t i = 0;
  int32_t curr_koef = 1;
  int32_t end_a_array = 0;
  int32_t byte_position = 0;
  uint8_t haf_result[(number_parity_bits + kBitsInByte * byte_block_ + kBitsInByte) / kBitsInByte] { 0 };
  int32_t haf_result_position = 0;
  int32_t curr_ind = 0;
  int32_t number_size = number_.size();
  while (curr_ind < number_size + kBitsInByte * (1 + number_parity_bits / kBitsInByte)) {
    if (byte_position == kBitsInByte) {
      haf_result[haf_result_position] = hamming_result;
      hamming_result = 0;
      ++haf_result_position;
      byte_position = 0;
      curr_koef = 1;
    }
    if (curr_ind == end_parity_bits - 1) {
      if (parity_bits[end_a_array]) {
        hamming_result += curr_koef;
      }
      total_xor ^= parity_bits[end_a_array];
      ++end_a_array;
      end_parity_bits *= 2;
    } else {
      if (i < number_size) {
        if (number_[i]) {
          hamming_result += curr_koef;
        }
      }
      total_xor ^= number_[i];
      ++i;
    }
    ++curr_ind;
    curr_koef *= 2;
    ++byte_position;
  }
  haf_result[haf_result_position] += (hamming_result + 128 * static_cast<uint8_t>(total_xor));
  of.write(reinterpret_cast<char*>(haf_result), (number_parity_bits
                                   + kBitsInByte * byte_block_ + kBitsInByte) / kBitsInByte);
}

std::vector<bool> Utility::DecodeHafCode() {
  uint32_t parity_bit = 1;
  uint32_t curr_ind = 0;
  uint32_t number_parity_bits = 0;
  std::vector<bool> decoded_number;
  bool total_xor = number_.back();
  while (curr_ind < number_.size() - 1) {
    if (curr_ind == parity_bit - 1) {
      parity_bit *= 2;
      ++number_parity_bits;
    } else {
      decoded_number.push_back(number_[curr_ind]);
    }
    ++curr_ind;
  }
  int32_t parity_bits[number_parity_bits] { 0 };
  FindParityBits(decoded_number, parity_bits, number_parity_bits);
  int32_t index_parity_bit = 1;
  int32_t index_error = 0;
  for (int32_t i = 1; i <= number_parity_bits; ++i) {
    if (parity_bits[i - 1] != number_[index_parity_bit - 1] && index_parity_bit != number_.size()) {
      index_error += i;
    }
    index_parity_bit *= 2;
  }
  if (index_error && total_xor == true) {
    number_[index_error - 1] = number_[index_error - 1] ^ 1;
  } else if (index_error) {
    std::cerr << "ERROR:\nThe archiver is too damaged.\nUnable to recover it.\n";
    std::exit(EXIT_FAILURE);
  }
  std::vector<bool> result;
  int mult = 1;
  for (int32_t i = 0; i < number_.size() - 1; ++i) {
    if (i != mult - 1) {
      result.push_back(number_[i]);
    } else {
      mult *= 2;
    }
  }
  result.push_back(false);

  return result;
}
