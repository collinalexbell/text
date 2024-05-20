cmake_minimum_required(VERSION 3.10)
project(text)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

file(GLOB SOURCES
	src/*.cpp
)
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/catch.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/bufferTest.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/InputProcessorTest.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

add_executable(text ${SOURCES} src/main.cpp)

find_library(NCURSES_LIBRARY NAMES ncurses)
target_link_libraries(text ${NCURSES_LIBRARY})

enable_testing()

add_executable(test_all
	${SOURCES}
	src/test.cpp
	src/catch.cpp
	src/bufferTest.cpp
	src/InputProcessorTest.cpp
)

target_link_libraries(test_all ${NCURSES_LIBRARY})

add_test(NAME test_all COMMAND test)

install(TARGETS text RUNTIME DESTINATION bin)
