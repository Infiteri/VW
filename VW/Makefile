BIN := ../Bin
OBJ := ../Bin-Obj
DLL ?= VW
ASSEMBLY := VW
ASSEMBLY_OBJ_DIR := $(OBJ)/$(ASSEMBLY)

COMPILE_FLAGS=-g
INCLUDE_FLAGS=
DEFINES=
LINK_FLAGS=-shared

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)) $(wildcard $1$2)

SRC_FILES := $(call rwildcard,Source/,*.cpp)
OBJ_FILES := $(SRC_FILES:%.cpp=$(ASSEMBLY_OBJ_DIR)/%.o)

.PHONY: all
all: $(DLL)
	@echo $0

$(ASSEMBLY_OBJ_DIR)/%.o: %.cpp
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
	@g++ -fPIC -c $< -o $@ $(COMPILE_FLAGS) $(DEFINES) $(INCLUDE_FLAGS)

$(DLL): $(OBJ_FILES)
	@g++ $(OBJ_FILES) $(LINK_FLAGS) -o $(BIN)/$(DLL).dll
