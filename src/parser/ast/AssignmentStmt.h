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
#include "AstNode.h"

class AssignmentStmt : public AstNode {
public:
    AssignmentStmt();
    ~AssignmentStmt();
    std::string ToString(bool nl) override;

    [[nodiscard]] const std::shared_ptr<AstNode>& GetRexpr() const {
        return _rexpr;
    }

    void SetRexpr(const std::shared_ptr<AstNode>& rexpr) {
        _rexpr = rexpr;
    }

    [[nodiscard]] const std::shared_ptr<AstNode>& GetLexpr() const {
        return _lexpr;
    }

    void SetLexpr(const std::shared_ptr<AstNode>& lexpr) {
        _lexpr = lexpr;
    }

    [[nodiscard]] const std::shared_ptr<AstNode>& GetOffset() const {
        return _offset;
    }

    void SetOffset(const std::shared_ptr<AstNode>& offset) {
        _offset = offset;
    }

    [[nodiscard]] const std::shared_ptr<AstNode>& GetIdx() const {
        return _idx;
    }

    void SetIdx(const std::shared_ptr<AstNode>& idx) {
        _idx = idx;
    }

private:
    std::shared_ptr<AstNode> _rexpr{};
    std::shared_ptr<AstNode> _lexpr = nullptr;
    std::shared_ptr<AstNode> _offset = nullptr;
    std::shared_ptr<AstNode> _idx = nullptr;
};
