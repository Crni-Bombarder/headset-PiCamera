# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = /home/fumeron/Documents/Internship_Panama/headset-PiCamera

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/fumeron/Documents/Internship_Panama/headset-PiCamera

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/fumeron/Documents/Internship_Panama/headset-PiCamera/CMakeFiles /home/fumeron/Documents/Internship_Panama/headset-PiCamera/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/fumeron/Documents/Internship_Panama/headset-PiCamera/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named RASPBE

# Build rule for target.
RASPBE: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 RASPBE
.PHONY : RASPBE

# fast build rule for target.
RASPBE/fast:
	$(MAKE) -f CMakeFiles/RASPBE.dir/build.make CMakeFiles/RASPBE.dir/build
.PHONY : RASPBE/fast

#=============================================================================
# Target rules for targets named RaspberryMain

# Build rule for target.
RaspberryMain: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 RaspberryMain
.PHONY : RaspberryMain

# fast build rule for target.
RaspberryMain/fast:
	$(MAKE) -f CMakeFiles/RaspberryMain.dir/build.make CMakeFiles/RaspberryMain.dir/build
.PHONY : RaspberryMain/fast

#=============================================================================
# Target rules for targets named COMMON

# Build rule for target.
COMMON: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 COMMON
.PHONY : COMMON

# fast build rule for target.
COMMON/fast:
	$(MAKE) -f CMakeFiles/COMMON.dir/build.make CMakeFiles/COMMON.dir/build
.PHONY : COMMON/fast

#=============================================================================
# Target rules for targets named ODROID

# Build rule for target.
ODROID: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 ODROID
.PHONY : ODROID

# fast build rule for target.
ODROID/fast:
	$(MAKE) -f CMakeFiles/ODROID.dir/build.make CMakeFiles/ODROID.dir/build
.PHONY : ODROID/fast

#=============================================================================
# Target rules for targets named OdroidMain

# Build rule for target.
OdroidMain: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 OdroidMain
.PHONY : OdroidMain

# fast build rule for target.
OdroidMain/fast:
	$(MAKE) -f CMakeFiles/OdroidMain.dir/build.make CMakeFiles/OdroidMain.dir/build
.PHONY : OdroidMain/fast

#=============================================================================
# Target rules for targets named TestCapture

# Build rule for target.
TestCapture: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 TestCapture
.PHONY : TestCapture

# fast build rule for target.
TestCapture/fast:
	$(MAKE) -f CMakeFiles/TestCapture.dir/build.make CMakeFiles/TestCapture.dir/build
.PHONY : TestCapture/fast

OdroidMain.o: OdroidMain.c.o

.PHONY : OdroidMain.o

# target to build an object file
OdroidMain.c.o:
	$(MAKE) -f CMakeFiles/OdroidMain.dir/build.make CMakeFiles/OdroidMain.dir/OdroidMain.c.o
.PHONY : OdroidMain.c.o

OdroidMain.i: OdroidMain.c.i

.PHONY : OdroidMain.i

# target to preprocess a source file
OdroidMain.c.i:
	$(MAKE) -f CMakeFiles/OdroidMain.dir/build.make CMakeFiles/OdroidMain.dir/OdroidMain.c.i
.PHONY : OdroidMain.c.i

OdroidMain.s: OdroidMain.c.s

.PHONY : OdroidMain.s

# target to generate assembly for a file
OdroidMain.c.s:
	$(MAKE) -f CMakeFiles/OdroidMain.dir/build.make CMakeFiles/OdroidMain.dir/OdroidMain.c.s
.PHONY : OdroidMain.c.s

RaspberryMain.o: RaspberryMain.c.o

.PHONY : RaspberryMain.o

# target to build an object file
RaspberryMain.c.o:
	$(MAKE) -f CMakeFiles/RaspberryMain.dir/build.make CMakeFiles/RaspberryMain.dir/RaspberryMain.c.o
.PHONY : RaspberryMain.c.o

RaspberryMain.i: RaspberryMain.c.i

.PHONY : RaspberryMain.i

# target to preprocess a source file
RaspberryMain.c.i:
	$(MAKE) -f CMakeFiles/RaspberryMain.dir/build.make CMakeFiles/RaspberryMain.dir/RaspberryMain.c.i
.PHONY : RaspberryMain.c.i

RaspberryMain.s: RaspberryMain.c.s

.PHONY : RaspberryMain.s

# target to generate assembly for a file
RaspberryMain.c.s:
	$(MAKE) -f CMakeFiles/RaspberryMain.dir/build.make CMakeFiles/RaspberryMain.dir/RaspberryMain.c.s
.PHONY : RaspberryMain.c.s

TestCapture.o: TestCapture.c.o

.PHONY : TestCapture.o

# target to build an object file
TestCapture.c.o:
	$(MAKE) -f CMakeFiles/TestCapture.dir/build.make CMakeFiles/TestCapture.dir/TestCapture.c.o
