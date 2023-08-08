/*****************************************************************************
* File: RnBuiltins_IO.h
* Description:
* Author: Malcolm Hall
* Date: 6/20/22
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

#include "RnBuiltins.h"

class RnScope;
class RnObject;

/**
 * With also want functions for slightly lower level file handling so
 * we can create file objects
 *
 * basically all the methods that would take a file descriptor as an input
 * so those can go on a FileIO class
 *
 * Other methods:
 *
 * file_copy
 * file_remove
 * file_move
 * file_create
 * file_open
 * _file_fd_open
 * _file_fd_close
 * _file_fd_write
 * _file_fd_read
 * _file_fd_truncate
 * file_truncate
 * file_rename
 *
 * log_info
 * log_err
 * log_dbg
 * set_log_level
 * log_level - returns current log level
 *
 */

#undef RN_BUILTIN_IO_REGISTRATIONS
#define RN_BUILTIN_IO_REGISTRATIONS                                       \
    RN_BUILTIN_FUNC(RnBuiltins_IO, file_size, RnType::RN_INT, 1)          \
    RN_BUILTIN_FUNC(RnBuiltins_IO, file_write, RnType::RN_INT, 2)         \
    RN_BUILTIN_FUNC(RnBuiltins_IO, print, RnType::RN_VOID, 1)             \
    RN_BUILTIN_FUNC(RnBuiltins_IO, file_read, RnType::RN_STRING, 1)       \
    RN_BUILTIN_FUNC(RnBuiltins_IO, prompt, RnType::RN_STRING, 1)          \
    RN_BUILTIN_FUNC(RnBuiltins_IO, _file_open, RnType::RN_INT, 1)         \
    RN_BUILTIN_FUNC(RnBuiltins_IO, _file_fd_close, RnType::RN_BOOLEAN, 1) \
    RN_BUILTIN_FUNC(RnBuiltins_IO, _file_fd_write, RnType::RN_INT, 1)     \
    RN_BUILTIN_FUNC(RnBuiltins_IO, _file_fd_read, RnType::RN_STRING, 1)   \
    RN_BUILTIN_FUNC(RnBuiltins_IO, _file_fd_truncate, RnType::RN_BOOLEAN, 1)

#undef RN_BUILTIN_FUNC
#define RN_BUILTIN_FUNC RN_BUILTIN_FUNC_DECLARE

class RnBuiltins_IO {
public:
    RN_BUILTIN_IO_REGISTRATIONS
};
