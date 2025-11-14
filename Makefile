.PHONY: all LibSys Installer Updater clean

all: LibSys Installer Updater

LibSys:
	$(MAKE) -C LibSys

Installer:
	$(MAKE) -C LibSysInstaller

Updater:
	$(MAKE) -C LibSysUpdater

clean:
	$(MAKE) -C LibSys clean
	$(MAKE) -C LibSysInstaller clean
	$(MAKE) -C LibSysUpdater clean
