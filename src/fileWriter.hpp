#ifndef FILE_WRITER_HPP
#define FILE_WRITER_HPP

#include <iostream>
#include <fstream>
#include <vector>

enum class OutputFileType { BINARY_INT, CSV_INT, NIST_TESTDATA };
class fileWriter {
private:
    OutputFileType fileType;
    std::ofstream file;
    std::string filePath;
    int outputBit;
    uint32_t buffer;
    int bufferBitCount;

public:
    fileWriter();
    void setFileType(int type);
    void setOutputBit(int bit);
    bool openFile(const std::string& path);
    void write(int val);
    void closeFile();
};

#endif