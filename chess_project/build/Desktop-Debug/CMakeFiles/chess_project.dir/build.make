# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/j/qt creator/chess_project"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/j/qt creator/chess_project/build/Desktop-Debug"

# Include any dependencies generated for this target.
include CMakeFiles/chess_project.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/chess_project.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/chess_project.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/chess_project.dir/flags.make

chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp: ../../chess_image.qrc
chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp: CMakeFiles/chess_project_autogen.dir/AutoRcc_chess_image_EWIEGA46WW_Info.json
chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp: ../../images/blackking.jpg
chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp: ../../images/blackqueen.jpg
chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp: ../../images/whiteturn.jpg
chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp: ../../images/whitequeen.jpg
chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp: ../../images/timer.jpg
chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp: ../../images/whitepawn.jpg
chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp: ../../images/blackcastle.jpg
chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp: ../../images/blackturn.jpg
chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp: ../../images/blackbishop.jpg
chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp: ../../images/whitecastle.jpg
chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp: ../../images/chess.jpg
chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp: ../../images/blackpawn.jpg
chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp: ../../images/whitebishop.jpg
chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp: ../../images/chessicon.jpg
chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp: ../../images/whiteking.jpg
chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp: ../../images/blackknight.jpg
chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp: ../../images/whiteknight.jpg
chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp: /usr/lib/qt5/bin/rcc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir="/home/j/qt creator/chess_project/build/Desktop-Debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Automatic RCC for chess_image.qrc"
	/usr/bin/cmake -E cmake_autorcc /home/j/qt\ creator/chess_project/build/Desktop-Debug/CMakeFiles/chess_project_autogen.dir/AutoRcc_chess_image_EWIEGA46WW_Info.json Debug

CMakeFiles/chess_project.dir/chess_project_autogen/mocs_compilation.cpp.o: CMakeFiles/chess_project.dir/flags.make
CMakeFiles/chess_project.dir/chess_project_autogen/mocs_compilation.cpp.o: chess_project_autogen/mocs_compilation.cpp
CMakeFiles/chess_project.dir/chess_project_autogen/mocs_compilation.cpp.o: CMakeFiles/chess_project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/j/qt creator/chess_project/build/Desktop-Debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/chess_project.dir/chess_project_autogen/mocs_compilation.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/chess_project.dir/chess_project_autogen/mocs_compilation.cpp.o -MF CMakeFiles/chess_project.dir/chess_project_autogen/mocs_compilation.cpp.o.d -o CMakeFiles/chess_project.dir/chess_project_autogen/mocs_compilation.cpp.o -c "/home/j/qt creator/chess_project/build/Desktop-Debug/chess_project_autogen/mocs_compilation.cpp"

CMakeFiles/chess_project.dir/chess_project_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess_project.dir/chess_project_autogen/mocs_compilation.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/j/qt creator/chess_project/build/Desktop-Debug/chess_project_autogen/mocs_compilation.cpp" > CMakeFiles/chess_project.dir/chess_project_autogen/mocs_compilation.cpp.i

CMakeFiles/chess_project.dir/chess_project_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess_project.dir/chess_project_autogen/mocs_compilation.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/j/qt creator/chess_project/build/Desktop-Debug/chess_project_autogen/mocs_compilation.cpp" -o CMakeFiles/chess_project.dir/chess_project_autogen/mocs_compilation.cpp.s

CMakeFiles/chess_project.dir/main.cpp.o: CMakeFiles/chess_project.dir/flags.make
CMakeFiles/chess_project.dir/main.cpp.o: ../../main.cpp
CMakeFiles/chess_project.dir/main.cpp.o: CMakeFiles/chess_project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/j/qt creator/chess_project/build/Desktop-Debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/chess_project.dir/main.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/chess_project.dir/main.cpp.o -MF CMakeFiles/chess_project.dir/main.cpp.o.d -o CMakeFiles/chess_project.dir/main.cpp.o -c "/home/j/qt creator/chess_project/main.cpp"

CMakeFiles/chess_project.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess_project.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/j/qt creator/chess_project/main.cpp" > CMakeFiles/chess_project.dir/main.cpp.i

