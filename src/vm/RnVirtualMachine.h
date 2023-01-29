/*****************************************************************************
* File: RnVirtualMachine.h
* Description:
* Author: Malcolm Hall
* Date: 6/19/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RNVIRTUALMACHINE_H
#define RONASCRIPT_RNVIRTUALMACHINE_H

#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include "RnScope.h"
#include "RnInternment.h"
#include "../util/RnStack.h"
#include "../codegen/RnInstruction.h"

class RnObject;
class RnFunctionObject;
class RnMemoryManager;


class RnVirtualMachine
{
 public:
	enum State
	{
		FUNCTION_CALL,
		CLASS_DECLARATION,
		GENERAL_EXECUTION,
	};

	~RnVirtualMachine();

	RnScope* GetScope()
	{
		return _scopes.back();
	}

	[[nodiscard]] std::vector<RnObject*>& GetStack()
	{
		return _scopes.back()->GetStack();
	}

	void CallFunction(RnFunctionObject* obj, uint32_t arg_cnt);
	void AddScope();
	RnIntNative Run();
	void LoadInstructions(std::vector<RnInstruction*> instructions);
	static RnVirtualMachine* GetInstance();
	RnObject* CreateObject(RnType::Type type);
	RnObject* CreateObject(RnStringNative data);
	RnObject* CreateObject(RnBoolNative data);
	RnObject* CreateObject(RnIntNative data);
	RnObject* CreateObject(RnFloatNative data);

 private:
	inline void ExecuteInstruction(RnInstruction* instruction, bool& break_scope,
		size_t& index);
	void RegisterBuiltins();
	RnVirtualMachine();

 private:
	static RnVirtualMachine* _instance;

 protected:
	RnInternment<std::string> _string_internment;
	RnInternment<double> _float_internment;
	RnInternment<long> _int_internment;
	std::vector<RnScope*> _scopes;
	std::vector<RnScope*> _call_stack;
	std::unordered_map<std::string, RnScope*> _namespaces;
	std::vector<RnInstruction*> _instructions;
	RnMemoryManager* _memory_manager;
	size_t i_idx = 0;
	size_t _gc_count = 0;
};

#endif //RONASCRIPT_RNVIRTUALMACHINE_H