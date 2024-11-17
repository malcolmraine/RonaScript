#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include "codegen/RnBinary.h"
#include "codegen/RnCodeGenerator.h"
#include "common/RnConfig.h"
#include "lexer/Lexer.h"
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

#include "codegen/RnCodeFrame.h"
#include "codegen/RnInstruction.h"
#include "vm/RnOpCode.h"

// @formatter:off
#include "common/RnBuildInfo.h"

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
        code_generator.SetInput(parser.ast.get());
        code_generator.Run();
        stopwatch.Stop();
        if (arg_parser.IsSet("-p")) {
            Log::INFO(code_generator.GetResult()->ToString());
        }
    } catch (const std::exception& e) {
        Log::ERROR("Codegen Error: " + std::string(e.what()));
        return;
    }
}

void Run(RnCodeFrame* frame) {
    if (!frame || !frame->GetInstructionCount()) {
        return;
    }
    auto vm = RnVirtualMachine::GetInstance();
    try {
        RnArrayNative argv;
        auto argv_strings = arg_parser.GetProgramArguments();
        argv.reserve(argv_strings.size());
        std::transform(
            argv_strings.begin(), argv_strings.end(), std::back_inserter(argv),
            [](const std::string& argument) { return RnObject::Create(argument); });
        RnConfig::SetArgv(argv);
        RnIntNative exit_code = vm->ExecuteCodeFrame(frame, vm->GetScope());
    } catch (const std::exception& e) {
        Log::ERROR("Runtime Error: " + std::string(e.what()));
        return;
    }
    delete vm;
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
        std::getline(std::cin, line);

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
                lexer.Run();

                parser.SetInput(lexer.tokens.data(), lexer.tokens.size());
                parser.Run();

                code_generator.Generate(parser.ast.get());
                vm->ExecuteCodeFrame(code_generator.GetResult(), vm->GetScope());
                parser.Reset();
                lexer.Reset();
            } catch (const std::exception& e) {
                Log::ERROR(e.what());
            }
        }
    }
}

/*****************************************************************************/
void RonaScriptMain(int argc, char* argv[]) {

    arg_parser.SetMainDescription(
        "Usage: RonaScript [options...] <file> [arguments...]");
    arg_parser.AddArgument("<file>", {}, "Input file (*.rn | *.rnc)");
    arg_parser.AddArgument("--repl", {}, "REPL");
    arg_parser.AddArgument("-c", {"--compile"},
                           "Compile to *.rnc file without running");
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
    } else if (arg_parser.IsSet("--repl")) {
        Repl();
        return;
    } else if (arg_parser.IsSet("-v")) {
        std::cout << "RonaScript " << std::string(RONASCRIPT_VERSION);
        return;
    } else if (arg_parser.GetInputFile().empty()) {
        Log::ERROR("RonaScript: Error: No input file");
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

    if (file.extension() == ".rnc") {
        auto frame = RnCodeFrame::CreateFromFile(file.string());
        Run(frame);
        return;
    } else {
        RnCodeGenerator code_generator;
        Compile(file, code_generator);
        auto frame = code_generator.GetResult();

        if (arg_parser.IsSet("--compile")) {
            frame->WriteToFile("matrix_test.rnc");
        } else {
            Run(frame);
        }
    }
}

#include "vm/RnBoolObject.h"
#include "vm/RnMemoryManager.h"
/*****************************************************************************/
int main(int argc, char* argv[]) {
    RonaScriptMain(argc, argv);
    return 0;
}
