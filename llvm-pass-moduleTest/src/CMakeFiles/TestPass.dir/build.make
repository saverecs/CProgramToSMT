# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest

# Include any dependencies generated for this target.
include src/CMakeFiles/TestPass.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/TestPass.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/TestPass.dir/flags.make

src/CMakeFiles/TestPass.dir/controllerConversion.cpp.o: src/CMakeFiles/TestPass.dir/flags.make
src/CMakeFiles/TestPass.dir/controllerConversion.cpp.o: src/controllerConversion.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/TestPass.dir/controllerConversion.cpp.o"
	cd /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TestPass.dir/controllerConversion.cpp.o -c /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src/controllerConversion.cpp

src/CMakeFiles/TestPass.dir/controllerConversion.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TestPass.dir/controllerConversion.cpp.i"
	cd /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src/controllerConversion.cpp > CMakeFiles/TestPass.dir/controllerConversion.cpp.i

src/CMakeFiles/TestPass.dir/controllerConversion.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TestPass.dir/controllerConversion.cpp.s"
	cd /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src/controllerConversion.cpp -o CMakeFiles/TestPass.dir/controllerConversion.cpp.s

src/CMakeFiles/TestPass.dir/controllerConversion.cpp.o.requires:

.PHONY : src/CMakeFiles/TestPass.dir/controllerConversion.cpp.o.requires

src/CMakeFiles/TestPass.dir/controllerConversion.cpp.o.provides: src/CMakeFiles/TestPass.dir/controllerConversion.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/TestPass.dir/build.make src/CMakeFiles/TestPass.dir/controllerConversion.cpp.o.provides.build
.PHONY : src/CMakeFiles/TestPass.dir/controllerConversion.cpp.o.provides

src/CMakeFiles/TestPass.dir/controllerConversion.cpp.o.provides.build: src/CMakeFiles/TestPass.dir/controllerConversion.cpp.o


src/CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.o: src/CMakeFiles/TestPass.dir/flags.make
src/CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.o: src/coreWrapperSSA.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.o"
	cd /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.o -c /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src/coreWrapperSSA.cpp

src/CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.i"
	cd /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src/coreWrapperSSA.cpp > CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.i

src/CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.s"
	cd /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src/coreWrapperSSA.cpp -o CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.s

src/CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.o.requires:

.PHONY : src/CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.o.requires

src/CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.o.provides: src/CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/TestPass.dir/build.make src/CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.o.provides.build
.PHONY : src/CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.o.provides

src/CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.o.provides.build: src/CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.o


src/CMakeFiles/TestPass.dir/IRssa.cpp.o: src/CMakeFiles/TestPass.dir/flags.make
src/CMakeFiles/TestPass.dir/IRssa.cpp.o: src/IRssa.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/TestPass.dir/IRssa.cpp.o"
	cd /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TestPass.dir/IRssa.cpp.o -c /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src/IRssa.cpp

src/CMakeFiles/TestPass.dir/IRssa.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TestPass.dir/IRssa.cpp.i"
	cd /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src/IRssa.cpp > CMakeFiles/TestPass.dir/IRssa.cpp.i

src/CMakeFiles/TestPass.dir/IRssa.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TestPass.dir/IRssa.cpp.s"
	cd /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src/IRssa.cpp -o CMakeFiles/TestPass.dir/IRssa.cpp.s

src/CMakeFiles/TestPass.dir/IRssa.cpp.o.requires:

.PHONY : src/CMakeFiles/TestPass.dir/IRssa.cpp.o.requires

src/CMakeFiles/TestPass.dir/IRssa.cpp.o.provides: src/CMakeFiles/TestPass.dir/IRssa.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/TestPass.dir/build.make src/CMakeFiles/TestPass.dir/IRssa.cpp.o.provides.build
.PHONY : src/CMakeFiles/TestPass.dir/IRssa.cpp.o.provides

src/CMakeFiles/TestPass.dir/IRssa.cpp.o.provides.build: src/CMakeFiles/TestPass.dir/IRssa.cpp.o


src/CMakeFiles/TestPass.dir/utilities.cpp.o: src/CMakeFiles/TestPass.dir/flags.make
src/CMakeFiles/TestPass.dir/utilities.cpp.o: src/utilities.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/TestPass.dir/utilities.cpp.o"
	cd /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TestPass.dir/utilities.cpp.o -c /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src/utilities.cpp

src/CMakeFiles/TestPass.dir/utilities.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TestPass.dir/utilities.cpp.i"
	cd /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src/utilities.cpp > CMakeFiles/TestPass.dir/utilities.cpp.i

src/CMakeFiles/TestPass.dir/utilities.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TestPass.dir/utilities.cpp.s"
	cd /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src/utilities.cpp -o CMakeFiles/TestPass.dir/utilities.cpp.s

