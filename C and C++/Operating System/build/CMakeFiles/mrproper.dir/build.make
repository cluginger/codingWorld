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

# Utility rule file for mrproper.

# Include the progress variables for this target.
include CMakeFiles/mrproper.dir/progress.make

CMakeFiles/mrproper:
	make clean
	../utils/prompt.sh rm\ -fR\ /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/*:\ remove\ all\ arguments\ recursively\ [Y/n]?\ 
	rm -fR /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/* || true
	cmake -DARCH=x86/64 /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2

mrproper: CMakeFiles/mrproper
mrproper: CMakeFiles/mrproper.dir/build.make

.PHONY : mrproper

# Rule to build all files generated by this target.
CMakeFiles/mrproper.dir/build: mrproper

.PHONY : CMakeFiles/mrproper.dir/build

CMakeFiles/mrproper.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mrproper.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mrproper.dir/clean

CMakeFiles/mrproper.dir/depend:
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2 /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2 /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles/mrproper.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mrproper.dir/depend

