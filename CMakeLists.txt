cmake_minimum_required(VERSION 3.30)

include(FetchContent)

macro(set_policy policy state)
    if(POLICY ${policy})
        cmake_policy(SET ${policy} ${state})
    else()
        message(
            WARNING 
            "Policy ${policy} is not"
            "available in this CMake version"    
        )
    endif()
endmacro()

set_policy(CMP0167 NEW)
set_policy(CMP0168 NEW)
set_policy(CMP0135 NEW)
set_policy(CMP0091 NEW)

cmake_path(
    APPEND CMAKE_CURRENT_SOURCE_DIR 
    "bin" 
    OUTPUT_VARIABLE OGLE_BIN_DIR
)

cmake_path(
    APPEND CMAKE_CURRENT_SOURCE_DIR 
    "libs" 
    OUTPUT_VARIABLE OGLE_LIBS_DIR
)

cmake_path(
    APPEND OGLE_LIBS_DIR 
    "glad" 
    OUTPUT_VARIABLE OGLE_GLAD_DIR
)

cmake_path(
    APPEND CMAKE_CURRENT_SOURCE_DIR 
    "src" 
    OUTPUT_VARIABLE OGLE_SRC_DIR
)

cmake_path(
    APPEND CMAKE_CURRENT_SOURCE_DIR 
    "shaders" 
    OUTPUT_VARIABLE OGLE_SHADERS_DIR
)

cmake_path(
    APPEND CMAKE_CURRENT_SOURCE_DIR 
    "content" 
    OUTPUT_VARIABLE OGLE_CONTENT_DIR
)

cmake_path(SET CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OGLE_BIN_DIR})

cmake_path(
    APPEND CMAKE_RUNTIME_OUTPUT_DIRECTORY 
    "Debug" 
    OUTPUT_VARIABLE CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG
)

cmake_path(
    APPEND CMAKE_RUNTIME_OUTPUT_DIRECTORY 
    "Release"
    OUTPUT_VARIABLE CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
    OGLExperiments 
    VERSION 0.0.1 
    LANGUAGES CXX C
)

file(
    GLOB_RECURSE OGLE_GLAD_FILES 
    ${OGLE_GLAD_DIR}/*    
)

file(
    GLOB_RECURSE 
    SRC_FILES 
    CONFIGURE_DEPENDS 
        ${OGLE_SRC_DIR}/*.h 
        ${OGLE_SRC_DIR}/*.cpp
)

file(
    GLOB
    EXTRA_FILES 
    CONFIGURE_DEPENDS 
    LIST_DIRECTORIES false
        ${CMAKE_CURRENT_SOURCE_DIR}/*
)

file(
    GLOB_RECURSE 
    SHADER_FILES 
    CONFIGURE_DEPENDS 
        ${OGLE_SHADERS_DIR}/*.vert
        ${OGLE_SHADERS_DIR}/*.frag
        ${OGLE_SHADERS_DIR}/*.geom
        ${OGLE_SHADERS_DIR}/*.comp
)

set_source_files_properties(
    ${EXTRA_FILES} 
    ${SHADER_FILES}
    PROPERTIES 
        HEADER_FILE_ONLY TRUE
)

add_library(
    glad 
    STATIC 
    ${OGLE_GLAD_FILES}
)

target_include_directories(
    glad
    PUBLIC
        ${OGLE_GLAD_DIR}/include
)


add_executable(
    Main 
    ${SRC_FILES}
    ${EXTRA_FILES}
    ${SHADER_FILES}
)

source_group(Shaders FILES ${SHADER_FILES})

cmake_path(SET FETCHCONTENT_BASE_DIR ${OGLE_LIBS_DIR})

FetchContent_Declare(
    XPERCMakeUtils
    GIT_REPOSITORY https://github.com/glasvizax/XPERCMakeUtils
    GIT_TAG v1.5
)

FetchContent_Declare(
	XPERMath 
	GIT_REPOSITORY https://github.com/glasvizax/XPERMath 
	GIT_TAG v1.3 
	SYSTEM
)

set(GLFW_INSTALL OFF)
FetchContent_Declare(
    GLFW
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    SYSTEM
)

FetchContent_MakeAvailable(
    GLFW 
    XPERCMakeUtils 
    XPERMath
)

include(fetch_stb NO_POLICY_SCOPE)

fetch_stb(
    DOWNLOAD_PATH ${OGLE_LIBS_DIR}
    HEADERS 
        stb_image.h
        stb_image_write.h
)

include(prepend_folder_to_targets)

prepend_folder_to_targets(
    FOLDER libs 
    TARGETS
        XPER::Math
        XPER::CMakeUtils
        stb
        glad
        glfw
        update_mappings
)

target_link_libraries(
    Main
    PRIVATE
        glfw
        opengl32.lib
        stb
        XPER::Math
        glad
)

target_compile_definitions(
    Main
    PRIVATE
        GLFW_INCLUDE_NONE
        OGLE_CONTENT_DIR="${OGLE_CONTENT_DIR}"
        OGLE_SHADERS_DIR="${OGLE_SHADERS_DIR}"
)

