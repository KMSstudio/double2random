#include "fileWriter.hpp"
#include <iomanip>

fileWriter::fileWriter() : fileType(OutputFileType::CSV_INT), outputBit(8), buffer(0), bufferBitCount(0) {}

void fileWriter::setFileType(int type) {
    if (type == 1) { fileType = OutputFileType::BINARY_INT; }
    else if (type == 2) { fileType = OutputFileType::CSV_INT; }
    else if (type == 3) { fileType = OutputFileType::NIST_TESTDATA; }
    else {
        std::cerr << "Incorrect output file type: " << type << std::endl;
        exit(1);
    }
}

void fileWriter::setOutputBit(int bit) {
    if (bit < 1 || bit > 32) { std::cerr << "OutputBit must be between 1 and 32" << std::endl; exit(1); }
    outputBit = bit;
}

bool fileWriter::openFile(const std::string& path) {
    filePath = path;
    if (fileType == OutputFileType::BINARY_INT) { file.open(filePath, std::ios::binary); }
    else { file.open(filePath); }

    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filePath << std::endl;
        return false;
    }
    return true;
}

void fileWriter::write(int val) {
    val &= (1 << outputBit) - 1;
    buffer = (buffer << outputBit) | val;
    bufferBitCount += outputBit;

    while (bufferBitCount >= 32) { // write
        uint32_t writeVal = buffer >> (bufferBitCount - 32);
        if (fileType == OutputFileType::BINARY_INT) {
            file.write(reinterpret_cast<char*>(&writeVal), sizeof(uint32_t)); }
        else if (fileType == OutputFileType::NIST_TESTDATA) { 
            for (int i = 31; i >= 0; --i) { file << ((writeVal >> i) & 1) << "\n"; } }
        else {
            file << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << writeVal << ","; }
        buffer >>= 32;
        bufferBitCount -= 32;
    }
}

void fileWriter::closeFile() {
    if (bufferBitCount > 0) {
        uint32_t writeVal = buffer;
        if (fileType == OutputFileType::BINARY_INT) {
            file.write(reinterpret_cast<char*>(&writeVal), sizeof(uint32_t)); }
        else if (fileType == OutputFileType::NIST_TESTDATA) {
            for (int i = bufferBitCount - 1; i >= 0; i--) { file << ((writeVal >> i) & 1) << "\n"; } }
        else {
            file << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << writeVal << ","; }
    }
    file.close();
}