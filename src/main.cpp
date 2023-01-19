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

int main(int argc, char** argv)
{
//    auto obj = RnObject::Create(RnType::RN_FLOAT);
//    obj->SetData(55.98F);
//
//    std::cout << obj->AsFloat() << std::endl;
//    auto instruction = new RnInstruction();
//    instruction->_opcode = OP_LOAD_FLOAT;
//    instruction->_arg1 = 1;
//    auto bytes = instruction->GetAsBytes();
//    std::cout << instruction->ToString() << std::endl;
//    std::cout << (int)bytes[0] << "\t";
//    std::cout << (int)bytes[1] << "\t";
//    std::cout << (int)bytes[2] << "\t";
//    std::cout << (int)bytes[3] << "\t";
//    std::cout << (int)bytes[4] << "\t";
//    std::cout << (int)bytes[5] << "\t";
//    std::cout << (int)bytes[6] << "\t";
//    std::cout << (int)bytes[7] << "\t" << std::endl;
//    auto other = new RnInstruction();
//    other->FromBytes(bytes);
//
//    auto bytes2 = instruction->GetAsBytes();
//    std::cout << other->ToString() << std::endl;
//    std::cout << (int)bytes2[0] << "\t";
//    std::cout << (int)bytes2[1] << "\t";
//    std::cout << (int)bytes2[2] << "\t";
//    std::cout << (int)bytes2[3] << "\t";
//    std::cout << (int)bytes2[4] << "\t";
//    std::cout << (int)bytes2[5] << "\t";
//    std::cout << (int)bytes2[6] << "\t";
//    std::cout << (int)bytes2[7] << "\t" << std::endl;
//    std::cout << other->ToString() << std::endl;
//
//    void *d = ::operator new(4);
//    *static_cast<uint32_t*>(d) = 1113582469;
//    std::cout << *static_cast<float*>(d);

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
//	std::ofstream wf(cfile, std::ios::out | std::ios::binary | std::ios::trunc);

//	RnStack<RnInstruction*> instructions;
	size_t index = 0;
	for (auto& instruction : code_generator.GetInstructions())
	{
//		instructions.Push(instruction);
		std::cout << String::Pad(std::to_string(index++), 6) << instruction->ToString()
				  << std::endl;
		char* b = instruction->GetAsBytes();
//		wf.write(b, 9);
	}
//	wf.close();
	auto vm = RnVirtualMachine();
	vm.LoadInstructions(code_generator.GetInstructions());
	vm.Run();

//	std::ifstream rf(cfile, std::ios::out | std::ios::binary);
//	InstructionBlock read_instructions;
//	char bytes[9];
//
//	for (size_t i = 0; i < code_generator.GetInstructions().size(); i++)
//	{
//		rf.read(bytes, 9);
//		auto* b = new RnInstruction();
//		b->FromBytes(bytes);
//		read_instructions.emplace_back(b);
//	}
//
//    for (auto &instruction : read_instructions) {
//        std::cout << instruction->ToString()  << std::endl;
//    }

	return 0;
}
