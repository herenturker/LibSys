.PHONY: all LibSys Installer Updater build clean

all: LibSys Installer Updater

LibSys:
	$(MAKE) -C LibSys

Installer:
	$(MAKE) -C LibSysInstaller

Updater:
	$(MAKE) -C LibSysUpdater

build:
	@echo "Building all projects using CMake..."
	cmake -B LibSys/build -G "MinGW Makefiles" LibSys
	cmake --build LibSys/build -j$(shell nproc)

	cmake -B LibSysInstaller/build -G "MinGW Makefiles" LibSysInstaller
	cmake --build LibSysInstaller/build -j$(shell nproc)

	cmake -B LibSysUpdater/build -G "MinGW Makefiles" LibSysUpdater
	cmake --build LibSysUpdater/build -j$(shell nproc)
	
clean:
	@echo "Cleaning all build directories..."
	rm -rf LibSys/build
	rm -rf LibSysInstaller/build
	rm -rf LibSysUpdater/build
	$(MAKE) -C LibSys clean 
	$(MAKE) -C LibSysInstaller clean
	$(MAKE) -C LibSysUpdater clean

build-LibSys:
	cmake -B LibSys/build -G "MinGW Makefiles" LibSys
	cmake --build LibSys/build -j$(shell nproc)

build-LibSysInstaller:
	cmake -B LibSysInstaller/build -G "MinGW Makefiles" LibSysInstaller
	cmake --build LibSysInstaller/build -j$(shell nproc)

build-LibSysUpdater:
	cmake -B LibSysUpdater/build -G "MinGW Makefiles" LibSysUpdater
	cmake --build LibSysUpdater/build -j$(shell nproc)
