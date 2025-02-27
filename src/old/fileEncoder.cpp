#include "fileEncoder.hpp"

void fileEncoder::setFileType(int type) {
    if (type == 1) { fileType = FileType::CSV; }
    else if (type == 2) { fileType = FileType::BINARY_DOUBLE; }
    else if (type == 3) { fileType = FileType::BINARY_FLOAT; }
    else {
        std::cerr << "Incorrect file type: " << type << std::endl;
        exit(1);
    }
}

bool fileEncoder::openFile(const std::string& path) {
    filePath = path;
    if (fileType == FileType::CSV) { file.open(filePath); }
    else { file.open(filePath, std::ios::binary); }

    if (!file.is_open()) {
        std::cerr << "Something wrong while opening file " << filePath << std::endl;
        return false;
    }
    return true;
}

bool fileEncoder::getValue(double& value) {
    if (!file.is_open()) {
        std::cerr << "File is not opened" << std::endl;
        return false;
    }

    if (fileType == FileType::CSV) {
        std::string line;
        if (std::getline(file, line, ',')) {
            try { value = std::stod(line); return true; }
            catch (const std::invalid_argument&) {
                std::cerr << "Invalid line input: " << line << std::endl;
                return false;
            }
        }
    } else if (fileType == FileType::BINARY_DOUBLE) {
        file.read(reinterpret_cast<char*>(&value), sizeof(double));
        return file.gcount() == sizeof(double);
    } else if (fileType == FileType::BINARY_FLOAT) {
        float floatValue;
        file.read(reinterpret_cast<char*>(&floatValue), sizeof(float));
        if (file.gcount() == sizeof(float)) {
            value = static_cast<double>(floatValue);
            return true;
        }
    }
    return false;
}

void fileEncoder::closeFile() {
    if (file.is_open()) { file.close(); }
}