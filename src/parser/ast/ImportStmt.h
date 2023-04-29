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
#include "Module.h"
#include "NodeType.h"

class ImportStmt : public AstNode {
public:
    ImportStmt();
    ~ImportStmt() override = default;
    std::string ToString(bool nl) override;
    [[nodiscard]] std::string GetFullSourceFile() const;

    std::string source_file;
};
