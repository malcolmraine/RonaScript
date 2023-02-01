#include <iostream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include <fstream>
#include "vm/RnMemoryManager.h"
#include "vm/RnObject.h"
#include "codegen/RnCodeGenerator.h"
#include "vm/RnVirtualMachine.h"
#include "util/MLib/String.h"
#include "util/ArgParser.h"
#include <set>
#include "RnVersionInfo.h"
#include "util/log.h"

/*****************************************************************************/
void RonaScriptMain(int argc, char* argv[])
{
	ArgParser arg_parser;
	arg_parser.SetMainDescription("Usage: RonaScript <file> [options...]");
	arg_parser.AddArgument("<file>", {}, "Input file (*.rn | *.rnc)");
	arg_parser.AddArgument("-c", {}, "Compile to *.rnc file");
	arg_parser.AddArgument("-norun", {}, "Compile to *.rnc file without running");
	arg_parser.AddArgument("-past", { "--print-ast" }, "Print AST after parsing");
	arg_parser.AddArgument("-ptok", { "--print-tokens" }, "Print tokens after lexing");
	arg_parser.AddArgument("-pcode",
		{ "--print-opcodes" },
		"Print opcodes after generation");
	arg_parser.AddArgument("-d", { "--debug" }, "Show various debug logging");
	arg_parser.AddArgument("-norun",
		{ "--print-opcodes" },
		"Compile to *.rnc file without running");
	arg_parser.AddArgument("-v", { "--version" }, "Show version information");
	arg_parser.AddArgument("-h", { "--help" }, "Show help");
	arg_parser.Parse(argc, argv);

	if (arg_parser.IsSet("-h"))
	{
		arg_parser.ShowHelp();
		return;
	}
	else if (arg_parser.IsSet("-v"))
	{
		std::cout << "RonaScript " << RONASCRIPT_VERSION << "\n";
		return;
	}
	else if (arg_parser.GetInputFile().empty())
	{
		Log::ERROR("RonaScript: Error: No input file");
		return;
	}

	std::filesystem::path const file = arg_parser.GetInputFile();
	std::filesystem::path const cfile = file.string() + "c";

	Lexer lexer;
	lexer.LoadFile(file);

	try
	{
		lexer.ProcessTokens();
	}
	catch (const std::exception& e)
	{
		Log::ERROR("Syntax Error: " + std::string(e.what()));
		return;
	}

	if (arg_parser.IsSet("-ptok"))
	{
		for (auto& token : lexer.tokens)
		{
			std::cout << token->ToString() << std::endl;
		}
	}

	Parser parser;
	parser.working_dir = file.parent_path();
	parser.file = file;
	parser.LoadTokens(lexer.tokens);

	try
	{
		parser.Parse();
	}
	catch (const std::exception& e)
	{
		Log::ERROR("Parse Error: " + std::string(e.what()));
		return;
	}

	if (arg_parser.IsSet("-past"))
	{
		std::cout << parser.DumpsAst();
	}

	RnCodeGenerator code_generator;
	code_generator.Generate(parser.ast);

	if (arg_parser.IsSet("-past"))
	{
		size_t index = 0;
		for (auto& instruction : code_generator.GetInstructions())
		{
			std::cout << String::Pad(std::to_string(index++), 6)
					  << instruction->ToString() << std::endl;
		}
	}
	auto vm = RnVirtualMachine::GetInstance();
	vm->LoadInstructions(code_generator.GetInstructions());

	try
	{
		std::ios_base::sync_with_stdio(false);
		RnIntNative exit_code = vm->Run();
	}
	catch (const std::exception& e)
	{
		Log::ERROR("Runtime Error: " + std::string(e.what()));
		return;
	}

	delete vm;
}

/*****************************************************************************/
int main(int argc, char* argv[])
{
	RonaScriptMain(argc, argv);

	return 0;
}
