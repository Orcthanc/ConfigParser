cmake_minimum_required( VERSION 3.7 )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

add_library( ConfigParserLib INTERFACE )

target_include_directories( ConfigParserLib INTERFACE "include" )

project( ConfigParser )

#add_subdirectory( external )
add_subdirectory( src )
