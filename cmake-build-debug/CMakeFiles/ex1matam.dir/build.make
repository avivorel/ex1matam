# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/ori/Documents/סמסטר ג מדעי המחשב/מתמ/עבודות /Ex1_matam/ex1matam"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/ori/Documents/סמסטר ג מדעי המחשב/מתמ/עבודות /Ex1_matam/ex1matam/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/ex1matam.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/ex1matam.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ex1matam.dir/flags.make

CMakeFiles/ex1matam.dir/matamikye2.c.o: CMakeFiles/ex1matam.dir/flags.make
CMakeFiles/ex1matam.dir/matamikye2.c.o: ../matamikye2.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/ori/Documents/סמסטר ג מדעי המחשב/מתמ/עבודות /Ex1_matam/ex1matam/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/ex1matam.dir/matamikye2.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ex1matam.dir/matamikye2.c.o -c "/Users/ori/Documents/סמסטר ג מדעי המחשב/מתמ/עבודות /Ex1_matam/ex1matam/matamikye2.c"

CMakeFiles/ex1matam.dir/matamikye2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ex1matam.dir/matamikye2.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/ori/Documents/סמסטר ג מדעי המחשב/מתמ/עבודות /Ex1_matam/ex1matam/matamikye2.c" > CMakeFiles/ex1matam.dir/matamikye2.c.i

CMakeFiles/ex1matam.dir/matamikye2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ex1matam.dir/matamikye2.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/ori/Documents/סמסטר ג מדעי המחשב/מתמ/עבודות /Ex1_matam/ex1matam/matamikye2.c" -o CMakeFiles/ex1matam.dir/matamikye2.c.s

# Object files for target ex1matam
ex1matam_OBJECTS = \
"CMakeFiles/ex1matam.dir/matamikye2.c.o"

# External object files for target ex1matam
ex1matam_EXTERNAL_OBJECTS =

ex1matam: CMakeFiles/ex1matam.dir/matamikye2.c.o
ex1matam: CMakeFiles/ex1matam.dir/build.make
ex1matam: CMakeFiles/ex1matam.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/ori/Documents/סמסטר ג מדעי המחשב/מתמ/עבודות /Ex1_matam/ex1matam/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ex1matam"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ex1matam.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ex1matam.dir/build: ex1matam
.PHONY : CMakeFiles/ex1matam.dir/build

CMakeFiles/ex1matam.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ex1matam.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ex1matam.dir/clean

CMakeFiles/ex1matam.dir/depend:
	cd "/Users/ori/Documents/סמסטר ג מדעי המחשב/מתמ/עבודות /Ex1_matam/ex1matam/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/ori/Documents/סמסטר ג מדעי המחשב/מתמ/עבודות /Ex1_matam/ex1matam" "/Users/ori/Documents/סמסטר ג מדעי המחשב/מתמ/עבודות /Ex1_matam/ex1matam" "/Users/ori/Documents/סמסטר ג מדעי המחשב/מתמ/עבודות /Ex1_matam/ex1matam/cmake-build-debug" "/Users/ori/Documents/סמסטר ג מדעי המחשב/מתמ/עבודות /Ex1_matam/ex1matam/cmake-build-debug" "/Users/ori/Documents/סמסטר ג מדעי המחשב/מתמ/עבודות /Ex1_matam/ex1matam/cmake-build-debug/CMakeFiles/ex1matam.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/ex1matam.dir/depend

