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

# Utility rule file for armv8_rpi3.

# Include the progress variables for this target.
include CMakeFiles/armv8_rpi3.dir/progress.make

CMakeFiles/armv8_rpi3:
	../utils/prompt.sh rm\ -fR\ /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/*:\ remove\ all\ arguments\ recursively\ [Y/n]?\ 
	rm -fR /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/* || true
	cmake -DARCH="armv8/rpi3" -DVIRTUALIZED_QEMU=1 /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2

armv8_rpi3: CMakeFiles/armv8_rpi3
armv8_rpi3: CMakeFiles/armv8_rpi3.dir/build.make

.PHONY : armv8_rpi3

# Rule to build all files generated by this target.
CMakeFiles/armv8_rpi3.dir/build: armv8_rpi3

.PHONY : CMakeFiles/armv8_rpi3.dir/build

CMakeFiles/armv8_rpi3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/armv8_rpi3.dir/cmake_clean.cmake
.PHONY : CMakeFiles/armv8_rpi3.dir/clean

CMakeFiles/armv8_rpi3.dir/depend:
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2 /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2 /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles/armv8_rpi3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/armv8_rpi3.dir/depend

