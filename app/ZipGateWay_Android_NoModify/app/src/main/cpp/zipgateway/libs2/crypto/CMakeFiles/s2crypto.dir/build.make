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
include libs2/crypto/CMakeFiles/s2crypto.dir/depend.make

# Include the progress variables for this target.
include libs2/crypto/CMakeFiles/s2crypto.dir/progress.make

# Include the compile flags for this target's objects.
include libs2/crypto/CMakeFiles/s2crypto.dir/flags.make

libs2/crypto/CMakeFiles/s2crypto.dir/ccm/ccm.c.o: libs2/crypto/CMakeFiles/s2crypto.dir/flags.make
libs2/crypto/CMakeFiles/s2crypto.dir/ccm/ccm.c.o: libs2/crypto/ccm/ccm.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libs2/crypto/CMakeFiles/s2crypto.dir/ccm/ccm.c.o"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/s2crypto.dir/ccm/ccm.c.o   -c /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/ccm/ccm.c

libs2/crypto/CMakeFiles/s2crypto.dir/ccm/ccm.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/s2crypto.dir/ccm/ccm.c.i"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/ccm/ccm.c > CMakeFiles/s2crypto.dir/ccm/ccm.c.i

libs2/crypto/CMakeFiles/s2crypto.dir/ccm/ccm.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/s2crypto.dir/ccm/ccm.c.s"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/ccm/ccm.c -o CMakeFiles/s2crypto.dir/ccm/ccm.c.s

libs2/crypto/CMakeFiles/s2crypto.dir/ccm/ccm.c.o.requires:
.PHONY : libs2/crypto/CMakeFiles/s2crypto.dir/ccm/ccm.c.o.requires

libs2/crypto/CMakeFiles/s2crypto.dir/ccm/ccm.c.o.provides: libs2/crypto/CMakeFiles/s2crypto.dir/ccm/ccm.c.o.requires
	$(MAKE) -f libs2/crypto/CMakeFiles/s2crypto.dir/build.make libs2/crypto/CMakeFiles/s2crypto.dir/ccm/ccm.c.o.provides.build
.PHONY : libs2/crypto/CMakeFiles/s2crypto.dir/ccm/ccm.c.o.provides

libs2/crypto/CMakeFiles/s2crypto.dir/ccm/ccm.c.o.provides.build: libs2/crypto/CMakeFiles/s2crypto.dir/ccm/ccm.c.o

libs2/crypto/CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.o: libs2/crypto/CMakeFiles/s2crypto.dir/flags.make
libs2/crypto/CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.o: libs2/crypto/aes-cmac/aes_cmac.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libs2/crypto/CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.o"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.o   -c /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/aes-cmac/aes_cmac.c

libs2/crypto/CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.i"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/aes-cmac/aes_cmac.c > CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.i

libs2/crypto/CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.s"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/aes-cmac/aes_cmac.c -o CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.s

libs2/crypto/CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.o.requires:
.PHONY : libs2/crypto/CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.o.requires

libs2/crypto/CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.o.provides: libs2/crypto/CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.o.requires
	$(MAKE) -f libs2/crypto/CMakeFiles/s2crypto.dir/build.make libs2/crypto/CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.o.provides.build
.PHONY : libs2/crypto/CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.o.provides

libs2/crypto/CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.o.provides.build: libs2/crypto/CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.o

libs2/crypto/CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.o: libs2/crypto/CMakeFiles/s2crypto.dir/flags.make
libs2/crypto/CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.o: libs2/crypto/ctr_drbg/ctr_drbg.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libs2/crypto/CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.o"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.o   -c /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/ctr_drbg/ctr_drbg.c

libs2/crypto/CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.i"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/ctr_drbg/ctr_drbg.c > CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.i

libs2/crypto/CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.s"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/ctr_drbg/ctr_drbg.c -o CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.s

libs2/crypto/CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.o.requires:
.PHONY : libs2/crypto/CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.o.requires

libs2/crypto/CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.o.provides: libs2/crypto/CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.o.requires
	$(MAKE) -f libs2/crypto/CMakeFiles/s2crypto.dir/build.make libs2/crypto/CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.o.provides.build
.PHONY : libs2/crypto/CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.o.provides

libs2/crypto/CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.o.provides.build: libs2/crypto/CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.o

