/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
* MIT License
*
* Copyright (c) 2021 Malcolm Hall
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
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
