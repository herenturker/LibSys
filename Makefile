.PHONY: all libsys installer updater clean

all: libsys installer updater

libsys:
	$(MAKE) -C libsys

installer:
	$(MAKE) -C libsys_installer

updater:
	$(MAKE) -C libsys_updater

clean:
	$(MAKE) -C libsys clean
	$(MAKE) -C libsys_installer clean
	$(MAKE) -C libsys_updater clean
