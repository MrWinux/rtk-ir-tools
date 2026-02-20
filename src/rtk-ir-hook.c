#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <pthread.h>
#include <linux/videodev2.h>
#include "rtk_ir_common.h"

static int (*real_ioctl)(int, unsigned long, ...) = NULL;
static pthread_once_t init_done = PTHREAD_ONCE_INIT;

static void find_real_ioctl() {
    real_ioctl = dlsym(RTLD_NEXT, "ioctl");
}

static void run_ir_init(int fd) {
    static int hardware_initialized = 0;
    if (hardware_initialized) return;

    struct v4l2_capability cap;
    if (real_ioctl(fd, VIDIOC_QUERYCAP, &cap) < 0) return;
    if (!(cap.device_caps & V4L2_CAP_VIDEO_CAPTURE)) return;

    if (rtk_ir_trigger(fd, real_ioctl) == 0) {
        hardware_initialized = 1;
    }
}

int ioctl(int fd, unsigned long request, ...) {
    pthread_once(&init_done, find_real_ioctl);

    va_list args;
    va_start(args, request);
    void *arg = va_arg(args, void *);
    va_end(args);

    if (request == VIDIOC_QUERYCAP || request == VIDIOC_S_FMT) {
        run_ir_init(fd);
    }

    return real_ioctl(fd, request, arg);
}
