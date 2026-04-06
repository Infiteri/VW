BIN=Bin
OBJ=Bin-Obj
DLL=VW
EXE=VW

.PHONY: help always dev win run kill vendor

help:
	@echo "VW build: "
	@echo "build outputs same executable: "
	@echo "				win - Builds the windows executable"
	@echo "				dev - Builds the dev executable"
	@echo "				run - Run the executable"
	@echo "				kill - Kill the running executable"
	@echo "				vendor - Builds vendor files"

always:
	@mkdir -p $(BIN)
	@mkdir -p $(OBJ)

dev: always
	@make -j12 -C VW --no-print-directory
	@make -j12 -C DevApp --no-print-directory

win: always
	@make -j12 -C VW --no-print-directory
	@make -j12 -C WinApp --no-print-directory

run:
	@start $(BIN)\$(EXE).exe

kill:
	@taskkill /F /IM $(EXE).exe 2>nul || true

vendor: always
	@make -j12 -C VW/Vendor/glad --no-print-directory
	@cp DevApp/Vendor/glfw/lib/glfw3.dll $(BIN)/glfw3.dll
