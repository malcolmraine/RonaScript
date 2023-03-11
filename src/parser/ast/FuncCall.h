/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef VARSTACKTEST_FUNCCALL_H
#define VARSTACKTEST_FUNCCALL_H

#include <string>
#include <vector>
#include "AstNode.h"

class FuncCall : public AstNode {
public:
    FuncCall();
    ~FuncCall() override;
    std::string ToString(bool nl) override;

    void SetDiscardReturnValue(bool flag) {
        _discard_return_value = flag;
    }

    [[nodiscard]] bool GetDiscardReturnValue() const {
        return _discard_return_value;
    }

    std::shared_ptr<AstNode> expr = nullptr;
    std::vector<std::shared_ptr<AstNode>> args;

private:
    bool _discard_return_value = false;
};

#endif  //VARSTACKTEST_FUNCCALL_H
