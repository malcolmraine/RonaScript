/*****************************************************************************
* File: RnBuiltins_IO.cpp
* Description:
* Author: Malcolm Hall
* Date: 2/1/22
* Version: 1
*
******************************************************************************/

#include "RnBuiltins_IO.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include "../vm/RnObject.h"
#include "../vm/RnScope.h"

/*****************************************************************************/
void RnBuiltins_IO::rn_builtin_file_size(RnScope* scope,
                                         const std::vector<RnObject*>& args,
                                         RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
    std::string path = args.front()->ToString();
    ret_val->SetData(static_cast<RnIntNative>(std::filesystem::file_size(path)));
}

/*****************************************************************************/
void RnBuiltins_IO::rn_builtin_file_write(RnScope* scope,
                                          const std::vector<RnObject*>& args,
                                          RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
}

/*****************************************************************************/
void RnBuiltins_IO::rn_builtin_print(RnScope* scope, const std::vector<RnObject*>& args,
                                     RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
    std::string s;
    for (auto arg : args) {
        assert(arg);
        s.append(arg->ToString());
    }
    printf("%s\n", s.c_str());
}

/*****************************************************************************/
void RnBuiltins_IO::rn_builtin_read(RnScope* scope, const std::vector<RnObject*>& args,
                                    RnObject* ret_val) {
    assert(ret_val);
    assert(scope);
    std::ifstream file;
    file.open(args.front()->ToString());

    std::string contents;
    file.seekg(0, std::ios::end);
    contents.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    contents.assign((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
    file.close();
    ret_val->SetData(contents);
}

/*****************************************************************************/
void RnBuiltins_IO::rn_builtin_prompt(RnScope* scope,
                                      const std::vector<RnObject*>& args,
                                      RnObject* ret_val) {
    std::string input;
    std::cout << args.front()->ToString();
    std::cin >> input;
    ret_val->SetData(input);
}