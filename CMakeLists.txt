cmake_minimum_required(VERSION 3.21)

project(Aetheria C)

if(APPLE)
    add_compile_definitions(GL_SILENCE_DEPRECATION)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# --- RAYLIB depuis third_party ---
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_POLICY_VERSION_MINIMUM 3.10 CACHE STRING "" FORCE)
add_subdirectory(third_party/raylib)

# --- CJSON ---
add_library(cjson STATIC third_party/cJSON/cJSON.c)
target_include_directories(cjson PUBLIC third_party/cJSON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF)

# Warnings : uniquement sur le code du jeu (pas sur raylib ni cjson).
# add_compile_options() ne touche que les cibles créées APRES son appel,
# donc on utilise target_compile_options() sur la cible du jeu.
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /utf-8)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE src/include)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib cjson)

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE m pthread dl)
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMENT "Copie des assets...")