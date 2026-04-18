export BIN=$(abspath Bin)
export OBJ=$(abspath Bin-Obj)
export DLL=VW
export EXE=VW

.PHONY: help always dev win run kill vendor assets

help:
	@echo "VW build: "
	@echo "build outputs same executable: "
	@echo "				win - Builds the windows executable"
	@echo "				dev - Builds the dev executable"
	@echo "				run - Run the executable"
	@echo "				kill - Kill the running executable"
	@echo "				vendor - Builds vendor files"
	@echo "				assets - Copies the assets needed by VW"
	@echo "				test - Builds the test executable"
	@echo "				run_tests - Builds and runs the tests"

always: 
	@mkdir -p $(BIN)
	@mkdir -p $(OBJ)

assets: always
	@echo Assetsing...
	@cp Shader.glsl $(BIN)/Shader.glsl
	@cp Screen.glsl $(BIN)/Screen.glsl

dev: always assets
	@make -j12 -C VW --no-print-directory
	@make -j12 -C DevApp --no-print-directory

win: always assets
	@make -j12 -C VW --no-print-directory
	@make -j12 -C WinApp --no-print-directory

run:
	@cd $(BIN) && start $(EXE).exe && cd ..

kill:
	@taskkill /F /IM $(EXE).exe 2>nul || true

vendor: always
	@make -j12 -C VW/Vendor/glad --no-print-directory
	@make -j12 -C DevApp/Vendor/imgui --no-print-directory
	@cp DevApp/Vendor/glfw/lib/glfw3.dll $(BIN)/glfw3.dll

test:
	@make -j12 -C Tests --no-print-directory

run_tests:
	@$(BIN)/Tests.exe --success
