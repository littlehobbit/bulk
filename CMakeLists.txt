cmake_minimum_required(VERSION 3.15)

set(VERSION "0.0.0" CACHE STRING "Project version")
project(bulk VERSION ${VERSION} LANGUAGES CXX)

option(BUILD_TESTS OFF)
option(BUILD_PACKAGE OFF)
option(RUN_CLANG_TIDY OFF)
option(RUN_IWYU OFF)
option(BUILD_DOCS OFF)
option(ENABLE_COVERAGE OFF)

add_subdirectory(libs)

add_library(
  ${PROJECT_NAME}_lib STATIC
  src/command_parser.cpp
)

target_include_directories(
  ${PROJECT_NAME}_lib PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(
  ${PROJECT_NAME}_lib PRIVATE -Wall -Wpedantic -Werror
)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(
  ${PROJECT_NAME} PRIVATE 
  ${PROJECT_NAME}_lib
  CLI11::CLI11
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wpedantic -Werror)

set_target_properties(
  ${PROJECT_NAME} ${PROJECT_NAME}_lib
  PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)

if (${BUILD_TESTS})
  enable_testing()
  add_subdirectory(tests)

  if (${ENABLE_COVERAGE})
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/code_coverage.cmake)

    setup_target_for_coverage_gcovr_html(
      NAME gcovr_html
      EXECUTABLE ctest
      DEPENDENCIES ${PROJECT_NAME}_lib ${PROJECT_NAME}_tests
      BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src"
    )

    setup_target_for_coverage_gcovr_xml(
      NAME gcovr
      EXECUTABLE ctest
      DEPENDENCIES ${PROJECT_NAME}_lib ${PROJECT_NAME}_tests
      BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src"
    )
  endif()
endif()

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

if (${BUILD_PACKAGE})
  set(CPACK_GENERATOR DEB)
  set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
  set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
  set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
  set(CPACK_PACKAGE_CONTACT gataullinruslandev@gmain.com)
  include(CPack)
endif()


if (${RUN_CLANG_TIDY})
  include(cmake/clang-tidy.cmake)
  setup_clang_tidy(${PROJECT_NAME} PUBLIC)
  setup_clang_tidy(${PROJECT_NAME}_lib INTERFACE)
endif()

if (${RUN_IWYU})
  include(cmake/iwyu.cmake)
  setup_iwyu_globally()
endif()

if (${BUILD_DOCS})
  include(docs/docs.cmake)
endif()
