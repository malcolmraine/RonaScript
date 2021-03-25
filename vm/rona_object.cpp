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
#include "rona_object.h"
#include <utility>
#include <sstream>


/******************************************************************************
 * @brief
 * @param value
 */
RonaObject::RonaObject(RonaClass *value) {
    this->_type = RONA_CLASS;
    this->data.emplace_back(value);
    add_reference_cnt();
}

/******************************************************************************
 * @brief
 * @param value
 */
RonaObject::RonaObject(RonaFunction *value) {
    this->_type = RONA_FUNCTION;
    this->data.emplace_back(value);
    add_reference_cnt();
}


/******************************************************************************
 * @brief
 * @param type
 */
RonaObject::RonaObject(RonaType_t type) {
    this->_type = type;
    add_reference_cnt();
}


/******************************************************************************
 * @brief
 */
RonaObject::RonaObject() {
    this->_type = RONA_ARRAY;
    add_reference_cnt();
}


/******************************************************************************
 * @brief
 * @param value
 */
RonaObject::RonaObject(long value) {
    this->_type = RONA_INT;
    this->data.emplace_back(value);
    add_reference_cnt();
}


/******************************************************************************
 * @brief
 * @param value
 */
RonaObject::RonaObject(bool value) {
    this->_type = RONA_STRING;
    this->data.emplace_back(value);
    add_reference_cnt();
}


/******************************************************************************
 * @brief
 * @param value
 */
RonaObject::RonaObject(double value) {
    this->_type = RONA_FLOAT;
    this->data.emplace_back(value);
    add_reference_cnt();
}


/******************************************************************************
 * @brief
 * @param value
 */
RonaObject::RonaObject(std::string value) {
    this->_type = RONA_STRING;
    this->data.emplace_back(value);
    add_reference_cnt();
}


/******************************************************************************
 * @brief
 */
RonaObject::~RonaObject() {
    if (this->_type == RONA_ARRAY) {
        for (auto &i : this->data) {
            try {
                delete std::get<RonaObject *>(i);
            } catch (int e) {

            }
        }
    }
}


/******************************************************************************
 * @brief
 * @return
 */
RonaType_t RonaObject::type() const {
    return this->_type;
}


/******************************************************************************
 * @brief
 * @param value
 */
