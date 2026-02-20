#define PAM_SM_AUTH
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <syslog.h>
#include <fcntl.h>
#include <unistd.h>
#include "rtk_ir_common.h"

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    const char *device = "/dev/video0";

    for (int i = 0; i < argc; i++) {
        if (strncmp(argv[i], "device=", 7) == 0) {
            device = argv[i] + 7;
        }
    }

    int fd = open(device, O_RDWR);
    if (fd == -1) {
        pam_syslog(pamh, LOG_ERR, "rtk-ir: Could not open device %s: %m", device);
        return PAM_IGNORE;
    }

    if (rtk_ir_trigger(fd, ioctl) == 0) {
        pam_syslog(pamh, LOG_INFO, "rtk-ir: IR Emitter successfully initialized on %s", device);
    } else {
        pam_syslog(pamh, LOG_ERR, "rtk-ir: Hardware sequence failed on %s", device);
    }

    close(fd);
    return PAM_IGNORE;
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}
