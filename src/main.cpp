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
            size_t index = 0;
            for (auto& instruction : code_generator.GetResult()) {
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

/**
 * @brief Runs the given instructions.
 *
 * This function executes a series of instructions provided in the `InstructionBlock`.
 * It first checks if the `instructions` vector is empty, and if so, it simply returns.
 * Otherwise, it gets an instance of the `RnVirtualMachine` using the `GetInstance` method.
 * It then loads the instructions into the virtual machine, creates an argument vector,
 * fills it with program arguments obtained from `arg_parser`, and sets it using the `SetArgv` method.
 * The virtual machine is then executed by calling the `Run` method on it and the exit
 * code is stored in a variable. If any exceptions occur during the execution, a runtime
 * error message is logged using the `ERROR` method. Finally, the instance of the virtual
 * machine is deleted.
 *
 * @param instructions The block of instructions to be executed.
 */
void Run(const InstructionBlock& instructions) {
    if (instructions.empty()) {
        return;
    }
    auto vm = RnVirtualMachine::GetInstance();
    try {
        vm->LoadInstructions(instructions);
        RnArrayNative argv;
        auto argv_strings = arg_parser.GetProgramArguments();
        argv.reserve(argv_strings.size());
        std::transform(
            argv_strings.begin(), argv_strings.end(), std::back_inserter(argv),
            [](const std::string& argument) { return RnObject::Create(argument); });
        RnConfig::SetArgv(argv);
        RnIntNative exit_code = vm->Run();
    } catch (const std::exception& e) {
        Log::ERROR("Runtime Error: " + std::string(e.what()));
        return;
    }
    delete vm;
}

/**
 * @brief Writes the given set of instructions to a binary file.
 *
 * This function takes a file path and a set of instructions and writes them to a binary file.
 * It creates a `BinaryWriter` object with the provided file path, sets the instructions using
 * the `SetInstructions` method, and then calls the `Write` method to write the instructions to the file.
 *
 * @param outfile The file path to write the instructions to.
 * @param instructions The set of instructions to write.
 */
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
                lexer.Run();

                parser.SetInput(lexer.tokens.data(), lexer.tokens.size());
                parser.Run();

                code_generator.Generate(parser.ast.get());
                vm->LoadInstructions(code_generator.GetResult());
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

    arg_parser.SetMainDescription(
        "Usage: RonaScript [options...] <file> [arguments...]");
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

    InstructionBlock instructions;

    if (file.extension() == ".rnc") {
        Read(file, instructions);
        //        PrintInstructions(instructions);
        Run(instructions);
        return;
    } else {
        RnCodeGenerator code_generator;
        Compile(file, code_generator);
        instructions = code_generator.GetResult();
        if (arg_parser.IsSet("-c")) {
            Write(std::filesystem::path(file.string() + "c"), instructions);
        }
        if (!arg_parser.IsSet("-r")) {
            Run(instructions);
        }
        //        PrintInstructions(instructions);
    }
}

#include "vm/RnBoolObject.h"
#include "vm/RnMemoryManager.h"
/*****************************************************************************/
int main(int argc, char* argv[]) {
    RonaScriptMain(argc, argv);
    return 0;
}
