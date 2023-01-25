cmake_minimum_required(VERSION 3.22)
project(mactest LANGUAGES C VERSION 0.1)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

option(USE_COMPONENTS "use components")


file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/mactest.c" [[
char *hello(void) {
    return "Hello, World!";
}
]])
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/mactest.h" [[
extern char *hello(void);
]])
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/mactestConfig.cmake.in" [[
@PACKAGE_INIT@
set(mactest_FOUND TRUE)
include("${CMAKE_CURRENT_LIST_DIR}/mactest-targets.cmake")
set(mactest_mactest_FOUND)
check_required_components(mactest)
]])

add_library(mactest SHARED
    "${CMAKE_CURRENT_BINARY_DIR}/mactest.c"
    "${CMAKE_CURRENT_BINARY_DIR}/mactest.h"
)
target_include_directories(mactest
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDE_DIR}>"
)
set_target_properties(mactest PROPERTIES
    PUBLIC_HEADER "${CMAKE_CURRENT_BINARY_DIR}/mactest.h"

    FRAMEWORK 1
    FRAMEWORK_VERSION "A"
)

if(APPLE)
    set(cmake_dir "mactest.framework/Resources/CMake")
else()
    set(cmake_dir "${CMAKE_INSTALL_LIBDIR}/cmake/mactest")
endif()

install(TARGETS mactest EXPORT mactest_EXPORTS
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" COMPONENT devel
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT devel
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT runtime
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT runtime
    RESOURCE DESTINATION "mactest.framework/Resources" COMPONENT runtime
    FRAMEWORK DESTINATION "." COMPONENT runtime
)
install(EXPORT mactest_EXPORTS
    DESTINATION "${cmake_dir}"
    NAMESPACE "mactest::"
    FILE "mactest-targets.cmake"
    COMPONENT devel
)

configure_package_config_file("${CMAKE_CURRENT_BINARY_DIR}/mactestConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/mactestConfig.cmake"
    INSTALL_DESTINATION "${cmake_dir}"
    NO_SET_AND_CHECK_MACRO
)
write_basic_package_version_file(mactestConfigVersion.cmake
    COMPATIBILITY AnyNewerVersion
)

if(USE_COMPONENTS)
    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/mactestConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/mactestConfigVersion.cmake"
        DESTINATION "${cmake_dir}" COMPONENT devel
    )
else()
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/mactestConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/mactestConfigVersion.cmake"
        DESTINATION "${cmake_dir}"
    )
endif()

if(APPLE)
    set(CPACK_GENERATOR DragNDrop)
else()
    set(CPACK_GENERATOR ZIP)
endif()

include(CPack)
