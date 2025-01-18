/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
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

#pragma once

#include <memory>
#include <string>
#include <vector>
#include "../../util/FileInfo.h"
#include "NodeType.h"

template <typename T>
using AstNodePtr = T*;

class RnCodeGenVisitor;

class AstNode {
public:
//    enum : size_t {
//        LEFT_CHILD_INDEX = 0,
//        RIGHT_CHILD_INDEX = 1,
//    };
    static const size_t LEFT_CHILD_INDEX = 0;
    static const size_t RIGHT_CHILD_INDEX = 1;
    static const size_t PRIMARY_EXPR_INDEX = 0;
    static const size_t TEST_INDEX = 0;
    static const size_t CONSEQUENT_INDEX = 1;
    static const size_t ALTERNATIVE_INDEX = 2;


    AstNode() = default;
    virtual ~AstNode();
    [[nodiscard]] bool IsLiteral() const;
    virtual std::string ToString(bool nl);
    void AddChild(const AstNodePtr<AstNode>& child);
    void SetChild(size_t index, AstNodePtr<AstNode> node);

    std::vector<AstNodePtr<AstNode>> GetChildren() const {
        return _children;
    }

    template <class T = AstNode>
    AstNodePtr<T> GetChild(size_t index) const {
        if (_children.size() < index + 1) {
            return nullptr;
        }
        return AstNode::CastNode<T>(_children.at(index));
    }

    template <class T, typename... Args>
    static AstNodePtr<T> CreateNode(Args... args) {
//        return std::make_shared<T>(std::forward<Args>(args)...);
        return new T(std::forward<Args>(args)...);
    }

    template <class TO, class FROM>
    static AstNodePtr<TO> CastNode(AstNodePtr<FROM> node) {
//        return std::dynamic_pointer_cast<TO>(node);
        return dynamic_cast<TO*>(node);
    }

    NodeType_t node_type = AST_DEFAULT;
    int nest_lvl = 0;  // For adding \t characters to string output
    FileInfo file_info;

protected:
    std::string MakeTabStr() const;
    std::vector<AstNodePtr<AstNode>> _children;
};
