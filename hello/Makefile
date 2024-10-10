KDIR = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd -P)

all default: modules

modules modules_install:
	make -C $(KDIR) M=$(PWD) $@

compile-commands:
	bear -- make

clean:
	rm -rf *.ko
