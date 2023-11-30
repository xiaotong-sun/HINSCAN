CC = g++
CFLAGS = -c -O3 -std=c++20 -I include

SRC_BASELINE = src/baseline
SRC_BEAN = src/bean
SRC_UTIL = src/util
TARGET_DIR = .obj

SRCS := $(wildcard $(SRC_BASELINE)/*.cpp $(SRC_BEAN)/*.cpp $(SRC_UTIL)/*.cpp)
OBJS := $(patsubst $(SRC_BASELINE)/%.cpp, $(TARGET_DIR)/%.o, $(filter $(SRC_BASELINE)/%.cpp, $(SRCS)))
OBJS += $(patsubst $(SRC_BEAN)/%.cpp, $(TARGET_DIR)/%.o, $(filter $(SRC_BEAN)/%.cpp, $(SRCS)))
OBJS += $(patsubst $(SRC_UTIL)/%.cpp, $(TARGET_DIR)/%.o, $(filter $(SRC_UTIL)/%.cpp, $(SRCS)))
OBJS += $(TARGET_DIR)/main.o

all: hinscan

hinscan: $(OBJS)
	$(CC) $(OBJS) -o hinscan

$(TARGET_DIR)/%.o: $(SRC_BASELINE)/%.cpp | $(TARGET_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(TARGET_DIR)/%.o: $(SRC_BEAN)/%.cpp | $(TARGET_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(TARGET_DIR)/%.o: $(SRC_UTIL)/%.cpp | $(TARGET_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(TARGET_DIR)/main.o: main.cpp | $(TARGET_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(TARGET_DIR):
	mkdir $(TARGET_DIR)

clean:
#	rd /s /q .obj
	rm -rf *o .obj
