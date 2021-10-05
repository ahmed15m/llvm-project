// RUN: %check_clang_tidy %s -std=c++17-or-later portability-non-portable-integer-constant %t

void regular() {
  // no-warnings
  0;
  00;
  0x0;
  0x00;
  0b0;
  0b00;
  0b0'0'0;

  -1;
  -0X1;

  127;
  0x7'f;

  -128;
  -0x80;

  256;
  0X100;

  42;
  0x2A;

  180079837;
  0xabbccdd;
}

// INT_MIN, INT_MAX, UINT_MAX, UINT_MAX-1
void limits_int() {
  -214748'3'648;  // no specific stratigy to catch such max no. (isMaxSignedValue() => True)
  // --CHECK-MESSAGES: :[[@LINE-1]]:3: warning: integer is being used in a non-portable manner

  -0x80'00'00'00;
  // CHECK-MESSAGES: :[[@LINE-1]]:4: warning: integer is being used in a non-portable manner

  -020000000000;  // still don't have a specific solution for octals 
  // --CHECK-MESSAGES: :[[@LINE-1]]:3: warning: integer is being used in a non-portable manner

  -0b10000000000000000000000000000000;
  // CHECK-MESSAGES: :[[@LINE-1]]:4: warning: integer is being used in a non-portable manner



  21474'83647;   // recognized as long
  // --CHECK-MESSAGES: :[[@LINE-1]]:3: warning: integer is being used in a non-portable manner

  0x7FFFFFFF;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: integer is being used in a non-portable manner

  017777777777;  // still don't have a specific solution for octals 
  // --CHECK-MESSAGES: :[[@LINE-1]]:3: warning: integer is being used in a non-portable manner

  0b1111111111111111111111111111111; // those are 31 bits, so not recognized as full-pattern
  // --CHECK-MESSAGES: :[[@LINE-1]]:3: warning: integer is being used in a non-portable manner



  429'4'96'7295u;  // no specific stratigy to catch such max no. (isMaxValue() => True)
  // --CHECK-MESSAGES: :[[@LINE-1]]:3: warning: integer is being used in a non-portable manner

  0xFFFFFFFF;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: integer is being used in a non-portable manner

  037777777777;  // still don't have a specific solution for octals or max
  // --CHECK-MESSAGES: :[[@LINE-1]]:3: warning: integer is being used in a non-portable manner

  0B11111111111111111111111111111111;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: integer is being used in a non-portable manner



  4294967294;  // recognized as long
  // --CHECK-MESSAGES: :[[@LINE-1]]:3: warning: integer is being used in a non-portable manner

  0XFFFFFFFE;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: integer is being used in a non-portable manner

  037777777776;  // still don't have a specific solution for octals 
  // --CHECK-MESSAGES: :[[@LINE-1]]:3: warning: integer is being used in a non-portable manner

  0B11111111111111111111111111111110;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: integer is being used in a non-portable manner
}

// no specific solution for maxs and limits except using (isMaxValue(), isMinValue(), isMaxSignedValue(), isMinSignedValue())
// LLONG_MIN, LLONG_MAX, ULLONG_MAX, ULLONG_MAX-1
// void limits_llong() {
//   -9'22337203'6854775808LL;
//   0x8000000000000000;
//   01000000000000000000000;
//   0b1000000000000000000000000000000000000000000000000000000000000000;

//   9223372036854775807;
//   0x7FFFFFFFFFFFFFFF;
//   0777777777777777777777;
//   0b111111111111111111111111111111111111111111111111111111111111111;

//   18446744073709551615llU;
//   0xFFFFFFFFFFFFFFFF;
//   01777777777777777777777;
//   0b1111111111111111111111111111111111111111111111111111111111111111;

//   18446744073709551614llU;
//   0xFFFFFFFFFFFFFFFe;
//   01777777777777777777776;
//   0b1111111111111111111111111111111111111111111111111111111111111110;
// }

void full_patterns() {
  0x7F'FF'FF'FF;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: integer is being used in a non-portable manner
  -0x80'00'00'00;
  // CHECK-MESSAGES: :[[@LINE-1]]:4: warning: integer is being used in a non-portable manner
  0xFFFFFFFFu;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: integer is being used in a non-portable manner
  0xFF'FF'FF'Fe;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: integer is being used in a non-portable manner


  0x7F'FF'FFFFFFFFFFFF;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: integer is being used in a non-portable manner
  -0x80'0000000000000'0LL;
  // CHECK-MESSAGES: :[[@LINE-1]]:4: warning: integer is being used in a non-portable manner
  0XFFffFFFffFFFFFFFllU;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: integer is being used in a non-portable manner
}
