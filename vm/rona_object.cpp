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
#include <utility>
#include <sstream>
#include "rona_object.h"

/******************************************************************************
 * @brief
 * @param value
 */
RonaObject::RonaObject(RonaClass *value) {
    this->_type = RN_TYPE_CLASS;
    this->data.emplace_back(value);
    
}

/******************************************************************************
 * @brief
 * @param value
 */
RonaObject::RonaObject(RonaFunction *value) {
    this->_type = RN_TYPE_FUNCTION;
    this->data.emplace_back(value);
}

/******************************************************************************
 * @brief
 * @param type
 */
RonaObject::RonaObject(RonaType_t type) {
    this->_type = type;
}

/******************************************************************************
 * @brief
 */
RonaObject::RonaObject() {
    this->_type = RN_TYPE_ARRAY;
    
}

/******************************************************************************
 * @brief
 * @param value
 */
RonaObject::RonaObject(long value) {
    this->_type = RN_TYPE_INT;
    this->data.emplace_back(value);
    
}

/******************************************************************************
 * @brief
 * @param value
 */
RonaObject::RonaObject(bool value) {
    this->_type = RN_TYPE_BOOL;
    this->data.emplace_back(value);
    
}

/******************************************************************************
 * @brief
 * @param value
 */
RonaObject::RonaObject(double value) {
    this->_type = RN_TYPE_FLOAT;
    this->data.emplace_back(value);
    
}

/******************************************************************************
 * @brief
 * @param value
 */
RonaObject::RonaObject(std::string value) {
    this->_type = RN_TYPE_STRING;
    this->data.emplace_back(value);
    
}

/******************************************************************************
 * @brief
 */
RonaObject::~RonaObject() {
    if (this->type() == RN_TYPE_CLASS) {
        delete std::get<RonaClass*>(this->data[0]);
    }
    this->data.clear();
}

/******************************************************************************
 * @brief
 * @return
 */
RonaType_t RonaObject::type() {
    return this->_type;
}

/******************************************************************************
 * @brief
 * @param value
 */
void RonaObject::set(long value) {
    switch (this->_type) {
        case RN_TYPE_INT:
            this->data[0] = value;
            break;
        case RN_TYPE_FLOAT:
            this->data[0] = static_cast<double>(value);
            break;
        case RN_TYPE_BOOL:
            this->data[0] = value != 0;
            break;
        default:
            break;
    }
}

/******************************************************************************
 * @brief
 * @param value
 */
void RonaObject::set(double value) {
    switch (this->_type) {
        case RN_TYPE_INT:
            this->data[0] = static_cast<long>(value);
            break;
        case RN_TYPE_FLOAT:
            this->data[0] = value;
            break;
        case RN_TYPE_BOOL:
            this->data[0] = value != 0;
            break;
        default:
            break;
    }
}

/******************************************************************************
 * @brief
 * @param value
 */
void RonaObject::set(bool value) {
    switch (this->_type) {
        case RN_TYPE_INT:
            if (this->data.empty()) {
                this->data.emplace_back(static_cast<long>(value));
            } else {
                this->data[0] = static_cast<long>(value);
            }
            break;
        case RN_TYPE_FLOAT:
            if (this->data.empty()) {
                this->data.emplace_back(static_cast<double>(value));
            } else {
                this->data[0] = static_cast<double>(value);
            }
            break;
        case RN_TYPE_BOOL:
            if (this->data.empty()) {
                this->data.emplace_back(value);
            } else {
                this->data[0] = value;
            }
            break;
        default:
            break;
    }
}

/******************************************************************************
 * @brief
 * @param value
 */
void RonaObject::set(std::string value) {
    if (this->_type == RN_TYPE_STRING) {
        if (this->data.empty()) {
            this->data.emplace_back(value);
        } else {
            this->data[0] = value;
        }
    } else {
        throw TypeAssignmentError(this->_type, RN_TYPE_STRING);
    }
}

/******************************************************************************
 * @brief
 * @param value
 */
void RonaObject::set(RonaClass *value) {
    if (this->_type == RN_TYPE_CLASS) {
        if (this->data.empty()) {
            this->data.emplace_back(value);
        } else {
            this->data[0] = value;
        }
    } else {
        throw std::exception();
    }
}

