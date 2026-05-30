include_guard(GLOBAL)

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

cmake_path(
    SET 
    CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OGLE_BIN_DIR}
)

cmake_path(
    APPEND CMAKE_CURRENT_SOURCE_DIR 
    "cmake" 
    OUTPUT_VARIABLE OGLE_CMAKE_DIR
)

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

cmake_path(
    SET 
    FETCHCONTENT_BASE_DIR ${OGLE_LIBS_DIR}
)
