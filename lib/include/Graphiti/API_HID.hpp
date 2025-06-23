#pragma once
//#include <hidapi/hidapi.hpp>
#include <vector>
#include <string>

class Graphiti_API_HID {
public:
    Graphiti_API_HID(unsigned short vid, unsigned short pid);
    ~Graphiti_API_HID();

    bool open();
    void close();
    bool write(const std::vector<unsigned char>& data);
    std::vector<unsigned char> read(size_t size);

private:
//    hid_device* device_;
    unsigned short vid_;
    unsigned short pid_;
};
