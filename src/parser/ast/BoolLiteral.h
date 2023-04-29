/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#pragma once

#include <string>
#include "AstNode.h"

class BoolLiteral : public AstNode {
public:
    BoolLiteral();
    explicit BoolLiteral(bool value);
    explicit BoolLiteral(long value);
    explicit BoolLiteral(double value);
    explicit BoolLiteral(const std::string& value);
    ~BoolLiteral() override;
    std::string ToString(bool nl) override;

    [[nodiscard]] bool GetData() const {
        return _data;
    }

    void SetData(bool data) {
        _data = data;
    }

private:
    bool _data = false;
};

