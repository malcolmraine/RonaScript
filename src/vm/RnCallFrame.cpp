/*****************************************************************************
* File: RnCallFrame.h
* Description:
* Author: Malcolm Hall
* Date: 11/6/23
* Version: 1
*
* MIT License
*
* Copyright (c) 2023 Malcolm Hall
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

#include "RnCallFrame.h"
#include "RnFunction.h"
#include "RnScope.h"
#include "RnObject.h"

/*****************************************************************************/
RnFunction* RnCallFrame::GetFunction(){
    return _func;
}

/*****************************************************************************/
void RnCallFrame::SetFunction(RnFunction* func) {
    _func = func;
}

/*****************************************************************************/
RnScope* RnCallFrame::GetScope() const{
    return _scope;
}

/*****************************************************************************/
void RnCallFrame::SetScope(RnScope* scope){
    _scope = scope;
}

/*****************************************************************************/
RnObject* RnCallFrame::GetReturnValue() const{
    return _ret_val;
}

/*****************************************************************************/
void RnCallFrame::SetReturnValue(RnObject* value){
    _ret_val = value;
}
