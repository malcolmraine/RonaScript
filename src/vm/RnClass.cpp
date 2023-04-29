/*****************************************************************************
* File: RnClass.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/29/22
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

#include "RnClass.h"
#include "RnObject.h"
#include "RnScope.h"
#include "RnSymbolTable.h"

/*****************************************************************************/
RnClass::RnClass(RnScope* parent) : RnScope(parent) {}

/*****************************************************************************/
RnClass::~RnClass() {}

/*****************************************************************************/
void RnClass::CopySymbols(RnScope* target) {
    auto symbol_table = GetSymbolTable();
    auto target_symbol_table = target->GetSymbolTable();

    for (const auto& symbol : symbol_table->GetSymbols()) {
        auto obj = symbol_table->GetObject(symbol);

        // TODO: Fix this to allow callables to be assigned to class attributes
        if (obj->GetType() != RnType::RN_FUNCTION) {
            target_symbol_table->SetSymbol(symbol, obj);
        }
    }
}
