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
include common/source/console/CMakeFiles/common_console.dir/depend.make

# Include the progress variables for this target.
include common/source/console/CMakeFiles/common_console.dir/progress.make

# Include the compile flags for this target's objects.
include common/source/console/CMakeFiles/common_console.dir/flags.make

common/source/console/CMakeFiles/common_console.dir/Console.cpp.o: common/source/console/CMakeFiles/common_console.dir/flags.make
common/source/console/CMakeFiles/common_console.dir/Console.cpp.o: ../common/source/console/Console.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object common/source/console/CMakeFiles/common_console.dir/Console.cpp.o"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/common_console.dir/Console.cpp.o -c /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/Console.cpp

common/source/console/CMakeFiles/common_console.dir/Console.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/common_console.dir/Console.cpp.i"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/Console.cpp > CMakeFiles/common_console.dir/Console.cpp.i

common/source/console/CMakeFiles/common_console.dir/Console.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/common_console.dir/Console.cpp.s"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/Console.cpp -o CMakeFiles/common_console.dir/Console.cpp.s

common/source/console/CMakeFiles/common_console.dir/FrameBufferConsole.cpp.o: common/source/console/CMakeFiles/common_console.dir/flags.make
common/source/console/CMakeFiles/common_console.dir/FrameBufferConsole.cpp.o: ../common/source/console/FrameBufferConsole.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object common/source/console/CMakeFiles/common_console.dir/FrameBufferConsole.cpp.o"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/common_console.dir/FrameBufferConsole.cpp.o -c /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/FrameBufferConsole.cpp

common/source/console/CMakeFiles/common_console.dir/FrameBufferConsole.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/common_console.dir/FrameBufferConsole.cpp.i"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/FrameBufferConsole.cpp > CMakeFiles/common_console.dir/FrameBufferConsole.cpp.i

common/source/console/CMakeFiles/common_console.dir/FrameBufferConsole.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/common_console.dir/FrameBufferConsole.cpp.s"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/FrameBufferConsole.cpp -o CMakeFiles/common_console.dir/FrameBufferConsole.cpp.s

common/source/console/CMakeFiles/common_console.dir/Terminal.cpp.o: common/source/console/CMakeFiles/common_console.dir/flags.make
common/source/console/CMakeFiles/common_console.dir/Terminal.cpp.o: ../common/source/console/Terminal.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object common/source/console/CMakeFiles/common_console.dir/Terminal.cpp.o"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/common_console.dir/Terminal.cpp.o -c /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/Terminal.cpp

common/source/console/CMakeFiles/common_console.dir/Terminal.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/common_console.dir/Terminal.cpp.i"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/Terminal.cpp > CMakeFiles/common_console.dir/Terminal.cpp.i

common/source/console/CMakeFiles/common_console.dir/Terminal.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/common_console.dir/Terminal.cpp.s"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/Terminal.cpp -o CMakeFiles/common_console.dir/Terminal.cpp.s

common/source/console/CMakeFiles/common_console.dir/TextConsole.cpp.o: common/source/console/CMakeFiles/common_console.dir/flags.make
common/source/console/CMakeFiles/common_console.dir/TextConsole.cpp.o: ../common/source/console/TextConsole.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object common/source/console/CMakeFiles/common_console.dir/TextConsole.cpp.o"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/common_console.dir/TextConsole.cpp.o -c /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/TextConsole.cpp

common/source/console/CMakeFiles/common_console.dir/TextConsole.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/common_console.dir/TextConsole.cpp.i"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/TextConsole.cpp > CMakeFiles/common_console.dir/TextConsole.cpp.i

common/source/console/CMakeFiles/common_console.dir/TextConsole.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/common_console.dir/TextConsole.cpp.s"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/TextConsole.cpp -o CMakeFiles/common_console.dir/TextConsole.cpp.s

common/source/console/CMakeFiles/common_console.dir/font_sun8x16.c.o: common/source/console/CMakeFiles/common_console.dir/flags.make
common/source/console/CMakeFiles/common_console.dir/font_sun8x16.c.o: ../common/source/console/font_sun8x16.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object common/source/console/CMakeFiles/common_console.dir/font_sun8x16.c.o"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/common_console.dir/font_sun8x16.c.o   -c /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/font_sun8x16.c

