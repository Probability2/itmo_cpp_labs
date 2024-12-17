#pragma once
#include <cinttypes>
#include <iostream>
#include <cstdint>

const uint8_t kBuffSize = 253;
const int32_t kNumberSystemBase = 256;
const int32_t kMaximalLengthOfString = 610;
const uint8_t kIndBeginReadNumber = 3;

struct int2023_t {
    uint8_t a[kBuffSize] {0};
};

static_assert(sizeof(int2023_t) <= kBuffSize, "Size of int2023_t must be no higher than 253 bytes");

int2023_t from_int(int32_t i);

int2023_t from_string(const char* buff);

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs);

bool operator==(const int2023_t& lhs, const int2023_t& rhs);

bool operator!=(const int2023_t& lhs, const int2023_t& rhs);

std::ostream& operator<<(std::ostream& stream, const int2023_t& value);
