// RUN: %check_clang_tidy %s portability-non-portable-integer-constant %t

#define ULONG_MAX 0xFFFFFFFFUL
// #include <limits.h>
// #include <stdio.h>

const unsigned long mask = 0xFFFFFFFF;
// CHECK-MESSAGES: :[[@LINE-1]]:28: warning: integer is being used in a non-portable manner

const unsigned long mask1 = 0xffffffffUL; // not matching it as not all bits are set for UL, but not sure?
// --CHECK-MESSAGES: :[[@LINE-1]]:29: warning: integer is being used in a non-portable manner

const unsigned long mask2 = 0x80000000;
// --CHECK-MESSAGES: :[[@LINE-1]]:29: warning: integer is being used in a non-portable manner

int stuff() {
  int mask3 = 0xff;
  // --CHECK-MESSAGES: :[[@LINE-1]]:15: warning: integer is being used in a non-portable manner
  // int value = 0x55;
  return mask3;
}


void test(){
  int s32_bin = 0b01; // warning
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: integer is being used in a non-portable manner
  int s32_oct = 001;  // warning
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: integer is being used in a non-portable manner
  // no-decimal-form
  int s32_hex = 0x01; // warning
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: integer is being used in a non-portable manner

  s32_bin = 0b0; // no-warning
  s32_oct = 00;  // no-warning
  int s32 = 0;   // no-warning
  s32_hex = 0x0; // no-warning

  // s32 = 0b1; // no-warning
  // s32 = 01;  // no-warning
  // s32 = 1;   // no-warning
  // s32 = 0x1; // no-warning
}
const unsigned long g = 0xC0;
// no warnings

const unsigned long x = -1;
// no warnings

const unsigned long y = ~(ULONG_MAX >> 1);
// no warnings

const unsigned long z = 0x11111111;
// no warnings
