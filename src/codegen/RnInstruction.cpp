/*****************************************************************************
* File: RnInstruction.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/1/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2020 - 2023 Malcolm Hall
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

#include "RnInstruction.h"
#include "../util/String.h"
#include "../vm/RnObject.h"

#define OPCODE_BYTES_CNT 1
#define ARG_BYTES_CNT 1

union argdata {
    char data;
    char bytes[ARG_BYTES_CNT];
};

/*****************************************************************************/
RnInstruction::RnInstruction(RnOpCode opcode, uint32_t arg1, uint32_t arg2,
                             uint32_t arg3)
    : _opcode(opcode), _arg1(arg1), _arg2(arg2), _arg3(arg3) {}

/*****************************************************************************/
RnInstruction::~RnInstruction() = default;

/*****************************************************************************/
auto RnInstruction::ToString() -> std::string {
    RnStringNative s = GetOpCodeAsString(_opcode);
    s = String::Pad(s, 20);
    std::string arg1_str;
    std::string arg2_str;

    for (const auto& arg : {_arg1, _arg2, _arg3}) {
        s += std::to_string(arg);
        if (arg <= 9) {
            s += "    ";
        } else if (arg <= 99) {
            s += "   ";
        } else {
            s += "  ";
        }
    }

    switch (_opcode) {
        case OP_LOAD_LITERAL:
            arg1_str =
                std::string(RnConstStore::GetInternedObject(_arg1)->ToString().c_str());
            break;
        case OP_ATTR_ACCESS:
            arg1_str = RnConstStore::GetInternedString(_arg1);
            break;
        case OP_UNARY_INCREMENT:
        case OP_UNARY_DECREMENT:
        case OP_LOAD_VALUE:
            arg1_str = RnConstStore::GetInternedString(_arg1);
            break;
        case OP_MAKE_LOCAL:
        case OP_MAKE_VAR:
        case OP_MAKE_CONST:
        case OP_MAKE_GLOBAL:
            arg1_str = RnType::TypeToString(static_cast<RnType::Type>(_arg1));
            arg2_str = RnConstStore::GetInternedString(_arg2);
            break;
        case OP_MAKE_ALIAS:
            arg1_str = RnConstStore::GetInternedString(_arg1);
            arg2_str = RnConstStore::GetInternedString(_arg2);
            break;
        case OP_RESOLVE_NAMESPACE:
        case OP_MAKE_MODULE:
        case OP_MAKE_CLASS:
            arg1_str = RnConstStore::GetInternedString(_arg1);
            break;
        case OP_MAKE_FUNC:
            arg1_str = RnConstStore::GetInternedString(_arg1);
            arg2_str = RnType::TypeToString(static_cast<RnType::Type>(_arg2));
            break;
        case OP_MAKE_ARG:
            arg1_str = RnType::TypeToString(static_cast<RnType::Type>(_arg1));
            arg2_str = RnConstStore::GetInternedString(_arg2);
            break;
        case OP_MAKE_CLOSURE:
            arg2_str = RnType::TypeToString(static_cast<RnType::Type>(_arg2));
            break;
        case OP_TRY_CONTEXT:
        case OP_JUMPF:
        case OP_JUMPB:
        case OP_EXIT:
        case OP_MAKE_ARRAY:
        case OP_JUMPF_IF:
        case OP_JUMPB_IF:
        case OP_CALL:
        case OP_DELETE:
        case OP_UNARY_INVERT:
        case OP_UNARY_NOT:
        default:
            break;
    }

    if (!arg1_str.empty()) {
        s += "  |  " + arg1_str;
        if (!arg2_str.empty()) {
            s += ", " + arg2_str;
        }
    }

    return s;
}
