//===--- NonPortableIntegerConstantCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "NonPortableIntegerConstantCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace portability {

void NonPortableIntegerConstantCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(integerLiteral().bind("integer"), this);
}

void NonPortableIntegerConstantCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *MatchedInt = Result.Nodes.getNodeAs<IntegerLiteral>("integer");

  StringRef LiteralStr =
      Lexer::getSourceText(
          CharSourceRange::getTokenRange(MatchedInt->getSourceRange()),
          *Result.SourceManager, Result.Context->getLangOpts(), nullptr)
          .lower();

  llvm::APInt LiteralValue = MatchedInt->getValue();

  StringRef StrippedLiteral{LiteralStr};
  StrippedLiteral.consume_front("0");
  StrippedLiteral.consume_front("b");
  StrippedLiteral.consume_front("x");
  StrippedLiteral = StrippedLiteral.take_while(llvm::isHexDigit);

  llvm::errs() << "\n str: " << LiteralStr << "\n APINT: " << LiteralValue
               << "  stripped:" << StrippedLiteral
               << "  isAllOnesValue:" << LiteralValue.isAllOnesValue()
               << "  isMask:" << LiteralValue.isMask() << "\n\n";

  if (StrippedLiteral.empty())
    return;

  const bool RepresentsZero = LiteralValue.isNullValue();
  const bool HasLeadingZeroes = StrippedLiteral[0] == '0';
  const bool AllBitsAreSet = LiteralValue.isAllOnesValue();
  const bool IntegralPattern =
      (HasLeadingZeroes && !RepresentsZero) || AllBitsAreSet;

  if (IntegralPattern)
    diag(MatchedInt->getBeginLoc(),
         "integer is being used in a non-portable manner");
}

} // namespace portability
} // namespace tidy
} // namespace clang
