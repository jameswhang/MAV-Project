# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.3

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
CMAKE_COMMAND = /opt/cmake-3.3.2/bin/cmake

# The command to remove a file.
RM = /opt/cmake-3.3.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tuofeichen/SLAM/MAV-Project/px4_ws/src/rgbd_slam

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tuofeichen/SLAM/MAV-Project/px4_ws/build/rgbd_slam

# Include any dependencies generated for this target.
include CMakeFiles/rgbd_slam.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/rgbd_slam.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rgbd_slam.dir/flags.make

CMakeFiles/rgbd_slam.dir/src/main.cpp.o: CMakeFiles/rgbd_slam.dir/flags.make
CMakeFiles/rgbd_slam.dir/src/main.cpp.o: /home/tuofeichen/SLAM/MAV-Project/px4_ws/src/rgbd_slam/src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tuofeichen/SLAM/MAV-Project/px4_ws/build/rgbd_slam/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/rgbd_slam.dir/src/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/rgbd_slam.dir/src/main.cpp.o -c /home/tuofeichen/SLAM/MAV-Project/px4_ws/src/rgbd_slam/src/main.cpp

CMakeFiles/rgbd_slam.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rgbd_slam.dir/src/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/tuofeichen/SLAM/MAV-Project/px4_ws/src/rgbd_slam/src/main.cpp > CMakeFiles/rgbd_slam.dir/src/main.cpp.i

CMakeFiles/rgbd_slam.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rgbd_slam.dir/src/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/tuofeichen/SLAM/MAV-Project/px4_ws/src/rgbd_slam/src/main.cpp -o CMakeFiles/rgbd_slam.dir/src/main.cpp.s

CMakeFiles/rgbd_slam.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/rgbd_slam.dir/src/main.cpp.o.requires

CMakeFiles/rgbd_slam.dir/src/main.cpp.o.provides: CMakeFiles/rgbd_slam.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/rgbd_slam.dir/build.make CMakeFiles/rgbd_slam.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/rgbd_slam.dir/src/main.cpp.o.provides

CMakeFiles/rgbd_slam.dir/src/main.cpp.o.provides.build: CMakeFiles/rgbd_slam.dir/src/main.cpp.o


# Object files for target rgbd_slam
rgbd_slam_OBJECTS = \
"CMakeFiles/rgbd_slam.dir/src/main.cpp.o"

# External object files for target rgbd_slam
rgbd_slam_EXTERNAL_OBJECTS =

/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: CMakeFiles/rgbd_slam.dir/src/main.cpp.o
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: CMakeFiles/rgbd_slam.dir/build.make
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /opt/ros/indigo/lib/libroscpp.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /usr/lib/x86_64-linux-gnu/libboost_signals.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /opt/ros/indigo/lib/librosconsole.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /opt/ros/indigo/lib/librosconsole_log4cxx.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /opt/ros/indigo/lib/librosconsole_backend_interface.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /usr/lib/liblog4cxx.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /usr/lib/x86_64-linux-gnu/libboost_regex.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /opt/ros/indigo/lib/libxmlrpcpp.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /opt/ros/indigo/lib/libroscpp_serialization.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /opt/ros/indigo/lib/librostime.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /opt/ros/indigo/lib/libcpp_common.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /usr/lib/x86_64-linux-gnu/libboost_system.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /usr/lib/x86_64-linux-gnu/libboost_thread.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /usr/lib/x86_64-linux-gnu/libpthread.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /usr/lib/x86_64-linux-gnu/libconsole_bridge.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/libfrontend.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /opt/ros/indigo/lib/libroscpp.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /usr/lib/x86_64-linux-gnu/libboost_signals.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /opt/ros/indigo/lib/librosconsole.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /opt/ros/indigo/lib/librosconsole_log4cxx.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /opt/ros/indigo/lib/librosconsole_backend_interface.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /usr/lib/liblog4cxx.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /usr/lib/x86_64-linux-gnu/libboost_regex.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /opt/ros/indigo/lib/libxmlrpcpp.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /opt/ros/indigo/lib/libroscpp_serialization.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /opt/ros/indigo/lib/librostime.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /opt/ros/indigo/lib/libcpp_common.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /usr/lib/x86_64-linux-gnu/libboost_system.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /usr/lib/x86_64-linux-gnu/libboost_thread.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /usr/lib/x86_64-linux-gnu/libpthread.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: /usr/lib/x86_64-linux-gnu/libconsole_bridge.so
/home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam: CMakeFiles/rgbd_slam.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tuofeichen/SLAM/MAV-Project/px4_ws/build/rgbd_slam/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rgbd_slam.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rgbd_slam.dir/build: /home/tuofeichen/SLAM/MAV-Project/px4_ws/devel/.private/rgbd_slam/lib/rgbd_slam/rgbd_slam

.PHONY : CMakeFiles/rgbd_slam.dir/build

CMakeFiles/rgbd_slam.dir/requires: CMakeFiles/rgbd_slam.dir/src/main.cpp.o.requires

.PHONY : CMakeFiles/rgbd_slam.dir/requires

CMakeFiles/rgbd_slam.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rgbd_slam.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rgbd_slam.dir/clean

CMakeFiles/rgbd_slam.dir/depend:
	cd /home/tuofeichen/SLAM/MAV-Project/px4_ws/build/rgbd_slam && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tuofeichen/SLAM/MAV-Project/px4_ws/src/rgbd_slam /home/tuofeichen/SLAM/MAV-Project/px4_ws/src/rgbd_slam /home/tuofeichen/SLAM/MAV-Project/px4_ws/build/rgbd_slam /home/tuofeichen/SLAM/MAV-Project/px4_ws/build/rgbd_slam /home/tuofeichen/SLAM/MAV-Project/px4_ws/build/rgbd_slam/CMakeFiles/rgbd_slam.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rgbd_slam.dir/depend

