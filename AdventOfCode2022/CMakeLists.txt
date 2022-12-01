# CMakeList.txt : CMake project for AdventOfCode2022, include source and define
# project specific logic here.
#

find_package(range-v3 REQUIRED)

# Add source to this project's executable.
add_executable (AdventOfCode2022 "main.cpp" "Challenge/Day1.h")
target_link_libraries(AdventOfCode2022 range-v3::range-v3)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AdventOfCode2022 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
