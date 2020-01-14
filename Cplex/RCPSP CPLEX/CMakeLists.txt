# The Compiler must be set BEFORE the Project declaration, otherwise, it will create an endless loop
set( CMAKE_SKIP_BUILD_RPATH true)

cmake_minimum_required (VERSION 2.8)
project (CPSCHEDULER)

# Generate JSON compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# The version number.
set (CPSCHEDULER_VERSION_MAJOR 1)
set (CPSCHEDULER_VERSION_MINOR 0)
set (CPSCHEDULER_VERSION_PATCH 0)
set (CPSCHEDULER_VERSION ${CMP_VERSION_MAJOR}.${CMP_VERSION_MINOR}.${CMP_VERSION_PATCH})

message(STATUS "Using ${CMAKE_CXX_COMPILER} as C++ compiler.")
message(STATUS "Using ${CMAKE_C_COMPILER} as C compiler.")
message(STATUS " ")
message(STATUS "Entering ${PROJECT_NAME}")
message(STATUS "Compiling in ${CMAKE_BUILD_TYPE} mode!!!")
string( TOLOWER "${CMAKE_BUILD_TYPE}" build_type )

if(NOT(build_type MATCHES "debug") AND NOT(build_type MATCHES "release") AND NOT(build_type MATCHES "relwithdebinfo"))
	message(FATAL_ERROR "Check your setting of CMAKE_BUILD_TYPE, only \"Debug\", \"Release\" and \"RelWithDebInfo\" are supported.\nUse \"cmake -DCMAKE_BUILD_TYPE=<build type>\" to set the appropiate build type.")
endif()

if((build_type MATCHES "debug") OR (build_type MATCHES "release"))
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}") # For the executables and, in case of Windows, the DLLs
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}") # For the static libraries
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}") # For the dynamic libraries
	set(CMAKE_PDB_OUTPUT_DIRECTORY     "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}") # For the symbols in Windows
elseif(build_type MATCHES "relwithdebinfo")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Profile") # For the executables and, in case of Windows, the DLLs
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/Profile") # For the static libraries
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/Profile") # For the dynamic libraries
	set(CMAKE_PDB_OUTPUT_DIRECTORY     "${CMAKE_SOURCE_DIR}/lib/Profile") # For the symbols in Windows
endif()

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)


# SUPPORTS_C99
check_c_compiler_flag(-std=c99 COMPILER_SUPPORTS_C99)
if(COMPILER_SUPPORTS_C99)
	message(STATUS "The compiler ${CMAKE_C_COMPILER} has C99 support.")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
else()
	message(FATAL_ERROR "The compiler ${CMAKE_C_COMPILER} doesn't have C99 support. Please use a different C compiler.")
endif()

# SUPPORTS_GNU99
check_c_compiler_flag(-std=gnu99 COMPILER_SUPPORTS_GNU99)
if(COMPILER_SUPPORTS_GNU99)
	message(STATUS "The compiler ${CMAKE_C_COMPILER} has GNU99 support.")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
else()
	message(FATAL_ERROR "The compiler ${CMAKE_C_COMPILER} doesn't have GNU99 support. Please use a different C compiler.")
endif()

# Stop compilation at the first error that appears
# HAS_FERROR_LIMIT
check_c_compiler_flag(-ferror-limit=1 C_HAS_FERROR_LIMIT)
if (C_HAS_FERROR_LIMIT)
	message(STATUS "The compiler ${CMAKE_C_COMPILER} supports \"-ferror-limit\"")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ferror-limit=1")
else()
	message(WARNING "The compiler ${CMAKE_C_COMPILER} doesn't support \"-ferror-limit\"")
endif()

check_cxx_compiler_flag(-ferror-limit=1 CXX_HAS_FERROR_LIMIT)
if (CXX_HAS_FERROR_LIMIT)
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} supports \"-ferror-limit\"")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ferror-limit=1")
else()
	message(WARNING "The compiler ${CMAKE_CXX_COMPILER} doesn't support \"-ferror-limit\"")
endif()

# HAS_FMAX_ERRORS
if(NOT C_HAS_FERROR_LIMIT)
	check_c_compiler_flag(-fmax-errors=1 C_HAS_FMAX_ERRORS)
	if (C_HAS_FMAX_ERRORS)
		message(STATUS "The compiler ${CMAKE_C_COMPILER} supports \"-fmax-errors\"")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmax-errors=1")
	else()
		message(WARNING "The compiler ${CMAKE_C_COMPILER} doesn't support \"-fmax-errors\"")
	endif()
endif()

