# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -I./src/include
LDFLAGS = -L./src/lib -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .

# Target executable
TARGET = $(BIN_DIR)/main.exe

# Source and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Build target
$(TARGET): $(OBJ_FILES)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

# Phony targets
.PHONY: clean
