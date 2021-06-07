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

#include "virtual_machine.h"
#include "op_codes.h"
#include "builtins.h"
#include "rona_null.h"
#include <memory>

/******************************************************************************
 * @brief
 */
VirtualMachine::VirtualMachine() {
    this->global_scope = new Scope();
    this->global_scope->parent = nullptr;
    this->curr_scope = this->global_scope;
    this->memory = Memory::get_instance();
    //RONA_NULL_VALUE = new RonaObject(RN_TYPE_NULL);
    this->cls_construct_method_name = new RonaObject(static_cast<std::string>("construct"));
    this->cls_construct_method_name->set_type(RN_TYPE_STRING);
}

/******************************************************************************
 * @brief
 */
VirtualMachine::~VirtualMachine() {

}

/******************************************************************************
 * @brief
 */
void VirtualMachine::run() {
    this->i_cnt = this->instructions.size();

    while (this->i_idx < this->i_cnt) {

        if (this->global_scope->mem_if->memory->heap_size_bytes() > 1000) {
//            std::cout << "Cleaning up.. Memory Size: " << this->global_scope->mem_if->memory->heap_size_bytes();
//            gc_mark();
//            this->global_scope->mem_if->memory->sweep();
//            std::cout << ".. Memory Size After: " << this->global_scope->mem_if->memory->heap_size_bytes() << std::endl;
        }

        if (this->exit_flag) {
            break;
        }

        // Execute the instruction and increment the instruction index
        execute(instructions[this->i_idx]);
        this->i_idx++;
    }
}

/******************************************************************************
 * @brief
 * @param instruction
 */
