CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O3
LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system

TARGET = gol 
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $^ $(LDLIBS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
