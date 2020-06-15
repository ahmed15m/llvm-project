//===--- IntegerconstantCheck.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "IntegerconstantCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/Decl.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace cert {

// static StringRef exprToStr(const Expr *E,
//   const MatchFinder::MatchResult &Result) {
//   return Lexer::getSourceText(
//       CharSourceRange::getTokenRange(E->getSourceRange()),
//       *Rescddult.SourceManager, Result.Context->getLangOpts(), 0);
// }

void IntegerconstantCheck::registerMatchers(MatchFinder *Finder) {
    // Finder->addMatcher(integerLiteral(hasAncestor(varDecl( has(isConstQualified())))).bind("const"), this);
    Finder->addMatcher(integerLiteral().bind("int"), this);
}

void IntegerconstantCheck::check(const MatchFinder::MatchResult &Result) {

  const auto *Matched_Int = Result.Nodes.getNodeAs<Stmt>("int");

  StringRef MaskStr = Lexer::getSourceText(
      CharSourceRange::getTokenRange(Matched_Int->getSourceRange()),
      *Result.SourceManager, Result.Context->getLangOpts(), 0);

  llvm::errs() << " \n\n str :  " << MaskStr << "\n\n";

  if(Matched_Int){

    diag(Matched_Int->getBeginLoc() , "integer is used in a nonportable manner ");
   
  }

}

} // namespace cert
} // namespace tidy
} // namespace clang
