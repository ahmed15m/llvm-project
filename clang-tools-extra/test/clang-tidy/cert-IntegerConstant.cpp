// RUN: %check_clang_tidy %s cert-IntegerConstant %t

const unsigned long mask = 0xFFFFFFFF; 
// CHECK-MESSAGES: :[[@LINE-1]]:28: warning: integer is used in a nonportable manner [cert-IntegerConstant]
 


const unsigned long mask2 = 0x80000000;

// CHECK-MESSAGES: :[[@LINE-2]]:29: warning: integer is used in a nonportable manner [cert-IntegerConstant]


// const unsigned long x = -1;
// // no warnings

// const unsigned long y = ~(ULONG_MAX >> 1);
// // no warnings

unsigned long flipbits(unsigned long x) {
  return x ^ mask;
}

