/*****************************************************************************
* File: RnBinary.cpp
* Description:
* Author: Malcolm Hall
* Date: 4/20/23
* Version: 1
*
*****************************************************************************/

#include "RnBinary.h"
#include "../common/RnInternment.h"
#include "../util/Time.h"
#include "../vm/RnObject.h"

const std::string CONST_HEADER = "$$CONST$$";
const std::string CODE_HEADER = "$$CODE$$";

#define INSTRUCTION_BYTE_COUNT (4)

typedef union {
    RnIntNative i_value;
    char bytes[RN_SIZE_BYTES_LENGTH];
} IntByteConversion;

#define STRING_SIZE(s) static_cast<std::streamsize>(s.length())

/*****************************************************************************/
BinaryWriter::BinaryWriter(std::string file) : _outfile(std::move(file)) {}

/*****************************************************************************/
BinaryWriter::~BinaryWriter() = default;

/*****************************************************************************/
void BinaryWriter::SetInstructions(const InstructionBlock& instructions) {
    _instructions = instructions;
}

/*****************************************************************************/
void WriteHeader(std::fstream& fs, const std::string& header) {
    fs.write(header.c_str(), STRING_SIZE(header));
}

/*****************************************************************************/
bool BinaryWriter::Write() {
    std::fstream fs;

    if (fs.is_open()) {
        fs.close();
    }
    fs.open(_outfile.c_str(), std::ios_base::out | std::ios_base::trunc);

    if (fs.fail()) {
        throw std::runtime_error("Failed to open file: " + _outfile);
    }

    WriteHeader(fs, CONST_HEADER);
    IntByteConversion const_count;
    std::memset(const_count.bytes, 0, RN_SIZE_BYTES_LENGTH);
    const_count.i_value = RnConstStore::object_internment->GetIndex();
    fs.write(const_count.bytes, RN_SIZE_BYTES_LENGTH);

    size_t buf_size = 0;
    char* buf = nullptr;
    for (RnIntNative i = 1; i <= RnConstStore::object_internment->GetIndex(); i++) {
        auto object = RnConstStore::object_internment->GetInternedItem(i);
        size_t byte_size = object->GetByteSize();
        if (byte_size > buf_size) {
            buf = static_cast<char*>(std::realloc(buf, byte_size));
            buf_size = byte_size;
        }
        std::memset(buf, 0, object->GetByteSize());
        object->GetBytes(buf);
        std::cout << i << ": " << object->ToString() << " - " << object->GetByteSize()
                  << std::endl;
        fs.write(buf, static_cast<std::streamsize>(object->GetByteSize()));
    }
    delete[] buf;

    WriteHeader(fs, CODE_HEADER);
    IntByteConversion instruction_count;
    std::memset(instruction_count.bytes, 0, RN_SIZE_BYTES_LENGTH);
    instruction_count.i_value = static_cast<RnIntNative>(_instructions.size());
    fs.write(instruction_count.bytes, RN_SIZE_BYTES_LENGTH);
    for (auto instruction : _instructions) {
        char instruction_buf[INSTRUCTION_BYTE_COUNT] = {0};
        instruction_buf[0] = static_cast<char>(instruction->GetOpcode());
        instruction_buf[1] = static_cast<char>(instruction->GetArg1());
        instruction_buf[2] = static_cast<char>(instruction->GetArg2());
        instruction_buf[3] = static_cast<char>(instruction->GetArg3());
        fs.write(instruction_buf, INSTRUCTION_BYTE_COUNT);
    }
    fs.close();

    return true;
}

/*****************************************************************************/
/*****************************************************************************/
BinaryReader::BinaryReader(std::string file) : _infile(std::move(file)) {}

/*****************************************************************************/
BinaryReader::~BinaryReader() = default;

/*****************************************************************************/
RnIntNative GetSizeFromBuf(const char* buf) {
    IntByteConversion conversion;
    std::memset(conversion.bytes, 0, RN_SIZE_BYTES_LENGTH);
    for (int i = 0; i < RN_SIZE_BYTES_LENGTH; i++) {
        conversion.bytes[i] = buf[i];
    }
    return conversion.i_value;
}

/*****************************************************************************/
void ReadHeader(char* buf, const std::string& header, const std::string& label,
                size_t& idx) {
    if (std::string(buf, STRING_SIZE(header)) != header) {
        throw std::runtime_error("Invalid " + label + " section header");
    }
    idx += STRING_SIZE(header);
}

/*****************************************************************************/
bool BinaryReader::Read(InstructionBlock& instructions) {
    std::ifstream fs;
    fs.open(_infile.c_str(), std::ios_base::in);

    if (fs.fail()) {
        throw std::runtime_error("Failed to open file: " + _infile);
    }

    char* const_header = new char[CONST_HEADER.length() + 1];
    char* code_header = new char[CODE_HEADER.length() + 1];

    try {
        std::memset(const_header, '\0', CONST_HEADER.length() + 1);
        std::memset(code_header, '\0', CODE_HEADER.length() + 1);

        size_t file_size = std::filesystem::file_size(_infile);
        char* buf = new char[file_size];
        std::memset(buf, 0, file_size);
        fs.read(buf, static_cast<std::streamsize>(file_size));
        size_t idx = 0;
        ReadHeader(buf, CONST_HEADER, "const", idx);

        // Read constants
        RnIntNative const_count = GetSizeFromBuf(buf + idx);
        idx += RN_SIZE_BYTES_LENGTH;
        for (RnIntNative i = 0; i < const_count; i++) {
            // type, size, data
            auto type = static_cast<RnType::Type>(buf[idx++]);
            auto object = RnObject::Create(type);
            RnIntNative data_length = GetSizeFromBuf(buf + idx);
            idx += RN_SIZE_BYTES_LENGTH;
            object->SetBytes(buf + idx, data_length);
            idx += data_length;
            RnConstStore::LoadObject(object);
        }

        // Read instructions
        ReadHeader(buf + idx, CODE_HEADER, "code", idx);
        RnIntNative instruction_count = GetSizeFromBuf(buf + idx);
        idx += RN_SIZE_BYTES_LENGTH;
        for (RnIntNative i = 0; i < instruction_count; i++) {
            auto opcode = static_cast<RnOpCode>(buf[idx++]);
            auto instruction = new RnInstruction(opcode);
            instruction->SetArg1(buf[idx++]);
            instruction->SetArg2(buf[idx++]);
            instruction->SetArg3(buf[idx++]);
            instructions.push_back(instruction);
        }
    } catch (const std::exception& e) {
        delete[] const_header;
        delete[] code_header;
        throw e;
    }

    delete[] const_header;
    delete[] code_header;

    return true;
}