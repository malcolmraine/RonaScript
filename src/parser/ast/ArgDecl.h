/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_ARGDECL_H
#define RONASCRIPT_ARGDECL_H

#include <memory>
#include <string>
#include "AstNode.h"

class Name;
class RnTypeComposite;

class ArgDecl : public AstNode {
public:
    ArgDecl();
    ~ArgDecl() override;
    std::string ToString(bool nl) override;

    [[nodiscard]] const std::shared_ptr<Name>& GetId() const {
        return _id;
    }

    void SetId(const std::shared_ptr<Name>& id) {
        _id = id;
    }

    [[nodiscard]] const std::shared_ptr<RnTypeComposite>& GetType() const {
        return _type;
    }

    void SetType(const std::shared_ptr<RnTypeComposite>& type) {
        _type = type;
    }

private:
    std::shared_ptr<Name> _id{};
    std::shared_ptr<RnTypeComposite> _type;
};

#endif  //RONASCRIPT_ARGDECL_H
