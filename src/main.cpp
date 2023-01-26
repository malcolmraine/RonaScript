#include <iostream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include <map>
#include <fstream>
#include "util/FileInfo.h"
#include "vm/RnInternment.h"
#include <unordered_set>
#include "vm/RnMemoryManager.h"
#include "vm/RnObject.h"
#include "codegen/RnCodeGenerator.h"
#include "vm/RnVirtualMachine.h"
#include "util/MLib/String.h"
#include <dlfcn.h>
#include "util/ArgParser.h"

/*****************************************************************************/
void RonaScriptMain(int argc, char* argv[])
{
	ArgParser arg_parser;
	arg_parser.SetMainDescription("Usage: RonaScript <file> [options...]");
	arg_parser.AddArgument("<file>", "Input file (*.rn | *.rnc)");
	arg_parser.AddArgument("-c", "Compile to *.rnc file");
	arg_parser.AddArgument("-norun", "Compile to *.rnc file without running");
	arg_parser.Parse(argc, argv);

	if (arg_parser.IsSet("-h"))
	{
		arg_parser.ShowHelp();
		return;
	} else if (arg_parser.GetInputFile().empty()) {
		std::cout << "\033[31m" << "RonaScript: Error: No input file\n";
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
		std::cout << "\033[31m" << "Lexer Error: " << e.what() << std::endl;
		return;
	}

	for (auto& token : lexer.tokens)
	{
		std::cout << token->ToString() << std::endl;
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
		std::cout << "\033[31m" << "Parse Error: " << e.what() << std::endl;
		return;
	}

	std::cout << parser.DumpsAst();

	RnCodeGenerator code_generator;
	code_generator.Generate(parser.ast);

	size_t index = 0;
	for (auto& instruction : code_generator.GetInstructions())
	{
		std::cout << String::Pad(std::to_string(index++), 6) << instruction->ToString()
				  << std::endl;
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
		std::cout << "\033[31m" << "Runtime Error: " << e.what() << std::endl;
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
