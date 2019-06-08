#include "LimitedBuffer.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <exception>

using namespace custom_containers;
using namespace ::testing;

TEST(LimitedBufferTest, BufferCreationTest) {
    LimitedBuffer<int, int, 100> buffer;
    EXPECT_THAT(buffer.size(), Eq(0u));
    EXPECT_THAT(buffer.maxSize(), Eq(100u));
}

TEST(LimitedBufferTest, BufferSimpleInsertionAndAccessTest) {
    LimitedBuffer<int, int, 1> buffer;
    buffer.insert(1, 1);
    EXPECT_THAT(buffer.maxSize(), Eq(1u));
    EXPECT_THAT(buffer.value(1), Eq(1));
}

TEST(LimitedBufferTest, BufferMultiInsertionAndAccessTest) {
    LimitedBuffer<int, int, 2> buffer;
    buffer.insert(1, 1);
    buffer.insert(1, 2);
    buffer.insert(1, 3);
    EXPECT_THAT(buffer.value(1), Eq(3));
}

TEST(LimitedBufferTest, BufferBadAccessTest) {
    LimitedBuffer<int, int, 1> buffer;
    EXPECT_THROW(buffer.value(2), std::out_of_range);
}

TEST(LimitedBufferTest, BufferLessFreqValueTest) {
    LimitedBuffer<int, int, 3> buffer;
    EXPECT_THAT(buffer.size(), Eq(0u));
    ASSERT_THAT(buffer.maxSize(), Eq(3u));
    buffer.insert(1, 12);
    buffer.insert(2, 13);
    buffer.insert(4, 14);
    buffer.value(1);
    buffer.value(2);
    buffer.insert(5, 15);
    EXPECT_THROW(buffer.value(4), std::out_of_range);
    buffer.insert(1, 12);
    buffer.insert(6, 62);
    EXPECT_THROW(buffer.value(1), std::out_of_range);
}

TEST(LimitedBufferTest, BufferOldValueTest) {
    LimitedBuffer<int, int, 3> buffer;
    EXPECT_THAT(buffer.size(), Eq(0u));
    ASSERT_THAT(buffer.maxSize(), Eq(3u));
    buffer.insert(1, 12);
    buffer.insert(2, 13);
    buffer.insert(4, 14);
    buffer.insert(5, 15);
    EXPECT_THROW(buffer.value(1), std::out_of_range);
}

TEST(LimitedBufferTest, SomeUsageTest) {
    LimitedBuffer<int, int, 3> buffer;
    EXPECT_THAT(buffer.size(), Eq(0u));
    ASSERT_THAT(buffer.maxSize(), Eq(3u));
    buffer.insert(1, 12);
    buffer.insert(2, 13);
    buffer.insert(4, 14);
    EXPECT_THAT(buffer.size(), Eq(3u));
    EXPECT_THROW(buffer.value(0), std::out_of_range);
    EXPECT_THAT(buffer.value(1), Eq(12));
    EXPECT_THAT(buffer.value(2), Eq(13));
    EXPECT_THAT(buffer.value(4), Eq(14));
    buffer.insert(5, 15);
    EXPECT_THROW(buffer.value(1), std::out_of_range);
    EXPECT_THAT(buffer.value(5), Eq(15));
    buffer.value(2);
    buffer.value(4);
    buffer.insert(6, 66);
    EXPECT_THROW(buffer.value(5), std::out_of_range);
    buffer.value(6);
    buffer.value(6);
    buffer.insert(7, 77);
    EXPECT_THROW(buffer.value(2), std::out_of_range);
}

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
