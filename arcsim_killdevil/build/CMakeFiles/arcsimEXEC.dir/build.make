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
CMAKE_COMMAND = /nas02/apps/cmake-2.8.12.2/bin/cmake

# The command to remove a file.
RM = /nas02/apps/cmake-2.8.12.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /nas02/apps/cmake-2.8.12.2/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /nas02/home/a/l/alexyang/arcsim

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /nas02/home/a/l/alexyang/arcsim/build

# Include any dependencies generated for this target.
include CMakeFiles/arcsimEXEC.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/arcsimEXEC.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/arcsimEXEC.dir/flags.make

CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.o: CMakeFiles/arcsimEXEC.dir/flags.make
CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.o: ../adaptiveCloth/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /nas02/home/a/l/alexyang/arcsim/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.o -c /nas02/home/a/l/alexyang/arcsim/adaptiveCloth/main.cpp

CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /nas02/home/a/l/alexyang/arcsim/adaptiveCloth/main.cpp > CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.i

CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /nas02/home/a/l/alexyang/arcsim/adaptiveCloth/main.cpp -o CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.s

CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.o.requires:
.PHONY : CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.o.requires

CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.o.provides: CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/arcsimEXEC.dir/build.make CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.o.provides.build
.PHONY : CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.o.provides

CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.o.provides.build: CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.o

# Object files for target arcsimEXEC
arcsimEXEC_OBJECTS = \
"CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.o"

# External object files for target arcsimEXEC
arcsimEXEC_EXTERNAL_OBJECTS =

arcsimEXEC: CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.o
arcsimEXEC: CMakeFiles/arcsimEXEC.dir/build.make
arcsimEXEC: libarcsim.a
arcsimEXEC: /usr/lib64/atlas/libcblas.so
arcsimEXEC: /usr/lib64/atlas/libatlas.so
arcsimEXEC: /usr/lib64/liblapack.so
arcsimEXEC: /usr/lib64/libblas.so
arcsimEXEC: /nas02/apps/boost-1.55.0/lib/libboost_filesystem.so
arcsimEXEC: /nas02/apps/boost-1.55.0/lib/libboost_system.so
arcsimEXEC: /nas02/apps/boost-1.55.0/lib/libboost_timer.so
arcsimEXEC: /nas02/apps/boost-1.55.0/lib/libboost_thread.so
arcsimEXEC: CMakeFiles/arcsimEXEC.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable arcsimEXEC"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/arcsimEXEC.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/arcsimEXEC.dir/build: arcsimEXEC
.PHONY : CMakeFiles/arcsimEXEC.dir/build

CMakeFiles/arcsimEXEC.dir/requires: CMakeFiles/arcsimEXEC.dir/adaptiveCloth/main.cpp.o.requires
.PHONY : CMakeFiles/arcsimEXEC.dir/requires

CMakeFiles/arcsimEXEC.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/arcsimEXEC.dir/cmake_clean.cmake
.PHONY : CMakeFiles/arcsimEXEC.dir/clean

CMakeFiles/arcsimEXEC.dir/depend:
	cd /nas02/home/a/l/alexyang/arcsim/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /nas02/home/a/l/alexyang/arcsim /nas02/home/a/l/alexyang/arcsim /nas02/home/a/l/alexyang/arcsim/build /nas02/home/a/l/alexyang/arcsim/build /nas02/home/a/l/alexyang/arcsim/build/CMakeFiles/arcsimEXEC.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/arcsimEXEC.dir/depend
