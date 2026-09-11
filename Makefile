# Makefile for Stick Shift C++ with SFML
# 
# Usage:
#   make              Build the program
#   make run          Build and run
#   make clean        Delete executable
#   make rebuild      Clean and build
#   make help         Show this help

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -O2
CXXFLAGS_DEBUG = -std=c++17 -g -O0

# Paths (adjust these to your installation)
SFML_PATH = C:\SFML-3.0
MINGW_PATH = C:\mingw64

# SFML configuration
SFML_INCLUDE = -I"$(SFML_PATH)\include"
SFML_LIB = -L"$(SFML_PATH)\lib" \
           -lsfml-graphics \
           -lsfml-audio \
           -lsfml-window \
           -lsfml-system

# Windows-specific
WINFLAGS = -mwindows

# Output and source
OUTPUT = stick-dance.exe
OUTPUT_DEBUG = stick-dance-debug.exe
SOURCE = main.cpp

# Targets
.PHONY: all build run clean rebuild help debug copy-dlls

# Default target
all: build

# Build optimized version
build: $(OUTPUT)

# Compile optimized executable
$(OUTPUT): $(SOURCE)
	@echo Compiling $(SOURCE)...
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_INCLUDE) $(SFML_LIB) $(WINFLAGS)
	@echo Build complete: $(OUTPUT)
	@if exist song.mp3 (echo Found song.mp3) else (echo WARNING: No song.mp3 found)

# Build debug version (shows console, no optimization)
debug: $(OUTPUT_DEBUG)

$(OUTPUT_DEBUG): $(SOURCE)
	@echo Compiling debug version...
	$(CXX) $(CXXFLAGS_DEBUG) -o $@ $^ $(SFML_INCLUDE) $(SFML_LIB)
	@echo Debug build complete: $(OUTPUT_DEBUG)

# Copy DLL files from SFML
copy-dlls:
	@echo Copying SFML DLL files...
	@if exist "$(SFML_PATH)\bin\sfml-graphics-3.dll" (copy "$(SFML_PATH)\bin\sfml-graphics-3.dll" . && echo Copied sfml-graphics-3.dll)
	@if exist "$(SFML_PATH)\bin\sfml-audio-3.dll" (copy "$(SFML_PATH)\bin\sfml-audio-3.dll" . && echo Copied sfml-audio-3.dll)
	@if exist "$(SFML_PATH)\bin\sfml-window-3.dll" (copy "$(SFML_PATH)\bin\sfml-window-3.dll" . && echo Copied sfml-window-3.dll)
	@if exist "$(SFML_PATH)\bin\sfml-system-3.dll" (copy "$(SFML_PATH)\bin\sfml-system-3.dll" . && echo Copied sfml-system-3.dll)
	@if exist "$(SFML_PATH)\bin\openal32.dll" (copy "$(SFML_PATH)\bin\openal32.dll" . && echo Copied openal32.dll)
	@echo DLL copy complete

# Run the program
run: build copy-dlls
	@echo.
	@echo Running $(OUTPUT)...
	@echo.
	@$(OUTPUT)

# Run debug version
run-debug: debug
	@echo.
	@echo Running debug version $(OUTPUT_DEBUG)...
	@echo.
	@$(OUTPUT_DEBUG)

# Clean up compiled files
clean:
	@echo Cleaning...
	@if exist $(OUTPUT) (del $(OUTPUT) && echo Deleted $(OUTPUT)) else (echo $(OUTPUT) not found)
	@if exist $(OUTPUT_DEBUG) (del $(OUTPUT_DEBUG) && echo Deleted $(OUTPUT_DEBUG)) else (echo $(OUTPUT_DEBUG) not found)
	@echo Clean complete

# Rebuild: clean and build
rebuild: clean build

# Full rebuild with DLL copy
full-build: clean build copy-dlls
	@echo Full build and setup complete!

# Show help
help:
	@echo.
	@echo Makefile for Stick Shift (C++ with SFML)
	@echo.
	@echo Targets:
	@echo   make            Build optimized executable
	@echo   make debug      Build debug version (with console)
	@echo   make run        Build and run optimized version
	@echo   make run-debug  Build and run debug version
	@echo   make clean      Delete executables
	@echo   make rebuild    Clean and build
	@echo   make copy-dlls  Copy SFML DLL files to current directory
	@echo   make full-build Clean build + DLL copy
	@echo   make help       Show this help
	@echo.
	@echo Configuration:
	@echo   CXX      = $(CXX)
	@echo   SFML     = $(SFML_PATH)
	@echo   MinGW    = $(MINGW_PATH)
	@echo   Output   = $(OUTPUT)
	@echo.
	@echo To modify paths, edit SFML_PATH and MINGW_PATH in this Makefile
	@echo.

# Phony targets that don't produce files
.PHONY: help clean rebuild all build debug run run-debug copy-dlls full-build
