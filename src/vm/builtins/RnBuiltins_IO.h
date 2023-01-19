/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_VM_BUILTINS_RNBUILTINS_IO_H_
#define RONASCRIPT_VM_BUILTINS_RNBUILTINS_IO_H_

#include <vector>

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

class RnBuiltins_IO
{
 public:
	static void file_size(RnScope* scope, const std::vector<RnObject*>& args,
		RnObject* ret_val);
	static void file_write(RnScope* scope, const std::vector<RnObject*>& args,
		RnObject* ret_val);
	static void rn_builtin_print(RnScope* scope, const std::vector<RnObject*>& args,
		RnObject* ret_val);
	static void read(RnScope* scope, const std::vector<RnObject*>& args,
		RnObject* ret_val);
	static void rn_builtin_prompt(RnScope* scope, const std::vector<RnObject*>& args,
		RnObject* ret_val);
};

#endif //RONASCRIPT_VM_BUILTINS_RNBUILTINS_IO_H_