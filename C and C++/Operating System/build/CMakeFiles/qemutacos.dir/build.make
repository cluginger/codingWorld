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

# Utility rule file for qemutacos.

# Include the progress variables for this target.
include CMakeFiles/qemutacos.dir/progress.make

CMakeFiles/qemutacos:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Executing \`qemu-system-x86_64 -hda SWEB.qcow2 -m 8M -snapshot -monitor pipe:qemu -nographic -debugcon stdio\`"
	qemu-system-x86_64 -hda SWEB.qcow2 -m 8M -snapshot -monitor pipe:qemu -nographic -debugcon stdio

qemutacos: CMakeFiles/qemutacos
qemutacos: CMakeFiles/qemutacos.dir/build.make

.PHONY : qemutacos

# Rule to build all files generated by this target.
CMakeFiles/qemutacos.dir/build: qemutacos

.PHONY : CMakeFiles/qemutacos.dir/build

CMakeFiles/qemutacos.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/qemutacos.dir/cmake_clean.cmake
.PHONY : CMakeFiles/qemutacos.dir/clean

CMakeFiles/qemutacos.dir/depend:
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2 /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2 /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles/qemutacos.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/qemutacos.dir/depend
