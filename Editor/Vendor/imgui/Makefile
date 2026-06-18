LIB_NAME = imgui.dll
CXX = g++
CXXFLAGS = -O3 -g -Wall -Wformat
CPPFLAGS = -I. -I./backend -I../glfw/include

SOURCES = imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp
SOURCES += backends/imgui_impl_glfw.cpp backends/imgui_impl_opengl3.cpp

OBJS = $(addprefix $(OBJ)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))
vpath %.cpp . backends

.PHONY: all clean

all: $(BIN)/$(LIB_NAME)

# Link the shared library
$(BIN)/$(LIB_NAME): $(OBJS)
	@mkdir -p $(dir $@)
	@echo "Linking $(LIB_NAME)..."
	@$(CXX) -shared -o $@ $(OBJS) -L$(BIN) -lglfw3

$(OBJ)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling ImGui: $<"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(BIN)/$(LIB_NAME)
