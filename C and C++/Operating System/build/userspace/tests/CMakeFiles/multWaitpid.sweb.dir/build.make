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
include userspace/tests/CMakeFiles/multWaitpid.sweb.dir/depend.make

# Include the progress variables for this target.
include userspace/tests/CMakeFiles/multWaitpid.sweb.dir/progress.make

# Include the compile flags for this target's objects.
include userspace/tests/CMakeFiles/multWaitpid.sweb.dir/flags.make

userspace/tests/CMakeFiles/multWaitpid.sweb.dir/multWaitpid.c.o: userspace/tests/CMakeFiles/multWaitpid.sweb.dir/flags.make
userspace/tests/CMakeFiles/multWaitpid.sweb.dir/multWaitpid.c.o: ../userspace/tests/multWaitpid.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object userspace/tests/CMakeFiles/multWaitpid.sweb.dir/multWaitpid.c.o"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/userspace/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/multWaitpid.sweb.dir/multWaitpid.c.o   -c /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/userspace/tests/multWaitpid.c

userspace/tests/CMakeFiles/multWaitpid.sweb.dir/multWaitpid.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/multWaitpid.sweb.dir/multWaitpid.c.i"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/userspace/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/userspace/tests/multWaitpid.c > CMakeFiles/multWaitpid.sweb.dir/multWaitpid.c.i

userspace/tests/CMakeFiles/multWaitpid.sweb.dir/multWaitpid.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/multWaitpid.sweb.dir/multWaitpid.c.s"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/userspace/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/userspace/tests/multWaitpid.c -o CMakeFiles/multWaitpid.sweb.dir/multWaitpid.c.s

# Object files for target multWaitpid.sweb
multWaitpid_sweb_OBJECTS = \
"CMakeFiles/multWaitpid.sweb.dir/multWaitpid.c.o"

# External object files for target multWaitpid.sweb
multWaitpid_sweb_EXTERNAL_OBJECTS =

userspace/multWaitpid.sweb: userspace/tests/CMakeFiles/multWaitpid.sweb.dir/multWaitpid.c.o
userspace/multWaitpid.sweb: userspace/tests/CMakeFiles/multWaitpid.sweb.dir/build.make
userspace/multWaitpid.sweb: lib/libuserspace_libc.a
userspace/multWaitpid.sweb: userspace/tests/CMakeFiles/multWaitpid.sweb.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ../multWaitpid.sweb"
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/userspace/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/multWaitpid.sweb.dir/link.txt --verbose=$(VERBOSE)
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/userspace/tests && ../../add-dbg /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/userspace/multWaitpid.sweb /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/userspace/multWaitpid.sweb.dbg
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/userspace/tests && /usr/bin/objcopy --remove-section .swebdbg /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/userspace/multWaitpid.sweb
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/userspace/tests && /usr/bin/objcopy --add-section .swebdbg="/home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/userspace/multWaitpid.sweb.dbg" --set-section-flags .swebdbg=noload,readonly /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/userspace/multWaitpid.sweb
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/userspace/tests && /usr/local/bin/cmake -E remove -f /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/userspace/multWaitpid.sweb.dbg

# Rule to build all files generated by this target.
userspace/tests/CMakeFiles/multWaitpid.sweb.dir/build: userspace/multWaitpid.sweb

.PHONY : userspace/tests/CMakeFiles/multWaitpid.sweb.dir/build

userspace/tests/CMakeFiles/multWaitpid.sweb.dir/clean:
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/userspace/tests && $(CMAKE_COMMAND) -P CMakeFiles/multWaitpid.sweb.dir/cmake_clean.cmake
.PHONY : userspace/tests/CMakeFiles/multWaitpid.sweb.dir/clean

userspace/tests/CMakeFiles/multWaitpid.sweb.dir/depend:
	cd /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2 /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/userspace/tests /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/userspace/tests /home/chrislug/Schreibtisch/SEW/10.Semester/bss23c2/build/userspace/tests/CMakeFiles/multWaitpid.sweb.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : userspace/tests/CMakeFiles/multWaitpid.sweb.dir/depend

