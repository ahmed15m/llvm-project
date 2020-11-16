//===--- NonPortableIntegerConstantCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_PORTABILITY_NON_PORTABLE_INTEGER_CONSTANT_CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_PORTABILITY_NON_PORTABLE_INTEGER_CONSTANT_CHECK_H

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace portability {

/// Finds masks that are being used in a non-portable manner
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/portability-non_portable_integer_constant.html
class NonPortableIntegerConstantCheck : public ClangTidyCheck {
public:
  NonPortableIntegerConstantCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace portability
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_PORTABILITY_NON_PORTABLE_INTEGER_CONSTANT_CHECK_H