libs2/crypto/CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.o: libs2/crypto/CMakeFiles/s2crypto.dir/flags.make
libs2/crypto/CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.o: libs2/crypto/nextnonce/nextnonce.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libs2/crypto/CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.o"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.o   -c /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/nextnonce/nextnonce.c

libs2/crypto/CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.i"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/nextnonce/nextnonce.c > CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.i

libs2/crypto/CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.s"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/nextnonce/nextnonce.c -o CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.s

libs2/crypto/CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.o.requires:
.PHONY : libs2/crypto/CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.o.requires

libs2/crypto/CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.o.provides: libs2/crypto/CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.o.requires
	$(MAKE) -f libs2/crypto/CMakeFiles/s2crypto.dir/build.make libs2/crypto/CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.o.provides.build
.PHONY : libs2/crypto/CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.o.provides

libs2/crypto/CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.o.provides.build: libs2/crypto/CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.o

libs2/crypto/CMakeFiles/s2crypto.dir/kderiv/kderiv.c.o: libs2/crypto/CMakeFiles/s2crypto.dir/flags.make
libs2/crypto/CMakeFiles/s2crypto.dir/kderiv/kderiv.c.o: libs2/crypto/kderiv/kderiv.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libs2/crypto/CMakeFiles/s2crypto.dir/kderiv/kderiv.c.o"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/s2crypto.dir/kderiv/kderiv.c.o   -c /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/kderiv/kderiv.c

libs2/crypto/CMakeFiles/s2crypto.dir/kderiv/kderiv.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/s2crypto.dir/kderiv/kderiv.c.i"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/kderiv/kderiv.c > CMakeFiles/s2crypto.dir/kderiv/kderiv.c.i

libs2/crypto/CMakeFiles/s2crypto.dir/kderiv/kderiv.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/s2crypto.dir/kderiv/kderiv.c.s"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/kderiv/kderiv.c -o CMakeFiles/s2crypto.dir/kderiv/kderiv.c.s

libs2/crypto/CMakeFiles/s2crypto.dir/kderiv/kderiv.c.o.requires:
.PHONY : libs2/crypto/CMakeFiles/s2crypto.dir/kderiv/kderiv.c.o.requires

libs2/crypto/CMakeFiles/s2crypto.dir/kderiv/kderiv.c.o.provides: libs2/crypto/CMakeFiles/s2crypto.dir/kderiv/kderiv.c.o.requires
	$(MAKE) -f libs2/crypto/CMakeFiles/s2crypto.dir/build.make libs2/crypto/CMakeFiles/s2crypto.dir/kderiv/kderiv.c.o.provides.build
.PHONY : libs2/crypto/CMakeFiles/s2crypto.dir/kderiv/kderiv.c.o.provides

libs2/crypto/CMakeFiles/s2crypto.dir/kderiv/kderiv.c.o.provides.build: libs2/crypto/CMakeFiles/s2crypto.dir/kderiv/kderiv.c.o

libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.o: libs2/crypto/CMakeFiles/s2crypto.dir/flags.make
libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.o: libs2/crypto/curve25519/generic/curve25519.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.o"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.o   -c /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/curve25519/generic/curve25519.c

libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.i"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/curve25519/generic/curve25519.c > CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.i

libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.s"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/curve25519/generic/curve25519.c -o CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.s

libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.o.requires:
.PHONY : libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.o.requires

libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.o.provides: libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.o.requires
	$(MAKE) -f libs2/crypto/CMakeFiles/s2crypto.dir/build.make libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.o.provides.build
.PHONY : libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.o.provides

libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.o.provides.build: libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.o

libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.o: libs2/crypto/CMakeFiles/s2crypto.dir/flags.make
libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.o: libs2/crypto/curve25519/generic/fe25519.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.o"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.o   -c /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/curve25519/generic/fe25519.c

libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.i"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/curve25519/generic/fe25519.c > CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.i

libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.s"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/curve25519/generic/fe25519.c -o CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.s

libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.o.requires:
.PHONY : libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.o.requires

libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.o.provides: libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.o.requires
	$(MAKE) -f libs2/crypto/CMakeFiles/s2crypto.dir/build.make libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.o.provides.build
.PHONY : libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.o.provides

libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.o.provides.build: libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.o

libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.o: libs2/crypto/CMakeFiles/s2crypto.dir/flags.make
libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.o: libs2/crypto/curve25519/generic/bigint.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.o"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.o   -c /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/curve25519/generic/bigint.c

libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.i"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/curve25519/generic/bigint.c > CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.i

libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.s"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && /home/leon_deng/android-ndk-r12/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/curve25519/generic/bigint.c -o CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.s

libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.o.requires:
.PHONY : libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.o.requires

libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.o.provides: libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.o.requires
	$(MAKE) -f libs2/crypto/CMakeFiles/s2crypto.dir/build.make libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.o.provides.build
.PHONY : libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.o.provides

libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.o.provides.build: libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.o

# Object files for target s2crypto
s2crypto_OBJECTS = \
"CMakeFiles/s2crypto.dir/ccm/ccm.c.o" \
"CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.o" \
"CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.o" \
"CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.o" \
"CMakeFiles/s2crypto.dir/kderiv/kderiv.c.o" \
"CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.o" \
"CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.o" \
"CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.o"

# External object files for target s2crypto
s2crypto_EXTERNAL_OBJECTS =

lib/libs2crypto.a: libs2/crypto/CMakeFiles/s2crypto.dir/ccm/ccm.c.o
lib/libs2crypto.a: libs2/crypto/CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.o
lib/libs2crypto.a: libs2/crypto/CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.o
lib/libs2crypto.a: libs2/crypto/CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.o
lib/libs2crypto.a: libs2/crypto/CMakeFiles/s2crypto.dir/kderiv/kderiv.c.o
lib/libs2crypto.a: libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.o
lib/libs2crypto.a: libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.o
lib/libs2crypto.a: libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.o
lib/libs2crypto.a: libs2/crypto/CMakeFiles/s2crypto.dir/build.make
lib/libs2crypto.a: libs2/crypto/CMakeFiles/s2crypto.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C static library ../../lib/libs2crypto.a"
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && $(CMAKE_COMMAND) -P CMakeFiles/s2crypto.dir/cmake_clean_target.cmake
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/s2crypto.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libs2/crypto/CMakeFiles/s2crypto.dir/build: lib/libs2crypto.a
.PHONY : libs2/crypto/CMakeFiles/s2crypto.dir/build

libs2/crypto/CMakeFiles/s2crypto.dir/requires: libs2/crypto/CMakeFiles/s2crypto.dir/ccm/ccm.c.o.requires
libs2/crypto/CMakeFiles/s2crypto.dir/requires: libs2/crypto/CMakeFiles/s2crypto.dir/aes-cmac/aes_cmac.c.o.requires
libs2/crypto/CMakeFiles/s2crypto.dir/requires: libs2/crypto/CMakeFiles/s2crypto.dir/ctr_drbg/ctr_drbg.c.o.requires
libs2/crypto/CMakeFiles/s2crypto.dir/requires: libs2/crypto/CMakeFiles/s2crypto.dir/nextnonce/nextnonce.c.o.requires
libs2/crypto/CMakeFiles/s2crypto.dir/requires: libs2/crypto/CMakeFiles/s2crypto.dir/kderiv/kderiv.c.o.requires
libs2/crypto/CMakeFiles/s2crypto.dir/requires: libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/curve25519.c.o.requires
libs2/crypto/CMakeFiles/s2crypto.dir/requires: libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/fe25519.c.o.requires
libs2/crypto/CMakeFiles/s2crypto.dir/requires: libs2/crypto/CMakeFiles/s2crypto.dir/curve25519/generic/bigint.c.o.requires
.PHONY : libs2/crypto/CMakeFiles/s2crypto.dir/requires

libs2/crypto/CMakeFiles/s2crypto.dir/clean:
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto && $(CMAKE_COMMAND) -P CMakeFiles/s2crypto.dir/cmake_clean.cmake
.PHONY : libs2/crypto/CMakeFiles/s2crypto.dir/clean

libs2/crypto/CMakeFiles/s2crypto.dir/depend:
	cd /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto /home/leon_deng/zipgateway/zipgatew_61_0/zipgateway-2.61.0-Source/usr/local/libs2/crypto/CMakeFiles/s2crypto.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libs2/crypto/CMakeFiles/s2crypto.dir/depend

