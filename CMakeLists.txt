cmake_minimum_required( VERSION 3.7 )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

project( ConfigParser )

option( BUILD_TESTS "Build test programs" OFF )

add_library( ConfigParserLib INTERFACE )

target_include_directories( ConfigParserLib INTERFACE "include" )

if( BUILD_TESTS )
	add_subdirectory( src )
endif( BUILD_TESTS )
