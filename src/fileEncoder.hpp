#ifndef FILE_ENCODER_HPP
#define FILE_ENCODER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

enum class FileType { CSV, BINARY_DOUBLE, BINARY_FLOAT };

class fileEncoder {
private:
    FileType fileType;
    std::ifstream file;
    std::string filePath;

public:
    void setFileType(int type);
    bool openFile(const std::string& path);
    bool getValue(double& value);
    void closeFile();
};

#endif