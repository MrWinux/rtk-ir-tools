#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "rtk_ir_common.h"

int main(int argc, char *argv[]) {
    char *dev = (argc > 1) ? argv[1] : "/dev/video0";

    if (access(dev, W_OK) != 0) {
        fprintf(stderr, "Error: Insufficient permissions for %s. Try running with sudo.\n", dev);
        return 1;
    }

    int fd = open(dev, O_RDWR);
    if (fd < 0) {
        perror("Error opening device");
        return 1;
    }


    if (rtk_ir_trigger(fd, ioctl) < 0) {
        fprintf(stderr, "Error: The IR hardware sequence failed.\n");
        close(fd);
        return 1;
    }

    printf("Success: Hardware switch toggled to IR mode.\n");
    close(fd);
    return 0;
}