void RonaObject::set(long value) {
    switch (this->_type) {
        case RONA_INT:
            this->data[0] = value;
            break;
        case RONA_FLOAT:
            this->data[0] = static_cast<double>(value);
            break;
        case RONA_BOOL:
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
        case RONA_INT:
            this->data[0] = static_cast<long>(value);
            break;
        case RONA_FLOAT:
            this->data[0] = value;
            break;
        case RONA_BOOL:
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
        case RONA_INT:
            if (this->data.empty()) {
                this->data.emplace_back(static_cast<long>(value));
            } else {
                this->data[0] = static_cast<long>(value);
            }
            break;
        case RONA_FLOAT:
            if (this->data.empty()) {
                this->data.emplace_back(static_cast<double>(value));
            } else {
                this->data[0] = static_cast<double>(value);
            }
            break;
        case RONA_BOOL:
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
    if (this->_type == RONA_STRING) {
        if (this->data.empty()) {
            this->data.emplace_back(value);
        } else {
            this->data[0] = value;
        }
    } else {
        throw TypeAssignmentError(this->_type, RONA_STRING);
    }
}


/******************************************************************************
 * @brief
 * @param value
 */
void RonaObject::set(RonaClass *value) {
    if (this->_type == RONA_CLASS) {
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
//    if (_type == RONA_ARRAY) {
//        data[offset] = value;
//    }
//}


/******************************************************************************
 * @brief
 * @return
 */
std::string RonaObject::to_string() {
    switch (this->_type) {
        case RONA_STRING:
            return std::get<std::string>(this->data[0]);
        case RONA_FLOAT:
            return std::to_string(std::get<double>(this->data[0]));
        case RONA_INT:
            return std::to_string(std::get<long>(this->data[0]));
        case RONA_ARRAY:
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
        case RONA_BOOL:
            return std::get<bool>(this->data[0]) ? "True" : "False";
        case RONA_CLASS: {
            std::stringstream addr;
            addr << std::hex << address();
            return "{RonaClass:" + std::get<RonaClass *>(this->data[0])->get_name()->to_string() + " at 0x" +
                   addr.str() + "}";
        }
        case RONA_NULL:
            return "NULL";
        case RONA_CLASS_CONSTRUCTOR:
            return "";
        case RONA_FUNCTION:
            std::stringstream addr;
            addr << std::hex << address();
            return "{RonaFunction at 0x" + addr.str() + "}";
    }
}


/******************************************************************************
 * @brief
 * @return
 */
long RonaObject::to_int() {
    switch (_type) {
        case RONA_FLOAT:
            return static_cast<long>(std::get<double>(data[0]));
        case RONA_INT:
            return std::get<long>(data[0]);
        case RONA_NULL:
            return 0;
        case RONA_BOOL:
            return std::get<bool>(data[0]) ? 1 : 0;
        case RONA_ARRAY:
        case RONA_STRING:
        case RONA_CLASS_CONSTRUCTOR:
        case RONA_FUNCTION:
        case RONA_CLASS:
            throw TypeCastError(_type, RONA_INT);
    }
}


/******************************************************************************
 * @brief
 * @return
 */
RonaFunction *RonaObject::as_function() {
    if (_type == RONA_FUNCTION) {
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
RonaObject *RonaObject::operator[](int idx) const {
    if (_type == RONA_ARRAY) {
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
RonaObject *RonaObject::operator+(RonaObject *other) const {
    RonaObject *output = nullptr;

    if (_type == RONA_INT) {
        if (other->type() == RONA_INT) {
            output = new RonaObject(std::get<long>(data[0]) + std::get<long>(other->data[0]));
        } else if (other->type() == RONA_FLOAT) {
            output = new RonaObject(std::get<long>(data[0]) + std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else if (_type == RONA_FLOAT) {
        if (other->type() == RONA_INT) {
            output = new RonaObject(std::get<double>(data[0]) + std::get<long>(other->data[0]));
        } else if (other->type() == RONA_FLOAT) {
            output = new RonaObject(std::get<double>(data[0]) + std::get<double>(other->data[0]));
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
RonaObject *RonaObject::operator-(RonaObject *other) const {
    RonaObject *output = nullptr;

    if (_type == RONA_INT) {
        if (other->type() == RONA_INT) {
            output = new RonaObject(std::get<long>(data[0]) - std::get<long>(other->data[0]));
        } else if (other->type() == RONA_FLOAT) {
            output = new RonaObject(std::get<long>(data[0]) - std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else if (_type == RONA_FLOAT) {
        if (other->type() == RONA_INT) {
            output = new RonaObject(std::get<double>(data[0]) - std::get<long>(other->data[0]));
        } else if (other->type() == RONA_FLOAT) {
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
 * @param other
 * @return
 */
RonaObject *RonaObject::operator/(RonaObject *other) const {
    RonaObject *output = nullptr;

    if (_type == RONA_INT) {
        if (other->type() == RONA_INT) {
            output = new RonaObject(std::get<long>(data[0]) / std::get<long>(other->data[0]));
        } else if (other->type() == RONA_FLOAT) {
            output = new RonaObject(std::get<long>(data[0]) / std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else if (_type == RONA_FLOAT) {
        if (other->type() == RONA_INT) {
            output = new RonaObject(std::get<double>(data[0]) / std::get<long>(other->data[0]));
        } else if (other->type() == RONA_FLOAT) {
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
RonaObject *RonaObject::operator%(RonaObject *other) const {
    RonaObject *output = nullptr;

    if (_type == RONA_INT && other->type() == RONA_INT) {
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
RonaObject *RonaObject::operator*(RonaObject *other) const {
    RonaObject *output = nullptr;

    if (_type == RONA_INT) {
        if (other->type() == RONA_INT) {
            output = new RonaObject(std::get<long>(data[0]) * std::get<long>(other->data[0]));
        } else if (other->type() == RONA_FLOAT) {
            output = new RonaObject(std::get<long>(data[0]) * std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else if (_type == RONA_FLOAT) {
        if (other->type() == RONA_INT) {
            output = new RonaObject(std::get<double>(data[0]) * std::get<long>(other->data[0]));
        } else if (other->type() == RONA_FLOAT) {
            output = new RonaObject(std::get<double>(data[0]) * std::get<double>(other->data[0]));
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
RonaObject *RonaObject::operator>(RonaObject *other) const {
    RonaObject *output = nullptr;

    if (_type == RONA_INT) {
        if (other->type() == RONA_INT) {
            output = new RonaObject(std::get<long>(data[0]) > std::get<long>(other->data[0]));
        } else if (other->type() == RONA_FLOAT) {
            output = new RonaObject(std::get<long>(data[0]) > std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else if (_type == RONA_FLOAT) {
        if (other->type() == RONA_INT) {
            output = new RonaObject(std::get<double>(data[0]) > std::get<long>(other->data[0]));
        } else if (other->type() == RONA_FLOAT) {
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
RonaObject *RonaObject::operator<(RonaObject *other) const {
    RonaObject *output = nullptr;

    if (_type == RONA_INT) {
        if (other->type() == RONA_INT) {
            output = new RonaObject(std::get<long>(data[0]) < std::get<long>(other->data[0]));
        } else if (other->type() == RONA_FLOAT) {
            output = new RonaObject(std::get<long>(data[0]) < std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else if (_type == RONA_FLOAT) {
        if (other->type() == RONA_INT) {
            output = new RonaObject(std::get<double>(data[0]) < std::get<long>(other->data[0]));
        } else if (other->type() == RONA_FLOAT) {
            output = new RonaObject(std::get<double>(data[0]) < std::get<double>(other->data[0]));
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
RonaObject *RonaObject::operator>=(RonaObject *other) const {
    RonaObject *output = nullptr;

    if (_type == RONA_INT) {
        if (other->type() == RONA_INT) {
            output = new RonaObject(std::get<long>(data[0]) >= std::get<long>(other->data[0]));
        } else if (other->type() == RONA_FLOAT) {
            output = new RonaObject(std::get<long>(data[0]) >= std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else if (_type == RONA_FLOAT) {
        if (other->type() == RONA_INT) {
            output = new RonaObject(std::get<double>(data[0]) >= std::get<long>(other->data[0]));
        } else if (other->type() == RONA_FLOAT) {
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
RonaObject *RonaObject::operator<=(RonaObject *other) const {
    RonaObject *output = nullptr;

    if (_type == RONA_INT) {
        if (other->type() == RONA_INT) {
            output = new RonaObject(std::get<long>(data[0]) <= std::get<long>(other->data[0]));
        } else if (other->type() == RONA_FLOAT) {
            output = new RonaObject(std::get<long>(data[0]) <= std::get<double>(other->data[0]));
        } else {
            // Raise exception
        }
    } else if (_type == RONA_FLOAT) {
        if (other->type() == RONA_INT) {
            output = new RonaObject(std::get<double>(data[0]) <= std::get<long>(other->data[0]));
        } else if (other->type() == RONA_FLOAT) {
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
RonaObject *RonaObject::operator>>(RonaObject *other) const {
    RonaObject *output = nullptr;

    if (_type == RONA_INT && other->type() == RONA_INT) {
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
RonaObject *RonaObject::operator<<(RonaObject *other) const {
    RonaObject *output = nullptr;

    if (_type == RONA_INT && other->type() == RONA_INT) {
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

    if (_type == RONA_INT) {
        if (other->type() == RONA_INT) {
            output = new RonaObject(std::get<long>(data[0]) == std::get<long>(other->data[0]));
        } else if (other->type() == RONA_FLOAT) {
            output = new RonaObject(std::get<long>(data[0]) == std::get<double>(other->data[0]));
        } else {
            output = new RonaObject(false);
        }
    } else if (_type == RONA_FLOAT) {
        if (other->type() == RONA_INT) {
            output = new RonaObject(std::get<double>(data[0]) == std::get<long>(other->data[0]));
        } else if (other->type() == RONA_FLOAT) {
            output = new RonaObject(std::get<double>(data[0]) == std::get<double>(other->data[0]));
        } else {
            output = new RonaObject(false);
        }
    } else if (_type == RONA_STRING && other->type() == RONA_STRING) {
        output = new RonaObject(to_string() == other->to_string());
    } else if (_type == RONA_ARRAY && other->type() == RONA_ARRAY) {
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
        output = new RonaObject(_type == RONA_NULL && other->type() == RONA_NULL);
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
RonaObject *RonaObject::operator|(RonaObject *other) const {
    return nullptr;
}


/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator&(RonaObject *other) const {
    return nullptr;
}


/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator^(RonaObject *other) const {
    return nullptr;
}


/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator&&(RonaObject *other) const {
    return nullptr;
}


/******************************************************************************
 * @brief
 * @param other
 * @return
 */
RonaObject *RonaObject::operator||(RonaObject *other) const {
    return nullptr;
}


/******************************************************************************
 * @brief
 */
void RonaObject::free_reference_cnt() {
    //_reference_cnt = _reference_cnt > 0 ? _reference_cnt - 1 : 0;
}


/******************************************************************************
 * @brief
 */
void RonaObject::add_reference_cnt() {
    //_reference_cnt++;
}


/******************************************************************************
 * @brief
 * @return
 */
int RonaObject::get_reference_cnt() {
    return 2;
}


/******************************************************************************
 * @brief
 * @return
 */
long RonaObject::address() {
    return reinterpret_cast<long>(this);
}










