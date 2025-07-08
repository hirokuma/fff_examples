#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int sfd = -1;

int my_open(const char *pTty)
{
    if (sfd != -1) {
        return -100;
    }

    int fd = open(pTty, O_RDWR);
    if (fd == -1) {
        return -1;
    }
    sfd = fd;
    return sfd;
}

void my_close(void)
{
    close(sfd);
    sfd = -1;
}

int my_access(void)
{
    const uint8_t WRT[] = { 0x00, 0x01, 0x02, 0x03, 0x04 };

    uint8_t buf[20];
    ssize_t sz;

    sz = write(sfd, WRT, sizeof(WRT));
    if (sz != sizeof(WRT)) {
        return -2;
    }

    sz = read(sfd, buf, 5);
    if ((sz != 5) || (buf[4] != 0xff)) {
        return -3;
    }
    sz = read(sfd, &buf[5], 3);
    if ((sz != 3) || (buf[7] != 0x35)) {
        return -4;
    }

    return 0;
}
