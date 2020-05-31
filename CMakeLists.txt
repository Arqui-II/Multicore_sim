cmake_minimum_required (VERSION 2.6)

project(Multicore_sim)

#Executable names
set(EXEC Multicore_sim)

#Directory variables
set(PROCESSOR src/processor)
set(MEMORY src/memory)
set(LOGGER src/logger)
set(DISPLAY src/display)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) 

#Link all the directories with .h files
include_directories(${PROJECT_SOURCE_DIR}/include)


# Add all the source files needed to build the executable
add_executable(${EXEC} 
               src/main.cpp 
               ${PROCESSOR}/Instruction_gen.cpp
               ${PROCESSOR}/Processor.cpp
               ${PROCESSOR}/Core.cpp
               ${MEMORY}/CacheL1.cpp
               ${MEMORY}/CacheL2.cpp
               ${MEMORY}/Controller.cpp
               ${MEMORY}/RAM.cpp
               ${LOGGER}/Logger.cpp
               ${DISPLAY}/Display.cpp
               )
               
TARGET_LINK_LIBRARIES( ${EXEC} pthread ncurses)

set_target_properties(${EXEC} PROPERTIES CXX_STANDARD 11)

