cmake_minimum_required(VERSION 3.30)

include(cmake/ProjectPrelude.cmake NO_POLICY_SCOPE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
    OGLExperiments 
    VERSION 0.0.1 
    LANGUAGES CXX C
)

file(
    GLOB_RECURSE OGLE_GLAD_FILES 
    LIST_DIRECTORIES false
    ${OGLE_GLAD_DIR}/*.h
    ${OGLE_GLAD_DIR}/*.c   
)

file(
    GLOB_RECURSE 
    SRC_FILES 
    CONFIGURE_DEPENDS 
    LIST_DIRECTORIES false
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
    GLOB
    CMAKE_FILES 
    CONFIGURE_DEPENDS 
    LIST_DIRECTORIES false
        ${OGLE_CMAKE_DIR}/*
)

file(
    GLOB
    CONTENT_FILES 
    CONFIGURE_DEPENDS 
    LIST_DIRECTORIES false
        ${OGLE_CONTENT_DIR}/*
)

file(
    GLOB_RECURSE 
    SHADER_FILES 
    CONFIGURE_DEPENDS 
    LIST_DIRECTORIES false
        ${OGLE_SHADERS_DIR}/*.vert
        ${OGLE_SHADERS_DIR}/*.frag
        ${OGLE_SHADERS_DIR}/*.geom
        ${OGLE_SHADERS_DIR}/*.comp
)

set(NOT_CPP_FILES
    ${EXTRA_FILES} 
    ${SHADER_FILES}
    ${CMAKE_FILES}
    ${CONTENT_FILES}
)

set_source_files_properties(
    ${NOT_CPP_FILES}
    PROPERTIES 
        HEADER_FILE_ONLY ON
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
    ${NOT_CPP_FILES}
)

source_group(Shaders FILES ${SHADER_FILES})
source_group(Content FILES ${CONTENT_FILES})
source_group(CMake FILES ${CMAKE_FILES})

cmake_path(
    SET 
    FETCHCONTENT_BASE_DIR ${OGLE_LIBS_DIR}
)

FetchContent_Declare(
    XPERCMakeUtils
    GIT_REPOSITORY https://github.com/glasvizax/XPERCMakeUtils
    GIT_TAG v1.5
)

FetchContent_Declare(
    GLFW
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    SYSTEM
)

FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 1.0.3
)

set(GLFW_INSTALL OFF)
FetchContent_MakeAvailable(
    GLFW 
    XPERCMakeUtils 
    glm
)

include(fetch_stb)

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
        XPER::CMakeUtils
        stb
        glad
        glfw
        update_mappings
        glm::glm
)

target_link_libraries(
    Main
    PRIVATE
        glfw
        opengl32.lib
        stb
        glad
        glm::glm
)

target_compile_definitions(
    Main
    PRIVATE
        GLFW_INCLUDE_NONE
        OGLE_CONTENT_DIR="${OGLE_CONTENT_DIR}"
        OGLE_SHADERS_DIR="${OGLE_SHADERS_DIR}"
)

