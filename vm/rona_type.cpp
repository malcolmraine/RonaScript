/******************************************************************************
* File:
* Description:
* Author: Malcolm Hall
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
*******************************************************************************/

#include "rona_type.h"


/******************************************************************************
 * @brief
 * @param type
 * @return
 */
RonaType_t get_type(std::string type) {
    std::string normalized_string = std::move(type);
    transform(normalized_string.begin(), normalized_string.end(), normalized_string.begin(), ::tolower);

    if (normalized_string == "int") {
        return RN_TYPE_INT;
    } else if (normalized_string == "float") {
        return RN_TYPE_FLOAT;
    } else if (normalized_string == "array") {
        return RN_TYPE_ARRAY;
    } else if (normalized_string == "string") {
        return RN_TYPE_STRING;
    } else if (normalized_string == "bool") {
        return RN_TYPE_BOOL;
    } else if (normalized_string == "class" || normalized_string == "object") {
        return RN_TYPE_CLASS;
    } else {
        return RN_TYPE_NULL;
    }
}

/******************************************************************************
 * @brief
 * @param type
 * @return
 */
std::string get_type_as_string(RonaType_t type) {
    switch (type) {
        case RN_TYPE_INT:
            return "int";
        case RN_TYPE_FLOAT:
            return "float";
        case RN_TYPE_BOOL:
            return "bool";
        case RN_TYPE_CLASS:
            return "class";
        case RN_TYPE_STRING:
            return "string";
        case RN_TYPE_ARRAY:
            return "array";
        case RN_TYPE_NULL:
            return "null";
        default:
            return "";
    }
}