src/CMakeFiles/TestPass.dir/utilities.cpp.o.requires:

.PHONY : src/CMakeFiles/TestPass.dir/utilities.cpp.o.requires

src/CMakeFiles/TestPass.dir/utilities.cpp.o.provides: src/CMakeFiles/TestPass.dir/utilities.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/TestPass.dir/build.make src/CMakeFiles/TestPass.dir/utilities.cpp.o.provides.build
.PHONY : src/CMakeFiles/TestPass.dir/utilities.cpp.o.provides

src/CMakeFiles/TestPass.dir/utilities.cpp.o.provides.build: src/CMakeFiles/TestPass.dir/utilities.cpp.o


src/CMakeFiles/TestPass.dir/allStackVariables.cpp.o: src/CMakeFiles/TestPass.dir/flags.make
src/CMakeFiles/TestPass.dir/allStackVariables.cpp.o: src/allStackVariables.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/CMakeFiles/TestPass.dir/allStackVariables.cpp.o"
	cd /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TestPass.dir/allStackVariables.cpp.o -c /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src/allStackVariables.cpp

src/CMakeFiles/TestPass.dir/allStackVariables.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TestPass.dir/allStackVariables.cpp.i"
	cd /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src/allStackVariables.cpp > CMakeFiles/TestPass.dir/allStackVariables.cpp.i

src/CMakeFiles/TestPass.dir/allStackVariables.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TestPass.dir/allStackVariables.cpp.s"
	cd /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src/allStackVariables.cpp -o CMakeFiles/TestPass.dir/allStackVariables.cpp.s

src/CMakeFiles/TestPass.dir/allStackVariables.cpp.o.requires:

.PHONY : src/CMakeFiles/TestPass.dir/allStackVariables.cpp.o.requires

src/CMakeFiles/TestPass.dir/allStackVariables.cpp.o.provides: src/CMakeFiles/TestPass.dir/allStackVariables.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/TestPass.dir/build.make src/CMakeFiles/TestPass.dir/allStackVariables.cpp.o.provides.build
.PHONY : src/CMakeFiles/TestPass.dir/allStackVariables.cpp.o.provides

src/CMakeFiles/TestPass.dir/allStackVariables.cpp.o.provides.build: src/CMakeFiles/TestPass.dir/allStackVariables.cpp.o


# Object files for target TestPass
TestPass_OBJECTS = \
"CMakeFiles/TestPass.dir/controllerConversion.cpp.o" \
"CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.o" \
"CMakeFiles/TestPass.dir/IRssa.cpp.o" \
"CMakeFiles/TestPass.dir/utilities.cpp.o" \
"CMakeFiles/TestPass.dir/allStackVariables.cpp.o"

# External object files for target TestPass
TestPass_EXTERNAL_OBJECTS =

src/libTestPass.so: src/CMakeFiles/TestPass.dir/controllerConversion.cpp.o
src/libTestPass.so: src/CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.o
src/libTestPass.so: src/CMakeFiles/TestPass.dir/IRssa.cpp.o
src/libTestPass.so: src/CMakeFiles/TestPass.dir/utilities.cpp.o
src/libTestPass.so: src/CMakeFiles/TestPass.dir/allStackVariables.cpp.o
src/libTestPass.so: src/CMakeFiles/TestPass.dir/build.make
src/libTestPass.so: src/CMakeFiles/TestPass.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX shared module libTestPass.so"
	cd /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TestPass.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/TestPass.dir/build: src/libTestPass.so

.PHONY : src/CMakeFiles/TestPass.dir/build

src/CMakeFiles/TestPass.dir/requires: src/CMakeFiles/TestPass.dir/controllerConversion.cpp.o.requires
src/CMakeFiles/TestPass.dir/requires: src/CMakeFiles/TestPass.dir/coreWrapperSSA.cpp.o.requires
src/CMakeFiles/TestPass.dir/requires: src/CMakeFiles/TestPass.dir/IRssa.cpp.o.requires
src/CMakeFiles/TestPass.dir/requires: src/CMakeFiles/TestPass.dir/utilities.cpp.o.requires
src/CMakeFiles/TestPass.dir/requires: src/CMakeFiles/TestPass.dir/allStackVariables.cpp.o.requires

.PHONY : src/CMakeFiles/TestPass.dir/requires

src/CMakeFiles/TestPass.dir/clean:
	cd /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src && $(CMAKE_COMMAND) -P CMakeFiles/TestPass.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/TestPass.dir/clean

src/CMakeFiles/TestPass.dir/depend:
	cd /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src /home/amit/MyData/1ProjectFMSafe/llvm-pass-moduleTest/src/CMakeFiles/TestPass.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/TestPass.dir/depend

