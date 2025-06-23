#pragma once
#include <vector>

class GraphitiConnection {
public:
    virtual ~GraphitiConnection() {}
    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool write(const std::vector<unsigned char>& data) = 0;
    virtual std::vector<unsigned char> read(size_t size) = 0;
};

