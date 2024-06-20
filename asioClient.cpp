#include <asio.hpp>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/core.h>
#include <hexUtil.h>
#include <asioData.h>

void sync_client() {
  asio::io_context io_context;
  asio::ip::tcp::socket socket(io_context);
  asio::ip::tcp::resolver resolver(io_context);

  asio::connect(socket, resolver.resolve("127.0.0.1", "8888"));

  // 组装数据
  std::vector<uint8_t> data{0x00, 0x00}; // packet size 占位
  uint16_t packet_size = 0;
#if 1
  std::string message = "Hello, asio";
  make_data(PacketType::kStringData, data, packet_size, message);
#else
  std::array<uint8_t, 4> message = {0x01, 0x02, 0x03, 0x04};
  make_data(PacketType::kBytesData, data, packet_size, message);
#endif

  // 设置 packet size
  data[0] = packet_size >> 8;
  data[1] = packet_size & 0xFF;

  // print data hex
  std::string hex_data = bytesToHexString(data, '\0');
  fmt::print("Send data: {}\n", hex_data);

  // 发送数据
  try {
    auto result = asio::write(socket, asio::buffer(data));
    fmt::print("Send data size: {}\n", result);
  } catch (const std::exception &e) {
    fmt::print("Send data error: {}\n", e.what());
  }

  try {
    std::vector<uint8_t> receive_data(1024);
    uint16_t packet_size = 0;
    auto result = socket.read_some(asio::buffer(&packet_size, sizeof(packet_size)));
    fmt::print("Receive data size: {}\n", result);

    packet_size = ntohs(packet_size);

    result = socket.read_some(asio::buffer(receive_data, packet_size));
    fmt::print("Receive data size: {}\n", result);

    std::string_view receive_data_view(reinterpret_cast<char *>(receive_data.data() + 1), packet_size - 1);
    fmt::print("Receive data: {}\n", receive_data_view);
  } catch (const std::exception &e) {
    fmt::print("Receive data error: {}\n", e.what());
  }

#if 0
  // 关闭链接
  std::error_code ec;
  socket.shutdown(asio::ip::tcp::socket::shutdown_send, ec);
  if (ec) {
    fmt::print("Shutdown error: {}\n", ec.message());
  }
  socket.close();
#endif

  io_context.run();
}

void async_client() {

}

int main(int argc, const char **argv) {
    sync_client();
}