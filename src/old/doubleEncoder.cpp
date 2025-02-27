#include "DoubleEncoder.hpp"

doubleEncoder::doubleEncoder() : encodeType(EncodeType::Basic), mean(0), stdDev(1) {}

void doubleEncoder::setEncodeType(int type) {
    if (type == 1) { encodeType = EncodeType::NormalWithPopulation; }
    else if (type == 2) { encodeType = EncodeType::Exponent; }
    else if (type == 3) { encodeType = EncodeType::Union01Box; }
    else if (type == 4) { encodeType = EncodeType::Basic; }
    else {
        std::cerr << "Incorrect encode type: " << type << std::endl;
        exit(1);
    }
}

void doubleEncoder::setWriteParams(int from, int len) {
    if (from < 0 || from > 22) {
        std::cerr << "write_from must be between 0 and 22" << std::endl;
        exit(1);
    }
    if (len < 1 || len > 23 - from) {
        std::cerr << "write_len must be between 1 and " << (23 - from) << std::endl;
        exit(1);
    }
    write_from = from;
    write_len = len;
}

void doubleEncoder::setNormalParameters(double mu, double sigma) {
    if (sigma <= 0) {
        std::cerr << "std > 0 must be satisfied" << std::endl;
        exit(1);
    }
    mean = mu;
    stdDev = sigma;
}

int doubleEncoder::encode(double val) {
    uint32_t fractionBits;

    if (encodeType == EncodeType::NormalWithPopulation) {
        double normValue = 0.5 * (1 + erf((val - mean) / (stdDev * std::sqrt(2)))); // CDF
        fractionBits = static_cast<uint32_t>(normValue * (1 << 23));
    }
    else if (encodeType == EncodeType::Union01Box) {
        double boundedVal = std::min(1.0, std::max(0.0, val)); // 0~1 union
        fractionBits = static_cast<uint32_t>(boundedVal * (1 << 23));
    }
    else if (encodeType == EncodeType::Exponent) { 
        uint64_t bitPattern = *reinterpret_cast<uint64_t*>(&val);
        fractionBits = static_cast<uint32_t>(bitPattern & ((1ULL << 23) - 1)); // ext fraction
    }
    else {
        return static_cast<uint32_t>(val * (1ULL << write_len)) & ((1ULL << write_len)-1);
    }

    return (fractionBits >> write_from) & ((1 << write_len) - 1);
}