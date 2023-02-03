/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef VARSTACKTEST_CATCH_BLOCK_H
#define VARSTACKTEST_CATCH_BLOCK_H

#include "AstNode.h"

class Name;
class ScopeNode;

class CatchBlock : public AstNode {
public:
    CatchBlock();
    ~CatchBlock();
    std::string ToString(bool nl) override;

    [[nodiscard]] const std::vector<std::shared_ptr<Name>>& GetExceptionIds() const {
        return _exception_ids;
    }

    void SetExceptionIds(const std::vector<std::shared_ptr<Name>>& exceptionIds) {
        _exception_ids = exceptionIds;
    }

    [[nodiscard]] const std::shared_ptr<ScopeNode>& GetScope() const {
        return _scope;
    }

    void SetScope(const std::shared_ptr<ScopeNode>& scope) {
        _scope = scope;
    }

    void AddExceptionId(std::shared_ptr<Name> id) {
        _exception_ids.push_back(id);
    }

private:
    std::vector<std::shared_ptr<Name>> _exception_ids;
    std::shared_ptr<ScopeNode> _scope = nullptr;
};

#endif  //VARSTACKTEST_CATCH_BLOCK_H
