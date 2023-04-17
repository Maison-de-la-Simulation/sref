cmake_minimum_required(VERSION 3.21)

project(sref
  VERSION 0.0.0
  LANGUAGES CXX)

add_library(sref INTERFACE)
add_library(sref::sref ALIAS sref)
target_include_directories(sref INTERFACE $<INSTALL_INTERFACE:include>)
target_compile_features(sref INTERFACE cxx_std_17)

install(
  TARGETS sref
  EXPORT sref-targets)

install(
  EXPORT sref-targets
  NAMESPACE sref::
  DESTINATION lib/cmake/sref)

install(
  DIRECTORY include/sref
  DESTINATION include)

include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/sref-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/sref-config.cmake
  INSTALL_DESTINATION lib/cmake/sref)

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/sref-config-version.cmake
  COMPATIBILITY SameMajorVersion)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/sref-config.cmake ${CMAKE_CURRENT_BINARY_DIR}/sref-config-version.cmake
  DESTINATION lib/cmake/sref)
