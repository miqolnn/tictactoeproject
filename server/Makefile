CXX=g++
CXXFLAGS=-std=c++17 -pthread
LDFLAGS=-lcurl

SRC=$(wildcard controller/*.cpp model/*.cpp service/*.cpp) main.cpp
TARGET=server

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)