void VirtualMachine::execute(Instruction *instruction) {
    //std::cout << instruction->to_string() << std::endl;
    for (auto x : instruction->operands) {
        this->memory->add_to_heap(x); // Push to heap so it's handled by the garbage collector
        this->curr_scope->stack.emplace_back(x);
    }

    switch (instruction->opcode) {
        case OP_BINARY_ADD:
            op_binary_add();
            break;
        case OP_BINARY_SUB:
            op_binary_sub();
            break;
        case OP_BINARY_MUL:
            op_binary_mul();
            break;
        case OP_BINARY_DIV:
            op_binary_div();
            break;
        case OP_BINARY_MOD:
            op_binary_mod();
            break;
        case OP_BINARY_GTE:
            op_binary_gte();
            break;
        case OP_BINARY_LTE:
            op_binary_lte();
            break;
        case OP_BINARY_GT:
            op_binary_gt();
            break;
        case OP_BINARY_LT:
            op_binary_lt();
            break;
        case OP_BINARY_EQ:
            op_binary_eq();
            break;
        case OP_STORE:
            op_store();
            break;
        case OP_LOAD_LITERAL:
            op_load_literal();
            break;
        case OP_LOAD_NULL:
            op_load_null();
            break;
        case OP_LOAD_VAR:
            op_load_var();
            break;
        case OP_CALL_FUNC:
            op_call_func();
            break;
        case OP_MAKE_VAR:
            op_make_var();
            break;
        case OP_MAKE_CONST:
            op_make_const();
            break;
        case OP_MAKE_CLASS:
            op_make_class();
            break;
        case OP_MAKE_FUNC:
            op_make_func();
            break;
        case OP_CREATE_CONTEXT:
            op_create_context();
            break;
        case OP_DESTROY_CONTEXT:
            op_destroy_context();
            break;
        case OP_BINARY_RSH:
            op_binary_rsh();
            break;
        case OP_BINARY_LSH:
            op_binary_lsh();
            break;
        case OP_BINARY_OR:
            op_binary_or();
            break;
        case OP_BINARY_XOR:
            op_binary_xor();
            break;
        case OP_BINARY_AND:
            op_binary_and();
            break;
        case OP_DELETE:
            op_delete();
            break;
        case OP_POP:
            op_pop();
            break;
        case OP_UNARY_NOT:
            op_unary_not();
            break;
        case OP_BREAK:
            op_break();
            break;
        case OP_UNARY_INVERT:
            unary_invert();
            break;
        case OP_NOP:
            op_nop();
            break;
        case OP_JUMP:
            op_jump();
            break;
        case OP_COMPARE:
            op_compare();
            break;
        case OP_MAKE_MODULE:
            op_make_module();
            break;
        case OP_EXIT:
            op_exit();
            break;
        case OP_BINARY_POWER:
            op_binary_power();
            break;
        case OP_LOGICAL_OR:
            op_logical_or();
            break;
        case OP_LOGICAL_AND:
            op_logical_and();
            break;
        case OP_CONTINUE:
            op_continue();
            break;
        case OP_TRY_CONTEXT:
            break;
        case OP_UNARY_DECREMENT:
            op_unary_decrement();
            break;
        case OP_UNARY_INCREMENT:
            op_unary_increment();
            break;
        case OP_RETURN:
            break;
        case OP_ARRAY_STORE:
            op_array_store();
            break;
        case OP_ARRAY_ACCESS:
            op_array_access();
            break;
        case OP_MAKE_ARG:
            op_make_arg();
            break;
        case OP_BUILD_ARRAY:
            op_build_array();
            break;
        case OP_MAKE_ALIAS:
            op_make_alias();
            break;
    }
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_binary_add() {
    RonaObject *b = this->curr_scope->pop_from_stack();
    RonaObject *a = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a + b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_binary_sub() {
    RonaObject *b = this->curr_scope->pop_from_stack();
    RonaObject *a = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a - b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_binary_mul() {
    RonaObject *b = this->curr_scope->pop_from_stack();
    RonaObject *a = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a * b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_binary_div() {
    RonaObject *b = this->curr_scope->pop_from_stack();
    RonaObject *a = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a / b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_binary_mod() {
    RonaObject *b = this->curr_scope->pop_from_stack();
    RonaObject *a = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a % b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_binary_gte() {
    RonaObject *b = this->curr_scope->pop_from_stack();
    RonaObject *a = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a >= b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_binary_lte() {
    RonaObject *b = this->curr_scope->pop_from_stack();
    RonaObject *a = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a <= b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_binary_gt() {
    RonaObject *b = this->curr_scope->pop_from_stack();
    RonaObject *a = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a > b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_binary_lt() {
    RonaObject *b = this->curr_scope->pop_from_stack();
    RonaObject *a = this->curr_scope->pop_from_stack();

    this->curr_scope->stack.push_back(*a < b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_binary_eq() {
    RonaObject *b = this->curr_scope->pop_from_stack();
    RonaObject *a = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a == b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_binary_rsh() {
    RonaObject *a = this->curr_scope->pop_from_stack();
    RonaObject *b = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a >> b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_binary_lsh() {
    RonaObject *a = this->curr_scope->pop_from_stack();
    RonaObject *b = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a << b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_binary_or() {
    RonaObject *a = this->curr_scope->pop_from_stack();
    RonaObject *b = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a | b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_binary_xor() {
    RonaObject *a = this->curr_scope->pop_from_stack();
    RonaObject *b = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a ^ b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_binary_and() {
    RonaObject *a = this->curr_scope->pop_from_stack();
    RonaObject *b = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a & b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_store() {
    RonaObject *var_id = this->curr_scope->pop_from_stack();
    RonaObject *value = this->curr_scope->pop_from_stack();

    this->curr_scope->set(var_id, value);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_load_literal() {
    // This would be redundant as the value would already be on the stack
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_load_null() {
    this->curr_scope->stack.emplace_back(new RonaObject(RN_TYPE_NULL));
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_load_var() {
    RonaObject *var_id = this->curr_scope->pop_from_stack();
    RonaObject *value = this->curr_scope->get(var_id);

    this->curr_scope->stack.emplace_back(value);
}

/******************************************************************************
 * @brief
 * @param id
 * @param arg_cnt
 * @return
 */
RonaObject *VirtualMachine::call_func_by_name(RonaObject *id, int arg_cnt) {
    RonaObject *retval = nullptr;
    int current_idx = this->i_idx;

    if (this->curr_scope->mem_if->exists(id)) {
        auto func_scope = make_scope();
        RonaFunction *func = this->curr_scope->get(id)->as_function();

        if (arg_cnt != func->args.size()) {
            // TODO: Throw function call error exception
        }

        // Load the argument values
        for (auto &arg : func->args) {
            RonaObject *arg_value = this->curr_scope->pop_from_stack();

            if (arg.second != arg_value->type()) {
                // TODO: Throw argument type error
            } else {
                RonaObject *arg_var = func_scope->mem_if->make_obj(arg.first, arg.second);
                arg_var->is_const = false;
                func_scope->mem_if->set(arg.first, arg_value);
            }
        }

        run_in_scope(func_scope, func->i_start, func->i_start + func->i_cnt);

        // Handle class constructor methods
        if (func->ret_type == RN_TYPE_CLASS_CONSTRUCTOR) {
            auto cls = new RonaClass();
            auto ctr = this->memory->obj_malloc();
            ctr->set_type(RN_TYPE_CLASS);
            ctr->data.emplace_back(cls);
            cls->scope = func_scope;
            cls->set_name(id);

            // Run the constructor method
            RonaObject *construct_ctr = cls->scope->get(this->cls_construct_method_name);
            RonaFunction *construct_method = construct_ctr->as_function();
            construct_method->scope = make_scope();
            construct_method->scope->parent = cls->scope;

            // Load the argument values
            for (auto &arg : construct_method->args) {
                RonaObject *arg_value = this->curr_scope->pop_from_stack();

                if (arg.second != arg_value->type()) {
                    // TODO: Throw argument type error
                } else {
                    RonaObject *arg_var = construct_method->scope->mem_if->make_obj(arg.first, arg.second);
                    arg_var->is_const = false;
                    construct_method->scope->mem_if->set(arg.first, arg_value);
                }
            }
            run_in_scope(construct_method->scope, construct_method->i_start, construct_method->i_start + construct_method->i_cnt);

            retval = ctr;
        } else {
            retval = func_scope->pop_from_stack();

            // Remove the scope from the child scopes vector
            for (int i = 0; i < this->child_scopes.size(); i++) {
                if (this->child_scopes[i] == func_scope) {
                    this->child_scopes.erase(this->child_scopes.begin() + i);
                    break;
                }
            }
            delete_scope(func_scope);
        }

    } else if (this->builtinMethodManager.exists(id)) {
        std::vector<RonaObject*> args;
        args.reserve(arg_cnt);

        for (int i = 0; i < arg_cnt; i++) {
            args.insert(args.begin(), this->curr_scope->pop_from_stack());
        }
        retval = this->builtinMethodManager.call_builtin_function(id, args);
        this->memory->add_to_heap(retval);
    }

    this->i_idx = current_idx;

    return retval;
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_call_func() {
    RonaObject *arg_cnt = this->curr_scope->pop_from_stack();
    RonaObject *func_name = this->curr_scope->pop_from_stack();
    RonaObject *retval = call_func_by_name(func_name, arg_cnt->to_int());
    this->curr_scope->stack.emplace_back(retval);
}

void VirtualMachine::run_in_scope(Scope *scope, long i_start, long i_end)
{
    Scope *previous_scope = this->curr_scope;
    this->curr_scope = scope;
    int previous_idx = this->i_idx;

    for (long i = i_start; i < i_end; i++) {
        std::cout << this->instructions[i]->to_string() << std::endl;
        execute(this->instructions[i]);

        // Instruction index changes only affect the global index so we need to handle
        // those here
        switch (this->instructions[i]->opcode) {
            case OP_JUMP:
            case OP_BREAK:
            case OP_CONTINUE:
                i += this->instructions[i]->operands[0]->to_int();
            case OP_MAKE_FUNC:
                {
                    // Correct the function start index
                    RonaObject *func_id = this->instructions[i]->operands[this->instructions[i]->operands.size() - 4];
                    RonaFunction *func = scope->get(func_id)->as_function();
                    func->i_start = i;
                    i += this->instructions[i]->operands[this->instructions[i]->operands.size() - 2]->to_int() - 1;
                }
            default:
                break;
        }
    }

    this->i_idx = previous_idx;
    this->curr_scope = previous_scope;
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_make_var() {
    RonaObject *id = this->curr_scope->pop_from_stack();
    RonaType_t type = get_type(this->curr_scope->pop_from_stack()->to_string());
    this->curr_scope->mem_if->make_obj(id, type);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_make_class() {
    // We just need the expr and number of instructions to declare a class.
    // When the constructor is called the class will be setup.
    RonaObject *class_i_cnt = this->curr_scope->pop_from_stack();
    RonaObject *class_name = this->curr_scope->pop_from_stack();
    RonaObject *obj = this->curr_scope->mem_if->make_obj(class_name, RN_TYPE_FUNCTION);
    obj->data.emplace_back(new RonaFunction());
    RonaFunction *func = obj->as_function();
    func->i_start = this->i_idx + 1;
    func->i_cnt = class_i_cnt->to_int();
    func->ret_type = RN_TYPE_CLASS_CONSTRUCTOR;
    this->i_idx += func->i_cnt;
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_make_func() {
    RonaObject *func_arg_cnt = this->curr_scope->pop_from_stack();
    RonaObject *func_i_cnt = this->curr_scope->pop_from_stack();
    RonaType_t ret_type = this->curr_scope->pop_from_stack()->type();
    RonaObject *id = this->curr_scope->pop_from_stack();

    RonaObject *obj = this->curr_scope->mem_if->make_obj(id, RN_TYPE_FUNCTION);
    obj->data.emplace_back(new RonaFunction());
    RonaFunction *func = obj->as_function();
    func->i_start = this->i_idx + 1;
    func->i_cnt = func_i_cnt->to_int();
    func->ret_type = ret_type;
    this->i_idx += func->i_cnt;

    for (int i = 0; i < func_arg_cnt->to_int(); i++) {
        RonaType_t arg_type = get_type(this->curr_scope->pop_from_stack()->to_string());
        func->args.emplace_back(std::make_pair(this->curr_scope->pop_from_stack(), arg_type));
    }
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_create_context() {
    auto scope = make_scope();
    scope->parent = this->curr_scope;
    this->curr_scope = scope;
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_destroy_context() {
    Scope *scope = this->curr_scope;
    this->curr_scope = this->curr_scope->parent;
    delete_scope(scope);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_delete() {
    RonaObject *id = this->curr_scope->pop_from_stack();
    this->curr_scope->mem_if->remove(id);
}

/******************************************************************************
 * @brief Function to handle the OP_POP opcode
 */
void VirtualMachine::op_pop() {
    delete this->curr_scope->pop_from_stack();
}

/******************************************************************************
 * @brief Function to handle the OP_MAKE_ALIAS opcode
 */
void VirtualMachine::op_make_alias() {
    RonaObject *base = this->curr_scope->pop_from_stack();
    RonaObject *alias = this->curr_scope->pop_from_stack();
    this->curr_scope->mem_if->sym_tbl[alias->to_string()] = this->curr_scope->get(base);
}

/******************************************************************************
 * @brief Function to handle the OP_UNARY_NOT opcode
 */
void VirtualMachine::op_unary_not() {

}

/******************************************************************************
 * @brief Function to handle the OP_BREAK opcode
 */
void VirtualMachine::op_break() {
    RonaObject *rel_idx = this->curr_scope->pop_from_stack();
    this->i_idx += rel_idx->to_int();
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_continue() {
    RonaObject *rel_idx = this->curr_scope->pop_from_stack();
    this->i_idx += rel_idx->to_int();
}

/******************************************************************************
 * @brief Function to handle the OP_UNARY_INVERT opcode
 */
void VirtualMachine::unary_invert() {

}

/******************************************************************************
 * @brief Function to handle the OP_NOP opcode
 */
void VirtualMachine::op_nop() {

}

/******************************************************************************
 * @brief Function to handle the OP_JUMP opcode
 */
void VirtualMachine::op_jump() {
    RonaObject *rel_idx = this->curr_scope->pop_from_stack();
    this->i_idx += rel_idx->to_int();
}

/******************************************************************************
 * @brief Function to handle the OP_COMPARE opcode
 */
void VirtualMachine::op_compare() {
    RonaObject *rel_idx = this->curr_scope->pop_from_stack();
    RonaObject *value = this->curr_scope->pop_from_stack();

    if (!value->to_bool()) {
        this->i_idx += rel_idx->to_int();
    }
}

/******************************************************************************
 * @brief Function to handle the OP_MAKE_MODULE opcode
 */
void VirtualMachine::op_make_module() {

}

/******************************************************************************
 * @brief Function to handle the OP_EXIT opcode
 */
void VirtualMachine::op_exit() {
    this->exit_flag = true;
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_make_const() {
    RonaObject *value = this->curr_scope->pop_from_stack();
    RonaObject *id = this->curr_scope->pop_from_stack();
    RonaType_t type = get_type(this->curr_scope->pop_from_stack()->to_string());
    RonaObject *obj = this->curr_scope->mem_if->make_obj(id, type);
    this->curr_scope->mem_if->set(id, value);
    obj->is_const = true;
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_array_store() {
    RonaObject *id = this->curr_scope->pop_from_stack();
    int offset = this->curr_scope->pop_from_stack()->to_int();
    RonaObject *value = this->curr_scope->pop_from_stack();
    RonaObject *array = this->curr_scope->get(id);
    array->data[offset] = value;
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_array_access() {
    RonaObject *array = this->curr_scope->pop_from_stack();
    int offset = this->curr_scope->pop_from_stack()->to_int();
    this->curr_scope->stack.push_back(std::get<RonaObject*>(array->data[offset]));
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_make_arg() {

}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_build_array() {
    RonaObject *item_cnt = this->curr_scope->pop_from_stack();
    auto array = new RonaObject(RN_TYPE_ARRAY);

    for (long i = 0; i < item_cnt->to_int(); i++) {
        array->data.emplace_back(this->curr_scope->pop_from_stack());
    }

    this->curr_scope->stack.push_back(array);
}


/******************************************************************************
 * @brief Mark all RonaObject instances
 * @return
 */
void VirtualMachine::gc_mark() {
    for (auto &[key, object] : this->global_scope->mem_if->sym_tbl) {
        object->gc_flag = true;
    }

    for (auto &scope : this->child_scopes) {
        for (auto &[key, object] : scope->mem_if->sym_tbl) {
            object->gc_flag = true;
        }
    }

}

/******************************************************************************
 * @brief
 * @return
 */
Scope *VirtualMachine::make_scope() {
    auto scope = new Scope();
    this->child_scopes.emplace_back(scope);

    return scope;
}

void VirtualMachine::op_logical_and() {

}

void VirtualMachine::op_binary_power() {

}

void VirtualMachine::op_logical_or() {

}

void VirtualMachine::op_unary_decrement() {

}

void VirtualMachine::op_unary_increment() {

}

void VirtualMachine::delete_scope(Scope *scope) {
    for (int i = 0; i < this->child_scopes.size(); i++) {
        if (this->child_scopes[i] == scope) {
            this->child_scopes.erase(this->child_scopes.begin() + i);
            break;
        }
    }

    delete scope;
}
