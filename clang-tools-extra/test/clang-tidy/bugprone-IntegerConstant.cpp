// RUN: %check_clang_tidy %s bugprone-IntegerConstant %t
// #define ULONG_MAX 0xFFFFFFFFUL
#include <limits.h>
#include <stdio.h>

const unsigned long mask = 0xFFFFFFFF;
// CHECK-MESSAGES: :[[@LINE-1]]:28: warning: integer is used in a nonportable

const unsigned long mask2 = 0x80000000;
// CHECK-MESSAGES: :[[@LINE-1]]:29: warning: integer is used in a nonportable

int stuff() {
  int mask3 = 0x0f;
  // CaHECK-MESSAGES: :[[@LINE-1]]:15: warning: integer is used in a nonportable
  int value = 0x55;
  return mask3;
}

const unsigned long x = -1;
// no warnings

const unsigned long y = ~(ULONG_MAX >> 1);
// no warnings
