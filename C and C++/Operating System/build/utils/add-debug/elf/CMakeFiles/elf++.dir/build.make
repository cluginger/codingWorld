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

# Include any dependencies generated for this target.
include utils/add-debug/elf/CMakeFiles/elf++.dir/depend.make

# Include the progress variables for this target.
include utils/add-debug/elf/CMakeFiles/elf++.dir/progress.make

# Include the compile flags for this target's objects.
include utils/add-debug/elf/CMakeFiles/elf++.dir/flags.make

utils/add-debug/elf/to_string.cc: ../utils/add-debug/elf/enum-print.py
utils/add-debug/elf/to_string.cc: ../utils/add-debug/elf/data.hh
utils/add-debug/elf/to_string.cc: ../utils/add-debug/elf/to_hex.hh
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating to_string.cc"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/utils/add-debug/elf && ../../../../utils/add-debug/elf/gen_to_string.sh /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/utils/add-debug/elf /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/utils/add-debug/elf/to_string.cc

utils/add-debug/elf/CMakeFiles/elf++.dir/elf.cc.o: utils/add-debug/elf/CMakeFiles/elf++.dir/flags.make
utils/add-debug/elf/CMakeFiles/elf++.dir/elf.cc.o: ../utils/add-debug/elf/elf.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object utils/add-debug/elf/CMakeFiles/elf++.dir/elf.cc.o"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/utils/add-debug/elf && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/elf++.dir/elf.cc.o -c /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/utils/add-debug/elf/elf.cc

utils/add-debug/elf/CMakeFiles/elf++.dir/elf.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/elf++.dir/elf.cc.i"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/utils/add-debug/elf && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/utils/add-debug/elf/elf.cc > CMakeFiles/elf++.dir/elf.cc.i

utils/add-debug/elf/CMakeFiles/elf++.dir/elf.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/elf++.dir/elf.cc.s"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/utils/add-debug/elf && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/utils/add-debug/elf/elf.cc -o CMakeFiles/elf++.dir/elf.cc.s

utils/add-debug/elf/CMakeFiles/elf++.dir/mmap_loader.cc.o: utils/add-debug/elf/CMakeFiles/elf++.dir/flags.make
utils/add-debug/elf/CMakeFiles/elf++.dir/mmap_loader.cc.o: ../utils/add-debug/elf/mmap_loader.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object utils/add-debug/elf/CMakeFiles/elf++.dir/mmap_loader.cc.o"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/utils/add-debug/elf && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/elf++.dir/mmap_loader.cc.o -c /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/utils/add-debug/elf/mmap_loader.cc

utils/add-debug/elf/CMakeFiles/elf++.dir/mmap_loader.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/elf++.dir/mmap_loader.cc.i"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/utils/add-debug/elf && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/utils/add-debug/elf/mmap_loader.cc > CMakeFiles/elf++.dir/mmap_loader.cc.i

utils/add-debug/elf/CMakeFiles/elf++.dir/mmap_loader.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/elf++.dir/mmap_loader.cc.s"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/utils/add-debug/elf && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/utils/add-debug/elf/mmap_loader.cc -o CMakeFiles/elf++.dir/mmap_loader.cc.s

utils/add-debug/elf/CMakeFiles/elf++.dir/to_string.cc.o: utils/add-debug/elf/CMakeFiles/elf++.dir/flags.make
utils/add-debug/elf/CMakeFiles/elf++.dir/to_string.cc.o: utils/add-debug/elf/to_string.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object utils/add-debug/elf/CMakeFiles/elf++.dir/to_string.cc.o"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/utils/add-debug/elf && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/elf++.dir/to_string.cc.o -c /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/utils/add-debug/elf/to_string.cc

utils/add-debug/elf/CMakeFiles/elf++.dir/to_string.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/elf++.dir/to_string.cc.i"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/utils/add-debug/elf && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/utils/add-debug/elf/to_string.cc > CMakeFiles/elf++.dir/to_string.cc.i

utils/add-debug/elf/CMakeFiles/elf++.dir/to_string.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/elf++.dir/to_string.cc.s"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/utils/add-debug/elf && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/utils/add-debug/elf/to_string.cc -o CMakeFiles/elf++.dir/to_string.cc.s

# Object files for target elf++
elf_______OBJECTS = \
"CMakeFiles/elf++.dir/elf.cc.o" \
"CMakeFiles/elf++.dir/mmap_loader.cc.o" \
"CMakeFiles/elf++.dir/to_string.cc.o"

# External object files for target elf++
elf_______EXTERNAL_OBJECTS =

lib/libelf++.a: utils/add-debug/elf/CMakeFiles/elf++.dir/elf.cc.o
lib/libelf++.a: utils/add-debug/elf/CMakeFiles/elf++.dir/mmap_loader.cc.o
lib/libelf++.a: utils/add-debug/elf/CMakeFiles/elf++.dir/to_string.cc.o
lib/libelf++.a: utils/add-debug/elf/CMakeFiles/elf++.dir/build.make
lib/libelf++.a: utils/add-debug/elf/CMakeFiles/elf++.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX static library ../../../lib/libelf++.a"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/utils/add-debug/elf && $(CMAKE_COMMAND) -P CMakeFiles/elf++.dir/cmake_clean_target.cmake
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/utils/add-debug/elf && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/elf++.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
utils/add-debug/elf/CMakeFiles/elf++.dir/build: lib/libelf++.a

.PHONY : utils/add-debug/elf/CMakeFiles/elf++.dir/build

utils/add-debug/elf/CMakeFiles/elf++.dir/clean:
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/utils/add-debug/elf && $(CMAKE_COMMAND) -P CMakeFiles/elf++.dir/cmake_clean.cmake
.PHONY : utils/add-debug/elf/CMakeFiles/elf++.dir/clean

utils/add-debug/elf/CMakeFiles/elf++.dir/depend: utils/add-debug/elf/to_string.cc
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2 /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/utils/add-debug/elf /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/utils/add-debug/elf /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/utils/add-debug/elf/CMakeFiles/elf++.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : utils/add-debug/elf/CMakeFiles/elf++.dir/depend

