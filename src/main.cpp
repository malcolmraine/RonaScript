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
#include "codegen/RnInstruction.h"
#include "codegen/RnCodeGenerator.h"
#include "vm/RnVirtualMachine.h"
#include "util/MLib/String.h"
#include <dlfcn.h>

/*****************************************************************************/
void RonaScriptMain()
{
	std::filesystem::path const file = "../examples/Test.rn";
	std::filesystem::path const cfile = file.string() + "c";

	Lexer lexer;
	lexer.LoadFile(file);
	lexer.ProcessTokens();

	for (auto& token : lexer.tokens)
	{
		std::cout << token->ToString() << std::endl;
	}

	Parser parser;
	parser.working_dir = file.parent_path();
	parser.file = file;
	parser.LoadTokens(lexer.tokens);
	parser.Parse();

	std::cout << parser.DumpsAst();

	RnCodeGenerator code_generator;
	code_generator.Generate(parser.ast);

	size_t index = 0;
	for (auto& instruction : code_generator.GetInstructions())
	{
		std::cout << String::Pad(std::to_string(index++), 6) << instruction->ToString()
				  << std::endl;
		char* b = instruction->GetAsBytes();
	}
	auto vm = RnVirtualMachine();
	vm.LoadInstructions(code_generator.GetInstructions());
	vm.Run();
}

/*****************************************************************************/
int main(int argc, char** argv)
{
//	void* handle = dlopen("../sandbox/libHello.dylib", RTLD_LOCAL);
//	if (handle) {
//		void (*b1a_name)(RnScope* scope, const std::vector<RnObject*>& args,
//			RnObject* ret_val) = nullptr;
//		b1a_name = (void (*)(RnScope* scope, const std::vector<RnObject*>& args,
//			RnObject* ret_val))dlsym(handle, "hello");
//
//		const char* (*LibraryName)() = nullptr;
//		LibraryName = (const char*(*)())dlsym(handle, "LibraryName");
//
//		if (LibraryName) {
//			std::cout << LibraryName() << std::endl;
//		}
//
//		if (b1a_name) {
//			auto obj = RnObject::Create(RnType::RN_STRING);
//			b1a_name(nullptr, {}, obj);
//			std::cout << obj->ToString() << std::endl;
//		}
//	}
//	else {
//		printf("[%s] Unable to open libBus1a.dylib: %s\n",
//			__FILE__, dlerror());
//	}
//	dlclose(handle);
	std::ios_base::sync_with_stdio(false);
	RonaScriptMain();
	return 0;
}
