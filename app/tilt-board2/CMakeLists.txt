cmake_minimum_required (VERSION 3.0.0)
project (tiltboard-2)

#platform detection
if (${CMAKE_SYSTEM_NAME} MATCHES Windows)
  set (OS "win")
  if (${CMAKE_CL_64})
    set (ARCH "x64")
  else ()
    set (ARCH "Win32")
  endif ()
elseif (${CMAKE_SYSTEM_NAME} MATCHES Linux)
  set (OS "lin")
  if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set (ARCH "x86_64")
  else ()
    set (ARCH "i686")
  endif ()
elseif (${CMAKE_SYSTEM_NAME} MATCHES Darwin)
  set (OS "mac")
  set (ARCH "x86_64")
endif ()

#enforce build type
if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Release CACHE STRING "Setting build mode to Release" FORCE)
endif()

#output location
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY                ${PROJECT_SOURCE_DIR}/../bin/${OS}-${ARCH})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${PROJECT_SOURCE_DIR}/../bin/${OS}-${ARCH})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     ${PROJECT_SOURCE_DIR}/../bin/${OS}-${ARCH})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${PROJECT_SOURCE_DIR}/../bin/${OS}-${ARCH})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${PROJECT_SOURCE_DIR}/../bin/${OS}-${ARCH})

find_package (CHAI3D 3.2 REQUIRED)
include_directories (${CHAI3D_INCLUDE_DIRS})
link_directories (${CHAI3D_LIBRARY_DIRS})
add_definitions (${CHAI3D_DEFINITIONS})

find_package (GLFW REQUIRED)
include_directories (${GLFW_INCLUDE_DIRS})
include_directories (${PROJECT_SOURCE_DIR})



# Windows global build options
if (${CMAKE_SYSTEM_NAME} MATCHES Windows)

  # VisualStudio compiler
  if (MSVC)
    add_definitions (-D_CRT_SECURE_NO_DEPRECATE)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /MP")
    if (${CMAKE_CL_64})
      add_definitions (-DWIN64)
    else ()
      add_definitions (-DWIN32)
    endif ()

  # MinGW compiler
  elseif (MINGW)
    add_definitions (-DWIN32)
    add_definitions (-DHAVE_GCC_DESTRUCTOR)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wno-deprecated -std=c++0x")
    set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -march=native -Wno-deprecated")
  endif ()

# Linux global build options
elseif (${CMAKE_SYSTEM_NAME} MATCHES Linux)
  add_definitions (-DLINUX)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -march=native -Wno-deprecated -std=c++0x")
  set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC -march=native -Wno-deprecated")

# Mac OS X global build options
elseif (${CMAKE_SYSTEM_NAME} MATCHES Darwin)
  add_definitions (-DMACOSX)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments -Wno-deprecated -std=c++0x -stdlib=libc++")
  set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Qunused-arguments -Wno-deprecated")
endif ()

#
# static library
#
set (PATH_TO_BULLET ${PROJECT_SOURCE_DIR}/../external/chai3d-3.2.0/modules/BULLET/)
#header files
include_directories (${PATH_TO_BULLET}/src)
include_directories (${PATH_TO_BULLET}/external/bullet/src)
include_directories (${PROJECT_SOURCE_DIR}/examples)


#static library source files
file (GLOB_RECURSE source RELATIVE ${PROJECT_SOURCE_DIR} ${PATH_TO_BULLET}/src/*.cpp ${PATH_TO_BULLET}/src/*.h)
#file (GLOB_RECURSE source RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/examples/*.cpp ${PROJECT_SOURCE_DIR}/examples/*.h)

# static library external dependencies source files
file (GLOB_RECURSE source_collision  RELATIVE ${PROJECT_SOURCE_DIR} ${PATH_TO_BULLET}/external/bullet/src/BulletCollision/*.cpp ${PATH_TO_BULLET}/external/bullet/src/BulletCollision/*.h)
file (GLOB_RECURSE source_dynamics   RELATIVE ${PROJECT_SOURCE_DIR} ${PATH_TO_BULLET}/external/bullet/src/BulletDynamics/*.cpp  ${PATH_TO_BULLET}/external/bullet/src/BulletDynamics/*.h)
file (GLOB_RECURSE source_softbody   RELATIVE ${PROJECT_SOURCE_DIR} ${PATH_TO_BULLET}/external/bullet/src/BulletSoftBody/*.cpp  ${PATH_TO_BULLET}/external/bullet/src/BulletSoftBody/*.h)
file (GLOB_RECURSE source_linearmath RELATIVE ${PROJECT_SOURCE_DIR} ${PATH_TO_BULLET}/external/bullet/src/LinearMath/*.cpp      ${PATH_TO_BULLET}/external/bullet/src/LinearMath/*.h)

# build flags
set (PROJECT_DEFINITIONS "${PROJECT_DEFINITIONS} -DBT_USE_DOUBLE_PRECISION")
add_definitions (${PROJECT_DEFINITIONS})

# group source files (MSVC likes this)
foreach (FILE ${source_collision} ${source_dynamics} ${source_softbody} ${source_linearmath})
  get_filename_component (PARENT_DIR "${FILE}" PATH)
  string (REGEX REPLACE "(\\./)?(src|include)/?" "" GROUP "${PARENT_DIR}")
  string (REPLACE "/" "\\" GROUP "${GROUP}")
  source_group ("${GROUP}" FILES "${FILE}")
endforeach ()

# internal library build flags
if (${CMAKE_BUILD_TYPE} MATCHES Debug)
  add_definitions ("-DDEBUG")
endif ()
if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
  add_definitions ("-DUSE_X86_64_ASM")
endif ()

# static library
add_library (chai3d-BULLET STATIC ${source} ${source_collision} ${source_dynamics} ${source_softbody} ${source_linearmath} )

# library exports
set (CHAI3D-BULLET_INCLUDE_DIRS "${PATH_TO_BULLET}/src ${PATH_TO_BULLET}/external/bullet/src")
set (CHAI3D-BULLET_LIBRARIES chai3d-BULLET)
set (CHAI3D-BULLET_DEFINITIONS ${PROJECT_DEFINITIONS})

#
# executables
#

# examples
if (EXISTS ${PROJECT_SOURCE_DIR}/examples)
  add_subdirectory (${PROJECT_SOURCE_DIR}/examples)
endif ()


