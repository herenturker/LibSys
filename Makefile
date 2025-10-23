# Compiler settings
CXX = g++
CC = gcc

# C++ compiler flags
CXXFLAGS = -O2 -std=c++17 \
  -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wfloat-equal -Wcast-align \
  -Wpointer-arith -Wstrict-overflow=5 -Wwrite-strings -Wno-unused-parameter \
  -Wstack-protector -Wconversion -Wsign-conversion -Wdouble-promotion \
  -Wnull-dereference -Wduplicated-cond -Wlogical-op \
  -fstack-protector-strong -fPIC -pipe \
  -I./src/headers \
  -I./sqlite \
  -IC:/msys64/mingw64/include/c++/15.1.0 \
  -IC:/msys64/mingw64/include \
  -IC:/Qt/6.9.2/mingw_64/include \
  -IC:/Qt/6.9.2/mingw_64/include/QtCore \
  -IC:/Qt/6.9.2/mingw_64/include/QtGui \
  -IC:/Qt/6.9.2/mingw_64/include/QtWidgets

# C compiler flags
CFLAGS = -O2 \
  -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wfloat-equal -Wcast-align \
  -Wpointer-arith -Wstrict-overflow=5 -Wwrite-strings -Wmissing-declarations \
  -Wmissing-prototypes -Wno-unused-parameter -Wstack-protector \
  -Wconversion -Wsign-conversion -Wdouble-promotion -Wnull-dereference \
  -Wduplicated-cond -Wlogical-op -Wjump-misses-init -Wstrict-prototypes \
  -fstack-protector-strong -fPIC -pipe -g \
  -I./src/headers \
  -I./sqlite \
  -IC:/msys64/mingw64/include

# Libraries
LIBS = -LC:/Qt/6.9.2/mingw_64/lib \
       -lQt6Core -lQt6Gui -lQt6Widgets -lpthread

# Source files
CPP_SOURCES = $(wildcard src/*.cpp)
C_SOURCES = $(filter-out sqlite/shell.c, $(wildcard sqlite/*.c))


# Object files
OBJ_CPP = $(CPP_SOURCES:.cpp=.o)
OBJ_C = $(C_SOURCES:.c=.o)

# Target executable
TARGET = libsys.exe

# Default target
all: $(TARGET)

# Build C objects
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build C++ objects
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link everything together
$(TARGET): $(OBJ_CPP) $(OBJ_C)
	$(CXX) $(OBJ_CPP) $(OBJ_C) -o $(TARGET) $(LIBS)

# Clean build files
clean:
	rm -f $(OBJ_CPP) $(OBJ_C) $(TARGET)