if(NOT CXX_HAS_FERROR_LIMIT)
	check_cxx_compiler_flag(-fmax-errors=1 CXX_HAS_FMAX_ERRORS)
	if (CXX_HAS_FMAX_ERRORS)
		message(STATUS "The compiler ${CMAKE_CXX_COMPILER} supports \"-fmax-errors\"")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmax-errors=1")
	else()
		message(WARNING "The compiler ${CMAKE_CXX_COMPILER} doesn't support \"-fmax-errors\"")
	endif()
endif()

# HAS_FATAL_ERRORS
if(NOT(C_HAS_FERROR_LIMIT) AND NOT(C_HAS_FMAX_ERRORS))
	check_c_compiler_flag(-Wfatal-errors C_HAS_FATAL_ERRORS)
	if(C_HAS_FATAL_ERRORS)
		message(STATUS "The compiler ${CMAKE_C_COMPILER} supports \"-Wfatal-errors\"")
#			set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wfatal-errors")
	else()
		message(WARNING "The compiler ${CMAKE_C_COMPILER} doesn't support \"-Wfatal-errors\"")
	endif()
endif()

# HAS_ALL
check_c_compiler_flag(-Wall C_HAS_ALL)
if(C_HAS_ALL)
	message(STATUS "The compiler ${CMAKE_C_COMPILER} supports \"-Wall\"")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
else()
	message(WARNING "The compiler ${CMAKE_C_COMPILER} doesn't support \"-Wall\"")
endif()

check_cxx_compiler_flag(-Wall CXX_HAS_ALL)
if(CXX_HAS_ALL)
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} supports \"-Wall\"")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
else()
	message(WARNING "The compiler ${CMAKE_CXX_COMPILER} doesn't support \"-Wall\"")
endif()

# HAS_PEDANTIC
check_c_compiler_flag(-pedantic C_HAS_PEDANTIC)
if(C_HAS_PEDANTIC)
	message(STATUS "The compiler ${CMAKE_C_COMPILER} supports \"-pedantic\"")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic")
else()
	message(WARNING "The compiler ${CMAKE_C_COMPILER} doesn't support \"-pedantic\"")
endif()

check_cxx_compiler_flag(-pedantic CXX_HAS_PEDANTIC)
if(CXX_HAS_PEDANTIC)
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} supports \"-pedantic\"")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
else()
	message(WARNING "The compiler ${CMAKE_CXX_COMPILER} doesn't support \"-pedantic\"")
endif()

# HAS_EXTRA
check_c_compiler_flag(-Wextra C_HAS_EXTRA)
if(C_HAS_EXTRA)
	message(STATUS "The compiler ${CMAKE_C_COMPILER} supports \"-Wextra\"")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra")
else()
	message(WARNING "The compiler ${CMAKE_C_COMPILER} doesn't support \"-Wextra\"")
endif()

check_cxx_compiler_flag(-Wextra CXX_HAS_EXTRA)
if(CXX_HAS_EXTRA)
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} supports \"-Wextra\"")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
else()
	message(WARNING "The compiler ${CMAKE_CXX_COMPILER} doesn't support \"-Wextra\"")
endif()

# HAS_MARCH
check_c_compiler_flag(-march=native C_HAS_MARCH)
if(C_HAS_MARCH)
	message(STATUS "The compiler ${CMAKE_C_COMPILER} supports \"-march=native\"")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native")
else()
	message(WARNING "The compiler ${CMAKE_C_COMPILER} doesn't support \"-march=native\"")
endif()

check_cxx_compiler_flag(-march=native CXX_HAS_MARCH)
if(CXX_HAS_MARCH)
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} supports \"-march=native\"")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
else()
	message(WARNING "The compiler ${CMAKE_CXX_COMPILER} doesn't support \"-march=native\"")
endif()

# SUPPORTS_CXX11
check_cxx_compiler_flag(-std=c++0x COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX0X)
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has C++0X support.")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
	message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} doesn't have neither C++11 nor C++0X support. Please use a different C++ compiler.")
endif()

if(build_type MATCHES "debug")
	# O0 on Debug mode
	check_cxx_compiler_flag(-O0 CXX_HAS_O0)
	if(CXX_HAS_O0)
		message(STATUS "The compiler ${CMAKE_CXX_COMPILER} supports \"-O0\"")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
	else()
		message(WARNING "The compiler ${CMAKE_CXX_COMPILER} doesn't support \"-O0\"")
	endif()

	check_c_compiler_flag(-O0 C_HAS_O0)
	if(C_HAS_O0)
		message(STATUS "The compiler ${CMAKE_C_COMPILER} supports \"-O0\"")
		set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
	else()
		message(WARNING "The compiler ${CMAKE_C_COMPILER} doesn't support \"-O0\"")
	endif()
