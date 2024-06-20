#include <iostream>
#include <array>

void bytesToHex(const std::array<int8_t, 32>& arr) {
    uint32_t i = 0;
    for(const auto& byte : arr) {
        printf("%02X", (uint8_t)byte);
    }
    printf("\n");

}

int main(int argc, const char** argv) {
    std::array<int8_t, 32> arr = {10, 10, 71, 119, 82, -69, 15, 65, 43, -10, -110, -103, -10, -113, 102, -17, -118, 82, 56, -124, 102, 65, 51, -68, -61, 59, -52, -101, 112, -38, 103, -123};
    bytesToHex(arr);


    std::array<int8_t, 32> arr2 = {10, 45, -78, -61, -21, -25, -121, 105, 104, 35, 64, -127, 100, -96, -114, 0, -46, 20, 64, -9, 44, -41, -28, -40, -106, -88, 38, 18, -64, 28, 8, 20};
    bytesToHex(arr2);
    return 0;
}