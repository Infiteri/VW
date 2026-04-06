BIN ?= ../Bin
OBJ ?= ../Bin-Obj
EXE ?= VW
ASSEMBLY := WinApp
ASSEMBLY_OBJ_DIR := $(OBJ)/$(ASSEMBLY)

COMPILE_FLAGS=-g
INCLUDE_FLAGS=-I../VW/Source
DEFINES=
LINK_FLAGS= -L$(BIN) -lVW -lopengl32 -lgdi32 -luser32 

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)) $(wildcard $1$2)

SRC_FILES := $(call rwildcard,Source/,*.cpp)
OBJ_FILES := $(SRC_FILES:%.cpp=$(ASSEMBLY_OBJ_DIR)/%.o)

.PHONY: all
all: $(EXE)
	@echo $0

$(ASSEMBLY_OBJ_DIR)/%.o: %.cpp
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
	@g++ -fPIC -c $< -o $@ $(COMPILE_FLAGS) $(DEFINES) $(INCLUDE_FLAGS)

$(EXE): $(OBJ_FILES)
	@g++ $(OBJ_FILES) $(LINK_FLAGS) -o $(BIN)/$(EXE)
