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
include arch/x86/64/source/CMakeFiles/arch_x86_64.dir/depend.make

# Include the progress variables for this target.
include arch/x86/64/source/CMakeFiles/arch_x86_64.dir/progress.make

# Include the compile flags for this target's objects.
include arch/x86/64/source/CMakeFiles/arch_x86_64.dir/flags.make

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchCommon.cpp.o: arch/x86/64/source/CMakeFiles/arch_x86_64.dir/flags.make
arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchCommon.cpp.o: ../arch/x86/64/source/ArchCommon.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchCommon.cpp.o"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/arch_x86_64.dir/ArchCommon.cpp.o -c /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/ArchCommon.cpp

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchCommon.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/arch_x86_64.dir/ArchCommon.cpp.i"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/ArchCommon.cpp > CMakeFiles/arch_x86_64.dir/ArchCommon.cpp.i

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchCommon.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/arch_x86_64.dir/ArchCommon.cpp.s"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/ArchCommon.cpp -o CMakeFiles/arch_x86_64.dir/ArchCommon.cpp.s

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchInterrupts.cpp.o: arch/x86/64/source/CMakeFiles/arch_x86_64.dir/flags.make
arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchInterrupts.cpp.o: ../arch/x86/64/source/ArchInterrupts.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchInterrupts.cpp.o"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/arch_x86_64.dir/ArchInterrupts.cpp.o -c /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/ArchInterrupts.cpp

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchInterrupts.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/arch_x86_64.dir/ArchInterrupts.cpp.i"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/ArchInterrupts.cpp > CMakeFiles/arch_x86_64.dir/ArchInterrupts.cpp.i

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchInterrupts.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/arch_x86_64.dir/ArchInterrupts.cpp.s"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/ArchInterrupts.cpp -o CMakeFiles/arch_x86_64.dir/ArchInterrupts.cpp.s

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchMemory.cpp.o: arch/x86/64/source/CMakeFiles/arch_x86_64.dir/flags.make
arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchMemory.cpp.o: ../arch/x86/64/source/ArchMemory.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchMemory.cpp.o"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/arch_x86_64.dir/ArchMemory.cpp.o -c /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/ArchMemory.cpp

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchMemory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/arch_x86_64.dir/ArchMemory.cpp.i"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/ArchMemory.cpp > CMakeFiles/arch_x86_64.dir/ArchMemory.cpp.i

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchMemory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/arch_x86_64.dir/ArchMemory.cpp.s"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/ArchMemory.cpp -o CMakeFiles/arch_x86_64.dir/ArchMemory.cpp.s

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchThreads.cpp.o: arch/x86/64/source/CMakeFiles/arch_x86_64.dir/flags.make
arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchThreads.cpp.o: ../arch/x86/64/source/ArchThreads.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchThreads.cpp.o"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/arch_x86_64.dir/ArchThreads.cpp.o -c /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/ArchThreads.cpp

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchThreads.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/arch_x86_64.dir/ArchThreads.cpp.i"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/ArchThreads.cpp > CMakeFiles/arch_x86_64.dir/ArchThreads.cpp.i

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchThreads.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/arch_x86_64.dir/ArchThreads.cpp.s"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/ArchThreads.cpp -o CMakeFiles/arch_x86_64.dir/ArchThreads.cpp.s

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/InterruptUtils.cpp.o: arch/x86/64/source/CMakeFiles/arch_x86_64.dir/flags.make
arch/x86/64/source/CMakeFiles/arch_x86_64.dir/InterruptUtils.cpp.o: ../arch/x86/64/source/InterruptUtils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object arch/x86/64/source/CMakeFiles/arch_x86_64.dir/InterruptUtils.cpp.o"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/arch_x86_64.dir/InterruptUtils.cpp.o -c /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/InterruptUtils.cpp

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/InterruptUtils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/arch_x86_64.dir/InterruptUtils.cpp.i"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/InterruptUtils.cpp > CMakeFiles/arch_x86_64.dir/InterruptUtils.cpp.i

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/InterruptUtils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/arch_x86_64.dir/InterruptUtils.cpp.s"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/InterruptUtils.cpp -o CMakeFiles/arch_x86_64.dir/InterruptUtils.cpp.s

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/arch_backtrace.cpp.o: arch/x86/64/source/CMakeFiles/arch_x86_64.dir/flags.make
arch/x86/64/source/CMakeFiles/arch_x86_64.dir/arch_backtrace.cpp.o: ../arch/x86/64/source/arch_backtrace.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object arch/x86/64/source/CMakeFiles/arch_x86_64.dir/arch_backtrace.cpp.o"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/arch_x86_64.dir/arch_backtrace.cpp.o -c /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/arch_backtrace.cpp

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/arch_backtrace.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/arch_x86_64.dir/arch_backtrace.cpp.i"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/arch_backtrace.cpp > CMakeFiles/arch_x86_64.dir/arch_backtrace.cpp.i

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/arch_backtrace.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/arch_x86_64.dir/arch_backtrace.cpp.s"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/arch_backtrace.cpp -o CMakeFiles/arch_x86_64.dir/arch_backtrace.cpp.s

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/arch_interrupts.S.o: arch/x86/64/source/CMakeFiles/arch_x86_64.dir/flags.make
arch/x86/64/source/CMakeFiles/arch_x86_64.dir/arch_interrupts.S.o: ../arch/x86/64/source/arch_interrupts.S
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building ASM object arch/x86/64/source/CMakeFiles/arch_x86_64.dir/arch_interrupts.S.o"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/cc $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_FLAGS) -o CMakeFiles/arch_x86_64.dir/arch_interrupts.S.o -c /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/arch_interrupts.S

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/init_boottime_pagetables.cpp.o: arch/x86/64/source/CMakeFiles/arch_x86_64.dir/flags.make
arch/x86/64/source/CMakeFiles/arch_x86_64.dir/init_boottime_pagetables.cpp.o: ../arch/x86/64/source/init_boottime_pagetables.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object arch/x86/64/source/CMakeFiles/arch_x86_64.dir/init_boottime_pagetables.cpp.o"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/arch_x86_64.dir/init_boottime_pagetables.cpp.o -c /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/init_boottime_pagetables.cpp

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/init_boottime_pagetables.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/arch_x86_64.dir/init_boottime_pagetables.cpp.i"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/init_boottime_pagetables.cpp > CMakeFiles/arch_x86_64.dir/init_boottime_pagetables.cpp.i

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/init_boottime_pagetables.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/arch_x86_64.dir/init_boottime_pagetables.cpp.s"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source/init_boottime_pagetables.cpp -o CMakeFiles/arch_x86_64.dir/init_boottime_pagetables.cpp.s

