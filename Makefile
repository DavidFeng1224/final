# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -I./src/include -static
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
	$(CXX) -o $@ $^ $(LDFLAGS) -mwindows

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	$(TARGET)

# Clean up object files and executable
clean:
	del /Q obj\*.o main.exe

# Phony targets
.PHONY: clean
