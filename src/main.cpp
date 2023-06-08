#include <fstream>
#include <iostream>
#include <set>
#include "codegen/RnBinary.h"
#include "codegen/RnCodeGenerator.h"
#include "common/RnInternment.h"
#include "lexer/Lexer.h"
#include "lexer/Token.h"
#include "parser/Parser.h"
#include "parser/RnAstValidator.h"
#include "util/ArgParser.h"
#include "util/File.h"
#include "util/StopWatch.h"
#include "util/String.h"
#include "util/log.h"
#include "vm/RnMemoryManager.h"
#include "vm/RnObject.h"
#include "vm/RnVirtualMachine.h"

// @formatter:off
#include "RnBuildInfo.h"

ArgParser arg_parser;

void Compile(const std::filesystem::path& infile, RnCodeGenerator& code_generator) {
    Lexer lexer;
    try {
        lexer.LoadFile(std::filesystem::absolute(infile));
        lexer.ProcessTokens();
    } catch (const std::exception& e) {
        Log::ERROR("Syntax Error: " + std::string(e.what()));
        return;
    }

    if (arg_parser.IsSet("-t")) {
        for (size_t i = 0; i < lexer.tokens.size(); i++) {
            Log::INFO(String::Pad(std::to_string(i), 6, ' ') +
                      lexer.tokens[i]->ToString());
        }
    }

    if (lexer.tokens.empty()) {
        return;
    }

    Parser parser;
    try {
        parser.working_dir = infile.parent_path();
        parser.file = infile;
        parser.SetFromPtr(lexer.tokens.data(), lexer.tokens.size());
        parser.AdvanceBuffer(2);
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

    try {
        auto stopwatch = StopWatch();
        stopwatch.Start();
        code_generator.Generate(parser.ast.get());
        stopwatch.Stop();
        if (arg_parser.IsSet("-p")) {
            size_t index = 0;
            for (auto& instruction : code_generator.GetInstructions()) {
                Log::INFO(String::Pad(std::to_string(index++), 6) +
                          instruction->ToString());
            }
        }
        //        Log::INFO("CodeGen Duration: " + std::to_string(stopwatch.Duration()));
    } catch (const std::exception& e) {
        Log::ERROR("Codegen Error: " + std::string(e.what()));
        return;
    }
}

/*****************************************************************************/
void Run(const InstructionBlock& instructions) {
    if (instructions.empty()) {
        return;
    }
    auto vm = RnVirtualMachine::GetInstance();
    try {
        vm->LoadInstructions(instructions);
        RnIntNative exit_code = vm->Run();
    } catch (const std::exception& e) {
        Log::ERROR("Runtime Error: " + std::string(e.what()));
        return;
    }
    delete vm;
}

/*****************************************************************************/
void Write(const std::filesystem::path& outfile, const InstructionBlock& instructions) {
    BinaryWriter writer(std::filesystem::absolute(outfile));
    writer.SetInstructions(instructions);
    writer.Write();
}

/*****************************************************************************/
void Read(const std::filesystem::path& infile, InstructionBlock& instructions) {
    BinaryReader reader(std::filesystem::absolute(infile));
    reader.Read(instructions);
}

/*****************************************************************************/
void PrintInstructions(const InstructionBlock& instructions) {
    if (arg_parser.IsSet("-p")) {
        size_t index = 0;
        for (auto& instruction : instructions) {
            Log::INFO(String::Pad(std::to_string(index++), 6) +
                      instruction->ToString());
        }
    }
}

/*****************************************************************************/
void Repl() {
    std::cout << "RonaScript (" << std::string(RONASCRIPT_VERSION) << ")" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::string line;
    Lexer lexer;
    Parser parser;
    RnCodeGenerator code_generator;
    auto vm = RnVirtualMachine::GetInstance();

    while (true) {
        line.clear();
        std::cout << ">>> ";
        std::cin >> line;

        if (line.empty()) {
            std::cout << std::endl;
            continue;
        }

        if (line == "q") {
            break;
        } else {
            try {
                lexer.SetFromPtr(line.c_str(), line.length());
                lexer.AdvanceBuffer(2);
                lexer.ProcessTokens();

                parser.SetFromPtr(lexer.tokens.data(), lexer.tokens.size());
                parser.AdvanceBuffer(2);
                parser.Parse();

                code_generator.Generate(parser.ast.get());
                vm->LoadInstructions(code_generator.GetInstructions());
                vm->Run();
                parser.Reset();
                lexer.Reset();
            } catch (const std::exception& e) {
                Log::ERROR(e.what());
            }
            //            if (!vm->GetStack().empty()) {
            //                std::cout << "\n" << vm->GetStack().back()->ToString() << std::endl;
            //            }
        }
    }
}

/*****************************************************************************/
void RonaScriptMain(int argc, char* argv[]) {

    arg_parser.SetMainDescription("Usage: RonaScript <file> [options...]");
    arg_parser.AddArgument("<file>", {}, "Input file (*.rn | *.rnc)");
    arg_parser.AddArgument("-c", {}, "Compile to *.rnc file");
    arg_parser.AddArgument("--repl", {}, "REPL");
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

    if (arg_parser.IsSet("--repl")) {
        Repl();
        return;
    }

    std::filesystem::path const file = arg_parser.GetInputFile();
    if (!std::filesystem::exists(file)) {
        Log::ERROR("RonaScript: Error: Input file does not exist: " + file.string());
        return;
    }
    if (std::filesystem::file_size(file) == 0) {
        return;
    }

    InstructionBlock instructions;

    if (file.extension() == ".rnc") {
        Read(file, instructions);
        //        PrintInstructions(instructions);
        Run(instructions);
        return;
    } else {
        RnCodeGenerator code_generator;
        if (arg_parser.IsSet("-c")) {
            Compile(file, code_generator);
            instructions = code_generator.GetInstructions();
            Write(std::filesystem::path(file.string() + "c"), instructions);
        }
        if (arg_parser.IsSet("-r")) {
            Run(instructions);
        }
        //        PrintInstructions(instructions);
    }
}

/*****************************************************************************/
int main(int argc, char* argv[]) {
    RonaScriptMain(argc, argv);

    return 0;
}
