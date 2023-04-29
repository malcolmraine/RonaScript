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
#include "../../common/RnType.h"
#include "AstNode.h"

class Name;

enum AliasType { NAME_ALIAS, TYPE_ALIAS };

class AliasDecl : public AstNode {
public:
    AliasDecl();
    ~AliasDecl() override;
    std::string ToString(bool nl) override;

    [[nodiscard]] const std::shared_ptr<Name>& GetBaseName() const {
        return base_name;
    }

    void SetBaseName(const std::shared_ptr<Name>& baseName) {
        base_name = baseName;
    }

    [[nodiscard]] const std::shared_ptr<Name>& GetAliasName() const {
        return alias_name;
    }

    [[nodiscard]] const std::shared_ptr<RnTypeComposite>& GetBaseType() const {
        return base_type;
    }

    void SetAliasName(const std::shared_ptr<Name>& aliasName) {
        alias_name = aliasName;
    }

    std::shared_ptr<Name> base_name = nullptr;
    std::shared_ptr<Name> alias_name = nullptr;
    std::shared_ptr<RnTypeComposite> base_type;
    AliasType alias_type;
};
