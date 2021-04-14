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

/// The mask is problematic if:
/// - The highest bit is set and everything else are zeroes, for example: `0x80000000`.
/// - All the bits are set to one, for example: `0xFFFFFFFF`.
// bool isProblematicMask(StringRef HexNumber) {
//  // Check whether only the highest bit is set.
//   if (HexNumber[0] == '8' && HexNumber[1] == '0') {
//     // Consume the first non-zero.
//     HexNumber = HexNumber.drop_front();
//     // Consume all the left zeroes.
//     HexNumber = HexNumber.drop_while([](char C) { return C == '0'; });
//     return HexNumber.empty();
//   }

//   // Check whether all the bits are set to one by consuming all the 'F's.
//   HexNumber = HexNumber.drop_while([](char C) { return C == 'F' || C == 'f'; });
//   return HexNumber.empty();
// }

void NonPortableIntegerConstantCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(integerLiteral().bind("integer"), this);
}

void NonPortableIntegerConstantCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedInt = Result.Nodes.getNodeAs<IntegerLiteral>("integer");

  StringRef LiteralStr = Lexer::getSourceText(CharSourceRange::getTokenRange(MatchedInt->getSourceRange()),
  *Result.SourceManager, Result.Context->getLangOpts(), nullptr).lower();


  // StringRef MaskStr = Lexer::getSourceText(CharSourceRange::getTokenRange(MatchedInt->getSourceRange()),
  //     *Result.SourceManager, Result.Context->getLangOpts(), 0);


  llvm::APInt LiteralValue = MatchedInt->getValue();
  //  llvm::errs()<< "\n\n MaskStr: " << MaskStr << "\n str: " << LiteralStr << "\n APINT: " << LiteralValue << "\n\n";

  StringRef StrippedLiteral{LiteralStr};
  // llvm::errs()<< "\n\n" << StrippedLiteral<<"\n";
  // StrippedLiteral.consume_front("0b");
  // StrippedLiteral.consume_front("0x");
  // StrippedLiteral = StrippedLiteral.take_while(llvm::isHexDigit);
  // llvm::erase_value(StrippedLiteral, '\''); // Skip digit separators.

  // llvm::errs()<< StrippedLiteral << "\n\n";

  // const bool RepresentsZero = LiteralValue.isNullValue();
  // const bool HasLeadingZeroes = StrippedLiteral[0] == '0';
  // const bool IntegralPattern = HasLeadingZeroes && !RepresentsZero;

  if (StrippedLiteral.consume_front("0b") || StrippedLiteral.consume_front("0x")){
    const bool RepresentsZero = LiteralValue.isNullValue();
    const bool HasLeadingZeroes = StrippedLiteral[0] == '0';
    const bool IntegralPattern = HasLeadingZeroes && !RepresentsZero;

    if (IntegralPattern)
      diag(MatchedInt->getBeginLoc(),
          "integer is being used in a non-portable manner");
  }


  // if (MatchedInt) {
  //   StringRef MaskStr = Lexer::getSourceText(
  //       CharSourceRange::getTokenRange(MatchedInt->getSourceRange()),
  //       *Result.SourceManager, Result.Context->getLangOpts(), 0);

  //   MaskStr = StringRef(MaskStr.lower());
  //   if (!MaskStr.consume_front("0x"))
  //     return;

  //   MaskStr = MaskStr.take_while(llvm::isHexDigit);

  //   if (!isProblematicMask(MaskStr))
  //     return;

  //  diag(MatchedInt->getBeginLoc(),
  //       "integer is being used in a non-portable manner");
  // }
}

} // namespace portability
} // namespace tidy
} // namespace clang

