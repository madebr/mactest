cmake_minimum_required(VERSION 3.22)
project(mactest LANGUAGES C VERSION 0.1)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(resources "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
add_library(mactest SHARED
    mactest.c
    mactest.h
    ${resources}
)
target_include_directories(mactest
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDE_DIR}>"
)
set_target_properties(mactest PROPERTIES
    PUBLIC_HEADER "mactest.h"

    FRAMEWORK TRUE
    FRAMEWORK_VERSION "ABC"
    MACOSX_FRAMEWORK_IDENTIFIER "io.github.madebr.mactest"
    OSX_ARCHITECTURES "arm64;x86_64"
    RESOURCE "${resources}"
)

add_library(mactest-static STATIC
    mactest.c
    mactest.h
)
target_compile_definitions(mactest-static
    PUBLIC MACTEST_STATIC
)
target_include_directories(mactest-static
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDE_DIR}>"
)
set_target_properties(mactest-static PROPERTIES
    PUBLIC_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/mactest.h"

    FRAMEWORK TRUE
    FRAMEWORK_VERSION "ABC"
    MACOSX_FRAMEWORK_IDENTIFIER "io.github.madebr.mactest"
    OSX_ARCHITECTURES "arm64;x86_64"
)
if(NOT APPLE AND NOT MSVC)
    set_target_properties(mactest-static PROPERTIES OUTPUT_NAME mactest)
endif()

#add_executable(macmain main.c)
#target_link_libraries(macmain PRIVATE mactest)
if(APPLE)
    set(cmake_dir "mactest.framework/Resources/CMake")
else()
    set(cmake_dir "${CMAKE_INSTALL_LIBDIR}/cmake/mactest")
endif()

configure_package_config_file(mactest-config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/mactest-config.cmake"
    INSTALL_DESTINATION "${cmake_dir}"
    NO_SET_AND_CHECK_MACRO
)
write_basic_package_version_file(mactest-config-version.cmake
    COMPATIBILITY AnyNewerVersion
)

install(TARGETS mactest EXPORT mactest_EXPORTS
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    RESOURCE DESTINATION "mactest.framework/Resources"
    FRAMEWORK DESTINATION "."
)
install(EXPORT mactest_EXPORTS
    DESTINATION "${cmake_dir}"
    NAMESPACE "mactest::"
    FILE "mactest-targets.cmake"
)

install(TARGETS mactest-static EXPORT macteststatic_EXPORTS
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    RESOURCE DESTINATION "mactest-static.framework/Resources"
    FRAMEWORK DESTINATION "."
)
install(EXPORT macteststatic_EXPORTS
    DESTINATION "${cmake_dir}"
    NAMESPACE "mactest::"
    FILE "mactest-static-targets.cmake"
)
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/mactest-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/mactest-config-version.cmake"
    DESTINATION "${cmake_dir}"
)

if(APPLE)
    set(CPACK_GENERATOR DragNDrop)
else()
    set(CPACK_GENERATOR ZIP)
endif()

include(CPack)
