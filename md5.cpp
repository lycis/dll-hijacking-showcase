#include <iostream>
#include <array>
#include <cstring>
#include <iomanip>

#include <sstream>
#include <fstream>

namespace {
    constexpr std::array<uint32_t, 64> MD5_T{
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };

    constexpr std::array<uint32_t, 64> MD5_Shifts{
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };

    constexpr uint32_t LEFT_ROTATE(uint32_t x, uint32_t c) {
        return (x << c) | (x >> (32 - c));
    }

    std::array<uint32_t, 4> md5ProcessBlock(const uint8_t* block) {
        std::array<uint32_t, 4> state{ 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };

        auto F = [](uint32_t x, uint32_t y, uint32_t z) { return (x & y) | (~x & z); };
        auto G = [](uint32_t x, uint32_t y, uint32_t z) { return (x & z) | (y & ~z); };
        auto H = [](uint32_t x, uint32_t y, uint32_t z) { return x ^ y ^ z; };
        auto I = [](uint32_t x, uint32_t y, uint32_t z) { return y ^ (x | ~z); };

        uint32_t A = state[0], B = state[1], C = state[2], D = state[3];

        std::array<uint32_t, 16> M;
        std::memcpy(M.data(), block, 64);

        for (int i = 0; i < 64; ++i) {
            uint32_t F_value, g;
            if (i < 16) {
                F_value = F(B, C, D);
                g = i;
            } else if (i < 32) {
                F_value = G(B, C, D);
                g = (5 * i + 1) % 16;
            } else if (i < 48) {
                F_value = H(B, C, D);
                g = (3 * i + 5) % 16;
            } else {
                F_value = I(B, C, D);
                g = (7 * i) % 16;
            }

            uint32_t temp = D;
            D = C;
            C = B;
            B += LEFT_ROTATE(A + F_value + MD5_T[i] + M[g], MD5_Shifts[i]);
            A = temp;
        }

        state[0] += A;
        state[1] += B;
        state[2] += C;
        state[3] += D;

        return state;
    }

    std::string md5ToString(const std::array<uint32_t, 4>& state) {
        std::ostringstream result;
        for (uint32_t part : state) {
            result << std::hex << std::setfill('0') << std::setw(8) << part;
        }
        return result.str();
    }
}

std::string md5Checksum(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }

    std::array<uint32_t, 4> state{ 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };
    uint64_t bitCount = 0;

    uint8_t buffer[64];
    while (file.good()) {
        file.read(reinterpret_cast<char*>(buffer), 64);
        std::streamsize bytesRead = file.gcount();

        if (bytesRead == 64) {
            state = md5ProcessBlock(buffer);
            bitCount += 512;
        } else {
            bitCount += bytesRead * 8;

            // Padding
            buffer[bytesRead] = 0x80;
            if (bytesRead + 1 <= 56) {
                std::memset(buffer + bytesRead + 1, 0, 56 - (bytesRead + 1));
            } else {
                std::memset(buffer + bytesRead + 1, 0, 64 - (bytesRead + 1));
                state = md5ProcessBlock(buffer);
                std::memset(buffer, 0, 56);
            }

            // Append bit length
            uint64_t bitCountLE = bitCount;
            std::memcpy(buffer + 56, &bitCountLE, sizeof(bitCountLE));
            state = md5ProcessBlock(buffer);
        }
    }

    return md5ToString(state);
}

#ifdef STANDALONE
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::string filePath = argv[1];
    std::string checksum = md5Checksum(filePath);

    if (!checksum.empty()) {
        std::cout << checksum << std::endl;
    }

    return 0;
}
#endif

