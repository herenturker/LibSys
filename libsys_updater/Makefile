CXX = g++
CC = gcc
MOC = C:/Qt/6.9.2/mingw_64/bin/moc

OBJDIR = obj
MOCDIR = moc

CXXFLAGS = -O2 -std=c++17 -Wall -Wextra -Wpedantic -Wshadow \
  -Wformat=2 -Wfloat-equal -Wcast-align -Wpointer-arith -Wstrict-overflow=5 \
  -Wwrite-strings -Wno-unused-parameter -fstack-protector-strong -fPIC \
  -pipe -mwindows \
  -I./src -I./src/headers -I../sqlite \
  -IC:/Qt/6.9.2/mingw_64/include \
  -IC:/Qt/6.9.2/mingw_64/include/QtCore \
  -IC:/Qt/6.9.2/mingw_64/include/QtGui \
  -IC:/Qt/6.9.2/mingw_64/include/QtWidgets \
  -IC:/Qt/6.9.2/mingw_64/include/QtNetwork \
  -IC:/Qt/6.9.2/mingw_64/include/QtSql \
  -IC:/Qt/6.9.2/mingw_64/include/QtNetwork

CFLAGS = -O2 -Wall -Wextra -Wpedantic -Wshadow \
  -I./src -I../sqlite -fPIC -pipe -g -mwindows

LIBS = -LC:/Qt/6.9.2/mingw_64/lib \
  -lQt6Core -lQt6Gui -lQt6Widgets -lQt6Network -lQt6Sql -lpthread

CPP_SOURCES = $(wildcard src/*.cpp)
MOC_HEADERS = $(wildcard src/headers/*.h)
MOC_CPP = $(MOC_HEADERS:src/headers/%.h=$(MOCDIR)/moc_%.cpp)

OBJ_CPP = $(CPP_SOURCES:%.cpp=$(OBJDIR)/%.o)
OBJ_MOC = $(MOC_CPP:$(MOCDIR)/%.cpp=$(OBJDIR)/%.o)

TARGET = libsys_updater.exe

define MAKE_DIR
	mkdir -p $(dir $1)
endef

all: $(TARGET)

$(MOCDIR)/moc_%.cpp: src/headers/%.h
	$(call MAKE_DIR,$@)
	$(MOC) $< -o $@

$(OBJDIR)/%.o: %.cpp
	$(call MAKE_DIR,$@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(MOCDIR)/%.cpp
	$(call MAKE_DIR,$@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJ_CPP) $(OBJ_MOC)
	$(CXX) $^ -o $@ $(LIBS)

clean:
	rm -rf $(OBJDIR) $(MOCDIR) $(TARGET)
