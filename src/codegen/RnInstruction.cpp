/*****************************************************************************
* File: RnInstruction.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/1/22
* Version: 1
*
******************************************************************************/

#include "RnInstruction.h"
#include "../util/RnStringUtil.h"

#define OPCODE_BYTES_CNT 1
#define ARGCNT_BYTES_CNT 1
#define ARG_BYTES_CNT 1

union argdata {
    char data;
    char bytes[ARG_BYTES_CNT];
};

/*****************************************************************************/
RnInstruction::RnInstruction() = default;

/*****************************************************************************/
RnInstruction::RnInstruction(RnOpCode opcode) {
    _opcode = opcode;
}

/*****************************************************************************/
RnInstruction::RnInstruction(RnOpCode opcode, uint32_t arg1) {
    _opcode = opcode;
    _arg1 = arg1;
}

/*****************************************************************************/
RnInstruction::RnInstruction(RnOpCode opcode, uint32_t arg1, uint32_t arg2) {
    _opcode = opcode;
    _arg1 = arg1;
    _arg2 = arg2;
}

/*****************************************************************************/
RnInstruction::RnInstruction(RnOpCode opcode, uint32_t arg1, uint32_t arg2,
                             uint32_t arg3) {
    _opcode = opcode;
    _arg1 = arg1;
    _arg2 = arg2;
    _arg3 = arg3;
}

/*****************************************************************************/
RnInstruction::~RnInstruction() = default;

/*****************************************************************************/
auto RnInstruction::FromBytes(const char* bytes) -> bool {
    _opcode = static_cast<RnOpCode>(bytes[0]);
    argdata data{};
    int i = 0;

    for (char& byte : data.bytes) {
        byte = bytes[i++];
    }
    _arg1 = data.data;

    for (char& byte : data.bytes) {
        byte = bytes[i++];
    }
    _arg2 = data.data;

    return true;
}

/*****************************************************************************/
char* RnInstruction::GetAsBytes() const {
    auto* bytes = new char[OPCODE_BYTES_CNT + ARG_BYTES_CNT * 2];
    int i = 1;
    bytes[0] = static_cast<char>(_opcode);
    argdata argdata{};

    argdata.data = _arg1;
    for (char const byte : argdata.bytes) {
        bytes[i++] = byte;
    }

    argdata.data = _arg2;
    for (char byte : argdata.bytes) {
        bytes[i++] = byte;
    }

    return bytes;
}

/*****************************************************************************/
auto RnInstruction::ToString() -> std::string {
    std::string s = GetOpCodeAsString(_opcode);
    s = RnStringUtil::Pad(s, 20);
    std::string arg1_str;
    std::string arg2_str;

    switch (_opcode) {
        case OP_LOAD_INT:
            arg1_str = std::to_string(RnObject::GetInternedInt(_arg1));
            s += std::to_string(_arg1) + "\t";
            break;
        case OP_LOAD_FLOAT:
            arg1_str = std::to_string(RnObject::GetInternedFloat(_arg1));
            s += std::to_string(_arg1) + "\t";
            break;
        case OP_LOAD_STRING:
            arg1_str = "\"" + RnObject::GetInternedString(_arg1) + "\"";
            s += std::to_string(_arg1) + "\t\t";
            break;
        case OP_ATTR_ACCESS:
            arg1_str = RnObject::GetInternedString(_arg1);
            s += std::to_string(_arg1) + "\t";
            break;
        case OP_UNARY_NOT:
        case OP_UNARY_INCREMENT:
        case OP_UNARY_DECREMENT:
        case OP_UNARY_INVERT:
        case OP_LOAD_VALUE:
            arg1_str = RnObject::GetInternedString(_arg1);
            s += std::to_string(_arg1) + "\t\t";
            break;
        case OP_MAKE_LOCAL:
        case OP_MAKE_VAR:
            arg1_str = RnType::TypeToString(static_cast<RnType::Type>(_arg1));
            arg2_str = RnObject::GetInternedString(_arg2);
            s += RnStringUtil::Join<uint32_t>({_arg1, _arg2}, "\t");
            break;
        case OP_LOAD_BOOL:
            arg1_str = _arg1 == 1 ? "false" : "true";
            s += std::to_string(_arg1) + "\t\t";
            break;
        case OP_MAKE_ALIAS:
        case OP_RESOLVE_NAMESPACE:
            arg1_str = RnObject::GetInternedString(_arg1);
            arg2_str = RnObject::GetInternedString(_arg2);
            s += RnStringUtil::Join<uint32_t>({_arg1, _arg2}, "\t");
            break;
        case OP_MAKE_FUNC:
            arg1_str = RnObject::GetInternedString(_arg1);
            arg2_str = RnType::TypeToString(static_cast<RnType::Type>(_arg1));
            s += RnStringUtil::Join<uint32_t>({_arg1, _arg2}, "\t");
            break;
        case OP_MAKE_ARG:
            arg1_str = RnType::TypeToString(static_cast<RnType::Type>(_arg1));
            arg2_str = RnObject::GetInternedString(_arg2);
            s += RnStringUtil::Join<uint32_t>({_arg1, _arg2}, "\t");
            break;
        case OP_TRY_CONTEXT:
        case OP_JUMPF:
        case OP_JUMPB:
        case OP_EXIT:
        case OP_MAKE_ARRAY:
            s += std::to_string(_arg1) + "\t\t";
            break;
        case OP_JUMPF_IF:
        case OP_JUMPB_IF:
        case OP_CALL:
        case OP_MAKE_CONST:
        case OP_MAKE_CLASS:
        case OP_MAKE_MODULE:
        case OP_DELETE:
            s += RnStringUtil::Join<uint32_t>({_arg1, _arg2}, "\t");
            break;
        default:
            break;
    }

    if (!arg1_str.empty()) {
        s += "\t|\t" + arg1_str;
        if (!arg2_str.empty()) {
            s += ", " + arg2_str;
        }
    }

    return s;
}
