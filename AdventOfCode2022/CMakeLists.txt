# CMakeList.txt : CMake project for AdventOfCode2022, include source and define
# project specific logic here.
#

find_package(range-v3 REQUIRED)
find_package(fmt REQUIRED)

# Add source to this project's executable.
add_executable (AdventOfCode2022 "main.cpp" "Challenge/Day1.h" "Challenge/Day3.h" "Challenge/Day4.h" "Challenge/Day5.h" "Challenge/Day7.h" "Challenge/Day8.h" "Challenge/Day9.h" "Challenge/Day10.h" "Challenge/Day11.h")
target_link_libraries(AdventOfCode2022 range-v3::range-v3 fmt::fmt)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AdventOfCode2022 PROPERTY CXX_STANDARD 23)
endif()

# TODO: Add tests and install targets if needed.
