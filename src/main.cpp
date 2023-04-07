#include <fstream>
#include <iostream>
#include <set>
#include "RnVersionInfo.h"
#include "codegen/RnCodeGenerator.h"
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "parser/RnAstValidator.h"
#include "util/ArgParser.h"
#include "util/String.h"
#include "util/log.h"
#include "vm/RnMemoryManager.h"
#include "vm/RnObject.h"
#include "vm/RnVirtualMachine.h"

/*****************************************************************************/
void RonaScriptMain(int argc, char* argv[]) {
    ArgParser arg_parser;
    arg_parser.SetMainDescription("Usage: RonaScript <file> [options...]");
    arg_parser.AddArgument("<file>", {}, "Input file (*.rn | *.rnc)");
    arg_parser.AddArgument("-c", {}, "Compile to *.rnc file");
    arg_parser.AddArgument("-r", {"--norun"}, "Compile to *.rnc file without running");
    arg_parser.AddArgument("--no-validation", {}, "Don't perform AST validation");
    arg_parser.AddArgument("-a", {"--print-ast"}, "Print AST after parsing");
    arg_parser.AddArgument("-t", {"--print-tokens"}, "Print tokens after lexing");
    arg_parser.AddArgument("-p", {"--print-opcodes"}, "Print opcodes after generation");
    arg_parser.AddArgument("-d", {"--debug"}, "Show various debug logging");
    arg_parser.AddArgument("-v", {"--version"}, "Show version information");
    arg_parser.AddArgument("-h", {"--help", "help"}, "Show help");
    arg_parser.Parse(argc, argv);

    if (arg_parser.IsSet("-h")) {
        arg_parser.ShowHelp();
        return;
    } else if (arg_parser.IsSet("-v")) {
        std::cout << "RonaScript " << std::string(RONASCRIPT_VERSION);
        return;
    } else if (arg_parser.GetInputFile().empty()) {
        Log::ERROR("RonaScript: Error: No input file");
        return;
    }

    std::filesystem::path const file = arg_parser.GetInputFile();
    std::filesystem::path const cfile = file.string() + "c";

    if (std::filesystem::file_size(file) == 0) {
        return;
    }

    Lexer lexer;
    try {
        lexer.LoadFile(file);
        lexer.ProcessTokens();
    } catch (const std::exception& e) {
        Log::ERROR("Syntax Error: " + std::string(e.what()));
        return;
    }

    if (arg_parser.IsSet("-t")) {
        for (auto& token : lexer.tokens) {
            Log::INFO(token->ToString());
        }
    }

    Parser parser;
    try {
        parser.working_dir = file.parent_path();
        parser.file = file;
        parser.LoadTokens(lexer.tokens);
        parser.Parse();

        if (arg_parser.IsSet("-a")) {
            Log::INFO(parser.DumpsAst());
        }

        if (!arg_parser.IsSet("--no-validation")) {
            RnAstValidator validator;
            validator.Visit(parser.ast->root.get());
        }
    } catch (const std::exception& e) {
        Log::ERROR("Parse Error: " + std::string(e.what()));
        return;
    }

    RnCodeGenerator code_generator;
    try {
        code_generator.Generate(parser.ast);
        if (arg_parser.IsSet("-p")) {
            size_t index = 0;
            for (auto& instruction : code_generator.GetInstructions()) {
                Log::INFO(String::Pad(std::to_string(index++), 6) +
                          instruction->ToString());
            }
        }
    } catch (const std::exception& e) {
        Log::ERROR("Codegen Error: " + std::string(e.what()));
        return;
    }

    std::ofstream ofs;
    ofs.open( cfile.c_str(), std::ios::binary);
    ofs.write("$$CONST$$", 9);
    for (size_t i = 1; i <= RnObject::object_internment->GetIndex(); i++) {
        auto object = RnObject::object_internment->GetInternedItem(i);
        char* buf = new char[object->GetByteSize()];
        object->GetBytes(buf);
        ofs.write(buf, object->GetByteSize());
        delete[] buf;
    }

    for (auto instruction : code_generator.GetInstructions()) {
        char buf[4] = {};
        buf[0] = static_cast<char>(instruction->GetOpcode());
        buf[1] = static_cast<char>(instruction->GetArg1());
        buf[2] = static_cast<char>(instruction->GetArg2());
        buf[3] = static_cast<char>(instruction->GetArg3());
        ofs.write(buf, 4);
    }
    ofs.close();

    auto vm = RnVirtualMachine::GetInstance();
    try {
        vm->LoadInstructions(code_generator.GetInstructions());
        RnIntNative exit_code = vm->Run();
    } catch (const std::exception& e) {
        Log::ERROR("Runtime Error: " + std::to_string(vm->GetIndex()) + ": " + std::string(e.what()));
        return;
    }

    delete vm;
}

/*****************************************************************************/
int main(int argc, char* argv[]) {
    RonaScriptMain(argc, argv);

    return 0;
}
