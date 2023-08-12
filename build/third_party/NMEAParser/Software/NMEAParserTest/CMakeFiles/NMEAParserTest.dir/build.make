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

# Include any dependencies generated for this target.
include third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/compiler_depend.make

# Include the progress variables for this target.
include third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/progress.make

# Include the compile flags for this target's objects.
include third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/flags.make

third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/main.cpp.o: third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/flags.make
third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/main.cpp.o: ../third_party/NMEAParser/Software/NMEAParserTest/main.cpp
third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/main.cpp.o: third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/data/embedded_vision_platform/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/main.cpp.o"
	cd /mnt/data/embedded_vision_platform/build/third_party/NMEAParser/Software/NMEAParserTest && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/main.cpp.o -MF CMakeFiles/NMEAParserTest.dir/main.cpp.o.d -o CMakeFiles/NMEAParserTest.dir/main.cpp.o -c /mnt/data/embedded_vision_platform/third_party/NMEAParser/Software/NMEAParserTest/main.cpp

third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/NMEAParserTest.dir/main.cpp.i"
	cd /mnt/data/embedded_vision_platform/build/third_party/NMEAParser/Software/NMEAParserTest && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/data/embedded_vision_platform/third_party/NMEAParser/Software/NMEAParserTest/main.cpp > CMakeFiles/NMEAParserTest.dir/main.cpp.i

third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/NMEAParserTest.dir/main.cpp.s"
	cd /mnt/data/embedded_vision_platform/build/third_party/NMEAParser/Software/NMEAParserTest && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/data/embedded_vision_platform/third_party/NMEAParser/Software/NMEAParserTest/main.cpp -o CMakeFiles/NMEAParserTest.dir/main.cpp.s

# Object files for target NMEAParserTest
NMEAParserTest_OBJECTS = \
"CMakeFiles/NMEAParserTest.dir/main.cpp.o"

# External object files for target NMEAParserTest
NMEAParserTest_EXTERNAL_OBJECTS =

third_party/NMEAParser/Software/NMEAParserTest/NMEAParserTest: third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/main.cpp.o
third_party/NMEAParser/Software/NMEAParserTest/NMEAParserTest: third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/build.make
third_party/NMEAParser/Software/NMEAParserTest/NMEAParserTest: third_party/NMEAParser/Software/NMEAParserLib/libNMEAParserLib.a
third_party/NMEAParser/Software/NMEAParserTest/NMEAParserTest: third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/data/embedded_vision_platform/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable NMEAParserTest"
	cd /mnt/data/embedded_vision_platform/build/third_party/NMEAParser/Software/NMEAParserTest && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/NMEAParserTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/build: third_party/NMEAParser/Software/NMEAParserTest/NMEAParserTest
.PHONY : third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/build

third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/clean:
	cd /mnt/data/embedded_vision_platform/build/third_party/NMEAParser/Software/NMEAParserTest && $(CMAKE_COMMAND) -P CMakeFiles/NMEAParserTest.dir/cmake_clean.cmake
.PHONY : third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/clean

third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/depend:
	cd /mnt/data/embedded_vision_platform/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/data/embedded_vision_platform /mnt/data/embedded_vision_platform/third_party/NMEAParser/Software/NMEAParserTest /mnt/data/embedded_vision_platform/build /mnt/data/embedded_vision_platform/build/third_party/NMEAParser/Software/NMEAParserTest /mnt/data/embedded_vision_platform/build/third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : third_party/NMEAParser/Software/NMEAParserTest/CMakeFiles/NMEAParserTest.dir/depend

