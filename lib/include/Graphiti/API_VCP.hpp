#pragma once

#define ASIO_STANDALONE
#include <asio.hpp>
using namespace asio;
#include <vector>
#include <string>

class Graphiti_API_VCP {
public:
    explicit Graphiti_API_VCP(const std::string& port);
    ~Graphiti_API_VCP();

    bool open();
    void close();
    bool write(const std::vector<unsigned char>& data);
    std::vector<unsigned char> read(size_t size);

private:
    std::string port_;
    asio::io_context io_;
    asio::serial_port serial_;
};