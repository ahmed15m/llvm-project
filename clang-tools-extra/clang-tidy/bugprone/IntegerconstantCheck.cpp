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

/// The mask is problematic if:
/// - The highest bit is set and everything else are zeroes.
/// - (didn't understand the rest of the cases yet!)
bool isProblematicMask(StringRef HexNumber) {

  // if (HexNumber.size() != 4 && HexNumber.size() != 8 &&
  //     HexNumber.size() != 16)
  //   return false;

  unsigned int FCount = 0;
  unsigned int ZeroCount = 0;

  for (unsigned int i = HexNumber.size() - 1; i > 0; i--) {
    if (HexNumber[i] == 'f')
      FCount++;
    else
      break;
  }
  if (FCount >= 2)
    return true;

  for (unsigned int i = HexNumber.size() - 1; i > 0; i--) {
    if (HexNumber[i] == '0')
      ZeroCount++;
    else
      break;
  }
  if (ZeroCount >= 2)
    return true;

  return false;
}

void IntegerconstantCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(integerLiteral().bind("int"), this);
}

void IntegerconstantCheck::check(const MatchFinder::MatchResult &Result) {

  const auto *MatchedInt = Result.Nodes.getNodeAs<IntegerLiteral>("int");

  if (MatchedInt) {

    StringRef MaskStr = Lexer::getSourceText(
        CharSourceRange::getTokenRange(MatchedInt->getSourceRange()),
        *Result.SourceManager, Result.Context->getLangOpts(), 0);

    MaskStr = StringRef(MaskStr.lower());
    if (!MaskStr.consume_front("0x"))
      return;

    MaskStr = MaskStr.take_while(llvm::isHexDigit);

    if (MaskStr.empty())
      return;

    if (!isProblematicMask(MaskStr))
      return;

    diag(MatchedInt->getBeginLoc(),
         "integer is being used in a non-portable manner ");
  }
}

} // namespace bugprone
} // namespace tidy
} // namespace clang