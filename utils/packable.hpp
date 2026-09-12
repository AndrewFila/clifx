#include <cstdint>
#include <cstring>
#include <vector>

namespace Utils {
class Packable {
  public:
    Packable() = default;
    virtual void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const = 0;
    virtual void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) = 0;

  protected:
    template <typename T>
    static void write(std::vector<std::uint8_t> &buffer, size_t &offset, T val) {
        const uint8_t *bytes = reinterpret_cast<uint8_t *>(&val);
        buffer.insert(buffer.end(), bytes, bytes + sizeof(T));
        offset += sizeof(T);
    }

    template <typename T> static T read(const std::vector<std::uint8_t> &buffer, size_t &offset) {
        T val;
        std::memcpy(&val, buffer.data() + offset, sizeof(T));
        offset += sizeof(T);
        return val;
    }
};
} // namespace Utils
