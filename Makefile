CC ?= gcc
CFLAGS ?= -O2 -g
LDFLAGS ?=
PREFIX ?= /usr
DESTDIR ?=

all: rtk-ir-ctl rtk-ir-hook.so pam_rtk_ir.so

rtk-ir-ctl: src/rtk-ir-ctl.c
	$(CC) $(CFLAGS) $(LDFLAGS) src/rtk-ir-ctl.c -o rtk-ir-ctl

rtk-ir-hook.so: src/rtk-ir-hook.c
	$(CC) $(CFLAGS) $(LDFLAGS) -fPIC -shared src/rtk-ir-hook.c -o rtk-ir-hook.so -ldl -lpthread

pam_rtk_ir.so: src/pam_rtk_ir.c
	$(CC) $(CFLAGS) $(LDFLAGS) -fPIC -shared src/pam_rtk_ir.c -o pam_rtk_ir.so -lpam

install: all
	install -Dm755 rtk-ir-ctl $(DESTDIR)$(PREFIX)/bin/rtk-ir-ctl
	install -Dm755 rtk-ir-hook.so $(DESTDIR)$(PREFIX)/lib/rtk-ir-hook.so
	install -Dm755 pam_rtk_ir.so $(DESTDIR)$(PREFIX)/lib/security/pam_rtk_ir.so

clean:
	rm -f rtk-ir-ctl rtk-ir-hook.so pam_rtk_ir.so
