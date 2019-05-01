cmake_minimum_required(VERSION 3.0)
project(Graphics-Engine)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL -DIMGUI_IMPL_OPENGL_LOADER_GLAD)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(Engine/Vendor/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(Engine/Vendor/assimp)

#option(BUILD_BULLET2_DEMOS OFF)
#option(BUILD_CPU_DEMOS OFF)
#option(BUILD_EXTRAS OFF)
#option(BUILD_OPENGL3_DEMOS OFF)
#option(BUILD_UNIT_TESTS OFF)
#add_subdirectory(Engine/Vendor/bullet)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(Engine/Headers/
                    Engine/Vendor/assimp/include/
                    Engine/Vendor/bullet/src/
                    Engine/Vendor/glad/include/
                    Engine/Vendor/glfw/include/
                    Engine/Vendor/imgui/
                    Engine/Vendor/glm/
                    Engine/Vendor/stb/)

file(GLOB VENDORS_SOURCES Engine/Vendor/glad/src/glad.c
                          Engine/Vendor/imgui/imgui/*.cpp)
file(GLOB PROJECT_HEADERS Engine/Headers/*.hpp)
file(GLOB PROJECT_SOURCES Engine/Sources/*.cpp
                          Engine/Sources/Tests/*.cpp
                          Engine/Sources/Core/*.cpp)
file(GLOB PROJECT_SHADERS Engine/Shaders/*.shader)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)



source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_executable(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/Samples/terrain_viewer.cpp" ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${PROJECT_SHADERS} ${PROJECT_CONFIGS} ${VENDORS_SOURCES})
target_link_libraries(${PROJECT_NAME} assimp glfw ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})