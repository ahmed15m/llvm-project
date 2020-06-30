//===--- IntegerconstantCheck.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "IntegerconstantCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bugprone {

bool is_mask(std::string s) {
  bool is_hex = s.compare(0, 2, "0x") == 0 && s.size() > 2;
  if (is_hex) {

    std::string hex_number =
        s.substr(2, s.find_first_not_of("0x123456789abcdefABCDEF") - 2);

    if (hex_number.size() != 4 && hex_number.size() != 8 &&
        hex_number.size() != 16)
      return false;

    unsigned int count = 0;

    for (unsigned int i = hex_number.size() - 1; i > 0; i--) {
      if (hex_number[i] == 'F' || hex_number[i] == 'f')
        count++;
      else
        break;

      if (count >= 2)
        return true;
    }

    count = 0;

    for (unsigned int i = hex_number.size() - 1; i > 0; i--) {
      if (hex_number[i] == '0')
        count++;
      else
        break;

      if (count >= 2)
        return true;
    }
  }

  return false;
}

void IntegerconstantCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(integerLiteral().bind("int"), this);
}

void IntegerconstantCheck::check(const MatchFinder::MatchResult &Result) {

  const auto *Matched_Int = Result.Nodes.getNodeAs<Stmt>("int");

  StringRef MaskStr = Lexer::getSourceText(
      CharSourceRange::getTokenRange(Matched_Int->getSourceRange()),
      *Result.SourceManager, Result.Context->getLangOpts(), 0);

  if (Matched_Int && is_mask(MaskStr.str())) {

    diag(Matched_Int->getBeginLoc(),
         "integer is used in a nonportable manner ");
  }
}

} // namespace bugprone
} // namespace tidy
} // namespace clang
