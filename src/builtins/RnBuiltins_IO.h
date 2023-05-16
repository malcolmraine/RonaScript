/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
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
#define RN_BUILTIN_IO_REGISTRATIONS                                 \
    RN_BUILTIN_FUNC(RnBuiltins_IO, file_size, RnType::RN_INT, 1)    \
    RN_BUILTIN_FUNC(RnBuiltins_IO, file_write, RnType::RN_INT, 2)   \
    RN_BUILTIN_FUNC(RnBuiltins_IO, print, RnType::RN_VOID, 1)       \
    RN_BUILTIN_FUNC(RnBuiltins_IO, file_read, RnType::RN_STRING, 1) \
    RN_BUILTIN_FUNC(RnBuiltins_IO, prompt, RnType::RN_STRING, 1)

#undef RN_BUILTIN_FUNC
#define RN_BUILTIN_FUNC RN_BUILTIN_FUNC_DECLARE

class RnBuiltins_IO {
public:
    RN_BUILTIN_IO_REGISTRATIONS
};
