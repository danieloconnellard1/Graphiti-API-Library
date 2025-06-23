#ifndef GRAPHITI_CONNECTION_VCP_H
#define GRAPHITI_CONNECTION_VCP_H

#include <string>
#include <vector>
#define ASIO_STANDALONE
#include <asio.hpp>
using namespace asio;
#include "Connection.hpp"

class GraphitiConnectionVCP : public GraphitiConnection {
public:
    explicit GraphitiConnectionVCP(const std::string& port);

    bool open();
    void close();
    bool write(const std::vector<unsigned char>& data);
    std::vector<unsigned char> read(size_t size);

private:
    asio::io_context io_;
    asio::serial_port serial_;
    std::string port_;
};

#endif // GRAPHITI_CONNECTION_VCP_H
