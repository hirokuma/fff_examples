//
//https://github.com/meekrosoft/fff
//
#include "gtest/gtest.h"
#include <string.h>
#include "fff.h"
DEFINE_FFF_GLOBALS;

extern "C" {

//評価対象本体
#include "func.c"
}

////////////////////////////////////////////////////////////////////////
FAKE_VALUE_FUNC_VARARG(int, open, const char*, int, ...)
FAKE_VALUE_FUNC(int, close, int);
FAKE_VALUE_FUNC(ssize_t, read, int, void*, size_t);
FAKE_VALUE_FUNC(ssize_t, write, int, const void*, size_t);
////////////////////////////////////////////////////////////////////////

class test: public testing::Test {
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

TEST_F(test, open_1)
{
    sfd = 3;

    int fd = my_open("/dev/ttyS1");

    ASSERT_EQ(-100, fd);
}

TEST_F(test, open_2)
{
    sfd = -1;

    open_fake.return_val = -1;

    int fd = my_open("/dev/ttyS1");

    ASSERT_EQ(-1, fd);
}

TEST_F(test, open_3)
{
    sfd = -1;

    open_fake.return_val = 5;

    int fd = my_open("/dev/ttyS1");

    ASSERT_EQ(5, fd);
}

////////////////////////////////////////////////////////////////////////

TEST_F(test, close_1)
{
    sfd = 5;
    
    my_close();

    ASSERT_EQ(-1, sfd);
}

TEST_F(test, close_2)
{
    sfd = -1;
    
    my_close();

    ASSERT_EQ(-1, sfd);
}

////////////////////////////////////////////////////////////////////////

TEST_F(test, access_1)
{
    sfd = 5;

    class dummy {
    public:
        static ssize_t read(int fd, void *p, size_t sz) {
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
        }
    };

    write_fake.return_val = 5;
    read_fake.custom_fake = dummy::read;

    int ret = my_access();

    ASSERT_EQ(0, ret);
    ASSERT_EQ(2, read_fake.call_count);     //2回呼んでる
}

TEST_F(test, access_2)
{
    sfd = 5;

    class dummy {
    public:
        static ssize_t read(int fd, void *p, size_t sz) {
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
        }
    };

    write_fake.return_val = 5;
    read_fake.custom_fake = dummy::read;

    int ret = my_access();

    ASSERT_EQ(-3, ret);
    ASSERT_EQ(1, read_fake.call_count);     //1回だけ呼ぶ
}

TEST_F(test, access_3)
{
    sfd = 5;

    class dummy {
    public:
        static ssize_t read(int fd, void *p, size_t sz) {
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
        }
    };

    write_fake.return_val = 5;
    read_fake.custom_fake = dummy::read;

    int ret = my_access();

    ASSERT_EQ(-4, ret);
    ASSERT_EQ(2, read_fake.call_count);     //2回呼んでる
}

TEST_F(test, access_4)
{
    sfd = 5;

    write_fake.return_val = 4;

    int ret = my_access();

    ASSERT_EQ(-2, ret);
    ASSERT_EQ(0, read_fake.call_count);     //お呼びでない
}