.PHONY : TestCapture.c.o

TestCapture.i: TestCapture.c.i

.PHONY : TestCapture.i

# target to preprocess a source file
TestCapture.c.i:
	$(MAKE) -f CMakeFiles/TestCapture.dir/build.make CMakeFiles/TestCapture.dir/TestCapture.c.i
.PHONY : TestCapture.c.i

TestCapture.s: TestCapture.c.s

.PHONY : TestCapture.s

# target to generate assembly for a file
TestCapture.c.s:
	$(MAKE) -f CMakeFiles/TestCapture.dir/build.make CMakeFiles/TestCapture.dir/TestCapture.c.s
.PHONY : TestCapture.c.s

common/Frame.o: common/Frame.c.o

.PHONY : common/Frame.o

# target to build an object file
common/Frame.c.o:
	$(MAKE) -f CMakeFiles/COMMON.dir/build.make CMakeFiles/COMMON.dir/common/Frame.c.o
.PHONY : common/Frame.c.o

common/Frame.i: common/Frame.c.i

.PHONY : common/Frame.i

# target to preprocess a source file
common/Frame.c.i:
	$(MAKE) -f CMakeFiles/COMMON.dir/build.make CMakeFiles/COMMON.dir/common/Frame.c.i
.PHONY : common/Frame.c.i

common/Frame.s: common/Frame.c.s

.PHONY : common/Frame.s

# target to generate assembly for a file
common/Frame.c.s:
	$(MAKE) -f CMakeFiles/COMMON.dir/build.make CMakeFiles/COMMON.dir/common/Frame.c.s
.PHONY : common/Frame.c.s

common/Packet.o: common/Packet.c.o

.PHONY : common/Packet.o

# target to build an object file
common/Packet.c.o:
	$(MAKE) -f CMakeFiles/COMMON.dir/build.make CMakeFiles/COMMON.dir/common/Packet.c.o
.PHONY : common/Packet.c.o

common/Packet.i: common/Packet.c.i

.PHONY : common/Packet.i

# target to preprocess a source file
common/Packet.c.i:
	$(MAKE) -f CMakeFiles/COMMON.dir/build.make CMakeFiles/COMMON.dir/common/Packet.c.i
.PHONY : common/Packet.c.i

common/Packet.s: common/Packet.c.s

.PHONY : common/Packet.s

# target to generate assembly for a file
common/Packet.c.s:
	$(MAKE) -f CMakeFiles/COMMON.dir/build.make CMakeFiles/COMMON.dir/common/Packet.c.s
.PHONY : common/Packet.c.s

odroid/ClientTCP.o: odroid/ClientTCP.c.o

.PHONY : odroid/ClientTCP.o

# target to build an object file
odroid/ClientTCP.c.o:
	$(MAKE) -f CMakeFiles/ODROID.dir/build.make CMakeFiles/ODROID.dir/odroid/ClientTCP.c.o
.PHONY : odroid/ClientTCP.c.o

odroid/ClientTCP.i: odroid/ClientTCP.c.i

.PHONY : odroid/ClientTCP.i

# target to preprocess a source file
odroid/ClientTCP.c.i:
	$(MAKE) -f CMakeFiles/ODROID.dir/build.make CMakeFiles/ODROID.dir/odroid/ClientTCP.c.i
.PHONY : odroid/ClientTCP.c.i

odroid/ClientTCP.s: odroid/ClientTCP.c.s

.PHONY : odroid/ClientTCP.s

# target to generate assembly for a file
odroid/ClientTCP.c.s:
	$(MAKE) -f CMakeFiles/ODROID.dir/build.make CMakeFiles/ODROID.dir/odroid/ClientTCP.c.s
.PHONY : odroid/ClientTCP.c.s

odroid/Display.o: odroid/Display.c.o

.PHONY : odroid/Display.o

# target to build an object file
odroid/Display.c.o:
	$(MAKE) -f CMakeFiles/ODROID.dir/build.make CMakeFiles/ODROID.dir/odroid/Display.c.o
.PHONY : odroid/Display.c.o

odroid/Display.i: odroid/Display.c.i

.PHONY : odroid/Display.i

# target to preprocess a source file
odroid/Display.c.i:
	$(MAKE) -f CMakeFiles/ODROID.dir/build.make CMakeFiles/ODROID.dir/odroid/Display.c.i
.PHONY : odroid/Display.c.i

odroid/Display.s: odroid/Display.c.s

.PHONY : odroid/Display.s

# target to generate assembly for a file
odroid/Display.c.s:
	$(MAKE) -f CMakeFiles/ODROID.dir/build.make CMakeFiles/ODROID.dir/odroid/Display.c.s
.PHONY : odroid/Display.c.s

odroid/UDPReception.o: odroid/UDPReception.c.o

.PHONY : odroid/UDPReception.o

# target to build an object file
odroid/UDPReception.c.o:
	$(MAKE) -f CMakeFiles/ODROID.dir/build.make CMakeFiles/ODROID.dir/odroid/UDPReception.c.o
