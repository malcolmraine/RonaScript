/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_ATTRIBUTEACCESS_H
#define RONASCRIPT_ATTRIBUTEACCESS_H

#include <string>
#include "AstNode.h"

class AttributeAccess : public AstNode {
public:
    AttributeAccess();
    ~AttributeAccess() override;
    std::string ToString(bool nl) override;

    [[nodiscard]] const std::shared_ptr<AstNode>& GetParent() const {
        return _parent;
    }

    void SetParent(const std::shared_ptr<AstNode>& parent) {
        _parent = parent;
    }

    [[nodiscard]] const std::shared_ptr<AstNode>& GetName() const {
        return _name;
    }
    void SetName(const std::shared_ptr<AstNode>& name) {
        _name = name;
    }

    [[nodiscard]] const std::string& GetOp() const {
        return _op;
    }

    void SetOp(const std::string& op) {
        _op = op;
    }

private:
    std::shared_ptr<AstNode> _parent = nullptr;
    std::shared_ptr<AstNode> _name = nullptr;
    std::string _op;
};

#endif  //RONASCRIPT_ATTRIBUTEACCESS_H
