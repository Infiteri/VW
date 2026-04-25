export BIN=$(abspath Bin)
export OBJ=$(abspath Bin-Obj)
export DLL=VW
export EXE=VW

.PHONY: help all clean always dev win run kill vendor assets

help:
	@echo "VW build: "
	@echo "build outputs same executable: "
	@echo "				all - Run for full project build (dev target)"
	@echo "				clean - Cleans project (removes Bin and Bin-Obj, run before full rebuilds)"
	@echo "				win - Builds the windows executable"
	@echo "				dev - Builds the dev executable"
	@echo "				run - Run the executable"
	@echo "				vendor - Builds vendor files"
	@echo "				assets - Copies the assets needed by VW"
	@echo "				test - Builds the test executable"
	@echo "				run_tests - Builds and runs the tests"

all: always assets vendor dev

clean:
	@echo Cleaning...
	@rm -r $(BIN)
	@rm -r $(OBJ)

always: 
	@mkdir -p $(BIN)
	@mkdir -p $(OBJ)

assets: always
	@echo Copying assets...
	@cp -r VW/Assets/* $(BIN)

dev: always assets
	@echo Building dev...
	@make -j12 -C VW --no-print-directory
	@make -j12 -C DevApp --no-print-directory

win: always assets
	@echo Building win...
	@make -j12 -C VW --no-print-directory
	@make -j12 -C WinApp --no-print-directory

run:
	@cd $(BIN) && start $(EXE).exe && cd ..

vendor: always
	@echo Building vendor...
	@cp DevApp/Vendor/glfw/lib/glfw3.dll $(BIN)/glfw3.dll
	@make -j12 -C VW/Vendor/glad --no-print-directory
	@make -j12 -C DevApp/Vendor/imgui --no-print-directory

test:
	@echo Building tests...
	@make -j12 -C Tests --no-print-directory

run_tests:
	@$(BIN)/Tests.exe --success