# Object files for target arch_x86_64
arch_x86_64_OBJECTS = \
"CMakeFiles/arch_x86_64.dir/ArchCommon.cpp.o" \
"CMakeFiles/arch_x86_64.dir/ArchInterrupts.cpp.o" \
"CMakeFiles/arch_x86_64.dir/ArchMemory.cpp.o" \
"CMakeFiles/arch_x86_64.dir/ArchThreads.cpp.o" \
"CMakeFiles/arch_x86_64.dir/InterruptUtils.cpp.o" \
"CMakeFiles/arch_x86_64.dir/arch_backtrace.cpp.o" \
"CMakeFiles/arch_x86_64.dir/arch_interrupts.S.o" \
"CMakeFiles/arch_x86_64.dir/init_boottime_pagetables.cpp.o"

# External object files for target arch_x86_64
arch_x86_64_EXTERNAL_OBJECTS = \
"/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source/CMakeFiles/arch_x86_64_archobjs.dir/boot.32.C.o"

lib/libarch_x86_64.a: arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchCommon.cpp.o
lib/libarch_x86_64.a: arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchInterrupts.cpp.o
lib/libarch_x86_64.a: arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchMemory.cpp.o
lib/libarch_x86_64.a: arch/x86/64/source/CMakeFiles/arch_x86_64.dir/ArchThreads.cpp.o
lib/libarch_x86_64.a: arch/x86/64/source/CMakeFiles/arch_x86_64.dir/InterruptUtils.cpp.o
lib/libarch_x86_64.a: arch/x86/64/source/CMakeFiles/arch_x86_64.dir/arch_backtrace.cpp.o
lib/libarch_x86_64.a: arch/x86/64/source/CMakeFiles/arch_x86_64.dir/arch_interrupts.S.o
lib/libarch_x86_64.a: arch/x86/64/source/CMakeFiles/arch_x86_64.dir/init_boottime_pagetables.cpp.o
lib/libarch_x86_64.a: arch/x86/64/source/CMakeFiles/arch_x86_64_archobjs.dir/boot.32.C.o
lib/libarch_x86_64.a: arch/x86/64/source/CMakeFiles/arch_x86_64.dir/build.make
lib/libarch_x86_64.a: arch/x86/64/source/CMakeFiles/arch_x86_64.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX static library ../../../../lib/libarch_x86_64.a"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && $(CMAKE_COMMAND) -P CMakeFiles/arch_x86_64.dir/cmake_clean_target.cmake
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/arch_x86_64.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
arch/x86/64/source/CMakeFiles/arch_x86_64.dir/build: lib/libarch_x86_64.a

.PHONY : arch/x86/64/source/CMakeFiles/arch_x86_64.dir/build

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/clean:
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source && $(CMAKE_COMMAND) -P CMakeFiles/arch_x86_64.dir/cmake_clean.cmake
.PHONY : arch/x86/64/source/CMakeFiles/arch_x86_64.dir/clean

arch/x86/64/source/CMakeFiles/arch_x86_64.dir/depend:
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2 /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/arch/x86/64/source /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/arch/x86/64/source/CMakeFiles/arch_x86_64.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : arch/x86/64/source/CMakeFiles/arch_x86_64.dir/depend

