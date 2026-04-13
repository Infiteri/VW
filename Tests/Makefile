BIN := ../Bin
OBJ := ../Bin-Obj
TEST_EXE := Tests

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)) $(wildcard $1$2)

TEST_SRC := $(call rwildcard,Source/,*.cpp)
TEST_OBJ := $(TEST_SRC:%.cpp=$(OBJ)/Tests/%.o)

INCLUDE_FLAGS=-ISource -IVendor -I../VW/Source

COMPILE_FLAGS=-g
LINK_FLAGS=-L$(BIN) -lVW

.PHONY: test

test: $(TEST_EXE)
	@echo Tests built....

$(TEST_EXE): $(TEST_OBJ)
	@g++ $^ $(LINK_FLAGS) -o $(BIN)/$(TEST_EXE).exe

$(OBJ)/Tests/%.o: %.cpp
	@mkdir -p $(dir $@)
	@g++ -c $< -o $@ $(COMPILE_FLAGS) $(INCLUDE_FLAGS)