.PHONY : odroid/UDPReception.c.o

odroid/UDPReception.i: odroid/UDPReception.c.i

.PHONY : odroid/UDPReception.i

# target to preprocess a source file
odroid/UDPReception.c.i:
	$(MAKE) -f CMakeFiles/ODROID.dir/build.make CMakeFiles/ODROID.dir/odroid/UDPReception.c.i
.PHONY : odroid/UDPReception.c.i

odroid/UDPReception.s: odroid/UDPReception.c.s

.PHONY : odroid/UDPReception.s

# target to generate assembly for a file
odroid/UDPReception.c.s:
	$(MAKE) -f CMakeFiles/ODROID.dir/build.make CMakeFiles/ODROID.dir/odroid/UDPReception.c.s
.PHONY : odroid/UDPReception.c.s

raspberry/ServerTCP.o: raspberry/ServerTCP.c.o

.PHONY : raspberry/ServerTCP.o

# target to build an object file
raspberry/ServerTCP.c.o:
	$(MAKE) -f CMakeFiles/RASPBE.dir/build.make CMakeFiles/RASPBE.dir/raspberry/ServerTCP.c.o
.PHONY : raspberry/ServerTCP.c.o

raspberry/ServerTCP.i: raspberry/ServerTCP.c.i

.PHONY : raspberry/ServerTCP.i

# target to preprocess a source file
raspberry/ServerTCP.c.i:
	$(MAKE) -f CMakeFiles/RASPBE.dir/build.make CMakeFiles/RASPBE.dir/raspberry/ServerTCP.c.i
.PHONY : raspberry/ServerTCP.c.i

raspberry/ServerTCP.s: raspberry/ServerTCP.c.s

.PHONY : raspberry/ServerTCP.s

# target to generate assembly for a file
raspberry/ServerTCP.c.s:
	$(MAKE) -f CMakeFiles/RASPBE.dir/build.make CMakeFiles/RASPBE.dir/raspberry/ServerTCP.c.s
.PHONY : raspberry/ServerTCP.c.s

raspberry/UDPStreaming.o: raspberry/UDPStreaming.c.o

.PHONY : raspberry/UDPStreaming.o

# target to build an object file
raspberry/UDPStreaming.c.o:
	$(MAKE) -f CMakeFiles/RASPBE.dir/build.make CMakeFiles/RASPBE.dir/raspberry/UDPStreaming.c.o
.PHONY : raspberry/UDPStreaming.c.o

raspberry/UDPStreaming.i: raspberry/UDPStreaming.c.i

.PHONY : raspberry/UDPStreaming.i

# target to preprocess a source file
raspberry/UDPStreaming.c.i:
	$(MAKE) -f CMakeFiles/RASPBE.dir/build.make CMakeFiles/RASPBE.dir/raspberry/UDPStreaming.c.i
.PHONY : raspberry/UDPStreaming.c.i

raspberry/UDPStreaming.s: raspberry/UDPStreaming.c.s

.PHONY : raspberry/UDPStreaming.s

# target to generate assembly for a file
raspberry/UDPStreaming.c.s:
	$(MAKE) -f CMakeFiles/RASPBE.dir/build.make CMakeFiles/RASPBE.dir/raspberry/UDPStreaming.c.s
.PHONY : raspberry/UDPStreaming.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... RASPBE"
	@echo "... edit_cache"
	@echo "... RaspberryMain"
	@echo "... COMMON"
	@echo "... ODROID"
	@echo "... OdroidMain"
	@echo "... TestCapture"
	@echo "... OdroidMain.o"
	@echo "... OdroidMain.i"
	@echo "... OdroidMain.s"
	@echo "... RaspberryMain.o"
	@echo "... RaspberryMain.i"
	@echo "... RaspberryMain.s"
	@echo "... TestCapture.o"
	@echo "... TestCapture.i"
	@echo "... TestCapture.s"
	@echo "... common/Frame.o"
	@echo "... common/Frame.i"
	@echo "... common/Frame.s"
	@echo "... common/Packet.o"
	@echo "... common/Packet.i"
	@echo "... common/Packet.s"
	@echo "... odroid/ClientTCP.o"
	@echo "... odroid/ClientTCP.i"
	@echo "... odroid/ClientTCP.s"
	@echo "... odroid/Display.o"
	@echo "... odroid/Display.i"
	@echo "... odroid/Display.s"
	@echo "... odroid/UDPReception.o"
	@echo "... odroid/UDPReception.i"
	@echo "... odroid/UDPReception.s"
	@echo "... raspberry/ServerTCP.o"
	@echo "... raspberry/ServerTCP.i"
	@echo "... raspberry/ServerTCP.s"
	@echo "... raspberry/UDPStreaming.o"
	@echo "... raspberry/UDPStreaming.i"
	@echo "... raspberry/UDPStreaming.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

