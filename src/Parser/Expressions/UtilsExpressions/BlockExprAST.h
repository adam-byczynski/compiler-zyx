/*
    This is an expression which captures the indented block of code which
    follows some scope opening expression like if/else or while.


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


#ifndef BLOCKEXPRAST_H
#define BLOCKEXPRAST_H

#include <memory>

#include "../AbstractExprAST.h"
#include "../../../BasicComponents/Token.h"
#include <vector>


class BlockExprAST : public virtual AbstractExprAST {
public:
    BlockExprAST(
        int id,
        std::vector<std::unique_ptr<AbstractExprAST> > expressions,
        Token &blockExpressionsRefTok,
        bool newScope = true)
        : id(id),
          blockExpressions(std::move(expressions)),
          blockExprsRefTok(blockExpressionsRefTok),
          createsNewScope(newScope) {
    }

    int getID() const { return id; }
    const std::vector<std::unique_ptr<AbstractExprAST> > &getExpressions() const { return blockExpressions; };
    bool hasNewScope() const { return createsNewScope; };

    Token &getBlockExprsRefTok() {
        return blockExprsRefTok;
    }

private:
    int id;

    std::vector<std::unique_ptr<AbstractExprAST> > blockExpressions;
    Token &blockExprsRefTok;
    bool createsNewScope;

    ExpressionType expressionType = ExpressionType::BlockExpr;
};

#endif //BLOCKEXPRAST_H
