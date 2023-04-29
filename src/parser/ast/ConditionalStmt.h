
#pragma once

#include "AstNode.h"

class ConditionalStmt : public AstNode {
public:
    ConditionalStmt();
    ~ConditionalStmt() override;
    std::string ToString(bool nl) override;
    std::shared_ptr<AstNode> test = nullptr;
    std::shared_ptr<AstNode> consequent = nullptr;
    std::shared_ptr<AstNode> alternative = nullptr;
};
