/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_DELETESTMT_H
#define RONASCRIPT_DELETESTMT_H

#include <string>
#include "AstNode.h"
#include "NodeType.h"

class DeleteStmt : public AstNode {
public:
    DeleteStmt();
    ~DeleteStmt() override;
    std::string ToString(bool nl) override;

    [[nodiscard]] const std::shared_ptr<AstNode>& GetName() const {
        return _name;
    }

    void SetName(const std::shared_ptr<AstNode>& name) {
        _name = name;
    }

private:
    std::shared_ptr<AstNode> _name = nullptr;
};

#endif  //RONASCRIPT_DELETESTMT_H
