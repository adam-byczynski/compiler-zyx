/*
    This is an expression which captures every While loop expression from source code.

Copyright (C) 2026 Adam Byczyński.

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


#ifndef LOOPWHILEEXPRAST_H
#define LOOPWHILEEXPRAST_H

#include <memory>

#include "../AbstractExprAST.h"

#include "../../../basicComponents/Token.h"


class LoopWhileExprAST : public virtual AbstractExprAST {
public:
    LoopWhileExprAST(
        int id,
        std::unique_ptr<AbstractExprAST> condition,
        std::unique_ptr<AbstractExprAST> body,
        Token &conditionRefTok,
        Token &bodyRefTok)
        : id(id),
          condition(std::move(condition)),
          body(std::move(body)),
          conditionRefTok(conditionRefTok),
          bodyRefTok(bodyRefTok) {
    }

    int getID() const {return id;}

    const std::unique_ptr<AbstractExprAST> &getCondition() {
        return condition;
    }

    const std::unique_ptr<AbstractExprAST> &getBody() {
        return body;
    }

    Token &getConditionRefTok() {
        return conditionRefTok;
    }

    Token &getBodyRefTok() {
        return bodyRefTok;
    }

private:
    int id;

    std::unique_ptr<AbstractExprAST> condition;
    std::unique_ptr<AbstractExprAST> body;

    Token &conditionRefTok;
    Token &bodyRefTok;

    ExpressionType expressionType = ExpressionType::LoopWhileExpr;
};


#endif //LOOPWHILEEXPRAST_H
