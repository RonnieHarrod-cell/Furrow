cmake_minimum_required(VERSION 3.16)
project(FarmEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Dependencies (fetched automatically at configure time — no manual install)
# ---------------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG v5.4.3
)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(assimp)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.91.0
)
FetchContent_MakeAvailable(imgui)

# Dear ImGui ships without a CMakeLists, so we build it as a small static lib.
add_library(imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
  ${imgui_SOURCE_DIR}
  ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui PUBLIC glfw)

# GLAD (OpenGL function loader) — see third_party/glad/README.md for setup.
add_library(glad STATIC third_party/glad/src/glad.c)
target_include_directories(glad PUBLIC third_party/glad/include)

# ---------------------------------------------------------------------------
# Main executable
# ---------------------------------------------------------------------------
add_executable(FarmEngine
  src/main.cpp
  src/engine/core/Application.cpp
  src/engine/core/Window.cpp
  src/engine/render/Shader.cpp
  src/engine/render/Camera.cpp
  src/engine/render/Mesh.cpp
  src/engine/render/Raycast.cpp
)

target_include_directories(FarmEngine PRIVATE src)

target_link_libraries(FarmEngine PRIVATE
  glfw
  glad
  glm
  assimp
  imgui
)

if (APPLE)
  target_link_libraries(FarmEngine PRIVATE "-framework OpenGL")
elseif (UNIX)
  target_link_libraries(FarmEngine PRIVATE GL dl pthread)
elseif (WIN32)
  target_link_libraries(FarmEngine PRIVATE opengl32)
endif()

# Copy assets next to the built executable so relative paths work.
add_custom_command(TARGET FarmEngine POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:FarmEngine>/assets
)
