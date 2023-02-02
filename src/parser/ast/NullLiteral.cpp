/*****************************************************************************
* File: NullLiteral.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/20/22
* Version: 1
*
******************************************************************************/

#include "NullLiteral.h"
#include "../../codegen/RnCodeGenVisitor.h"

/*****************************************************************************/
std::string NullLiteral::ToString(bool nl) {
    return MakeTabStr() + "NullLiteral( )" + (nl ? "\n" : "");
}
