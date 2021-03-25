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

#include "rona_class.h"

#include <utility>


/******************************************************************************
 * @brief
 */
RonaClass::RonaClass() {
    this->scope = new Scope;
}

/******************************************************************************
 * @brief
 */
RonaClass::~RonaClass() {
    this->scope->cleanup();
}

/******************************************************************************
 * @brief
 * @param id
 * @return
 */
RonaObject *RonaClass::get_var(RonaObject *id) {
    return this->scope->get_obj(id);
}

/******************************************************************************
 * @brief
 * @param id
 * @param value
 * @param by_reference
 * @return
 */
bool RonaClass::set_var(RonaObject *id, RonaObject *value, bool by_reference) {
    if (by_reference) {
        auto *copied_value = new RonaObject;
        *copied_value = *value;
        this->scope->set(id, copied_value);
    } else {
        this->scope->set(id, value);
    }

    return true;
}

/******************************************************************************
 * @brief
 * @param name
 */
void RonaClass::set_name(RonaObject *name) {
    _name = name;
}

/******************************************************************************
 * @brief
 * @return
 */
RonaObject *RonaClass::get_name() {
    return _name;
}
