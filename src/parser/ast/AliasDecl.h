/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_ALIASDECL_H
#define RONASCRIPT_ALIASDECL_H

#include <string>
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

    void SetAliasName(const std::shared_ptr<Name>& aliasName) {
        alias_name = aliasName;
    }

    [[nodiscard]] AliasType GetAliasType() const {
        return alias_type;
    }

    void SetAliasType(AliasType aliasType) {
        alias_type = aliasType;
    }

    std::shared_ptr<Name> base_name = nullptr;
    std::shared_ptr<Name> alias_name = nullptr;
    AliasType alias_type;
};

#endif  //RONASCRIPT_ALIASDECL_H