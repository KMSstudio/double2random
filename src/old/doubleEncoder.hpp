#ifndef DOUBLE_ENCODER_HPP
#define DOUBLE_ENCODER_HPP

#include <iostream>
#include <cmath>
#include <limits>

enum class EncodeType { NormalWithPopulation, Exponent, Union01Box, Basic };

class doubleEncoder {
private:
    EncodeType encodeType;
    int write_from;
    int write_len;
    double mean;
    double stdDev;

public:
    doubleEncoder();

    void setEncodeType(int type);
    void setWriteParams(int from, int len);
    void setNormalParameters(double mu, double sigma);
    int encode(double val);
};

#endif