// API_VCP.cpp
#include <Graphiti/API_VCP.hpp>
#include <iostream>

Graphiti_API_VCP::Graphiti_API_VCP(const std::string& port)
    : port_(port), serial_(io_) {
}

Graphiti_API_VCP::~Graphiti_API_VCP() {
    close();
}

bool Graphiti_API_VCP::open() {
    try {
        serial_.open(port_);
        serial_.set_option(asio::serial_port_base::baud_rate(115200));
        serial_.set_option(asio::serial_port_base::character_size(8));
        serial_.set_option(asio::serial_port_base::stop_bits(asio::serial_port_base::stop_bits::one));
        serial_.set_option(asio::serial_port_base::parity(asio::serial_port_base::parity::none));
        serial_.set_option(asio::serial_port_base::flow_control(asio::serial_port_base::flow_control::none));
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to open port: " << e.what() << "\n";
        return false;
    }
}

void Graphiti_API_VCP::close() {
    if (serial_.is_open()) {
        asio::error_code ec;
        serial_.cancel(ec);
        serial_.close(ec);
    }
}

bool Graphiti_API_VCP::write(const std::vector<unsigned char>& data) {
    try {
        asio::write(serial_, asio::buffer(data));
        return true;
    }
    catch (...) {
        return false;
    }
}

std::vector<unsigned char> Graphiti_API_VCP::read(size_t size) {
    std::vector<unsigned char> buffer(size);
    asio::read(serial_, asio::buffer(buffer));
    return buffer;
}
