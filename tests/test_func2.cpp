//
//https://github.com/meekrosoft/fff
//
#include "gtest/gtest.h"
#include <string.h>
#include "fff/fff.h"
#include "fakes.h"

extern "C" {

//評価対象本体
#include "../func2.c"
}

////////////////////////////////////////////////////////////////////////

class TestFunc2: public testing::Test {
    void SetUp() {
        RESET_FAKE(open);
        RESET_FAKE(close);
        RESET_FAKE(write);
        RESET_FAKE(read);
        FFF_RESET_HISTORY();
    }
    void TearDown() {
    }
};

////////////////////////////////////////////////////////////////////////

TEST_F(TestFunc2, open_1)
{
    sfd = 3;

    int fd = my_open2("/dev/ttyS1");

    ASSERT_EQ(fd, -100);
    ASSERT_EQ(open_fake.call_count, 0);
}

TEST_F(TestFunc2, open_2)
{
    sfd = -1;

    open_fake.return_val = -1;

    int fd = my_open2("/dev/ttyS1");

    ASSERT_EQ(fd, -1);
    ASSERT_EQ(open_fake.call_count, 1);
}

TEST_F(TestFunc2, open_3)
{
    sfd = -1;

    open_fake.return_val = 5;

    int fd = my_open2("/dev/ttyS1");

    ASSERT_EQ(fd, 5);
    ASSERT_EQ(sfd, 5);
    ASSERT_EQ(open_fake.call_count, 1);
}

////////////////////////////////////////////////////////////////////////

TEST_F(TestFunc2, close_1)
{
    sfd = 1;

    my_close2();

    ASSERT_EQ(sfd, -1);
    ASSERT_EQ(close_fake.call_count, 1);
}

TEST_F(TestFunc2, close_2)
{
    sfd = -1;

    my_close2();

    ASSERT_EQ(sfd, -1);
    ASSERT_EQ(close_fake.call_count, 1);
}

////////////////////////////////////////////////////////////////////////

TEST_F(TestFunc2, access_1)
{
    sfd = 5;

    auto dummy_read = [](int fd, void *p, size_t sz) -> ssize_t {
        static int count = 0;
        int ret = -1;
        uint8_t* buf = (uint8_t *)p;
        switch (count) {
        case 0:
            ret = 5;
            buf[4] = 0xff;
            break;
        case 1:
            ret = 3;
            buf[2] = 0x35;
            break;
        default:
            assert(0);
        }
        count++;
        return ret;
    };

    write_fake.return_val = 5;
    read_fake.custom_fake = dummy_read;

    int ret = my_access2();

    ASSERT_EQ(ret, 0);
    ASSERT_EQ(read_fake.call_count, 2);
}

TEST_F(TestFunc2, access_2)
{
    sfd = 5;

    auto dummy_read = [](int fd, void *p, size_t sz) -> ssize_t {
        static int count = 0;
        int ret = -1;
        uint8_t* buf = (uint8_t *)p;
        switch (count) {
        case 0:
            ret = 5;
            buf[4] = 0xfe;      //★期待外
            break;
        case 1:
            ret = 3;
            buf[2] = 0x35;
            break;
        default:
            assert(0);
        }
        count++;
        return ret;
    };

    write_fake.return_val = 5;
    read_fake.custom_fake = dummy_read;

    int ret = my_access2();

    ASSERT_EQ(ret, -3);
    ASSERT_EQ(read_fake.call_count, 1);
}

TEST_F(TestFunc2, access_3)
{
    sfd = 5;

    auto dummy_read = [](int fd, void *p, size_t sz) -> ssize_t {
        static int count = 0;
        int ret = -1;
        uint8_t* buf = (uint8_t *)p;
        switch (count) {
        case 0:
            ret = 5;
            buf[4] = 0xff;
            break;
        case 1:
            ret = 3;
            buf[2] = 0x34;      //★期待外
            break;
        default:
            assert(0);
        }
        count++;
        return ret;
    };

    write_fake.return_val = 5;
    read_fake.custom_fake = dummy_read;

    int ret = my_access2();

    ASSERT_EQ(ret, -4);
    ASSERT_EQ(read_fake.call_count, 2);
}

TEST_F(TestFunc2, access_4)
{
    sfd = 5;

    write_fake.return_val = 4;

    int ret = my_access2();

    ASSERT_EQ(ret, -2);
    ASSERT_EQ(read_fake.call_count, 0);
}
