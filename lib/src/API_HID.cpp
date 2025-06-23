#include <Graphiti/API_HID.hpp>
#include <iostream>

Graphiti_API_HID::Graphiti_API_HID(unsigned short vid, unsigned short pid)
    : vid_(vid), pid_(pid), device_(nullptr) {
    hid_init();
}

Graphiti_API_HID::~Graphiti_API_HID() {
    close();
    hid_exit();
}

bool Graphiti_API_HID::open() {
    device_ = hid_open(vid_, pid_, nullptr);
    return device_ != nullptr;
}

void Graphiti_API_HID::close() {
    if (device_) {
        hid_close(device_);
        device_ = nullptr;
    }
}

std::vector<unsigned char> Graphiti_API_HID::read(size_t size) {
    std::vector<unsigned char> buffer(size);
    int res = hid_read(device_, buffer.data(), size);
    if (res > 0) {
        buffer.resize(res);
        return buffer;
    }
    return {};
}

bool Graphiti_API_HID::write(const std::vector<unsigned char>& data) {
    if (!device_) return false;
    return hid_write(device_, data.data(), data.size()) >= 0;
}
