# If you'd like to build for
# - Linux x86 (32-bit), replace "x64" below with "x86".
# - Linux ARM64 (64-bit), replace "x64" below with "arm64".
TARGET_PLATFORM:=x64
CXXFLAGS := -std=c++17 -pedantic-errors -Wall -Wextra -Werror
BUILD    := ./build
SPEECHSDK_ROOT	:= /opt/speechsdk
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
TARGET   := which_card
LDFLAGS  := -L/usr/lib -L$(SPEECHSDK_ROOT)/lib/$(TARGET_PLATFORM) -lMicrosoft.CognitiveServices.Speech.core -lpthread -l:libasound.so.2
INCLUDE  := -Iinclude/ -I$(SPEECHSDK_ROOT)/include/cxx_api -I$(SPEECHSDK_ROOT)/include/c_api
SRC      := $(wildcard *.cpp)         \

OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@ $(LDFLAGS)

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

.PHONY: all build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*