.. title:: clang-tidy - bugprone-IntegerConstant

bugprone-IntegerConstant
========================

Finds masks that are being used in a nonportable manner.

.. code-block:

    const unsigned long mask = 0xFFFFFFFF;
    
    unsigned long flipbits(unsigned long x) {
      return x ^ mask;
    }