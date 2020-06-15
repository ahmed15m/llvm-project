.. title:: clang-tidy - cert-IntegerConstant

cert-IntegerConstant
====================

this check finds and fix Constant Integers that are used in a nonportable manner


CASE 1 :

--------------------------------------------
/* (Incorrect) Set all bits in mask to 1 */

const unsigned long mask = 0xFFFFFFFF;
 
unsigned long flipbits(unsigned long x) {
  return x ^ mask;
}
--------------------------------------------
                    |
                    |
                    v
--------------------------------------------                    
/* (Correct) Set all bits in mask to 1 */
const unsigned long mask = -1;
 
unsigned long flipbits(unsigned long x) {
  return x ^ mask;
}
--------------------------------------------





CASE 2 :

--------------------------------------------
const unsigned long mask = 0x80000000;
unsigned long x;
 
/* Initialize x */
 
x |= mask;
--------------------------------------------
                    |
                    |
                    v
---------------------------------------------
const unsigned long mask = ~(ULONG_MAX >> 1);
unsigned long x;
 
/* Initialize x */
 
x |= mask;
---------------------------------------------