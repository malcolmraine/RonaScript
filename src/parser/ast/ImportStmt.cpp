/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#include "ImportStmt.h"
#include <algorithm>
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
ImportStmt::ImportStmt() {
    node_type = AST_IMPORT;
}

/*****************************************************************************/
std::string ImportStmt::GetFullSourceFile() const {
    std::string file = source_file;
    std::replace(file.begin(), file.end(), '.', '/');

    return file + ".rn";
}

/*****************************************************************************/
std::string ImportStmt::ToString(bool nl) {
    std::string s = MakeTabStr() + "Import( " + source_file + " )";
    if (nl) {
        s += "\n";
    }
    return s;
}
