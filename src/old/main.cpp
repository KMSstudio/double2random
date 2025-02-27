#include <iostream>
#include "fileEncoder.hpp"
#include "DoubleEncoder.hpp"
#include "fileWriter.hpp"

int main() {
    fileEncoder file;
    doubleEncoder encoder;
    fileWriter writer;
    int fileType, encodeType, outputFileType;
    int write_from, write_len;
    std::string inputFilePath, outputFilePath;

    std::cout << "========== Select File Type ==========\n\n";
    std::cout << "1. Read CSV File\n";
    std::cout << "2. Read Binary (Double) File\n";
    std::cout << "3. Read Binary (Float) File\n";
    std::cout << "\n======================================\n: ";
    std::cin >> fileType;
    std::cin.ignore();

    file.setFileType(fileType);

    std::cout << "Input File Path: ";
    std::getline(std::cin, inputFilePath);

    if (!file.openFile(inputFilePath)) { return 1; }

    std::cout << "======== Select Encoding Type ========\n\n";
    std::cout << "1. NormalWithPopulation\n";
    std::cout << "2. Exponent\n";
    std::cout << "3. Union01Box\n";
    std::cout << "4. Basic\n";
    std::cout << "\n======================================\n: ";
    std::cin >> encodeType;
    encoder.setEncodeType(encodeType);

    std::cout << "Write From (0-22): ";
    std::cin >> write_from;

    std::cout << "Write Length (1-" << (23 - write_from) << "): ";
    std::cin >> write_len;

    encoder.setWriteParams(write_from, write_len);

    if (encodeType == 1) {
        double mean, stdDev;
        std::cout << "N. mean: ";
        std::cin >> mean;
        std::cout << "N.  std: ";
        std::cin >> stdDev;
        encoder.setNormalParameters(mean, stdDev);
    }

    std::cout << "======== Select Output FileType ========\n\n";
    std::cout << "1. Binary Int File\n";
    std::cout << "2. CSV Int File\n";
    std::cout << "3. NIST Test Data File\n";
    std::cout << "\n======================================\n: ";
    std::cin >> outputFileType;
    writer.setFileType(outputFileType);

    std::cout << "Output File Path: ";
    std::cin.ignore();
    std::getline(std::cin, outputFilePath);

    if (!writer.openFile(outputFilePath)) { return 1; }
    writer.setOutputBit(write_len);

    double value;
    while (file.getValue(value)) {
        writer.write(encoder.encode(value));
    }

    writer.closeFile();
    file.closeFile();
    return 0;
}