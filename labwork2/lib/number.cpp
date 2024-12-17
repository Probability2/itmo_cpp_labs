#include "number.h"
#include <cstring>

bool CompareNumbers(const int2023_t& lhs_copy, const int2023_t& rhs_copy, const int32_t ind_begin_left_number, const int32_t ind_begin_number_rhs) {
  bool is_new_number_greater = false;
  for (int16_t i = 0; i < kBuffSize - ind_begin_number_rhs; i++) {
    if (lhs_copy.a[i + ind_begin_left_number] < rhs_copy.a[ind_begin_number_rhs + i]) {
      return true;
    } else if (lhs_copy.a[i + ind_begin_left_number] > rhs_copy.a[ind_begin_number_rhs + i]) {
      return false;
    }
  }

  return is_new_number_greater;
}

int2023_t MultiplyNumber(int2023_t rhs_copy, const int32_t current_koef, const int32_t ind_begin_number_lhs) {
  uint8_t remainder_number = 0;
  for (int16_t i = kBuffSize - 1; i >= ind_begin_number_lhs - 1; i--) {
    uint8_t ai_copy = rhs_copy.a[i];
    rhs_copy.a[i] = (current_koef * rhs_copy.a[i] + remainder_number) % kNumberSystemBase;
    remainder_number = (current_koef * ai_copy + remainder_number) / kNumberSystemBase;
  }
  
  return rhs_copy;
}

int32_t BinSearch(const int2023_t rhs, const int2023_t lhs, const int32_t kf, const uint8_t ind_begin_number_lhs, const int32_t ind_begin_number_rhs) {
  int2023_t rhs_copy;
  uint8_t left_number = 0;
  int16_t right_number = kf + 1;
  uint8_t current_koef;
  bool is_new_number_greater;
  while (right_number > left_number + 1) {
    current_koef = (right_number + left_number) / 2;
    rhs_copy = MultiplyNumber(rhs, current_koef, ind_begin_number_lhs);
    if (rhs_copy.a[ind_begin_number_rhs - 1]) {
      is_new_number_greater = CompareNumbers(lhs, rhs_copy, ind_begin_number_lhs, ind_begin_number_rhs - 1);
    } else {
      is_new_number_greater = CompareNumbers(lhs, rhs_copy, ind_begin_number_lhs, ind_begin_number_rhs);
    }
    if (is_new_number_greater) {
      right_number = current_koef;
    } else {
      left_number = current_koef;
    }
  }

  return left_number;
}

void GetUnaryMinus(int2023_t& my_number) {
  for (uint8_t j = 0; j < kBuffSize; j++) {
    my_number.a[j] = ~my_number.a[j];
  }
  for (int16_t j = kBuffSize - 1; j >= 0; j--) {
    if (my_number.a[j] + 1 != kNumberSystemBase) {
      my_number.a[j]++;
      break;
    }
    my_number.a[j] = 0;
  }
}

int2023_t from_int(int32_t i) {
  int2023_t my_number;
  bool is_number_negative = (i < 0);
  i = std::abs(i);
  int16_t ind = kBuffSize - 1;
  while (ind >= 0) {
    my_number.a[ind] = i % kNumberSystemBase;
    i /= kNumberSystemBase;
    --ind;
  }
  if (is_number_negative) {
    GetUnaryMinus(my_number);
  }

  return my_number;
}

