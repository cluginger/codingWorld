# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build

# Utility rule file for x86_32.

# Include the progress variables for this target.
include CMakeFiles/x86_32.dir/progress.make

CMakeFiles/x86_32:
	../utils/prompt.sh rm\ -fR\ /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/*:\ remove\ all\ arguments\ recursively\ [Y/n]?\ 
	rm -fR /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/* || true
	cmake -DARCH="x86/32" /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2

x86_32: CMakeFiles/x86_32
x86_32: CMakeFiles/x86_32.dir/build.make

.PHONY : x86_32

# Rule to build all files generated by this target.
CMakeFiles/x86_32.dir/build: x86_32

.PHONY : CMakeFiles/x86_32.dir/build

CMakeFiles/x86_32.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/x86_32.dir/cmake_clean.cmake
.PHONY : CMakeFiles/x86_32.dir/clean

CMakeFiles/x86_32.dir/depend:
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2 /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2 /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles/x86_32.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/x86_32.dir/depend

