BIN := ../Bin
OBJ := ../Bin-Obj
DLL ?= VW
ASSEMBLY := VW
ASSEMBLY_OBJ_DIR := $(OBJ)/$(ASSEMBLY)

COMPILE_FLAGS=-g
INCLUDE_FLAGS=-ISource -IVendor/glad/include -IVendor/stb -IVendor/assimp/include -IVendor/assimp/build/include -IVendor/yaml/include
DEFINES=-DYAML_CPP_STATIC_DEFINE
LINK_FLAGS=-shared -L$(BIN) -LVendor/assimp/build/lib $(OBJ)/glad/glad.o -lassimp -lyaml

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