int2023_t from_string(const char* buff) {
  int2023_t my_number;
  uint16_t buff_size = strlen(buff);
  uint8_t curr_ind = kBuffSize - 1;
  uint16_t curr_number = 0;
  uint8_t str[kMaximalLengthOfString] {0};
  for (int32_t i = 0; i < buff_size; i++) {
    if (buff[i] != '-') {
      str[i] = buff[i] - '0';
    }
  }
  int32_t ind_str = 1;
  while (ind_str) {
    ind_str = 0;
    curr_number = 0;
    for (int16_t i = 0; i < buff_size; i++) {
      curr_number = curr_number * 10 + str[i];
      if (i >= kIndBeginReadNumber || curr_number >= kNumberSystemBase) {
        str[ind_str] = curr_number / kNumberSystemBase;
        curr_number = curr_number % kNumberSystemBase;
        ++ind_str;
      }
    }
    my_number.a[curr_ind] = curr_number;
    --curr_ind;
    buff_size = ind_str;
  }
  if (buff[0] == '-') {
    GetUnaryMinus(my_number);
  }

  return my_number;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t ans;
  uint8_t remainder_number = 0;
  for (int16_t j = kBuffSize - 1; j >= 0; j--) {
    if (lhs.a[j] + rhs.a[j] + remainder_number >= kNumberSystemBase) {
      ans.a[j] = lhs.a[j] + rhs.a[j] + remainder_number - kNumberSystemBase;
      remainder_number = 1;
    } else {
      ans.a[j] = lhs.a[j] + rhs.a[j] + remainder_number;
      remainder_number = 0;
    }
  }

  return ans;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t ans;
  int2023_t rhs_copy = rhs;
  GetUnaryMinus(rhs_copy);
  ans = operator+(lhs, rhs_copy);

  return ans;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t ans;
  uint8_t remainder_number = 0;
  uint8_t remainder_ans = 0;
  for (int16_t i = kBuffSize - 1; i >= 0; i--) {
    remainder_number = 0;
    remainder_ans = 0;
    if (rhs.a[i]) {
      for (int16_t j = kBuffSize - 1; j >= (kBuffSize - 1 - i); j--) {
        if ((ans.a[j - (kBuffSize - 1 - i)] + remainder_ans + (lhs.a[j] * rhs.a[i] + remainder_number) % kNumberSystemBase) >= kNumberSystemBase) {
          ans.a[j - (kBuffSize - 1 - i)] = (ans.a[j - (kBuffSize - 1 - i)] + remainder_ans + (lhs.a[j] * rhs.a[i] + remainder_number) % kNumberSystemBase) % kNumberSystemBase;
          remainder_ans = 1;
        } else {
          ans.a[j - (kBuffSize - 1 - i)] += (remainder_ans + lhs.a[j] * rhs.a[i] + remainder_number) % kNumberSystemBase;
          remainder_ans = 0;
        }
        remainder_number = (lhs.a[j] * rhs.a[i] + remainder_number) / kNumberSystemBase;
      }
    }
  }

  return ans;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
  int32_t ind_begin_number_lhs;
  int32_t ind_begin_number_rhs;
  int2023_t lhs_copy = lhs;
  int2023_t rhs_copy = rhs;
  int2023_t ans;
  bool is_lhs_negative = false;
  bool is_rhs_negative = false;
  if (lhs.a[0] == kNumberSystemBase - 1) {
    is_lhs_negative = true;
    GetUnaryMinus(lhs_copy);
  }
  if (rhs.a[0] == kNumberSystemBase - 1) {
    is_rhs_negative = true;
    GetUnaryMinus(rhs_copy);
  }
  bool is_lhs_equal_to_rhs = true;
  const int2023_t kLhs = lhs_copy;
  const int2023_t kRhs = rhs_copy;
  if (kLhs != kRhs) {
    is_lhs_equal_to_rhs = false;
  }
  for (uint8_t i = 0; i < kBuffSize; i++) {
    if (lhs_copy.a[i]) {
      ind_begin_number_lhs = i;
      break;
    }
  }
  for (uint8_t i = 0; i < kBuffSize; i++) {
    if (rhs_copy.a[i]) {
      ind_begin_number_rhs = i;
      break;
    }
  }
  uint8_t kf = 0;
  uint8_t curr_ind_rhs = 0;
  uint8_t number_end = 0;
  uint8_t koef_number = 0;
  if (is_lhs_equal_to_rhs) {
    ans.a[kBuffSize - 1] = 1;
    return ans;
  }
  for (int16_t i = ind_begin_number_lhs; i < kBuffSize; i++) {
    if (lhs_copy.a[i] > kRhs.a[ind_begin_number_rhs + curr_ind_rhs] && kf == 0) {
      if (kRhs.a[ind_begin_number_rhs + curr_ind_rhs]) {
        kf = lhs_copy.a[i] / kRhs.a[ind_begin_number_rhs + curr_ind_rhs];
      } else {
        kf = kNumberSystemBase - 1;
      }
      curr_ind_rhs++;
    }
    if ((kf && i - ind_begin_number_lhs + 1 == kBuffSize - ind_begin_number_rhs) || (i - ind_begin_number_lhs + 1 > kBuffSize - ind_begin_number_rhs) || (lhs_copy.a[i] == 0 && kf == 0)){
      if (i - ind_begin_number_lhs + 1 > kBuffSize - ind_begin_number_rhs) {
        kf = kNumberSystemBase - 1;
      }
      koef_number = BinSearch(kRhs, lhs_copy, kf, ind_begin_number_lhs, ind_begin_number_rhs);
      rhs_copy = MultiplyNumber(kRhs, koef_number, 1);
      for (int16_t j = i; j >= 0; j--) {
        if (lhs_copy.a[j] - rhs_copy.a[kBuffSize - (i - j) - 1] >= 0) {
          lhs_copy.a[j] -= rhs_copy.a[kBuffSize - (i - j) - 1];
        } else {
          lhs_copy.a[j] = lhs_copy.a[j] - rhs_copy.a[kBuffSize - (i - j) - 1] + kNumberSystemBase; 
          lhs_copy.a[j - 1]--;
        }
      }
      ans.a[kBuffSize - number_end - 1] = koef_number;
      curr_ind_rhs = 0;
      ind_begin_number_lhs = i + 1;
      for (int16_t j = 0; j <= i; j++) {
        if (lhs_copy.a[j]) {
          ind_begin_number_lhs = j;
          break;
        }
      }
      kf = lhs_copy.a[ind_begin_number_lhs] / kRhs.a[ind_begin_number_rhs];
      number_end++;
    }
  }
  for (int16_t i = kBuffSize - number_end; i < kBuffSize - (number_end / 2); i++) {
    uint8_t ans_i_copy = ans.a[i];
    ans.a[i] = ans.a[2 * kBuffSize - i - number_end - 1];
    ans.a[2 * kBuffSize - i - number_end - 1] = ans_i_copy;
  }
  bool is_ans_equal_to_zero = true;
  for (uint8_t i = 0; i < kBuffSize; i++) {
    if (ans.a[i]) {
      is_ans_equal_to_zero = false;
      break;
    }
  }
  if (is_lhs_negative != is_rhs_negative && !is_ans_equal_to_zero) {
    GetUnaryMinus(ans);
  }
  
  return ans;
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
  return std::equal(std::begin(lhs.a), std::end(lhs.a), std::begin(rhs.a));
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
  return !operator==(lhs, rhs);
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
  for (uint8_t i = 0; i < kBuffSize; i++) {
    stream << static_cast<int32_t> (value.a[i]) << " ";
  }

  return stream;
}