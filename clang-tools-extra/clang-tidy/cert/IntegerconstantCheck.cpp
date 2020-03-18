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

void IntegerconstantCheck::registerMatchers(MatchFinder *Finder) {
    Finder->addMatcher(integerLiteral(hasAncestor(varDecl( has(isConstQualified()))) ,equals(0xFFFFFFFF)).bind("infinity"), this);
    Finder->addMatcher(integerLiteral(hasAncestor(varDecl( has(isConstQualified()))) ,equals(0x80000000)).bind("max"), this);
}

void IntegerconstantCheck::check(const MatchFinder::MatchResult &Result) {

  const auto *Matched_Int_inifinity = Result.Nodes.getNodeAs<Stmt>("infinity");
  const auto *Matched_Int_Max = Result.Nodes.getNodeAs<Stmt>("max");

  if(Matched_Int_inifinity){
    diag(Matched_Int_inifinity->getBeginLoc() , "hexadecimal integer constant is used in a nonportable manner ");
    diag(Matched_Int_inifinity->getEndLoc(), "replace by -1 ", DiagnosticIDs::Note)
        << FixItHint::CreateReplacement(Matched_Int_inifinity->getBeginLoc(), " -1 ");
  }

  if(Matched_Int_Max){
    diag(Matched_Int_Max->getBeginLoc() , "hexadecimal integer constant is used in a nonportable manner ");
    diag(Matched_Int_Max->getEndLoc(), "replace by ~(ULONG_MAX >> 1) ", DiagnosticIDs::Note)
        << FixItHint::CreateReplacement(Matched_Int_Max->getBeginLoc(), " ~(ULONG_MAX >> 1) ");
  }

}

} // namespace cert
} // namespace tidy
} // namespace clang
