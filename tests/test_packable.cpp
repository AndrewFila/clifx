#include "utils/packable.hpp"
#include <gtest/gtest.h>

namespace {

// Utils::Packable's read/write helpers are protected — expose them for direct testing.
class TestPackable : public Utils::Packable {
  public:
    void Pack(std::vector<std::uint8_t> & /*buffer*/, size_t & /*offset*/) const override {}
    void Unpack(const std::vector<std::uint8_t> & /*buffer*/, size_t & /*offset*/) override {}

    template <typename T>
    static void CallWrite(std::vector<std::uint8_t> &buffer, size_t &offset, T val) {
        write(buffer, offset, val);
    }
    template <typename T> static T CallRead(const std::vector<std::uint8_t> &buffer, size_t &offset) {
        return read<T>(buffer, offset);
    }
};

} // namespace

TEST(PackableTest, WriteUint16IsLittleEndian) {
    std::vector<std::uint8_t> buffer;
    size_t offset = 0;
    TestPackable::CallWrite<std::uint16_t>(buffer, offset, 0x1234);

    ASSERT_EQ(buffer.size(), 2U);
    EXPECT_EQ(buffer[0], 0x34);
    EXPECT_EQ(buffer[1], 0x12);
    EXPECT_EQ(offset, 2U);
}

TEST(PackableTest, WriteReadRoundTripUint32) {
    std::vector<std::uint8_t> buffer;
    size_t writeOffset = 0;
    TestPackable::CallWrite<std::uint32_t>(buffer, writeOffset, 0xDEADBEEF);

    size_t readOffset = 0;
    auto value = TestPackable::CallRead<std::uint32_t>(buffer, readOffset);

    EXPECT_EQ(value, 0xDEADBEEFU);
    EXPECT_EQ(readOffset, 4U);
}

TEST(PackableTest, WriteAdvancesOffsetBySizeofT) {
    std::vector<std::uint8_t> buffer;
    size_t offset = 5; // starting at a nonzero offset, as happens mid-packet
    TestPackable::CallWrite<std::uint8_t>(buffer, offset, 0x42);
    EXPECT_EQ(offset, 6U);
}

TEST(PackableTest, ReadDoesNotConsumeUnrelatedTrailingBytes) {
    std::vector<std::uint8_t> buffer;
    size_t offset = 0;
    TestPackable::CallWrite<std::uint16_t>(buffer, offset, 1);
    TestPackable::CallWrite<std::uint16_t>(buffer, offset, 2);

    size_t readOffset = 0;
    auto first  = TestPackable::CallRead<std::uint16_t>(buffer, readOffset);
    auto second = TestPackable::CallRead<std::uint16_t>(buffer, readOffset);

    EXPECT_EQ(first, 1);
    EXPECT_EQ(second, 2);
    EXPECT_EQ(readOffset, 4U);
}