//void RonaObject::set(long offset, const RonaObject& value) {
//    if (this->_type == RN_TYPE_ARRAY) {
//        data[offset] = value;
//    }
//

/******************************************************************************
 * @brief
 * @return
 */
std::string RonaObject::to_string(bool sanitize_strings) {
    switch (type()) {
        case RN_TYPE_STRING:
            return sanitize_strings ? RonaObject::sanitize_string(std::get<std::string>(this->data[0])) : std::get<std::string>(this->data[0]);
        case RN_TYPE_FLOAT:
            return std::to_string(std::get<double>(this->data[0]));
        case RN_TYPE_INT:
            return std::to_string(std::get<long>(this->data[0]));
        case RN_TYPE_ARRAY:
            if (this->data.empty()) {
                return "[]";
            } else {
                std::string str = "[\n";
                for (int i = 0; i < this->data.size(); i++) {
                    str += "\t" + std::to_string(i) + " => " + std::get<RonaObject *>(this->data[i])->to_string() +
                           ",\n";
                }
                str += "]\n";
                return str;
            }
        case RN_TYPE_BOOL:
            return std::get<bool>(this->data[0]) ? "true" : "false";
        case RN_TYPE_CLASS: {
            std::stringstream addr;
            addr << std::hex << address();
            return "{RonaClass:" + std::get<RonaClass *>(this->data[0])->get_name()->to_string() + " at 0x" +
                   addr.str() + "}";
        }
        case RN_TYPE_NULL:
            return "null";
        case RN_TYPE_CLASS_CONSTRUCTOR:
            return "";
        case RN_TYPE_FUNCTION: {
            std::stringstream addr;
            addr << std::hex << address();
            return "{RonaFunction at 0x" + addr.str() + "}";
        }
        default:
            return "";
//        case RN_TYPE_CALLABLE:
//            break;
//        case RN_TYPE_ANY:
//            break;
    }
}

std::string RonaObject::sanitize_string(std::string str) {
    std::string sanitized_str = std::move(str);

    if (sanitized_str[0] == '\"') {
        sanitized_str.erase(0, 1);
    }

    if (sanitized_str[sanitized_str.length() - 1] == '\"') {
        sanitized_str.erase(sanitized_str.length() - 1, 1);
    }

    return sanitized_str;
}

/******************************************************************************
 * @brief
 * @return
 */
long RonaObject::to_int() {
    switch (this->_type) {
        case RN_TYPE_FLOAT:
            return static_cast<long>(std::get<double>(data[0]));
        case RN_TYPE_INT:
            return std::get<long>(data[0]);
        case RN_TYPE_NULL:
            return 0;
        case RN_TYPE_BOOL:
            return std::get<bool>(data[0]) ? 1 : 0;
        case RN_TYPE_ARRAY:
        case RN_TYPE_STRING:
        case RN_TYPE_CLASS_CONSTRUCTOR:
        case RN_TYPE_FUNCTION:
        case RN_TYPE_CALLABLE:
        case RN_TYPE_CLASS:
        case RN_TYPE_ANY:
            throw TypeCastError(this->_type, RN_TYPE_INT);
    }
}

/******************************************************************************
 * @brief
 * @return
 */
bool RonaObject::to_bool() {
    switch (this->_type) {
        case RN_TYPE_BOOL:
            return std::get<bool>(data[0]);
        case RN_TYPE_FLOAT:
            return std::get<double>(data[0]) != 0.0;
        case RN_TYPE_STRING:
            return std::get<std::string>(data[0]).empty();
        case RN_TYPE_INT:
            return std::get<long>(data[0]) != 0;
        case RN_TYPE_ARRAY:
            return this->data.empty();
        default:
            return false;
    }
}

/******************************************************************************
 * @brief
 * @return
 */
double RonaObject::to_float() {
    switch (this->_type) {
        case RN_TYPE_BOOL:
            return std::get<bool>(data[0]) ? 1.0 : 0.0;
        case RN_TYPE_FLOAT:
            return std::get<double>(data[0]);
        case RN_TYPE_INT:
            return static_cast<double>(std::get<long>(data[0]));
        default:
            throw std::exception();
    }
}

/******************************************************************************
 * @brief
 * @return
 */
