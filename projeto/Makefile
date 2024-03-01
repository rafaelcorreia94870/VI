CXX      := g++ 
CXXFLAGS := -std=c++11 -O3
LDFLAGS  := 
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
SHELL	 := /bin/bash

TARGET   := src

INCLUDE  := -Isrc/Camera/ -Isrc/Image -Isrc/Light -Isrc/Primitive -Isrc/Primitive/BRDF -Isrc/Primitive/Geometry -Isrc/Rays -Isrc/Renderer -Isrc/Scene -Isrc/Shader -Isrc/utils -Isrc/Scene/tinyobjloader/

SRC      :=                      \
   $(wildcard src/*.cpp) \
   $(wildcard src/Camera/*.cpp)         \
   $(wildcard src/Image/*.cpp)         \
   $(wildcard src/Primitive/BRDF/*.cpp)         \
   $(wildcard src/Primitive/Geometry/*.cpp)         \
   $(wildcard src/Renderer/*.cpp)         \
   $(wildcard src/Scene/*.cpp)         \
   $(wildcard src/Shader/*.cpp)         \

OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEPENDENCIES \
         := $(OBJECTS:.o=.d)

all:	build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

-include $(DEPENDENCIES)

.PHONY: all build clean 

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*


