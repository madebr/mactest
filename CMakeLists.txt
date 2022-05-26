cmake_minimum_required(VERSION 3.22)
project(mactest LANGUAGES C VERSION 0.1)

include(CMakePackageConfigHelpers)
include(GenerateExportHeader)
include(GNUInstallDirs)

add_library(mactest SHARED
    mactest.c
)
target_include_directories(mactest
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDE_DIR}>"
)
generate_export_header(mactest
    EXPORT_FILE_NAME mactest_exports.h
)
set_target_properties(mactest PROPERTIES
    PUBLIC_HEADER "mactest.h;${CMAKE_CURRENT_BINARY_DIR}/mactest_exports.h"

    FRAMEWORK TRUE
    FRAMEWORK_VERSION ABC
    OSX_ARCHITECTURES "arm64;x86_64"
)

add_executable(macmain main.c)
target_link_libraries(macmain PRIVATE mactest)

configure_package_config_file(mactest-config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/mactest-config.cmake"
    INSTALL_DESTINATION "Resources"
    NO_SET_AND_CHECK_MACRO
)
write_basic_package_version_file(mactest-config-version.cmake
    COMPATIBILITY AnyNewerVersion
)

install(TARGETS mactest macmain EXPORT mactest_EXPORTS
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    FRAMEWORK DESTINATION "."
)
install(EXPORT mactest_EXPORTS
    DESTINATION "Resources"
    NAMESPACE "mactest::"
    FILE "mactest-targets.cmake"
)
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/mactest-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/mactest-config-version.cmake"
    DESTINATION "Resources"
)

if(APPLE)
    set(CPACK_GENERATOR DragNDrop)
else()
    set(CPACK_GENERATOR ZIP)
endif()

include(CPack)
