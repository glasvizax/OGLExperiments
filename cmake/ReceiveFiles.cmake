include_guard(GLOBAL)

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

source_group(Shaders FILES ${SHADER_FILES})
source_group(Content FILES ${CONTENT_FILES})
source_group(CMake FILES ${CMAKE_FILES})
