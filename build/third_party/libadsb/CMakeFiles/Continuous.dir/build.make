# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/brianm/cmake-3.22.0-linux-aarch64/bin/cmake

# The command to remove a file.
RM = /home/brianm/cmake-3.22.0-linux-aarch64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/data/embedded_vision_platform

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/data/embedded_vision_platform/build

# Utility rule file for Continuous.

# Include any custom commands dependencies for this target.
include third_party/libadsb/CMakeFiles/Continuous.dir/compiler_depend.make

# Include the progress variables for this target.
include third_party/libadsb/CMakeFiles/Continuous.dir/progress.make

third_party/libadsb/CMakeFiles/Continuous:
	cd /mnt/data/embedded_vision_platform/build/third_party/libadsb && /home/brianm/cmake-3.22.0-linux-aarch64/bin/ctest -D Continuous

Continuous: third_party/libadsb/CMakeFiles/Continuous
Continuous: third_party/libadsb/CMakeFiles/Continuous.dir/build.make
.PHONY : Continuous

# Rule to build all files generated by this target.
third_party/libadsb/CMakeFiles/Continuous.dir/build: Continuous
.PHONY : third_party/libadsb/CMakeFiles/Continuous.dir/build

third_party/libadsb/CMakeFiles/Continuous.dir/clean:
	cd /mnt/data/embedded_vision_platform/build/third_party/libadsb && $(CMAKE_COMMAND) -P CMakeFiles/Continuous.dir/cmake_clean.cmake
.PHONY : third_party/libadsb/CMakeFiles/Continuous.dir/clean

third_party/libadsb/CMakeFiles/Continuous.dir/depend:
	cd /mnt/data/embedded_vision_platform/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/data/embedded_vision_platform /mnt/data/embedded_vision_platform/third_party/libadsb /mnt/data/embedded_vision_platform/build /mnt/data/embedded_vision_platform/build/third_party/libadsb /mnt/data/embedded_vision_platform/build/third_party/libadsb/CMakeFiles/Continuous.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : third_party/libadsb/CMakeFiles/Continuous.dir/depend

