LDFLAGS = -lglfw3 -lopengl32 -lgdi32
PROG=game.exe
SRC_DIR := src
INC_DIR := include
BIN_DIR := bin
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
DEPS := $(wildcard $(INC_DIR)/*.h)
CC=g++
CFLAGS=-Wall -std=c++20 -I$(INC_DIR)
DEBUG?=1

ifeq ($(DEBUG), 1)
    CFLAGS += -g
    OUTPUTDIR=$(BIN_DIR)/debug
    PROG=game-debug.exe
else
    CFLAGS += -g0 -O3
    OUTPUTDIR=$(BIN_DIR)/release
    PROG=game.exe
endif

OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OUTPUTDIR)/%.o,$(SOURCES))

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LDFLAGS)

$(OUTPUTDIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
ifeq ($(OS),Windows_NT)
	if not exist "$(dir $@)" mkdir "$(dir $@)"
else
	@mkdir -p $(dir $@)
endif
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@rm -rf $(BIN_DIR)/*

.PHONY: clean