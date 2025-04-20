CXX = g++
CXXFLAGS = -std=c++17 -pthread

SRC = main.cpp
TARGET = client

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

