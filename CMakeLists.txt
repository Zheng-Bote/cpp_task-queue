cmake_minimum_required(VERSION 3.23)
project(taskqueue 
        VERSION 0.2.0 
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Dependencies
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
  inicpp
  GIT_REPOSITORY https://github.com/SemaiCZE/inicpp.git
  GIT_TAG v1.2.2  # replace with a commit SHA for reproducible builds if desired
)
FetchContent_MakeAvailable(inicpp)

# Create a local INTERFACE target and an alias so we can refer to inicpp::inicpp safely
if(NOT TARGET inicpp_interface)
  add_library(inicpp_interface INTERFACE)
  target_include_directories(inicpp_interface INTERFACE ${inicpp_SOURCE_DIR}/include)
  add_library(inicpp::inicpp ALIAS inicpp_interface)
endif()

# Library
add_library(taskqueue SHARED
    src/task_loader.cpp
    src/task_loader_ini.cpp
    src/c_api.cpp
    src/worker_queue.cpp
)

target_include_directories(taskqueue
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/taskqueue>
)

# Link dependencies AFTER the target exists
target_link_libraries(taskqueue
    PRIVATE
        nlohmann_json::nlohmann_json
        inicpp::inicpp
)

# Install / Export
install(
    TARGETS taskqueue
    EXPORT taskqueueTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/taskqueue
)

install(
    EXPORT taskqueueTargets
    FILE taskqueueTargets.cmake
    NAMESPACE taskqueue::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/taskqueue
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/taskqueueConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/taskqueueConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/taskqueueConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/taskqueue
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/taskqueueConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/taskqueueConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/taskqueue
)

# pkg-config
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/taskqueue.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/taskqueue.pc
    @ONLY
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/taskqueue.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

# Tests
enable_testing()
add_executable(test_loader tests/test_loader.cpp)
target_link_libraries(test_loader PRIVATE taskqueue)
add_test(NAME loader COMMAND test_loader)

# Example
add_executable(example example/main.cpp)
target_link_libraries(example PRIVATE taskqueue)
