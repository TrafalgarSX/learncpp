#ifndef _ASIODATA_H_
#define _ASIODATA_H_

#include <vector>

enum class PacketType : uint8_t {
  kStringData = 0,
  kBytesData = 1,
};

const int PacketHeaderSize = 2;
const int PacketTypeSize = 1;

// Base case
inline void append_data(std::vector<uint8_t> &, uint16_t &) {}

template <typename T, typename... Args>
void append_data(std::vector<uint8_t> &data, uint16_t &packet_size, T &&t,
                 Args &&...args) {

  data.insert(data.end(), t.begin(), t.end());
  packet_size += static_cast<uint16_t>(t.size());
  append_data(data, packet_size, std::forward<Args>(args)...);
}
// 组装数据
template <typename... Args>
void make_data(PacketType message_type, std::vector<uint8_t> &data,
               uint16_t &packet_size, Args &&...args) {

  data.push_back(static_cast<uint8_t>(message_type));
  packet_size += 1;
  append_data(data, packet_size, std::forward<Args>(args)...);
}

#endif