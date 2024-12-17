#include <lib/17_bit_class.h>
#include <gtest/gtest.h>

#include <sstream>

TEST(Array17bitSuiteTest, SetValueTest) {
	ArrayBitsClass a = ArrayBitsClass::MakeArray(2, 3, 4);
	a[1][2][3] = 7987;
  a[0][2][3] = 2232;
  a[0][0][0] = 239;
	a[0][0][1] = 102342;
  a[0][0][2] = 131071;
  a[0][0][3] = 0;
	a[0][1][2] = 1241;
  a[0][3][3] = 67523;
  ASSERT_EQ(7987, a[1][2][3]);
  ASSERT_EQ(2232, a[0][2][3]);
  ASSERT_EQ(239, a[0][0][0]);
	ASSERT_EQ(102342, a[0][0][1]);
	ASSERT_EQ(131071, a[0][0][2]);
	ASSERT_EQ(0, a[0][0][3]);
	ASSERT_EQ(1241, a[0][1][2]);
	ASSERT_EQ(67523, a[0][3][3]);
}

TEST(Array17bitSuiteTest, AdditionTest) {
	ArrayBitsClass a = ArrayBitsClass::MakeArray(2, 3, 4);
	ArrayBitsClass b = ArrayBitsClass::MakeArray(2, 3, 4);
	ArrayBitsClass c = ArrayBitsClass::MakeArray(2, 3, 4);
	ArrayBitsClass d = ArrayBitsClass::MakeArray(3, 3, 4);
	a[1][2][3] = 7987;
  a[0][2][3] = 2232;
  a[0][0][0] = 239;
	a[0][0][1] = 102342;
  b[1][2][3] = 13214;
  b[0][2][3] = 42124;
  b[0][0][0] = 81219;
	b[0][0][1] = 1765;
	c = a + b;
  ASSERT_EQ(21201, c[1][2][3]);
  ASSERT_EQ(44356, c[0][2][3]);
  ASSERT_EQ(81458, c[0][0][0]);
	ASSERT_EQ(104107, c[0][0][1]);
}

TEST(Array17bitSuiteTest, SubtractionTest) {
	ArrayBitsClass a = ArrayBitsClass::MakeArray(2, 3, 4);
	ArrayBitsClass b = ArrayBitsClass::MakeArray(2, 3, 4);
	ArrayBitsClass c = ArrayBitsClass::MakeArray(2, 3, 4);
	ArrayBitsClass d = ArrayBitsClass::MakeArray(3, 3, 4);
	a[1][2][3] = 67987;
  a[0][2][3] = 88232;
  a[0][0][0] = 239;
	a[0][0][1] = 102342;
  b[1][2][3] = 13214;
  b[0][2][3] = 72124;
  b[0][0][0] = 219;
	b[0][0][1] = 1765;
	c = a - b;
  ASSERT_EQ(54773, c[1][2][3]);
  ASSERT_EQ(16108, c[0][2][3]);
  ASSERT_EQ(20, c[0][0][0]);
	ASSERT_EQ(100577, c[0][0][1]);
}

TEST(Array17bitSuiteTest, MultiplyValueTest) {
	ArrayBitsClass a = ArrayBitsClass::MakeArray(2, 3, 4);
	ArrayBitsClass c = ArrayBitsClass::MakeArray(2, 3, 4);
	a[1][2][3] = 57987;
  a[0][2][3] = 18232;
  a[0][0][0] = 23;
	a[0][0][1] = 64000;
	c = a * 2;
  ASSERT_EQ(115974, c[1][2][3]);
  ASSERT_EQ(36464, c[0][2][3]);
  ASSERT_EQ(46, c[0][0][0]);
	ASSERT_EQ(128000, c[0][0][1]);
}

TEST(Array17bitSuiteTest, MultiplyClassesTest) {
	ArrayBitsClass a = ArrayBitsClass::MakeArray(2, 2, 2);
	ArrayBitsClass b = ArrayBitsClass::MakeArray(2, 2, 2);
	ArrayBitsClass c = ArrayBitsClass::MakeArray(2, 2, 2);
	ArrayBitsClass d = ArrayBitsClass::MakeArray(3, 2, 2);

	a[0][0][0] = 57987;
  a[0][0][1] = 18232;
  a[0][1][0] = 239;
	a[0][1][1] = 102124;
  a[1][0][0] = 5122;
  a[1][0][1] = 12412;
	a[1][1][0] = 15999;
	a[1][1][1] = 131071;

	b[0][0][0] = 2;
  b[0][0][1] = 0;
  b[0][1][0] = 125;
	b[0][1][1] = 1;
  b[1][0][0] = 16;
  b[1][0][1] = 9;
	b[1][1][0] = 7;
	b[1][1][1] = 1;

	c = a * b;
  ASSERT_EQ(115974, c[0][0][0]);
  ASSERT_EQ(0, c[0][0][1]);
	ASSERT_EQ(29875, c[0][1][0]);
	ASSERT_EQ(102124, c[0][1][1]);
	ASSERT_EQ(81952, c[1][0][0]);
	ASSERT_EQ(111708, c[1][0][1]);
	ASSERT_EQ(111993, c[1][1][0]);
	ASSERT_EQ(131071, c[1][1][1]);
}

