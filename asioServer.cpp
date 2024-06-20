#include <asio.hpp>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/core.h>
#include <hexUtil.h>
#include <limits>
#include <asioData.h>

// https://www.codingwiththomas.com/blog/boost-asio-server-client-example

class Session : public std::enable_shared_from_this<Session> {
public:
  Session(asio::ip::tcp::socket socket) : m_socket(std::move(socket)) {
    // unsigned short max cpp define
    m_data.reserve(2048);
    data_write_.reserve(2048);
  }
  ~Session() {
    fmt::print("Close connection from {} : {}\n",
               m_socket.remote_endpoint().address().to_string(),
               m_socket.remote_endpoint().port());
  }

  void start() { wait_for_request(); }

private:
  void wait_for_request() {
    auto self(shared_from_this());

    asio::async_read(
        m_socket, asio::buffer(&m_packet_size, sizeof(m_packet_size)),
        [this, self](std::error_code ec, std::size_t length) {
          if (!ec) {
            m_packet_size = ntohs(m_packet_size);
            read_data(m_packet_size);
            fmt::print("Receive data size: {}\n", m_packet_size + 2);
          } else {
            fmt::print("Receive data error: {}\n", ec.message());
          }
        });
  }

  void read_data(uint16_t packet_size) {
    auto self(shared_from_this());
    m_data.resize(packet_size);
    asio::async_read(
        m_socket, asio::buffer(m_data),
        [this, self](std::error_code ec, std::size_t length) {
          if (!ec) {
            uint8_t type = m_data[0];
            if (type == static_cast<uint8_t>(PacketType::kStringData)) {
              std::string message(m_data.begin() + 1, m_data.end());
              fmt::print("Receive string data: {}\n", message);

              std::string return_message = "asio server hello";
              uint16_t packet_size = 0;
              data_write_[0] = {0};
              data_write_[1] = {0};
              make_data(PacketType::kStringData, data_write_, packet_size, return_message);
              data_write_[0] = packet_size >> 8;
              data_write_[1] = packet_size & 0xFF;

              write_data(packet_size + 2);

            } else if (type == static_cast<uint8_t>(PacketType::kBytesData)) {
              fmt::print("Receive bytes data: {}\n",
                         bytesToHexString(m_data, '\0'));
              wait_for_request();
            } else {
              fmt::print("Receive unknown data type: {}\n", type);
              wait_for_request();
            }

          } else {
            fmt::print("Receive data error: {}\n", ec.message());
          }
        });
  }

  void write_data(uint16_t packet_size) {
    auto self(shared_from_this());
    asio::async_write(m_socket, asio::buffer(data_write_, packet_size),
                      [this, self](std::error_code ec, std::size_t length) {
                        if (!ec) {
                          fmt::print("Send data size: {}\n", length);
                        } else {
                          fmt::print("Send data error: {}\n", ec.message());
                        }
                        wait_for_request();
                      });
  }

private:
  asio::ip::tcp::socket m_socket;
  std::vector<uint8_t> m_data;
  std::vector<uint8_t> data_write_;
  uint16_t m_packet_size = 0;
};

class Server {
public:
  Server(asio::io_context &io_context, short port)
      : m_acceptor(io_context,
                   asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {
    do_accept();
  }

private:
  void do_accept() {
    m_acceptor.async_accept(
        [this](std::error_code ec, asio::ip::tcp::socket socket) {
          if (!ec) {
            fmt::print("Accept new connection from {} : {}\n",
                       socket.remote_endpoint().address().to_string(),
                       socket.remote_endpoint().port());
            std::make_shared<Session>(std::move(socket))->start();
          } else {
            fmt::print("Accept error: {}\n", ec.message());
          }

          do_accept();
        });
  }

private:
  asio::ip::tcp::acceptor m_acceptor;
};

int main(int argc, const char **argv) {
  asio::io_context io_context;

  Server server(io_context, 8888);

  io_context.run();
  return 0;
}