/*
    This is an expression which captures every IF/ELSE statement from the source code.

Copyright (C) 2025 Adam Byczyński.

This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.
    If not, see <https://www.gnu.org/licenses/>.  */


#ifndef BRANCHINGIFEXPRAST_H
#define BRANCHINGIFEXPRAST_H

#include <memory>

#include "../AbstractExprAST.h"

#include "../../../BasicComponents/Token.h"


class BranchingIfExprAST : public virtual AbstractExprAST {
public:
    BranchingIfExprAST(
        int id,
        std::unique_ptr<AbstractExprAST> condition,
        std::unique_ptr<AbstractExprAST> thenBranchBlock,
        std::unique_ptr<AbstractExprAST> elseBranchBlock,
        Token &conditionRefTok,
        Token &thenBranchRefTok,
        Token &elseBranchRefTok
    )
        : id(id),
          condition(std::move(condition)),
          thenBranchBlock(std::move(thenBranchBlock)),
          elseBranchBlock(std::move(elseBranchBlock)),
          conditionRefTok(conditionRefTok),
          thenBranchRefTok(thenBranchRefTok),
          elseBranchRefTok(elseBranchRefTok) {
    }

    int getID() const { return id; }

    const std::unique_ptr<AbstractExprAST> &getCondition() {
        return condition;
    }

    const std::unique_ptr<AbstractExprAST> &getThenBranchExpr() {
        return thenBranchBlock;
    }

    const std::unique_ptr<AbstractExprAST> &getElseBranchExpr() {
        return elseBranchBlock;
    }

    Token &getConditionRefTok() {
        return conditionRefTok;
    }

    Token &getThenBranchRefTok() {
        return thenBranchRefTok;
    }

    Token &getElseBranchRefTok() {
        return elseBranchRefTok;
    }

private:
    int id;

    std::unique_ptr<AbstractExprAST> condition;
    std::unique_ptr<AbstractExprAST> thenBranchBlock;
    std::unique_ptr<AbstractExprAST> elseBranchBlock;

    Token &conditionRefTok;
    Token &thenBranchRefTok;
    Token &elseBranchRefTok;

    ExpressionType expressionType = ExpressionType::BranchingIfExpr;
};


#endif //BRANCHINGIFEXPRAST_H
