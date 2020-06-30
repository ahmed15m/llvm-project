// RUN: %check_clang_tidy %s bugprone-IntegerConstant %t

// FIXME: Add something that triggers the check here.
void f();
//

// FIXME: Verify the applied fix.
//   * Make the CHECK patterns specific enough and try to make verified lines
//     unique to avoid incorrect matches.
//   * Use {{}} for regular expressions.
//

// FIXME: Add something that doesn't trigger the check here.
void awesome_f2();
