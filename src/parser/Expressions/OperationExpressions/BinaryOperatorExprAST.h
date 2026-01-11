/*
    This is an expression which captures any arithmetical operation from source code.
    The supported operators are defined in /basicComponents/BinaryOperatorType.h
    The variable types are defined in /basicComponents/VariableType.h

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


#ifndef BINARYOPERATOREXPRAST_H
#define BINARYOPERATOREXPRAST_H

#include <memory>
#include <optional>

#include "../AbstractExprAST.h"
#include "../../../basicComponents/BinaryOperatorType.h"
#include "../../../basicComponents/Token.h"
#include "../../../basicComponents/VariableType.h"


class BinaryOperatorExprAST : public virtual AbstractExprAST {
public:
    BinaryOperatorExprAST(
        int id,
        std::unique_ptr<AbstractExprAST> lhsExpr,
        std::unique_ptr<AbstractExprAST> rhsExpr,
        Token &lhsRefTok,
        Token &rhsRefTok,
        BinaryOperatorType op)
        : id(id),
          LHSExpr(std::move(lhsExpr)),
          RHSExpr(std::move(rhsExpr)),
          operatorType(op),
          lhsRefTok(lhsRefTok),
          rhsRefTok(rhsRefTok) {
    }

    int getID() const { return id; }
    const std::unique_ptr<AbstractExprAST> &getLHS() const { return LHSExpr; }
    const std::unique_ptr<AbstractExprAST> &getRHS() const { return RHSExpr; }
    BinaryOperatorType getOperator() const { return operatorType; }
    Token &getLHSRefTok() { return lhsRefTok; }
    Token &getRHSRefTok() { return rhsRefTok; }
    VariableType getCalculationReturnType() const { return calculationReturnType; }

    bool setCalculationReturnType(VariableType resultType) { calculationReturnType = resultType; }

private:
    int id;

    std::unique_ptr<AbstractExprAST> LHSExpr;
    std::unique_ptr<AbstractExprAST> RHSExpr;
    BinaryOperatorType operatorType;

    VariableType calculationReturnType;

    Token &lhsRefTok;
    Token &rhsRefTok;

    ExpressionType expressionType = ExpressionType::BinaryOperatorExpr;
};

#endif //BINARYOPERATOREXPRAST_H