RonaFunction *RonaObject::as_function() {
    if (this->_type == RN_TYPE_FUNCTION) {
        return std::get<RonaFunction *>(data[0]);
    } else {
        throw std::exception();
    }
}

/******************************************************************************
 * @brief
 * @param idx
 * @return
 */
RonaObject *RonaObject::operator[](int idx) {
    if (this->_type == RN_TYPE_ARRAY) {
        return std::get<RonaObject *>(data[idx]);
    } else {
        return nullptr;
    }
}

/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator+(RonaObject *other) {
    RonaObject *output = nullptr;

    if (this->_type == RN_TYPE_INT) {
        if (other->type() == RN_TYPE_INT) {
            output = new RonaObject(std::get<long>(data[0]) + std::get<long>(other->data[0]));
        } else if (other->type() == RN_TYPE_FLOAT) {
            output = new RonaObject(std::get<long>(data[0]) + std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else if (this->_type == RN_TYPE_FLOAT) {
        if (other->type() == RN_TYPE_INT) {
            output = new RonaObject(std::get<double>(data[0]) + std::get<long>(other->data[0]));
        } else if (other->type() == RN_TYPE_FLOAT) {
            output = new RonaObject(std::get<double>(data[0]) + std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else if (this->_type == RN_TYPE_STRING && other->type() == RN_TYPE_STRING) {
        std::string str1 = RonaObject::sanitize_string(std::get<std::string>(data[0]));
        std::string str2 = RonaObject::sanitize_string(std::get<std::string>(other->data[0]));

        output = new RonaObject(str1 + str2);
    } else {
            // Raise exception
    }

    return output;
}

/******************************************************************************
 * @brief
 * @return
 */
RonaObject *RonaObject::operator+() {
    if (this->_type == RN_TYPE_INT) {
        return new RonaObject(+std::get<long>(this->data[0]));
    } else if (this->_type == RN_TYPE_FLOAT) {
        return new RonaObject(+std::get<double>(this->data[0]));
    } else {
        // TODO: Throw exception
        throw std::exception();
    }
}

/******************************************************************************
 * @brief
 * @param offset
 * @param value
 */
void RonaObject::set(long offset, const RonaObject &value) {

}

/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator-(RonaObject *other) {
    RonaObject *output = nullptr;

    if (this->_type == RN_TYPE_INT) {
        if (other->type() == RN_TYPE_INT) {
            output = new RonaObject(std::get<long>(data[0]) - std::get<long>(other->data[0]));
        } else if (other->type() == RN_TYPE_FLOAT) {
            output = new RonaObject(std::get<long>(data[0]) - std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else if (this->_type == RN_TYPE_FLOAT) {
        if (other->type() == RN_TYPE_INT) {
            output = new RonaObject(std::get<double>(data[0]) - std::get<long>(other->data[0]));
        } else if (other->type() == RN_TYPE_FLOAT) {
            output = new RonaObject(std::get<double>(data[0]) - std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else {
        // Raise exception
    }

    return output;
}

/******************************************************************************
 * @brief
 * @return
 */
RonaObject *RonaObject::operator-() {
    if (this->_type == RN_TYPE_INT) {
        return new RonaObject(-std::get<long>(this->data[0]));
    } else if (this->_type == RN_TYPE_FLOAT) {
        return new RonaObject(-std::get<double>(this->data[0]));
    } else {
        // TODO: Throw exception
        throw std::exception();
    }
}

/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator/(RonaObject *other) {
    RonaObject *output = nullptr;

    if (this->_type == RN_TYPE_INT) {
        if (other->type() == RN_TYPE_INT) {
            output = new RonaObject(std::get<long>(data[0]) / std::get<long>(other->data[0]));
        } else if (other->type() == RN_TYPE_FLOAT) {
            output = new RonaObject(std::get<long>(data[0]) / std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else if (this->_type == RN_TYPE_FLOAT) {
        if (other->type() == RN_TYPE_INT) {
            output = new RonaObject(std::get<double>(data[0]) / std::get<long>(other->data[0]));
        } else if (other->type() == RN_TYPE_FLOAT) {
            output = new RonaObject(std::get<double>(data[0]) / std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else {
        // Raise exception
    }

    return output;
}

/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator%(RonaObject *other) {
    RonaObject *output = nullptr;

    if (this->_type == RN_TYPE_INT && other->type() == RN_TYPE_INT) {
        output = new RonaObject(std::get<long>(data[0]) % std::get<long>(other->data[0]));
    } else {
        // Raise exception
    }

    return output;
}

/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator*(RonaObject *other) {
    RonaObject *output = nullptr;

    if (this->_type == RN_TYPE_INT) {
        if (other->type() == RN_TYPE_INT) {
            output = new RonaObject(std::get<long>(data[0]) * std::get<long>(other->data[0]));
        } else if (other->type() == RN_TYPE_FLOAT) {
            output = new RonaObject(std::get<long>(data[0]) * std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else if (this->_type == RN_TYPE_FLOAT) {
        if (other->type() == RN_TYPE_INT || other->type() == RN_TYPE_FLOAT) {
            output = new RonaObject(this->to_float() * other->to_float());
        } else {
            // Raise exception
        }
    } else if ((this->_type == RN_TYPE_STRING && other->type() == RN_TYPE_INT) || (this->_type == RN_TYPE_INT && other->type() == RN_TYPE_STRING)) {
        std::string str_accum;
        int mul = this->_type == RN_TYPE_INT ? this->to_int() : other->to_int();
        std::string str = this->_type == RN_TYPE_STRING ? this->to_string() : other->to_string();
        str = RonaObject::sanitize_string(str);

        for (int i = 0; i < mul; i++) {
            str_accum += str;
        }
        output = new RonaObject(str_accum);
    } else {
        // Raise exception
    }

    return output;
}

/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator>(RonaObject *other) {
    RonaObject *output = nullptr;

    if (this->_type == RN_TYPE_INT) {
        if (other->type() == RN_TYPE_INT) {
            output = new RonaObject(std::get<long>(data[0]) > std::get<long>(other->data[0]));
        } else if (other->type() == RN_TYPE_FLOAT) {
            output = new RonaObject(std::get<long>(data[0]) > std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else if (this->_type == RN_TYPE_FLOAT) {
        if (other->type() == RN_TYPE_INT) {
            output = new RonaObject(std::get<double>(data[0]) > std::get<long>(other->data[0]));
        } else if (other->type() == RN_TYPE_FLOAT) {
            output = new RonaObject(std::get<double>(data[0]) > std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else {
        // Raise exception
    }

    return output;
}

/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator<(RonaObject *other) {
    RonaObject *output = nullptr;

    if (this->_type == RN_TYPE_INT) {
        if (other->type() == RN_TYPE_INT) {
            output = new RonaObject(std::get<long>(data[0]) < std::get<long>(other->data[0]));
        } else if (other->type() == RN_TYPE_FLOAT) {
            output = new RonaObject(std::get<long>(data[0]) < std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else if (this->_type == RN_TYPE_FLOAT) {
        if (other->type() == RN_TYPE_INT) {
            output = new RonaObject(std::get<double>(data[0]) < std::get<long>(other->data[0]));
        } else if (other->type() == RN_TYPE_FLOAT) {
            output = new RonaObject(std::get<double>(data[0]) < std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else {
        // Raise exception
    }

    if (output != nullptr) {
        output->set_type(RN_TYPE_BOOL);
    }

    return output;
}

/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator>=(RonaObject *other) {
    RonaObject *output = nullptr;

    if (this->_type == RN_TYPE_INT) {
        if (other->type() == RN_TYPE_INT) {
            output = new RonaObject(std::get<long>(data[0]) >= std::get<long>(other->data[0]));
        } else if (other->type() == RN_TYPE_FLOAT) {
            output = new RonaObject(std::get<long>(data[0]) >= std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else if (this->_type == RN_TYPE_FLOAT) {
        if (other->type() == RN_TYPE_INT) {
            output = new RonaObject(std::get<double>(data[0]) >= std::get<long>(other->data[0]));
        } else if (other->type() == RN_TYPE_FLOAT) {
            output = new RonaObject(std::get<double>(data[0]) >= std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else {
        // Raise exception
    }

    return output;
}

/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator<=(RonaObject *other) {
    RonaObject *output = nullptr;

    if (this->_type == RN_TYPE_INT) {
        if (other->type() == RN_TYPE_INT) {
            output = new RonaObject(std::get<long>(data[0]) <= std::get<long>(other->data[0]));
        } else if (other->type() == RN_TYPE_FLOAT) {
            output = new RonaObject(std::get<long>(data[0]) <= std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else if (this->_type == RN_TYPE_FLOAT) {
        if (other->type() == RN_TYPE_INT) {
            output = new RonaObject(std::get<double>(data[0]) <= std::get<long>(other->data[0]));
        } else if (other->type() == RN_TYPE_FLOAT) {
            output = new RonaObject(std::get<double>(data[0]) <= std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else {
        // Raise exception
    }

    return output;
}

/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator>>(RonaObject *other) {
    RonaObject *output = nullptr;

    if (this->_type == RN_TYPE_INT && other->type() == RN_TYPE_INT) {
        output = new RonaObject(std::get<long>(data[0]) >> std::get<long>(other->data[0]));
    } else {
        // Raise exception
    }

    return output;
}

/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator<<(RonaObject *other) {
    RonaObject *output = nullptr;

    if (this->_type == RN_TYPE_INT && other->type() == RN_TYPE_INT) {
        output = new RonaObject(std::get<long>(data[0]) << std::get<long>(other->data[0]));
    } else {
        // Raise exception
    }

    return output;
}

/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator==(RonaObject *other) {
    RonaObject *output;

    if (this->_type == RN_TYPE_INT) {
        if (other->type() == RN_TYPE_INT) {
            output = new RonaObject(std::get<long>(data[0]) == std::get<long>(other->data[0]));
        } else if (other->type() == RN_TYPE_FLOAT) {
            output = new RonaObject(std::get<long>(data[0]) == std::get<double>(other->data[0]));
        } else {
            output = new RonaObject(false);
        }
    } else if (this->_type == RN_TYPE_FLOAT) {
        if (other->type() == RN_TYPE_INT) {
            output = new RonaObject(std::get<double>(data[0]) == std::get<long>(other->data[0]));
        } else if (other->type() == RN_TYPE_FLOAT) {
            output = new RonaObject(std::get<double>(data[0]) == std::get<double>(other->data[0]));
        } else {
            output = new RonaObject(false);
        }
    } else if (this->_type == RN_TYPE_STRING && other->type() == RN_TYPE_STRING) {
        output = new RonaObject(to_string() == other->to_string());
    } else if (this->_type == RN_TYPE_ARRAY && other->type() == RN_TYPE_ARRAY) {
        if (data.size() == other->data.size()) {
            for (long i = 0; i < data.size(); i++) {
                if (data[i] != other->data[i]) {
                    output = new RonaObject(false);
                }
            }
            output = new RonaObject(true);
        } else {
            output = new RonaObject(false);
        }
    } else {
        output = new RonaObject(this->_type == RN_TYPE_NULL && other->type() == RN_TYPE_NULL);
    }

    return output;
}

/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator!=(RonaObject *other) {
    return new RonaObject(!(this == other));
}

/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator|(RonaObject *other) {
    return nullptr;
}

/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator&(RonaObject *other) {
    return nullptr;
}

/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator^(RonaObject *other) {
    return nullptr;
}

/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator&&(RonaObject *other) {
    return nullptr;
}

/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator||(RonaObject *other) {
    return nullptr;
}

/******************************************************************************
 * @brief 
 * @return 
 */
RonaObject *RonaObject::operator!() {
    if (this->_type == RN_TYPE_INT) {
        return new RonaObject(!std::get<long>(this->data[0]));
    } else if (this->_type == RN_TYPE_FLOAT) {
        return new RonaObject(!std::get<double>(this->data[0]));
    } else {
        // TODO: Throw exception
        throw std::exception();
    }

}

/******************************************************************************
 * @brief 
 * @return 
 */
RonaObject *RonaObject::operator~() {
    if (this->_type == RN_TYPE_INT) {
        return new RonaObject(~std::get<long>(this->data[0]));
    } else {
        // TODO: Throw exception
        throw std::exception();
    }
}


/******************************************************************************
 * @brief
 * @return
 */
long RonaObject::address() {
    return reinterpret_cast<long>(this);
}

RonaObject *RonaObject::clone() {
    auto other = new RonaObject(this->type());
    other->data = this->data;

    return other;
}

void RonaObject::set_type(RonaType_t type) {
    this->_type = type;
}
