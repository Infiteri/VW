BIN=Bin
OBJ=Bin-Obj
DLL=VW
EXE=VW

.PHONY: help always dev win run kill compile_commands

help:
	@echo "VW build: "
	@echo "build outputs same executable: "
	@echo "				win - Builds the windows executable"
	@echo "				dev - Builds the dev executable"
	@echo "				run - Run the executable"
	@echo "				kill - Kill the running executable"
	@echo "				compile_commands - Generate compile_commands.json for clangd"

always:
	@mkdir -p $(BIN)
	@mkdir -p $(OBJ)

dev: always

win: always
	@make -j12 -C VW --no-print-directory
	@make -j12 -C WinApp --no-print-directory

run:
	@start $(BIN)\$(EXE).exe

kill:
	@taskkill /F /IM $(EXE).exe 2>nul || true
