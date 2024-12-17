#include "lib/17_bit_class.h"

int main() {
  int f = 1231;
  ArrayBitsClass a = ArrayBitsClass::MakeArray(2, 3, 4);
  ArrayBitsClass b = ArrayBitsClass::MakeArray(2, 3, 4);
  ArrayBitsClass c = ArrayBitsClass::MakeArray(2, 3, 4);

  ArrayBitsClass d = ArrayBitsClass::MakeArray(2, 3, 4);

  //a[0] = b[0];
  a[1][2][3] = b[0][0][0];
  a[0][2][3] = 2232;
  a[0][1][1] = 239;

  d = a + b;
  
  //std::cout << GetIntValue(a[1][2][3], 3*4 + 2*4 + 3) << "\n";

  a[1][0][0] = 100;
  //a = b;
  c = a * 13;

  std::cout << c << "\n";

  //std::cout << "AND MORE\n";
  //std::cout << (a + b) << "\n";
  //std::cout << c << "\n";
  //b[1][2][3] = a[1][2][3];
  //b[2][2][2] = a[1][2][3];
  //c = a + b;
  //a[1][2][3] = 100;

  //b[1][2][3];

  //std::cout << a[1][2][3] << "\n";

  //c = a * 7;
  //c = a * 7;

  //std::cout << "PRINT\n";
  //std::cout << c << "\n";
  
  return EXIT_SUCCESS;
}
