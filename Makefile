# Variables
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iheaders -I/opt/homebrew/include -I/opt/homebrew/include/SDL2
LIBS = -L/opt/homebrew/lib -lfreeimage -lSDL2
SRC = main.cpp
BUILD_DIR = debug
TARGET = $(BUILD_DIR)/app

# Reglas
.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBS)

run: all
	@./$(TARGET)

clean:
	rm -f $(TARGET)