common/source/console/CMakeFiles/common_console.dir/font_sun8x16.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/common_console.dir/font_sun8x16.c.i"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/font_sun8x16.c > CMakeFiles/common_console.dir/font_sun8x16.c.i

common/source/console/CMakeFiles/common_console.dir/font_sun8x16.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/common_console.dir/font_sun8x16.c.s"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/font_sun8x16.c -o CMakeFiles/common_console.dir/font_sun8x16.c.s

common/source/console/CMakeFiles/common_console.dir/kprintf.cpp.o: common/source/console/CMakeFiles/common_console.dir/flags.make
common/source/console/CMakeFiles/common_console.dir/kprintf.cpp.o: ../common/source/console/kprintf.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object common/source/console/CMakeFiles/common_console.dir/kprintf.cpp.o"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/common_console.dir/kprintf.cpp.o -c /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/kprintf.cpp

common/source/console/CMakeFiles/common_console.dir/kprintf.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/common_console.dir/kprintf.cpp.i"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/kprintf.cpp > CMakeFiles/common_console.dir/kprintf.cpp.i

common/source/console/CMakeFiles/common_console.dir/kprintf.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/common_console.dir/kprintf.cpp.s"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/kprintf.cpp -o CMakeFiles/common_console.dir/kprintf.cpp.s

common/source/console/CMakeFiles/common_console.dir/panic.cpp.o: common/source/console/CMakeFiles/common_console.dir/flags.make
common/source/console/CMakeFiles/common_console.dir/panic.cpp.o: ../common/source/console/panic.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object common/source/console/CMakeFiles/common_console.dir/panic.cpp.o"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/common_console.dir/panic.cpp.o -c /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/panic.cpp

common/source/console/CMakeFiles/common_console.dir/panic.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/common_console.dir/panic.cpp.i"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/panic.cpp > CMakeFiles/common_console.dir/panic.cpp.i

common/source/console/CMakeFiles/common_console.dir/panic.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/common_console.dir/panic.cpp.s"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console/panic.cpp -o CMakeFiles/common_console.dir/panic.cpp.s

# Object files for target common_console
common_console_OBJECTS = \
"CMakeFiles/common_console.dir/Console.cpp.o" \
"CMakeFiles/common_console.dir/FrameBufferConsole.cpp.o" \
"CMakeFiles/common_console.dir/Terminal.cpp.o" \
"CMakeFiles/common_console.dir/TextConsole.cpp.o" \
"CMakeFiles/common_console.dir/font_sun8x16.c.o" \
"CMakeFiles/common_console.dir/kprintf.cpp.o" \
"CMakeFiles/common_console.dir/panic.cpp.o"

# External object files for target common_console
common_console_EXTERNAL_OBJECTS =

lib/libcommon_console.a: common/source/console/CMakeFiles/common_console.dir/Console.cpp.o
lib/libcommon_console.a: common/source/console/CMakeFiles/common_console.dir/FrameBufferConsole.cpp.o
lib/libcommon_console.a: common/source/console/CMakeFiles/common_console.dir/Terminal.cpp.o
lib/libcommon_console.a: common/source/console/CMakeFiles/common_console.dir/TextConsole.cpp.o
lib/libcommon_console.a: common/source/console/CMakeFiles/common_console.dir/font_sun8x16.c.o
lib/libcommon_console.a: common/source/console/CMakeFiles/common_console.dir/kprintf.cpp.o
lib/libcommon_console.a: common/source/console/CMakeFiles/common_console.dir/panic.cpp.o
lib/libcommon_console.a: common/source/console/CMakeFiles/common_console.dir/build.make
lib/libcommon_console.a: common/source/console/CMakeFiles/common_console.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX static library ../../../lib/libcommon_console.a"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && $(CMAKE_COMMAND) -P CMakeFiles/common_console.dir/cmake_clean_target.cmake
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/common_console.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
common/source/console/CMakeFiles/common_console.dir/build: lib/libcommon_console.a

.PHONY : common/source/console/CMakeFiles/common_console.dir/build

common/source/console/CMakeFiles/common_console.dir/clean:
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console && $(CMAKE_COMMAND) -P CMakeFiles/common_console.dir/cmake_clean.cmake
.PHONY : common/source/console/CMakeFiles/common_console.dir/clean

common/source/console/CMakeFiles/common_console.dir/depend:
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2 /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/common/source/console /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/common/source/console/CMakeFiles/common_console.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : common/source/console/CMakeFiles/common_console.dir/depend

