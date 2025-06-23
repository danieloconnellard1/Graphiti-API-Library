#include <Graphiti/Connection/Connection_VCP.hpp>
#include <iostream>

GraphitiConnectionVCP::GraphitiConnectionVCP(const std::string& port)
    : io_(), serial_(io_), port_(port) {
}

bool GraphitiConnectionVCP::open() {
    try {
        serial_.open(port_);
        serial_.set_option(asio::serial_port_base::baud_rate(115200));
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to open port " << port_ << ": " << e.what() << std::endl;
        return false;
    }
}

void GraphitiConnectionVCP::close() {
    if (serial_.is_open()) {
        serial_.close();
    }
}

bool GraphitiConnectionVCP::write(const std::vector<unsigned char>& data) {
    try {
        asio::write(serial_, asio::buffer(data));
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Write failed: " << e.what() << std::endl;
        return false;
    }
}

std::vector<unsigned char> GraphitiConnectionVCP::read(size_t size) {
    std::vector<unsigned char> buffer(size);
    try {
        asio::read(serial_, asio::buffer(buffer, size));
    }
    catch (const asio::system_error& e) {
        // Only print errors that aren't expected during shutdown
        if (e.code() != asio::error::operation_aborted) {
            std::cerr << "Read failed: " << e.what() << std::endl;
        }
        buffer.clear();
    }
    catch (const std::exception& e) {
        std::cerr << "Read failed (non-ASIО): " << e.what() << std::endl;
        buffer.clear();
    }
    return buffer;
}