CMakeFiles/chess_project.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess_project.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/j/qt creator/chess_project/main.cpp" -o CMakeFiles/chess_project.dir/main.cpp.s

CMakeFiles/chess_project.dir/chess.cpp.o: CMakeFiles/chess_project.dir/flags.make
CMakeFiles/chess_project.dir/chess.cpp.o: ../../chess.cpp
CMakeFiles/chess_project.dir/chess.cpp.o: CMakeFiles/chess_project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/j/qt creator/chess_project/build/Desktop-Debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/chess_project.dir/chess.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/chess_project.dir/chess.cpp.o -MF CMakeFiles/chess_project.dir/chess.cpp.o.d -o CMakeFiles/chess_project.dir/chess.cpp.o -c "/home/j/qt creator/chess_project/chess.cpp"

CMakeFiles/chess_project.dir/chess.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess_project.dir/chess.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/j/qt creator/chess_project/chess.cpp" > CMakeFiles/chess_project.dir/chess.cpp.i

CMakeFiles/chess_project.dir/chess.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess_project.dir/chess.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/j/qt creator/chess_project/chess.cpp" -o CMakeFiles/chess_project.dir/chess.cpp.s

CMakeFiles/chess_project.dir/chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp.o: CMakeFiles/chess_project.dir/flags.make
CMakeFiles/chess_project.dir/chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp.o: chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp
CMakeFiles/chess_project.dir/chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp.o: CMakeFiles/chess_project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/j/qt creator/chess_project/build/Desktop-Debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/chess_project.dir/chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/chess_project.dir/chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp.o -MF CMakeFiles/chess_project.dir/chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp.o.d -o CMakeFiles/chess_project.dir/chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp.o -c "/home/j/qt creator/chess_project/build/Desktop-Debug/chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp"

CMakeFiles/chess_project.dir/chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess_project.dir/chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/j/qt creator/chess_project/build/Desktop-Debug/chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp" > CMakeFiles/chess_project.dir/chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp.i

CMakeFiles/chess_project.dir/chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess_project.dir/chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/j/qt creator/chess_project/build/Desktop-Debug/chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp" -o CMakeFiles/chess_project.dir/chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp.s

# Object files for target chess_project
chess_project_OBJECTS = \
"CMakeFiles/chess_project.dir/chess_project_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/chess_project.dir/main.cpp.o" \
"CMakeFiles/chess_project.dir/chess.cpp.o" \
"CMakeFiles/chess_project.dir/chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp.o"

# External object files for target chess_project
chess_project_EXTERNAL_OBJECTS =

chess_project: CMakeFiles/chess_project.dir/chess_project_autogen/mocs_compilation.cpp.o
chess_project: CMakeFiles/chess_project.dir/main.cpp.o
chess_project: CMakeFiles/chess_project.dir/chess.cpp.o
chess_project: CMakeFiles/chess_project.dir/chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp.o
chess_project: CMakeFiles/chess_project.dir/build.make
chess_project: /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5.15.3
chess_project: /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5.15.3
chess_project: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5.15.3
chess_project: CMakeFiles/chess_project.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/j/qt creator/chess_project/build/Desktop-Debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable chess_project"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/chess_project.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/chess_project.dir/build: chess_project
.PHONY : CMakeFiles/chess_project.dir/build

CMakeFiles/chess_project.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/chess_project.dir/cmake_clean.cmake
.PHONY : CMakeFiles/chess_project.dir/clean

CMakeFiles/chess_project.dir/depend: chess_project_autogen/EWIEGA46WW/qrc_chess_image.cpp
	cd "/home/j/qt creator/chess_project/build/Desktop-Debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/j/qt creator/chess_project" "/home/j/qt creator/chess_project" "/home/j/qt creator/chess_project/build/Desktop-Debug" "/home/j/qt creator/chess_project/build/Desktop-Debug" "/home/j/qt creator/chess_project/build/Desktop-Debug/CMakeFiles/chess_project.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/chess_project.dir/depend

