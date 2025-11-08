
CXX = g++
CC = gcc
MOC = C:/Qt/6.9.2/mingw_64/bin/moc
RCC = C:/Qt/6.9.2/mingw_64/bin/rcc
WINDRES = windres


OBJDIR = project/obj
MOCDIR = project/moc
RCCDIR = project/rcc


CXXFLAGS = -O2 -std=c++17 -Wall -Wextra -Wpedantic -Wshadow \
  -Wformat=2 -Wfloat-equal -Wcast-align -Wpointer-arith -Wstrict-overflow=5 \
  -Wwrite-strings -Wno-unused-parameter -fstack-protector-strong -fPIC \
  -pipe -mwindows \
  -I./libsys/src/headers -I./sqlite \
  -IC:/msys64/mingw64/include \
  -IC:/Qt/6.9.2/mingw_64/include \
  -IC:/Qt/6.9.2/mingw_64/include/QtCore \
  -IC:/Qt/6.9.2/mingw_64/include/QtGui \
  -IC:/Qt/6.9.2/mingw_64/include/QtWidgets \
  -IC:/Qt/6.9.2/mingw_64/include/QtSql

CFLAGS = -O2 -Wall -Wextra -Wpedantic -Wshadow \
  -I./libsys/src/headers -I./sqlite -IC:/msys64/mingw64/include \
  -fPIC -pipe -g -mwindows

LIBS = -LC:/Qt/6.9.2/mingw_64/lib -lQt6Core -lQt6Gui -lQt6Widgets -lQt6Sql -lpthread


CPP_SOURCES = $(wildcard libsys/src/*.cpp) $(wildcard libsys_updater/src/*.cpp) $(wildcard libsys_installer/src/*.cpp)
MOC_HEADERS = libsys/src/headers/LoginWindow.h libsys/src/headers/ProgramInterface.h
MOC_CPP = $(MOC_HEADERS:libsys/src/headers/%.h=$(MOCDIR)/moc_%.cpp)
OBJ_MOC = $(MOC_CPP:$(MOCDIR)/%.cpp=$(OBJDIR)/%.o)
OBJ_CPP = $(CPP_SOURCES:%.cpp=$(OBJDIR)/%.o)

RCC_FILES = icons/resources.qrc
RCC_CPP = $(RCC_FILES:%.qrc=$(RCCDIR)/qrc_%.cpp)
OBJ_RCC = $(RCC_CPP:$(RCCDIR)/%.cpp=$(OBJDIR)/%.o)
RC_OBJ = $(OBJDIR)/LibSysRes.o

TARGET = libsys.exe

all: $(TARGET)

define MAKE_DIR
	mkdir -p $(dir $1)
endef

$(OBJDIR)/LibSysRes.o: icons/LibSys.rc | $(OBJDIR)
	$(WINDRES) $< -O coff -o $@

$(MOCDIR)/moc_%.cpp: libsys/src/headers/%.h
	$(call MAKE_DIR,$@)
	$(MOC) $< -o $@

$(RCCDIR)/qrc_%.cpp: %.qrc
	$(call MAKE_DIR,$@)
	$(RCC) $< -o $@

$(OBJDIR)/%.o: %.cpp
	$(call MAKE_DIR,$@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(MOCDIR)/%.cpp
	$(call MAKE_DIR,$@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(RCCDIR)/%.cpp
	$(call MAKE_DIR,$@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.c
	$(call MAKE_DIR,$@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ_CPP) $(OBJ_MOC) $(OBJ_RCC) $(RC_OBJ)
	$(CXX) $^ -o $@ $(LIBS)

clean:
	rm -rf $(OBJDIR) $(MOCDIR) $(RCCDIR) $(TARGET)
