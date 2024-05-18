ROOT_DIR=$(shell cd)
BIN_DIR=$(ROOT_DIR)\bin

CC_X64=$(BIN_DIR)\x64\mingw64\bin\g++
CC_X86=$(BIN_DIR)\x86\mingw32\bin\g++

SRC_DIR=$(ROOT_DIR)
BUILD_DIR=$(ROOT_DIR)\build

BUNDLE_DIR=$(ROOT_DIR)\bundle
7ZIP_DIR=$(BIN_DIR)\7z
SFX_7ZIP_DIR=$(7ZIP_DIR)\sfx

SYSTEM_ARCH := $(shell powershell -Command "& {Get-WmiObject -Class Win32_Processor | Select-Object -ExpandProperty Architecture}")

ifeq ($(SYSTEM_ARCH),9)
    ARCH_TYPE = x64
else
    ARCH_TYPE = x86
endif

# SOURCES FILES
SRC_FILES = 							\
			$(SRC_DIR)\Win32API.cpp 	\
			$(SRC_DIR)\main.cpp

MANIFEST_FILE =							\
			$(SRC_DIR)\resource.rc

MANIFEST_OBJ = 							\
			$(BUILD_DIR)\resource.o

BUILD_FILE_64 = 									\
			$(BUILD_DIR)\shutdown_ctrl_x64.exe

BUILD_FILE_32 = 									\
			$(BUILD_DIR)\shutdown_ctrl_x86.exe

# COMPILER FLAGS
CXXFLAGS_X64= -std=c++11 -fpermissive -I. -m64 -D_WIN32_WINNT=0x0501 -DWIN32_WINNT=0x0501 -Ofast
CXXFLAGS_X86= -std=c++11 -fpermissive -I. -m32 -D_WIN32_WINNT=0x0501 -DWIN32_WINNT=0x0501 -Ofast

# LINKER FLAGS
LDFLAGS_X64= -lkernel32 -ladvapi32 -luser32 -lole32 -lshell32 -lshlwapi -lcomctl32 -lcomdlg32 -lgdi32 -lwinspool -static-libgcc -static-libstdc++ --static
LDFLAGS_X86= -lkernel32 -ladvapi32 -luser32 -lole32 -lshell32 -lshlwapi -lcomctl32 -lcomdlg32 -lgdi32 -lwinspool -static-libgcc -static-libstdc++ --static

.PHONY: all clean

all:
	$(MAKE) -C $(ROOT_DIR) clean
	mkdir $(BUILD_DIR)
	$(CC_X64) $(SRC_FILES) $(CXXFLAGS_X64) -o $(BUILD_FILE_64) $(LDFLAGS_X64)
	$(CC_X86) $(SRC_FILES) $(CXXFLAGS_X86) -o $(BUILD_FILE_32) $(LDFLAGS_X86)

	@echo Copying neccessary file
	copy $(SFX_7ZIP_DIR)\*.* $(BUILD_DIR)
	copy $(BUNDLE_DIR)\*.* $(BUILD_DIR)

ifeq ($(ARCH_TYPE),x64)
	@echo The machine is running in 64-bit
	copy $(7ZIP_DIR)\x64\*.* $(BUILD_DIR)
	cd /d $(BUILD_DIR) && 7za.exe a build.7z shutdown_ctrl_x64.exe shutdown_ctrl_x86.exe run.bat -r
	cd /d $(BUILD_DIR) && copy /b 7zSD.sfx + config.txt + run.bat + build.7z shutdown_ctrl_installer.exe
	del /f /s /q $(BUILD_DIR)\*.sfx
	del /f /s /q $(BUILD_DIR)\build.7z
	del /f /s /q $(BUILD_DIR)\7za* $(BUILD_DIR)\7zxa*
	del /f /s /q $(BUILD_DIR)\*.bat
	del /f /s /q $(BUILD_DIR)\*.txt
else
	@echo The machine is running in 32-bit
	copy $(7ZIP_DIR)\x86\*.* $(BUILD_DIR)
	cd /d $(BUILD_DIR) && 7za.exe a build.7z shutdown_ctrl_x64.exe shutdown_ctrl_x86.exe run.bat -r
	cd /d $(BUILD_DIR) && copy /b 7zSD.sfx + config.txt + run.bat + build.7z shutdown_ctrl_installer.exe
	del /f /s /q $(BUILD_DIR)\*.sfx
	del /f /s /q $(BUILD_DIR)\build.7z
	del /f /s /q $(BUILD_DIR)\7za* $(BUILD_DIR)\7zxa*
	del /f /s /q $(BUILD_DIR)\*.bat
	del /f /s /q $(BUILD_DIR)\*.txt
endif
clean:
	del /s /f /q $(BUILD_DIR)
	rmdir $(BUILD_DIR)