elseif(build_type MATCHES "release")
	# For some unknown reason (most likely, a CMake bug), CLang says it supports stripping but then fails in compilation on Mac.
	# Ergo, simply doesn't even test for that flag in MacOSX.
	if (NOT ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
		# Strip files on Release mode
		check_cxx_compiler_flag(-s CXX_HAS_STRIP)
		if(CXX_HAS_STRIP)
			message(STATUS "The compiler ${CMAKE_CXX_COMPILER} supports stripping")
			set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
		else()
			message(WARNING "The compiler ${CMAKE_CXX_COMPILER} doesn't support stripping")
		endif()

		check_c_compiler_flag(-s C_HAS_STRIP)
		if(C_HAS_STRIP)
			message(STATUS "The compiler ${CMAKE_C_COMPILER} supports stripping")
			set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
		else()
			message(WARNING "The compiler ${CMAKE_C_COMPILER} doesn't support stripping")
		endif()
	endif()


endif()

add_definitions(-DLAG_PRECEDENCES=0)
add_definitions(-DPP_USE_SCALING=0)
add_definitions(-DLOG_RED_LP=0)
add_definitions(-DBZ_STATS=0)
add_definitions(-DIL_STD)

# Specific compilation flags/preprocessor instructions for OSX enviroments
if(APPLE)
	add_definitions(-DCPSCHEDULER_OSX)
	# To see code in instruments profiling
	if(build_type MATCHES "release")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
		set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g")
	endif()
# Specific compilation flags/preprocessor instructions for Linux enviroments
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	add_definitions(-DCPSCHEDULER_LINUX)
endif() 

if(build_type MATCHES "debug")
    set(C_BUILD_FLAGS "${CMAKE_C_FLAGS_DEBUG}")
    set(CXX_BUILD_FLAGS "${CMAKE_CXX_FLAGS_DEBUG}")
elseif(build_type MATCHES "release")
    set(C_BUILD_FLAGS "${CMAKE_C_FLAGS_RELEASE}")
    set(CXX_BUILD_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")
elseif(build_type MATCHES "relwithdebinfo")
    set(C_BUILD_FLAGS "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
    set(CXX_BUILD_FLAGS "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
endif()

message(STATUS "Using the following C base flags:  ${CMAKE_C_FLAGS}")
message(STATUS "Using the following C extra flags: ${C_BUILD_FLAGS}")
message(STATUS "Using the following C++ base flags:  ${CMAKE_CXX_FLAGS}")
message(STATUS "Using the following C++ extra flags: ${CXX_BUILD_FLAGS}")

#set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
#set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
#set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
#set(INSTALL_DOC_DIR doc CACHE PATH "Installation directory for Doxygen files")

#Define directories where CPLEX should be searched for
if(CPLEX_DIR)
	if(NOT CPLEX_VERSION)
		message(FATAL_ERROR "In order to use CPLEX, CPLEX_VERSION must be defined.")
	endif()

	set (CPLEX_INC_DIR "${CPLEX_DIR}/cplex/include" CACHE PATH "Include directory of CPLEX" FORCE)
	set (CONCERT_INC_DIR "${CPLEX_DIR}/concert/include" CACHE PATH "Include directory of CPLEX" FORCE)
	set (CP_INC_DIR "${CPLEX_DIR}/cpoptimizer/include" CACHE PATH "Include directory of CPLEX" FORCE)

	if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set (CPLEX_STATIC_LIB_DIR "${CPLEX_DIR}/cplex/lib/x86-64_linux/static_pic" CACHE PATH "Static library directory of CPLEX" FORCE)
	set (CPLEX_SHARED_LIB_DIR "${CPLEX_DIR}/cplex/bin/x86-64_linux" CACHE PATH "Shared library directory of CPLEX" FORCE)
	set (CONCERT_STATIC_LIB_DIR "${CPLEX_DIR}/concert/lib/x86-64_linux/static_pic" CACHE PATH "Static library directory of Concert" FORCE)
	set (CONCERT_SHARED_LIB_DIR "${CPLEX_DIR}/concert/bin/x86-64_linux" CACHE PATH "Shared library directory of Concert" FORCE)
	set (CP_STATIC_LIB_DIR "${CPLEX_DIR}/cpoptimizer/lib/x86-64_linux/static_pic" CACHE PATH "Static library directory of CPOptimizer" FORCE)
	set (CP_SHARED_LIB_DIR "${CPLEX_DIR}/cpoptimizer/bin/x86-64_linux" CACHE PATH "Shared library directory of CPOptimizer" FORCE)
	elseif(APPLE)
	set (CPLEX_STATIC_LIB_DIR "${CPLEX_DIR}/cplex/lib/x86-64_osx/static_pic" CACHE PATH "Static library directory of CPLEX" FORCE)
	set (CPLEX_SHARED_LIB_DIR "${CPLEX_DIR}/cplex/bin/x86-64_osx" CACHE PATH "Shared library directory of CPLEX" FORCE)
	set (CONCERT_STATIC_LIB_DIR "${CPLEX_DIR}/concert/lib/x86-64_osx/static_pic" CACHE PATH "Static library directory of Concert" FORCE)
	set (CONCERT_SHARED_LIB_DIR "${CPLEX_DIR}/concert/bin/x86-64_osx" CACHE PATH "Shared library directory of Concert" FORCE)
	set (CP_STATIC_LIB_DIR "${CPLEX_DIR}/cpoptimizer/lib/x86-64_osx/static_pic" CACHE PATH "Static library directory of CPOptimizer" FORCE)
	set (CP_SHARED_LIB_DIR "${CPLEX_DIR}/cpoptimizer/bin/x86-64_osx" CACHE PATH "Shared library directory of CPOptimizer" FORCE)
	endif()

	find_library(CPLEX_STATIC_LIBRARY cplex PATH ${CPLEX_STATIC_LIB_DIR})
	find_library(CPLEX_SHARED_LIBRARY cplex${CPLEX_VERSION} PATH ${CPLEX_SHARED_LIB_DIR})
	find_library(CONCERT_STATIC_LIBRARY concert PATH ${CONCERT_STATIC_LIB_DIR})
	find_library(CONCERT_SHARED_LIBRARY concert${CPLEX_VERSION} PATH ${CONCERT_SHARED_LIB_DIR})
	find_library(CP_STATIC_LIBRARY cp PATH ${CP_STATIC_LIB_DIR})
	find_library(CP_SHARED_LIBRARY cp${CPLEX_VERSION} PATH ${CP_SHARED_LIB_DIR})

	message(STATUS "CPLEX_INC_DIR set to ${CPLEX_INC_DIR}")
	message(STATUS "CPLEX_STATIC_LIB_DIR set to ${CPLEX_STATIC_LIB_DIR}")
	message(STATUS "CPLEX_SHARED_LIB_DIR set to ${CPLEX_SHARED_LIB_DIR}")
	message(STATUS "CPLEX_STATIC_LIBRARY set to ${CPLEX_STATIC_LIBRARY}")
	message(STATUS "CPLEX_SHARED_LIBRARY set to ${CPLEX_SHARED_LIBRARY}")
	message(STATUS "CONCERT_INC_DIR set to ${CONCERT_INC_DIR}")
	message(STATUS "CONCERT_STATIC_LIB_DIR set to ${CONCERT_STATIC_LIB_DIR}")
	message(STATUS "CONCERT_SHARED_LIB_DIR set to ${CONCERT_SHARED_LIB_DIR}")
	message(STATUS "CONCERT_STATIC_LIBRARY set to ${CONCERT_STATIC_LIBRARY}")
	message(STATUS "CONCERT_SHARED_LIBRARY set to ${CONCERT_SHARED_LIBRARY}")
	message(STATUS "CP_INC_DIR set to ${CP_INC_DIR}")
	message(STATUS "CP_STATIC_LIB_DIR set to ${CP_STATIC_LIB_DIR}")
	message(STATUS "CP_SHARED_LIB_DIR set to ${CP_SHARED_LIB_DIR}")
	message(STATUS "CP_STATIC_LIBRARY set to ${CP_STATIC_LIBRARY}")
	message(STATUS "CP_SHARED_LIBRARY set to ${CP_SHARED_LIBRARY}")
else()
	message(FATAL_ERROR "CPLEX not defined, it is required.")
endif()

find_package(Threads REQUIRED)

message(STATUS "Entering ${PROJECT_NAME}")
message(STATUS "Compiling in ${CMAKE_BUILD_TYPE} mode!!!")

include_directories (include) 
add_subdirectory (src) 

set(${PROJECT_NAME}_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include" CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
set(${PROJECT_NAME}_LIBRARIES "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}" CACHE INTERNAL "${PROJECT_NAME}: Library Directories" FORCE)
