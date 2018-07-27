SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin

# List all the sources
SRCS = fact.cpp
COMPILER = clang++
INCLUDES = -I$(SRC_DIR)
CPPFLAGS = -std=c++14 -O2 -Wall -fsanitize=address
LDFLAGS = -fsanitize=address

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(COMPILER) -o $@ $(INCLUDES) $(CPPFLAGS) -c $^

TARGET_BIN = $(BIN_DIR)/fact

all : make_dirs $(TARGET_BIN)

$(TARGET_BIN) : $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
	$(COMPILER) $(LDFLAGS) -o $@ $^ $(LDLIBS)

make_dirs :
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)

run: all
	$(TARGET_BIN)

test: all
	$(TARGET_BIN)
