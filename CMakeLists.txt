cmake_minimum_required(VERSION 2.8)

project(libwiggleport)
set(LIB_MAJOR_VERSION "1")
set(LIB_MINOR_VERSION "0")
set(LIB_PATCH_VERSION "0")
set(LIB_VERSION_STRING "${LIB_MAJOR_VERSION}.${LIB_MINOR_VERSION}.${LIB_PATCH_VERSION}")

option(WIGGLEPORT_BUILD_DOC "Build libwiggleport documentation." ON)
option(WIGGLEPORT_BUILD_TESTS "Build libwiggleport unit tests." ON)
option(WIGGLEPORT_BUILD_UTILS "Build wiggleport command line utilities." ON)

add_subdirectory(external)

if (WIGGLEPORT_BUILD_DOC)
	add_subdirectory(doc)
endif()

if (WIGGLEPORT_BUILD_TESTS)
	add_subdirectory(tests)
endif()

if (WIGGLEPORT_BUILD_UTILS)
	add_subdirectory(utils)
endif()
