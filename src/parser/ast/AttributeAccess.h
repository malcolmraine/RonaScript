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

    void SetParent(const AstNodePtr<AstNode>& parent) {
        _parent = parent;
    }

    [[nodiscard]] const AstNodePtr<AstNode>& GetName() const {
        return _name;
    }

private:
    AstNodePtr<AstNode> _parent = nullptr;
    AstNodePtr<AstNode> _name = nullptr;
    std::string _op;
};
