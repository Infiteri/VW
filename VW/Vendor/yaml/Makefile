LIB_NAME = yaml.dll
CXX = g++
CXXFLAGS = -fPIC -O3 -g -Wall -Wformat -Dyaml_cpp_EXPORTS
CPPFLAGS = -Iinclude  # Adjust this to your actual include path

SOURCES = $(wildcard src/*.cpp)
OBJS = $(addprefix $(OBJ)/yaml/, $(notdir $(SOURCES:.cpp=.o)))
vpath %.cpp src

.PHONY: all clean

all: $(BIN)/$(LIB_NAME)

$(BIN)/$(LIB_NAME): $(OBJS)
	@mkdir -p $(dir $@)
	@echo "Linking $(LIB_NAME)..."
	$(CXX) -shared -o $@ $(OBJS)

$(OBJ)/yaml/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling YAML: $<"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)/yaml $(BIN)/$(LIB_NAME)
