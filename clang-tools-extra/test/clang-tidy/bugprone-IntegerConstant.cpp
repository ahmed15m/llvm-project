// RUN: %check_clang_tidy %s bugprone-IntegerConstant %t
// #define ULONG_MAX 0xFFFFFFFFUL
#include <limits.h>
#include <stdio.h>

const unsigned long mask = 0xFFFFFFFF;
// CHECK-MESSAGES: :[[@LINE-1]]:28: warning: integer is being used in a non-portable manner 

const unsigned long mask1 = 0xffffffffUL;
// CHECK-MESSAGES: :[[@LINE-1]]:29: warning: integer is being used in a non-portable manner

const unsigned long mask2 = 0x80000000;
// CHECK-MESSAGES: :[[@LINE-1]]:29: warning: integer is being used in a non-portable manner 

int stuff() {
  int mask3 = 0xff;
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: integer is being used in a non-portable manner
  int value = 0x55;
  return mask3;
}

const unsigned long g = 0xC0;
// no warnings

const unsigned long x = -1;
// no warnings

const unsigned long y = ~(ULONG_MAX >> 1);
// no warnings

const unsigned long z = 0x11111111;
// no warnings
