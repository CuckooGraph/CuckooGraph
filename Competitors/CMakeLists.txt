cmake_minimum_required(VERSION 3.0)
project(Test)

# set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O3)
add_compile_options(-march=native)
add_compile_options(-mtune=native)
add_compile_options(-fno-stack-protector)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
if (system STREQUAL "livegraph")
    add_definitions(-DHAVE_LIVEGRAPH)
elseif (system STREQUAL "sortledton")
    add_definitions(-DHAVE_SORTLEDTON)
elseif (system STREQUAL "spruce")
    add_definitions(-DHAVE_SPRUCE)
elseif (DEFINED system)
    message(FATAL_ERROR "\n\n\n\n\t\t\tInvalid system!")

else()
    message("\n\n\n\n\t\t\tYou did not choose a system\n\n\n\n")
endif()

if (DEFINED system)
    message("\n\n\n\n\t\t\tYou have chosen to build system: "  ${system} \n\n\n\n)
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/junction junction)

include_directories(${JUNCTION_ALL_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include/)
include_directories(${CMAKE_SOURCE_DIR}/include/livegraph)
include_directories(${CMAKE_SOURCE_DIR}/include/sortledton)
include_directories(${CMAKE_SOURCE_DIR}/include/spruce)
include_directories(${CMAKE_SOURCE_DIR}/utility)


# link_directories
add_library(sortledton STATIC IMPORTED)
set_target_properties(sortledton PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/libsortledton.a)
add_library(spruce STATIC IMPORTED)
set_target_properties(spruce PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/libspruce.a)

link_libraries(${CMAKE_SOURCE_DIR}/lib/liblivegraph.so)

set(testBinary "test-${system}")
add_executable(${testBinary} src/test.cpp 
src/TestAlgorithm.cpp 
src/GeneralGraph.cpp 
src/LiveGraphInterface.cpp 
src/SortledtonInterface.cpp
src/SpruceInterface.cpp
)

add_executable(extract 
utility/ExtractSubGraph.cpp 
src/GeneralGraph.cpp 
utility/cankao.cpp)

add_executable(SparseGenerator utility/SparseGenerator.cpp)


find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${testBinary}  PUBLIC OpenMP::OpenMP_CXX)
endif()

target_link_libraries(${testBinary}  PRIVATE ${JUNCTION_ALL_LIBRARIES} sortledton spruce numa pthread event_core tbb event_pthreads)