TEST(Array17bitSuiteTest, AllPossibleNumbersTest) {
	ArrayBitsClass a = ArrayBitsClass::MakeArray(1, 1, 131071);
	for (int i = 0; i < 131071; ++i) {
		a[0][0][i] = i;
	}
	for (int i = 0; i < 131071; ++i) {
		ASSERT_EQ(i, a[0][0][i]);
	}
}

TEST(Array17bitSuiteTest, AddValuesTest) {
	ArrayBitsClass a = ArrayBitsClass::MakeArray(50, 50, 50);
	ArrayBitsClass b = ArrayBitsClass::MakeArray(50, 50, 50);
	ArrayBitsClass с = ArrayBitsClass::MakeArray(50, 50, 50);
	for (int i = 0; i < 50; ++i) {
		for (int j = 0; j < 50; ++j) {
			for (int k = 0; k < 50; ++k) {
				a[i][j][k] = i + j + k;
				b[i][j][k] = 2 * i + j + 3 * k;
			}
		}
	}
	с = a + b;
	for (int i = 0; i < 50; ++i) {
		for (int j = 0; j < 50; ++j) {
			for (int k = 0; k < 50; ++k) {
				ASSERT_EQ(3 * i + 2 * j + 4 * k, с[i][j][k]);
			}
		}
	}
}

TEST(Array17bitSuiteTest, SubstractValuesTest) {
	ArrayBitsClass a = ArrayBitsClass::MakeArray(50, 50, 50);
	ArrayBitsClass b = ArrayBitsClass::MakeArray(50, 50, 50);
	ArrayBitsClass с = ArrayBitsClass::MakeArray(50, 50, 50);
	for (int i = 0; i < 50; ++i) {
		for (int j = 0; j < 50; ++j) {
			for (int k = 0; k < 50; ++k) {
				a[i][j][k] = 5 * i + 5 * j + 4 * k;
				b[i][j][k] = 2 * i + j + 3 * k;
			}
		}
	}
	с = a - b;
	for (int i = 0; i < 50; ++i) {
		for (int j = 0; j < 50; ++j) {
			for (int k = 0; k < 50; ++k) {
				ASSERT_EQ(3 * i + 4 * j + k, с[i][j][k]);
			}
		}
	}
}

TEST(Array17bitSuiteTest, MultiplyWholeClassTest) {
	ArrayBitsClass a = ArrayBitsClass::MakeArray(50, 50, 50);
	ArrayBitsClass b = ArrayBitsClass::MakeArray(50, 50, 50);
	ArrayBitsClass с = ArrayBitsClass::MakeArray(50, 50, 50);
	for (int i = 0; i < 50; ++i) {
		for (int j = 0; j < 50; ++j) {
			for (int k = 0; k < 50; ++k) {
				a[i][j][k] = i + 2 * j + 3 * k;
				b[i][j][k] = 2 * i + 3 * j + 4 * k;
			}
		}
	}
	с = a * b;
	for (int i = 0; i < 50; ++i) {
		for (int j = 0; j < 50; ++j) {
			for (int k = 0; k < 50; ++k) {
				ASSERT_EQ((i + 2 * j + 3 * k) * (2 * i + 3 * j + 4 * k), с[i][j][k]);
			}
		}
	}
}

TEST(Array17bitSuiteTest, MultiplyValueWholeClassTest) {
	ArrayBitsClass a = ArrayBitsClass::MakeArray(50, 50, 50);
	ArrayBitsClass b = ArrayBitsClass::MakeArray(50, 50, 50);
	ArrayBitsClass с = ArrayBitsClass::MakeArray(50, 50, 50);
	for (int i = 0; i < 50; ++i) {
		for (int j = 0; j < 50; ++j) {
			for (int k = 0; k < 50; ++k) {
				a[i][j][k] = i + 2 * j + 3 * k;
			}
		}
	}
	с = a * 69;
	for (int i = 0; i < 50; ++i) {
		for (int j = 0; j < 50; ++j) {
			for (int k = 0; k < 50; ++k) {
				ASSERT_EQ(69 * i + 138 * j + 207 * k, с[i][j][k]);
			}
		}
	}
}

TEST(Array17bitSuiteTest, InputTest) {
	std::stringstream stream;
	ArrayBitsClass a = ArrayBitsClass::MakeArray(20, 15, 10);
	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 15; ++j) {
			for (int k = 0; k < 10; ++k) {
				stream << (i * j * k) << " ";
			}
		}
	}
	stream >> a;
	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 15; ++j) {
			for (int k = 0; k < 10; ++k) {
				ASSERT_EQ(i * j * k, a[i][j][k]);
			}
		}
	}
}

TEST(Array17bitSuiteTest, OutputTest) {
	std::stringstream stream_expected;
	std::stringstream stream_class;
	ArrayBitsClass a = ArrayBitsClass::MakeArray(10, 15, 20);
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 15; ++j) {
			for (int k = 0; k < 20; ++k) {
				a[i][j][k] = i + j + k;
				stream_expected << (i + j + k) << " ";
			}
			stream_expected << "\n";
		}
		stream_expected << "||\n";
	}
	stream_class << a;
	ASSERT_EQ(stream_expected.str(), stream_class.str());
}
