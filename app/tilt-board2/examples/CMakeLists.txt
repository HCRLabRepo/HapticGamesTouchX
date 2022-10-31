# common settings required to build examples
find_package (GLFW REQUIRED)
include_directories (${GLFW_INCLUDE_DIRS})
include_directories (${PROJECT_SOURCE_DIR})

find_package (CHAI3D 3.2 REQUIRED)
include_directories (${CHAI3D_INCLUDE_DIRS})
link_directories (${CHAI3D_LIBRARY_DIRS})
add_definitions (${CHAI3D_DEFINITIONS})


set(SOURCES tilt-board2.cpp CGenericScene.cpp CScene1.cpp CScene2.cpp CScene3.cpp CGenericScene.h CScene1.h CScene2.h CScene3.h)
add_executable(tilt-board2-win ${SOURCES})
target_link_libraries (tilt-board2-win ${CHAI3D-BULLET_LIBRARIES} ${CHAI3D_LIBRARIES} ${GLFW_LIBRARIES})
target_include_directories(tilt-board2-win PRIVATE ${PROJECT_SOURCE_DIR}/examples)

# OS specific adjustments
if (${CMAKE_SYSTEM_NAME} MATCHES Darwin)
  add_custom_command (TARGET ${example} POST_BUILD COMMAND Rez -append ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../resources/icons/chai3d.rsrc -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${example} COMMAND SetFile -a C ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${example} VERBATIM)
endif ()
  
