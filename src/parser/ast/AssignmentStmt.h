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

class AssignmentStmt : public AstNode {
public:
    AssignmentStmt();
    ~AssignmentStmt();
    std::string ToString(bool nl) override;

    [[nodiscard]] const std::shared_ptr<AstNode>& GetRexpr() const {
        return _rexpr;
    }

    void SetRexpr(const std::shared_ptr<AstNode>& rexpr) {
        _rexpr = rexpr;
    }

    [[nodiscard]] const std::shared_ptr<AstNode>& GetLexpr() const {
        return _lexpr;
    }

    void SetLexpr(const std::shared_ptr<AstNode>& lexpr) {
        _lexpr = lexpr;
    }

    [[nodiscard]] const std::shared_ptr<AstNode>& GetOffset() const {
        return _offset;
    }

    void SetOffset(const std::shared_ptr<AstNode>& offset) {
        _offset = offset;
    }

    [[nodiscard]] const std::shared_ptr<AstNode>& GetIdx() const {
        return _idx;
    }

    void SetIdx(const std::shared_ptr<AstNode>& idx) {
        _idx = idx;
    }

private:
    std::shared_ptr<AstNode> _rexpr{};
    std::shared_ptr<AstNode> _lexpr = nullptr;
    std::shared_ptr<AstNode> _offset = nullptr;
    std::shared_ptr<AstNode> _idx = nullptr;
};
