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

class AttributeAccess : public AstNode {
public:
    AttributeAccess();
    ~AttributeAccess() override;
    std::string ToString(bool nl) override;

    void SetParent(const std::shared_ptr<AstNode>& parent) {
        _parent = parent;
    }

    [[nodiscard]] const std::shared_ptr<AstNode>& GetName() const {
        return _name;
    }

private:
    std::shared_ptr<AstNode> _parent = nullptr;
    std::shared_ptr<AstNode> _name = nullptr;
    std::string _op;
};
