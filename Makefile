CXX = g++
CC = gcc

CXXFLAGS = -O2 -std=c++17 \
  -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wfloat-equal -Wcast-align \
  -Wpointer-arith -Wstrict-overflow=5 -Wwrite-strings -Wno-unused-parameter \
  -fstack-protector-strong -fPIC -pipe -mwindows \
  -I./src/headers \
  -I./sqlite \
  -IC:/msys64/mingw64/include/c++/15.1.0 \
  -IC:/msys64/mingw64/include \
  -IC:/Qt/6.9.2/mingw_64/include \
  -IC:/Qt/6.9.2/mingw_64/include/QtCore \
  -IC:/Qt/6.9.2/mingw_64/include/QtGui \
  -IC:/Qt/6.9.2/mingw_64/include/QtWidgets

CFLAGS = -O2 -Wall -Wextra -Wpedantic -Wshadow \
  -I./src/headers -I./sqlite -IC:/msys64/mingw64/include -fPIC -pipe -g -mwindows

LIBS = -LC:/Qt/6.9.2/mingw_64/lib -lQt6Core -lQt6Gui -lQt6Widgets -lpthread

CPP_SOURCES = $(wildcard src/*.cpp)
# C_SOURCES = $(filter-out sqlite/shell.c, $(wildcard sqlite/*.c))
C_SOURCES = 

MOC_HEADERS = src/headers/LoginWindow.h src/headers/ProgramInterface.h
MOC_CPP = $(MOC_HEADERS:src/headers/%.h=src/moc_%.cpp)
OBJ_MOC = $(MOC_CPP:.cpp=.o)

OBJ_CPP = $(CPP_SOURCES:.cpp=.o)
OBJ_C = $(C_SOURCES:.c=.o)
RC_OBJ = icons/LibSys.o

TARGET = libsys.exe

all: $(TARGET)

# Generate MOC sources
src/moc_%.cpp: src/headers/%.h
	moc $< -o $@

# Compile C++ sources
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C sources
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile RC resource
icons/LibSys.o: icons/LibSys.rc
	windres $< -O coff -o $@

# Link all objects
$(TARGET): $(OBJ_CPP) $(OBJ_C) $(OBJ_MOC) $(RC_OBJ)
	$(CXX) $(OBJ_CPP) $(OBJ_C) $(OBJ_MOC) $(RC_OBJ) -o $@ $(LIBS)

# Clean build files
clean:
	rm -f $(OBJ_CPP) $(OBJ_C) $(OBJ_MOC) $(RC_OBJ) $(MOC_CPP) $(TARGET)
