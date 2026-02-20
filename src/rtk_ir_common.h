#ifndef RTK_IR_COMMON_H
#define RTK_IR_COMMON_H

#include <string.h>
#include <sys/ioctl.h>
#include <linux/uvcvideo.h>

/* UVC Standard Constants */
#ifndef UVC_SET_CUR
#define UVC_SET_CUR 0x01
#endif
#ifndef UVC_GET_CUR
#define UVC_GET_CUR 0x81
#endif

/* Dell/Realtek Extension Unit ID */
#define RTK_IR_XU_UNIT 0x04

/**
 * Triggers the 5-step hardware initialization sequence for the IR emitter.
 * * @param fd The file descriptor for the /dev/video device.
 * @param ioctl_func A pointer to the ioctl function to use.
 * (Allows the hook to pass 'real_ioctl' while others pass standard 'ioctl').
 * @return 0 on success, -1 on failure.
 */
static inline int rtk_ir_trigger(int fd, int (*ioctl_func)(int, unsigned long, ...)) {
    unsigned char buf[8];
    struct uvc_xu_control_query q = {
        .unit = RTK_IR_XU_UNIT,
        .size = 8,
        .data = buf
    };

    // 1. Reset/Initialize
    memset(buf, 0, 8); buf[0] = 0xff;
    q.selector = 0x0a; q.query = UVC_SET_CUR;
    if (ioctl_func(fd, UVCIOC_CTRL_QUERY, &q) < 0) return -1;

    // 2. Set Address 0xfb00 (Length 5)
    memset(buf, 0, 8); buf[1] = 0xfb; buf[4] = 0x05;
    q.selector = 0x0a; q.query = UVC_SET_CUR;
    if (ioctl_func(fd, UVCIOC_CTRL_QUERY, &q) < 0) return -1;

    // 3. Query State
    memset(buf, 0, 8);
    q.selector = 0x0b; q.query = UVC_GET_CUR;
    if (ioctl_func(fd, UVCIOC_CTRL_QUERY, &q) < 0) return -1;

    // 4. Set Address 0x9f00 (Length 1)
    memset(buf, 0, 8); buf[1] = 0x9f; buf[4] = 0x01;
    q.selector = 0x0a; q.query = UVC_SET_CUR;
    if (ioctl_func(fd, UVCIOC_CTRL_QUERY, &q) < 0) return -1;

    // 5. Final Toggle
    memset(buf, 0, 8);
    q.selector = 0x0b; q.query = UVC_SET_CUR;
    if (ioctl_func(fd, UVCIOC_CTRL_QUERY, &q) < 0) return -1;

    return 0;
}

#endif /* RTK_IR_COMMON_H */
