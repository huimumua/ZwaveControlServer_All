# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local

# Include any dependencies generated for this target.
include libs2/test/CMakeFiles/test_aes_cmac.dir/depend.make

# Include the progress variables for this target.
include libs2/test/CMakeFiles/test_aes_cmac.dir/progress.make

# Include the compile flags for this target's objects.
include libs2/test/CMakeFiles/test_aes_cmac.dir/flags.make

libs2/test/test_aes_cmac_runner.c: libs2/test/test_aes_cmac.c
libs2/test/test_aes_cmac_runner.c: libs2/TestFramework/gen_test_runner.py
	$(CMAKE_COMMAND) -E cmake_progress_report /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating test_aes_cmac_runner.c"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/test && /usr/bin/python /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/TestFramework/gen_test_runner.py test_aes_cmac.c > /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/test/test_aes_cmac_runner.c

libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.o: libs2/test/CMakeFiles/test_aes_cmac.dir/flags.make
libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.o: libs2/test/test_aes_cmac.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.o"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/test && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.o   -c /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/test/test_aes_cmac.c

libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.i"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/test && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/test/test_aes_cmac.c > CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.i

libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.s"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/test && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/test/test_aes_cmac.c -o CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.s

libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.o.requires:
.PHONY : libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.o.requires

libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.o.provides: libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.o.requires
	$(MAKE) -f libs2/test/CMakeFiles/test_aes_cmac.dir/build.make libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.o.provides.build
.PHONY : libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.o.provides

libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.o.provides.build: libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.o

libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.o: libs2/test/CMakeFiles/test_aes_cmac.dir/flags.make
libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.o: libs2/test/test_aes_cmac_runner.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.o"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/test && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.o   -c /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/test/test_aes_cmac_runner.c

libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.i"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/test && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/test/test_aes_cmac_runner.c > CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.i

libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.s"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/test && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/test/test_aes_cmac_runner.c -o CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.s

libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.o.requires:
.PHONY : libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.o.requires

libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.o.provides: libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.o.requires
	$(MAKE) -f libs2/test/CMakeFiles/test_aes_cmac.dir/build.make libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.o.provides.build
.PHONY : libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.o.provides

libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.o.provides.build: libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.o

# Object files for target test_aes_cmac
test_aes_cmac_OBJECTS = \
"CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.o" \
"CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.o"

# External object files for target test_aes_cmac
test_aes_cmac_EXTERNAL_OBJECTS =

libs2/test/test_aes_cmac: libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.o
libs2/test/test_aes_cmac: libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.o
libs2/test/test_aes_cmac: libs2/test/CMakeFiles/test_aes_cmac.dir/build.make
libs2/test/test_aes_cmac: lib/libs2crypto.a
libs2/test/test_aes_cmac: lib/libaes.a
libs2/test/test_aes_cmac: lib/libunity.a
libs2/test/test_aes_cmac: libs2/test/CMakeFiles/test_aes_cmac.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable test_aes_cmac"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_aes_cmac.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libs2/test/CMakeFiles/test_aes_cmac.dir/build: libs2/test/test_aes_cmac
.PHONY : libs2/test/CMakeFiles/test_aes_cmac.dir/build

libs2/test/CMakeFiles/test_aes_cmac.dir/requires: libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac.c.o.requires
libs2/test/CMakeFiles/test_aes_cmac.dir/requires: libs2/test/CMakeFiles/test_aes_cmac.dir/test_aes_cmac_runner.c.o.requires
.PHONY : libs2/test/CMakeFiles/test_aes_cmac.dir/requires

libs2/test/CMakeFiles/test_aes_cmac.dir/clean:
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/test && $(CMAKE_COMMAND) -P CMakeFiles/test_aes_cmac.dir/cmake_clean.cmake
.PHONY : libs2/test/CMakeFiles/test_aes_cmac.dir/clean

libs2/test/CMakeFiles/test_aes_cmac.dir/depend: libs2/test/test_aes_cmac_runner.c
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/test /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/test /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/test/CMakeFiles/test_aes_cmac.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libs2/test/CMakeFiles/test_aes_cmac.dir/depend

