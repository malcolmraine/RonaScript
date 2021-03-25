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
#include <memory>

/******************************************************************************
 * @brief
 */
VirtualMachine::VirtualMachine() {
    this->global_scope = new Scope();
    this->global_scope->parent = nullptr;
    this->curr_scope = this->global_scope;
}

/******************************************************************************
 * @brief
 */
VirtualMachine::~VirtualMachine() {
    cleanup();
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::run() {
    this->i_cnt = this->instructions.size();

    while (this->i_idx < this->i_cnt) {
        // Every n instructions we perform garbage collection
        if (this->i_idx - this->last_cleanup_idx >= this->cleanup_intvl) {
            cleanup();
            this->last_cleanup_idx = this->i_idx;
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
    for (auto x : instruction->operands) {
        this->curr_scope->stack.emplace_back(x);
    }

    switch (instruction->opcode) {
        case OP_ADD:
            op_add();
            break;
        case OP_SUB:
            op_sub();
            break;
        case OP_MUL:
            op_mul();
            break;
        case OP_DIV:
            op_div();
            break;
        case OP_MOD:
            op_mod();
            break;
        case OP_GTE:
            op_gte();
            break;
        case OP_LTE:
            op_lte();
            break;
        case OP_GT:
            op_gt();
            break;
        case OP_LT:
            op_lt();
            break;
        case OP_EQ:
            op_eq();
            break;
        case OP_MOV:
            op_mov();
            break;
        case OP_LDL:
            op_ldl();
            break;
        case OP_LDV:
            op_ldv();
            break;
        case OP_LDF:
            op_ldf();
            break;
        case OP_MKV:
            op_mkv();
            break;
        case OP_MKC:
            op_mkc();
            break;
        case OP_MKF:
            op_mkf();
            break;
        case OP_CTX:
            op_ctx();
            break;
        case OP_RSH:
            op_rsh();
            break;
        case OP_LSH:
            op_lsh();
            break;
        case OP_OR:
            op_or();
            break;
        case OP_XOR:
            op_xor();
            break;
        case OP_AND:
            op_and();
            break;
        case OP_DEL:
            op_del();
            break;
    }
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_add() {
    RonaObject *a = this->curr_scope->pop_from_stack();
    RonaObject *b = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a + b);
    delete_operands(a, b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_sub() {
    RonaObject *a = this->curr_scope->pop_from_stack();
    RonaObject *b = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a - b);
    delete_operands(a, b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_mul() {
    RonaObject *a = this->curr_scope->pop_from_stack();
    RonaObject *b = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a * b);
    delete_operands(a, b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_div() {
    RonaObject *a = this->curr_scope->pop_from_stack();
    RonaObject *b = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a / b);
    delete_operands(a, b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_mod() {
    RonaObject *a = this->curr_scope->pop_from_stack();
    RonaObject *b = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a % b);
    delete_operands(a, b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_gte() {
    RonaObject *a = this->curr_scope->pop_from_stack();
    RonaObject *b = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a >= b);
    delete_operands(a, b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_lte() {
    RonaObject *a = this->curr_scope->pop_from_stack();
    RonaObject *b = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a <= b);
    delete_operands(a, b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_gt() {
    RonaObject *a = this->curr_scope->pop_from_stack();
    RonaObject *b = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a > b);
    delete_operands(a, b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_lt() {
    RonaObject *a = this->curr_scope->pop_from_stack();
    RonaObject *b = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a < b);
    delete_operands(a, b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_eq() {
    RonaObject *a = this->curr_scope->pop_from_stack();
    RonaObject *b = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a == b);
    delete_operands(a, b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_mov() {
    RonaObject *var_id = this->curr_scope->pop_from_stack();
    RonaObject *value = this->curr_scope->pop_from_stack();
    this->curr_scope->set(var_id, value);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_ldl() {
    // This would be redundant as the value would already be on the stack
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_ldv() {
    RonaObject *var_id = this->curr_scope->pop_from_stack();
    RonaObject *value = this->curr_scope->get_obj(var_id);
    this->curr_scope->stack.push_back(value);
    delete_operands(var_id);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_ldf() {
    RonaObject *arg_cnt = this->curr_scope->pop_from_stack();
    RonaObject *func_name = this->curr_scope->pop_from_stack();
    RonaObject *func_ctr = this->curr_scope->get_obj(func_name); // Get the function container object
    RonaFunction *func_obj = func_ctr->as_function();

    if (func_obj == nullptr && is_builtin_func(func_name)) {
        // Run the builtin function using the given arguments
        std::vector<RonaObject *> args;

        for (long i = 0; i < arg_cnt->to_int(); i++) {
            args.push_back(this->curr_scope->pop_from_stack());
        }

        call_builtin_function(this->curr_scope, func_name, args);
    } else if (func_obj != nullptr) {
        // Run the function from any of the accessible scopes
        // If the function is a class constructor, setup the class first.
        if (func_obj->ret_type == RONA_CLASS_CONSTRUCTOR) {
            // Run the class instantiation code
            run_in_scope(func_obj->scope, func_obj->i_start, func_obj->i_start + func_obj->i_cnt);

            // Create a new class instance from the function scope
            auto *cls = new RonaClass();
            cls->set_name(func_name);
            *cls->scope = *func_obj->scope;
            this->curr_scope->stack.push_back(new RonaObject(cls));

            // Setup to run the constructor function
            auto *constructor_id = new RonaObject("construct");
            func_obj = func_obj->scope->get_obj(constructor_id)->as_function();
            Memory::collect(constructor_id);
        }

        // Set the function arguments. These become local variables for the function
        for (long i = 0; i < arg_cnt->to_int(); i++) {
            func_obj->scope->set(func_obj->args[i], curr_scope->pop_from_stack());
        }

        run_in_scope(func_obj->scope, func_obj->i_start, func_obj->i_start + func_obj->i_cnt);
        this->i_idx++;

        // Move the return value to the output stack
        if (func_obj->ret_type != RONA_NULL) {
            this->curr_scope->stack.push_back(func_obj->scope->pop_from_stack());
        }

        // Cleanup the function scope so we can use it again the future
        func_obj->scope->reset();
        func_obj->scope = new Scope();
    }

    Memory::collect(func_ctr);
    Memory::collect(arg_cnt);
    Memory::collect(func_name);

}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_mkv() {
    RonaType_t type = get_type(this->curr_scope->pop_from_stack()->to_string());
    RonaObject *id = this->curr_scope->pop_from_stack();
    this->curr_scope->make_var(id, type);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_mkc() {
    // We just need the id and number of instructions to declare a class.
    // When the constructor is called the class will be setup.
    RonaObject *id = this->curr_scope->pop_from_stack();
    RonaObject *cls_i_cnt = this->curr_scope->pop_from_stack();
    RonaFunction *cls = this->curr_scope->memory->make_func(id);
    cls->i_start = this->i_idx;
    cls->i_cnt = cls_i_cnt->to_int();
    cls->ret_type = RONA_CLASS_CONSTRUCTOR;
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_mkf() {

    RonaObject *func_i_cnt = this->curr_scope->pop_from_stack();
    RonaObject *ret_type = this->curr_scope->pop_from_stack();
    RonaObject *id = this->curr_scope->pop_from_stack();
    RonaFunction *func = this->curr_scope->memory->make_func(id);
    func->i_start = this->i_idx + 1;
    func->i_cnt = func_i_cnt->to_int();
    func->ret_type = get_type(ret_type->to_string());
    this->i_idx += func->i_cnt;
    delete_operands(func_i_cnt, ret_type, id);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_ctx() {
    RonaObject *flag = this->curr_scope->pop_from_stack();
    Scope *ctx;

    if (flag->to_int() == 1) {
        // Create a new context
        ctx = new Scope();
        run_in_scope(ctx, this->i_idx, this->instructions.size());
    } else {
        // Destroy the context and move to the parent scope
        ctx = this->curr_scope;
        this->curr_scope = this->curr_scope->parent;
        delete ctx;
    }
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_rsh() {
    RonaObject *a = this->curr_scope->pop_from_stack();
    RonaObject *b = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a >> b);
    delete_operands(a, b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_lsh() {
    RonaObject *a = this->curr_scope->pop_from_stack();
    RonaObject *b = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a << b);
    delete_operands(a, b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_or() {
    RonaObject *a = this->curr_scope->pop_from_stack();
    RonaObject *b = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a | b);
    delete_operands(a, b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_xor() {
    RonaObject *a = this->curr_scope->pop_from_stack();
    RonaObject *b = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a ^ b);
    delete_operands(a, b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_and() {
    RonaObject *a = this->curr_scope->pop_from_stack();
    RonaObject *b = this->curr_scope->pop_from_stack();
    this->curr_scope->stack.push_back(*a & b);
    delete_operands(a, b);
}

/******************************************************************************
 * @brief
 */
void VirtualMachine::op_del() {
    RonaObject *id = this->curr_scope->pop_from_stack();
    this->curr_scope->memory->remove(id);
}

/******************************************************************************
 * @brief
 * @param a
 */
void VirtualMachine::delete_operands(RonaObject *a) {
    Memory::collect(a);
}

/******************************************************************************
 * @brief
 * @param a
 * @param b
 */
void VirtualMachine::delete_operands(RonaObject *a, RonaObject *b) {
    Memory::collect(a);
    Memory::collect(b);
}

/******************************************************************************
 * @brief
 * @param a
 * @param b
 * @param c
 */
void VirtualMachine::delete_operands(RonaObject *a, RonaObject *b, RonaObject *c) {
    Memory::collect(a);
    Memory::collect(b);
    Memory::collect(c);
}

/******************************************************************************
 * @brief
 * @param a
 * @param b
 * @param c
 * @param d
 */
void VirtualMachine::delete_operands(RonaObject *a, RonaObject *b, RonaObject *c, RonaObject *d) {
    Memory::collect(a);
    Memory::collect(b);
    Memory::collect(c);
    Memory::collect(d);
}

/******************************************************************************
 * @brief
 * @param a
 * @param b
 * @param c
 * @param d
 * @param e
 */
void VirtualMachine::delete_operands(RonaObject *a, RonaObject *b, RonaObject *c, RonaObject *d, RonaObject *e) {
    Memory::collect(a);
    Memory::collect(b);
    Memory::collect(c);
    Memory::collect(d);
    Memory::collect(e);
}

/******************************************************************************
 * @brief
 * @param id
 * @return
 */
bool VirtualMachine::is_builtin_func(RonaObject *id) {
    return false;
}

/******************************************************************************
 * @brief
 * @param scope
 * @param i_start
 * @param i_end
 */
void VirtualMachine::run_in_scope(Scope *scope, long i_start, long i_end) {
    Scope *previous_scope = this->curr_scope;
    this->curr_scope = scope;

    for (long i = i_start; i < i_end - i_start; i++) {
        execute(this->instructions[i]);
    }

    // Cleanup and revert back to the previous scope
    this->curr_scope->cleanup();
    this->curr_scope = previous_scope;
}

/******************************************************************************
 * @brief Garbage collection
 */
void VirtualMachine::cleanup() const {
    this->curr_scope->cleanup();
    this->global_scope->cleanup();
}

/******************************************************************************
 * @brief
 * @param instruction
 */
void VirtualMachine::load_instruction(Instruction *instruction) {
    this->instructions.emplace_back(instruction);
}
